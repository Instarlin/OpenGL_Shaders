#pragma once

#include "../state/app_state.h"
#include <GLFW/glfw3.h>

namespace input {

/**
 * Менеджер ввода
 */
class InputManager {
public:
    /**
     * Устанавливает обработчики ввода
     * @param window Окно GLFW
     * @param state Состояние приложения
     */
    static void setupCallbacks(GLFWwindow* window, state::AppState* state);
    
    /**
     * Обработчик нажатий клавиш
     * @param window Окно GLFW
     * @param key Код клавиши
     * @param scancode Скан-код
     * @param action Действие (нажатие/отпускание)
     * @param mods Модификаторы
     */
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    /**
     * Обработчик прокрутки мыши
     * @param window Окно GLFW
     * @param xoffset Смещение по X
     * @param yoffset Смещение по Y
     */
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    
    /**
     * Обработчик нажатий кнопок мыши
     * @param window Окно GLFW
     * @param button Кнопка мыши
     * @param action Действие
     * @param mods Модификаторы
     */
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    
    /**
     * Обработчик движения мыши
     * @param window Окно GLFW
     * @param xpos Позиция X
     * @param ypos Позиция Y
     */
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    
    /**
     * Обновляет состояние ввода
     * @param state Состояние приложения
     */
    static void updateInputState(state::AppState& state);

private:
    static state::AppState* getAppState(GLFWwindow* window);
};

} // namespace input 