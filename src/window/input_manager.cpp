#include "input_manager.h"
#include "../rendering/shader_manager.h"

namespace input {

void InputManager::setupCallbacks(GLFWwindow* window, state::AppState* state) {
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    state::AppState* data = getAppState(window);
    if (!data) return;

    if (key == GLFW_KEY_LEFT_CONTROL) {
        data->inputState.isLeftCtrlDown = (action == GLFW_PRESS);
    }

    if (data->inputState.isLeftCtrlDown && action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_R:
                data->renderState.renderingProgram =
                    rendering::ShaderManager::recreateShaderProgram(*data, 1);
                rendering::ShaderManager::initializeUniforms(*data);
                break;
            case GLFW_KEY_1:
                data->renderState.menuCase = 0;
                break;
            case GLFW_KEY_2:
                data->renderState.menuCase = 1;
                break;
            case GLFW_KEY_3:
                data->renderState.menuCase = 2;
                break;
            case GLFW_KEY_4:
                data->renderState.menuCase = 3;
                break;
            case GLFW_KEY_S:
                if (data->settings.flags.settingsVisible) {
                    data->settings.flags.closeSettings = true;
                } else {
                    data->settings.flags.openSettings = true;
                }
                break;
            default:
                break;
        }
    }
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    state::AppState* data = getAppState(window);
    if (!data) return;

    data->renderState.distance += static_cast<float>(yoffset) * data->inputState.scrollSensitivity;
    // TODO: Switch renderState.dictance to inputState.mouseInput to generalize it
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    state::AppState* data = getAppState(window);
    if (!data) return;

    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            data->inputState.isLeftMouseDown = true;
            glfwGetCursorPos(window, &data->inputState.mouseX, &data->inputState.mouseY);
        } else if (action == GLFW_RELEASE) {
            data->inputState.isLeftMouseDown = false;

            data->inputState.offsetX += data->inputState.dx;
            data->inputState.offsetY += data->inputState.dy;

            data->inputState.dx = 0;
            data->inputState.dy = 0;
        }
    }
}

void InputManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    state::AppState* data = getAppState(window);
    if (!data) return;

    if (data->inputState.isLeftMouseDown) {
        data->inputState.dx = xpos - data->inputState.mouseX;
        data->inputState.dy = ypos - data->inputState.mouseY;

        data->inputState.totalX = data->inputState.offsetX + data->inputState.dx;
        data->inputState.totalY = data->inputState.offsetY + data->inputState.dy;
    }
}

void InputManager::updateInputState(state::AppState& state) {
    // Update input state if necessary
}

state::AppState* InputManager::getAppState(GLFWwindow* window) {
    return reinterpret_cast<state::AppState*>(glfwGetWindowUserPointer(window));
}

} // namespace input 
