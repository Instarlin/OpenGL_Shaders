#include "application.h"
#include "../ui/ui_manager.h"
#include "../rendering/renderer.h"
#include "../window/input_manager.h"
#include "../window/window_manager.h"
#include "../rendering/shader_manager.h"
#include <iostream>

namespace core {

Application::Application() 
    : data{
        .window = nullptr,
        .configManager = nullptr,
        .initialized = false
    } {
}

Application::~Application() {
    shutdown();
}

void Application::shutdown() {
    persistSettings();

    if (data.initialized) {
        ui::UIManager::shutdown();
        rendering::Renderer::shutdown(data.state);
        window::WindowManager::terminate();
        data.initialized = false;
    }

    if (data.configManager) {
        delete data.configManager;
        data.configManager = nullptr;
    }
}

bool Application::init() {
    // Config manager initialization
    data.configManager = new config::ConfigManager();
    const config::AppConfig& config = data.configManager->getConfig();

    if (!window::WindowManager::initGLFW()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return EXIT_FAILURE;
    }

    // Passing config to window manager
    window::WindowConfig windowConfig;
    windowConfig.width = config.window.width;
    windowConfig.height = config.window.height;
    windowConfig.title = config.window.title;

    data.window = window::WindowManager::createWindow(windowConfig);
    if (!data.window) {
        std::cerr << "Failed to create window!" << std::endl;
        return EXIT_FAILURE;
    }

    // Passing config to state
    data.state.renderState.width = config.window.width;
    data.state.renderState.height = config.window.height;
    data.state.renderState.vertexPath = config.shaders.vertex;
    data.state.renderState.fragmentPath = config.shaders.fragment;

    // Passing config to input state
    data.state.inputState.scrollSensitivity = config.controls.scrollSensitivity;

    // Load persisted settings
    data.state.settings = data.configManager->getSettings();
    data.state.settings.flags.openSettings = false;
    data.state.settings.flags.closeSettings = false;
    data.state.settings.flags.settingsVisible = false;
    data.state.settings.flags.dirty = false;

    // Setting up callbacks, both managed with GLFW
    window::WindowManager::setupCallbacks(data.window, &data.state);
    input::InputManager::setupCallbacks(data.window, &data.state);

    rendering::Renderer::init(data.state, data.window);

    if (!ui::UIManager::init(data.window)) return false;

    data.initialized = true;
    return true;
}

void Application::run() {
    if (!data.initialized) {
        std::cerr << "Application not initialized!" << std::endl;
        return;
    }

    while (!window::WindowManager::shouldClose(data.window)) {
        processEvents();
        update();
        render();
        
        glfwSwapBuffers(data.window);
    }
}

void Application::processEvents() {
    glfwPollEvents();
    updateShaderWatch();
    persistSettings();
}

void Application::update() {
    input::InputManager::updateInputState(data.state);
}

void Application::render() {
    double currentTime = glfwGetTime();
    
    rendering::Renderer::render(data.state, data.window, currentTime);

    ui::UIManager::render(data.state, data.window);
}

void Application::updateShaderWatch() {
    auto getTimestamp = [](const std::string& path, std::filesystem::file_time_type& outTimestamp) -> bool {
        try {
            std::filesystem::path shaderPath(path);
            if (!std::filesystem::exists(shaderPath)) {
                return false;
            }
            outTimestamp = std::filesystem::last_write_time(shaderPath);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Shader watch error (" << path << "): " << e.what() << std::endl;
            return false;
        }
    };

    if (!data.state.settings.interaction.shaderAutoReload) {
        data.shaderWatch.initialized = false;
        return;
    }

    if (!data.shaderWatch.initialized) {
        std::filesystem::file_time_type vertexTime{};
        std::filesystem::file_time_type fragmentTime{};
        getTimestamp(data.state.renderState.vertexPath, vertexTime);
        getTimestamp(data.state.renderState.fragmentPath, fragmentTime);
        data.shaderWatch.vertexTimestamp = vertexTime;
        data.shaderWatch.fragmentTimestamp = fragmentTime;
        data.shaderWatch.initialized = true;
        std::cout << "Shader auto-reload enabled." << std::endl;
        return;
    }

    bool shouldReload = false;
    std::filesystem::file_time_type vertexTime{};
    if (getTimestamp(data.state.renderState.vertexPath, vertexTime) &&
        vertexTime != data.shaderWatch.vertexTimestamp) {
        data.shaderWatch.vertexTimestamp = vertexTime;
        shouldReload = true;
    }

    std::filesystem::file_time_type fragmentTime{};
    if (getTimestamp(data.state.renderState.fragmentPath, fragmentTime) &&
        fragmentTime != data.shaderWatch.fragmentTimestamp) {
        data.shaderWatch.fragmentTimestamp = fragmentTime;
        shouldReload = true;
    }

    if (!shouldReload) {
        return;
    }

    try {
        data.state.renderState.renderingProgram =
            rendering::ShaderManager::recreateShaderProgram(data.state, 1);
        rendering::ShaderManager::initializeUniforms(data.state);
        std::cout << "Shaders reloaded (auto-watch)." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to reload shaders: " << e.what() << std::endl;
    }
}

void Application::persistSettings() {
    if (!data.configManager) {
        return;
    }

    if (!data.state.settings.flags.dirty) {
        return;
    }

    data.configManager->saveSettings(data.state.settings);
    data.state.settings.flags.dirty = false;
}

} // namespace core
