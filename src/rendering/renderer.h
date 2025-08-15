#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../state/app_state.h"

namespace rendering {

/**
 * Основной рендерер приложения
 */
class Renderer {
public:
    /**
     * Инициализирует рендерер
     * @param state Состояние приложения
     * @param window Окно GLFW
     */
    static void init(state::AppState& state, GLFWwindow* window);
    
    /**
     * Выполняет рендеринг кадра
     * @param state Состояние приложения
     * @param window Окно GLFW
     * @param currentTime Текущее время
     */
    static void render(state::AppState& state, GLFWwindow* window, double currentTime);
    
    /**
     * Обновляет uniform переменные
     * @param state Состояние приложения
     */
    static void updateUniforms(state::AppState& state);
    
    /**
     * Очищает ресурсы рендерера
     * @param state Состояние приложения
     */
    static void shutdown(state::AppState& state);

private:
    static void setupVertexArray(state::AppState& state);
    static void updateRenderState(state::AppState& state);
};

} // namespace rendering 