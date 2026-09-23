#include "state_logic.h"
#include "config.h"

SystemState evaluateSystemState(bool isActive, bool motionDetected, uint32_t timeSinceMotion) {
    // Kung naay motion, mobalik dayon sa ACTIVE
    if (motionDetected) {
        return SystemState::ACTIVE;
    }

    // Kung ACTIVE pero mo-exceed na sa INACTIVITY_TIMEOUT_MS nga walay motion, moadto sa INACTIVE
    if (isActive && timeSinceMotion >= INACTIVITY_TIMEOUT_MS) {
        return SystemState::INACTIVE;
    }

    // Kung INACTIVE na daan ug walay motion, magpabilin nga INACTIVE
    if (!isActive) {
        return SystemState::INACTIVE;
    }

    // Default: magpabilin nga ACTIVE
    return SystemState::ACTIVE;
}