#pragma once
#include <cstdint>

enum class SystemState { ACTIVE, INACTIVE };
SystemState evaluateSystemState(bool isActive, bool motionDetected, uint32_t timeSinceMotion);