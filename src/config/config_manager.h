#include <string>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <nlohmann/json.hpp>

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

    struct HotReload {
        bool enabled = true;
        std::string configPath = "config.json";
        std::chrono::milliseconds checkInterval = std::chrono::milliseconds(1000);
    } hotReload;

    struct Controls {
      float scrollSensitivity = 1.0f;
    } controls;

    std::chrono::system_clock::time_point lastModified;
    bool configChanged = false;
};

class ConfigManager {
private:
    AppConfig config;
    std::string configPath;
    std::chrono::system_clock::time_point lastCheck;

public:
    ConfigManager(const std::string& path = "config.json") : configPath(path) {
        loadConfig();
        lastCheck = std::chrono::system_clock::now();
    }

    const AppConfig& getConfig() const { return config; }
    
    bool checkForChanges() {
        auto now = std::chrono::system_clock::now();
        if (now - lastCheck < config.hotReload.checkInterval) {
            return false;
        }
        
        lastCheck = now;
        
        try {
            std::filesystem::path path(configPath);
            if (!std::filesystem::exists(path)) {
                return false;
            }
            
            auto currentTime = std::filesystem::last_write_time(path);
            auto currentTimePoint = std::chrono::system_clock::from_time_t(
                std::chrono::duration_cast<std::chrono::seconds>(
                    currentTime.time_since_epoch()).count());
            
            if (currentTimePoint > config.lastModified) {
                loadConfig();
                config.configChanged = true;
                return true;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error checking config file: " << e.what() << std::endl;
        }
        
        return false;
    }

    void resetChangeFlag() {
        config.configChanged = false;
    }

private:
    void loadConfig();
    void createDefaultConfig();
    void parseConfig(const json& j);
};

} // namespace config