#pragma once

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../settings/settings_state.h"

using json = nlohmann::json;

namespace config {

struct AppConfig {
    struct Window {
      int width = 1000;
      int height = 650;
      std::string title = "3D Shader";
    } window;

    struct Shaders {
        std::string vertex = "glsl/vertex.glsl";
        std::string fragment = "glsl/fragment.glsl";
    } shaders;

};

class ConfigManager {
private:
    AppConfig config;
    std::string configPath;
    std::string settingsPath;
    settings::Settings settingsData;

public:
    ConfigManager(const std::string& configPath = "config.json",
                  const std::string& settingsPath = "settings.json");

    const AppConfig& getConfig() const { return config; }
    const settings::Settings& getSettings() const { return settingsData; }
    void saveSettings(const settings::Settings& settings);

private:
    void loadConfig();
    void createDefaultConfig();
    void parseConfig(const json& j);
    void loadSettings();
    void createDefaultSettings();
    void parseSettings(const json& j);
};

} // namespace config
