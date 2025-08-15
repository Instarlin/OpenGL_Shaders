#include "config_manager.h"
#include <fstream>

namespace config {

void ConfigManager::loadConfig() {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::cout << "Config file not found, creating default config..." << std::endl;
        createDefaultConfig();
        return;
    }

    try {
        json j;
        file >> j;
        file.close();
        
        parseConfig(j);
        
        try {
            std::filesystem::path path(configPath);
            if (std::filesystem::exists(path)) {
                auto currentTime = std::filesystem::last_write_time(path);
                config.lastModified = std::chrono::system_clock::from_time_t(
                    std::chrono::duration_cast<std::chrono::seconds>(
                        currentTime.time_since_epoch()).count());
            }
        } catch (const std::exception& e) {
            std::cerr << "Error getting file modification time: " << e.what() << std::endl;
        }
    } catch (const json::exception& e) {
        std::cerr << "Error parsing JSON config: " << e.what() << std::endl;
        std::cout << "Creating default config..." << std::endl;
        createDefaultConfig();
    }
}

void ConfigManager::createDefaultConfig() {
    json defaultConfig = {
        {"window", {
            {"title", "3D Scene"},
            {"width", 1000},
            {"height", 650},
            
        }},
        {"shaders", {
            {"vertex", "glsl/vertex.glsl"},
            {"fragment", "glsl/fragment.glsl"}
        }},
        {"hotReload", {
            {"enabled", true},
            {"configPath", "config.json"},
            {"checkInterval", 1000}
        }},
        {"controls", {
            {"scrollSensitivity", 0.01f},
            {"_comment", "scrollInput*scrollSensitivity"}
        }}
    };

    std::ofstream file(configPath);
    if (!file.is_open()) {
        std::cerr << "Failed to create config file: " << configPath << std::endl;
        return;
    }

    file << defaultConfig.dump(4);
    file.close();
    std::cout << "Default config created: " << configPath << std::endl;
}

void ConfigManager::parseConfig(const json& j) {
    if (j.contains("window")) {
        const auto& window = j["window"];
        if (window.contains("width")) {
            config.window.width = window["width"];
        }
        if (window.contains("height")) {
            config.window.height = window["height"];
        }
        if (window.contains("title")) {
            config.window.title = window["title"];
        }
    }

    if (j.contains("shaders")) {
        const auto& shaders = j["shaders"];
        if (shaders.contains("vertex")) {
            config.shaders.vertex = shaders["vertex"];
        }
        if (shaders.contains("fragment")) {
            config.shaders.fragment = shaders["fragment"];
        }
    }

    if (j.contains("hotReload")) {
        const auto& hotReload = j["hotReload"];
        if (hotReload.contains("enabled")) {
            config.hotReload.enabled = hotReload["enabled"];
        }
        if (hotReload.contains("configPath")) {
            config.hotReload.configPath = hotReload["configPath"];
        }
        if (hotReload.contains("checkInterval")) {
            config.hotReload.checkInterval = std::chrono::milliseconds(hotReload["checkInterval"]);
        }
    }

    if (j.contains("controls")) {
        const auto& controls = j["controls"];
        if (controls.contains("scrollSensitivity")) {
          config.controls.scrollSensitivity = controls["scrollSensitivity"];
        }
    }

    std::cout << "Config loaded: " << config.window.width << "x" << config.window.height 
              << ", vertex: " << config.shaders.vertex 
              << ", fragment: " << config.shaders.fragment 
              << ", scrollSensitivity: " << config.controls.scrollSensitivity
              << std::endl;
} 

} // namespace config