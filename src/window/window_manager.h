#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "../state/app_state.h"

namespace window {

/**
 * Конфигурация окна
 */
struct WindowConfig {
    int width = 800;
    int height = 600;
    std::string title = "Shader Application";
    bool resizable = true;
    int majorVersion = 4;
    int minorVersion = 6;
};

/**
 * Менеджер окна GLFW
 */
class WindowManager {
public:
    /**
     * Инициализирует GLFW
     * @return true если инициализация успешна
     */
    static bool initGLFW();
    
    /**
     * Создает окно
     * @param config Конфигурация окна
     * @return Указатель на созданное окно или nullptr при ошибке
     */
    static GLFWwindow* createWindow(const WindowConfig& config);
    
    /**
     * Устанавливает обработчики событий
     * @param window Окно GLFW
     * @param state Состояние приложения
     */
    static void setupCallbacks(GLFWwindow* window, state::AppState* state);
    
    /**
     * Завершает работу GLFW
     */
    static void terminate();
    
    /**
     * Проверяет, должно ли окно быть закрыто
     * @param window Окно GLFW
     * @return true если окно должно быть закрыто
     */
    static bool shouldClose(GLFWwindow* window);
    
    /**
     * Обновляет размеры окна в состоянии
     * @param window Окно GLFW
     * @param state Состояние приложения
     */
    static void updateWindowSize(GLFWwindow* window, state::AppState& state);

private:
    static void errorCallback(int error, const char* description);
};

} // namespace window 
