// src/state_logic.cpp
#include "state_logic.h"

SystemState evaluateSystemState(SystemState cur, bool motion, uint32_t idleMs, uint32_t timeoutMs) {
    if (motion) return SystemState::ACTIVE;
    if (cur == SystemState::ACTIVE && idleMs >= timeoutMs) return SystemState::INACTIVE;
    return cur;
}