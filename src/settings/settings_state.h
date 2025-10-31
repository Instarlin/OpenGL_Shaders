#pragma once

namespace settings {

struct VisualSettings {
    float uiScale = 1.0f;
};

struct InteractionSettings {
    bool shaderAutoReload = false;
    float scrollSensitivity = 0.01f;
};

struct Settings {
    VisualSettings visual;
    InteractionSettings interaction;
    struct {
        bool openSettings = false;
        bool closeSettings = false;
        bool settingsVisible = false;
        bool dirty = false;
    } flags;

    void reset();
};

} // namespace settings
