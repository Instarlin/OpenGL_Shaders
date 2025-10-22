#include "settings_manager.h"

#include <imgui.h>
#include <cmath>

namespace settings {

namespace {
float g_lastScale = 1.0f;
constexpr float kDefaultFontSize = 13.0f;
bool g_initialized = false;
}

SettingsManager::SettingsManager() = default;

bool SettingsManager::apply(settings::Settings& settings) {
    float uiScale = settings.visual.uiScale;
    if (uiScale <= 0.0f) {
        uiScale = 1.0f;
    }

    ImGuiIO& io = ImGui::GetIO();

    if (!g_initialized) {
        //TODO: UI Is not persistent across restarts, some changes needed here
        g_lastScale = uiScale;
        g_initialized = true;
    }

    if (std::fabs(uiScale - g_lastScale) < 1e-4f) {
        return false;
    }

    float scaleRatio = uiScale / g_lastScale;

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(scaleRatio);

    io.FontGlobalScale = 1.0f;

    ImFontConfig fontCfg;
    fontCfg.SizePixels = kDefaultFontSize * uiScale;
    io.Fonts->Clear();
    io.Fonts->AddFontDefault(&fontCfg);
    io.Fonts->Build();

    g_lastScale = uiScale;
    settings.visual.uiScale = uiScale;
    return true;
}

} // namespace settings
