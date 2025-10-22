#include "config_manager.h"
#include <fstream>

namespace config {

ConfigManager::ConfigManager(const std::string& configPath, const std::string& settingsPath)
    : configPath(configPath),
      settingsPath(settingsPath) {
    loadConfig();
    loadSettings();
}

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

void ConfigManager::loadSettings() {
    std::ifstream file(settingsPath);
    if (!file.is_open()) {
        std::cout << "Settings file not found, creating default settings..." << std::endl;
        createDefaultSettings();
        return;
    }

    try {
        json j;
        file >> j;
        file.close();

        parseSettings(j);
        std::cout << "Settings loaded from " << settingsPath << std::endl;
    } catch (const json::exception& e) {
        std::cerr << "Error parsing settings JSON: " << e.what() << std::endl;
        createDefaultSettings();
    }
}

void ConfigManager::createDefaultSettings() {
    settingsData.reset();
    saveSettings(settingsData);
    std::cout << "Default settings created: " << settingsPath << std::endl;
}

void ConfigManager::parseSettings(const json& j) {
    settingsData.reset();

    if (j.contains("visual")) {
        const auto& visual = j["visual"];
        if (visual.contains("uiScale")) {
            float value = visual["uiScale"];
            if (value <= 0.0f) {
                value = 1.0f;
            }
            settingsData.visual.uiScale = value;
        }
    }

    if (j.contains("interaction")) {
        const auto& interaction = j["interaction"];
        if (interaction.contains("shaderAutoReload")) {
            settingsData.interaction.shaderAutoReload = interaction["shaderAutoReload"];
        }
    }
}

void ConfigManager::saveSettings(const settings::Settings& settings) {
    settingsData = settings;

    // Flags are runtime-only
    settingsData.flags.openSettings = false;
    settingsData.flags.closeSettings = false;
    settingsData.flags.settingsVisible = false;
    settingsData.flags.dirty = false;

    json j = {
        {"visual", {
            {"uiScale", settingsData.visual.uiScale}
        }},
        {"interaction", {
            {"shaderAutoReload", settingsData.interaction.shaderAutoReload}
        }}
    };

    std::ofstream file(settingsPath);
    if (!file.is_open()) {
        std::cerr << "Failed to write settings file: " << settingsPath << std::endl;
        return;
    }

    file << j.dump(4);
    file.close();
    std::cout << "Settings saved to " << settingsPath << std::endl;
}

} // namespace config
