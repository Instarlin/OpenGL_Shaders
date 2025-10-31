#include "window_manager.h"
#include <iostream>

namespace window {

bool WindowManager::initGLFW() {
    std::cout << "Initializing GLFW..." << std::endl;
    glfwSetErrorCallback(errorCallback);
    return glfwInit() == GLFW_TRUE;
}

GLFWwindow* WindowManager::createWindow(const WindowConfig& config) {
    std::cout << "Creating GLFW window..." << std::endl;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(config.width, config.height, 
                                         config.title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    return window;
}

void WindowManager::setupCallbacks(GLFWwindow* window, state::AppState* state) {
    glfwSetWindowUserPointer(window, state);

    // Callbacks are added in input_manager
    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow* win, int width, int height) {
            auto* appState =
                reinterpret_cast<state::AppState*>(glfwGetWindowUserPointer(win));
            if (!appState) {
                return;
            }

            appState->renderState.width = width;
            appState->renderState.height = height;
        });
}

void WindowManager::terminate() {
    glfwTerminate();
}

bool WindowManager::shouldClose(GLFWwindow* window) {
    return glfwWindowShouldClose(window);
}

void WindowManager::updateWindowSize(GLFWwindow* window, state::AppState& state) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    state.renderState.width = width;
    state.renderState.height = height;
}

void WindowManager::errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
    exit(1);
}

} // namespace window 
