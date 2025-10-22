#include "settings_state.h"

namespace settings {

void Settings::reset() {
    visual = VisualSettings{};
    interaction = InteractionSettings{};
    flags = {};
}

} // namespace settings
