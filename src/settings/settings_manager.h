#pragma once

#include "settings_state.h"

namespace settings {

class SettingsManager {
public:
    SettingsManager();

    bool apply(settings::Settings& settings);
};

} // namespace settings
