// include/state_logic.h
#pragma once
#include <cstdint>
enum class SystemState { ACTIVE, INACTIVE };
SystemState evaluateSystemState(SystemState current, bool motionDetected, uint32_t idleMs, uint32_t timeoutMs);