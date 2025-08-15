#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../state/app_state.h"

namespace rendering {

/**
 * Main renderer class
 */
class Renderer {
public:
    /**
     * Initializes the renderer
     * @param state Application state
     * @param window GLFW window
     */
    static void init(state::AppState& state, GLFWwindow* window);
    
    /**
     * Renders a frame
     * @param state Application state
     * @param window GLFW window
     * @param currentTime Current time
     */
    static void render(state::AppState& state, GLFWwindow* window, double currentTime);
    
    /**
     * Updates uniform variables
     * @param state Application state
     */
    static void updateUniforms(state::AppState& state);
    
    /**
     * Cleans up resources
     * @param state Application state
     */
    static void shutdown(state::AppState& state);

private:
    static void setupVertexArray(state::AppState& state);
    static void updateRenderState(state::AppState& state);
};

} // namespace rendering 