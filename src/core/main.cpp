#include "application.h"
#include <iostream>

int main() {
    core::Application app;
    
    if (!app.init()) {
        std::cerr << "Failed to initialize application!" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::cout << "Application initialized successfully!" << std::endl;
    std::cout << "Starting main loop..." << std::endl;
    
    app.run();
    
    std::cout << "Application finished." << std::endl;
    return EXIT_SUCCESS;
} 