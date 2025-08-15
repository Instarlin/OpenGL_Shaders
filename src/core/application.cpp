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
    checkConfigChanges();
}

void Application::update() {
    input::InputManager::updateInputState(data.state);
}

void Application::render() {
    double currentTime = glfwGetTime();
    
    rendering::Renderer::render(data.state, data.window, currentTime);

    ui::UIManager::render(data.state, data.window);
}

void Application::checkConfigChanges() {
    if (data.configManager && data.configManager->checkForChanges()) {
        const config::AppConfig& newConfig = data.configManager->getConfig();
        
        // Window size update
        if (newConfig.window.width != data.state.renderState.width || 
            newConfig.window.height != data.state.renderState.height) {
            data.state.renderState.width = newConfig.window.width;
            data.state.renderState.height = newConfig.window.height;
            glfwSetWindowSize(data.window, data.state.renderState.width, data.state.renderState.height);
            std::cout << "Window size updated to: " << data.state.renderState.width 
                      << "x" << data.state.renderState.height << std::endl;
        }

        // Shaders reload
        if (newConfig.shaders.vertex != data.state.renderState.vertexPath || 
            newConfig.shaders.fragment != data.state.renderState.fragmentPath) {
            data.state.renderState.vertexPath = newConfig.shaders.vertex;
            data.state.renderState.fragmentPath = newConfig.shaders.fragment;
            data.state.renderState.renderingProgram = rendering::ShaderManager::recreateShaderProgram(data.state, 1);
            std::cout << "Shaders reloaded: " << data.state.renderState.vertexPath 
                      << ", " << data.state.renderState.fragmentPath << std::endl;
        }

        // Controls reload
        if (newConfig.controls.scrollSensitivity != data.state.inputState.scrollSensitivity) {
            data.state.inputState.scrollSensitivity = newConfig.controls.scrollSensitivity;
            std::cout << "Controls reloaded: " << data.state.inputState.scrollSensitivity << std::endl;
        }
        
        data.configManager->resetChangeFlag();
    }
}

} // namespace core