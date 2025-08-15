#pragma once

#include "../config/config_manager.h"
#include "../state/app_state.h"

#include <GLFW/glfw3.h>

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
     * Checks for configuration changes
     */
    void checkConfigChanges();

private:
    struct {
        GLFWwindow *window = nullptr;
        state::AppState state;
        config::ConfigManager *configManager = nullptr;
        bool initialized = false;
    } data;
};

} // namespace core 