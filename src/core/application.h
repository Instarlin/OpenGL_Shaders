#pragma once

#include "../config/config_manager.h"
#include "../state/app_state.h"

#include <GLFW/glfw3.h>
#include <filesystem>

namespace core {

/**
 * Main application class
 */
class Application {
public:
    /**
     * Constructor
     */
    Application();
    
    /**
     * Destructor
     */
    ~Application();
    
    /**
     * Initializes the application
     * @return true if initialization is successful
     */
    bool init();
    
    /**
     * Runs the main loop of the application
     */
    void run();
    
    /**
     * Shuts down the application
     */
    void shutdown();

private:
    /**
     * Processes events
     */
    void processEvents();
    
    /**
     * Updates the state of the application
     */
    void update();
    
    /**
     * Renders a frame
     */
    void render();
    
    /**
     * Updates shader auto-reload state
     */
    void updateShaderWatch();

    /**
     * Persists settings when they change
     */
    void persistSettings();

private:
    struct {
        GLFWwindow *window = nullptr;
        state::AppState state;
        config::ConfigManager *configManager = nullptr;
        bool initialized = false;
        struct {
            bool initialized = false;
            std::filesystem::file_time_type vertexTimestamp{};
            std::filesystem::file_time_type fragmentTimestamp{};
        } shaderWatch;
    } data;
};

} // namespace core 
