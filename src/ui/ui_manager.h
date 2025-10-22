#pragma once

#include "../state/app_state.h"
#include "config/slider_config.h"
#include "../settings/settings_manager.h"
#include <GLFW/glfw3.h>
#include <imgui.h>

namespace ui {

/**
 * UI manager
 */
class UIManager {
public:
    /**
     * Initializes ImGui
     * @param window GLFW window
     * @return true if initialization is successful
     */
    static bool init(GLFWwindow* window);
    
    /**
     * Renders UI
     * @param state Application state
     * @param window GLFW window
     */
    static void render(state::AppState& state, GLFWwindow* window);
    
    /**
     * Ends ImGui
     */
    static void shutdown();

private:
    static void renderContextMenu(state::AppState& state);
    static void renderMainMenu(state::AppState& state);
    static void renderSettingsPopup(state::AppState& state);
};

} // namespace ui 
