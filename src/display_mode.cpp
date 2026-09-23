#include "display_mode.h"
static constexpr int COUNT = 4;
DisplayMode nextDisplayMode(DisplayMode m)     { return static_cast<DisplayMode>((static_cast<int>(m) + 1) % COUNT); }
DisplayMode previousDisplayMode(DisplayMode m) { return static_cast<DisplayMode>((static_cast<int>(m) + COUNT - 1) % COUNT); }