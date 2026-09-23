#include "state_logic.h"
#include <cstdint>

SystemState evaluateSystemState(bool isActive, bool motionDetected, uint32_t timeSinceMotion) {
    // Kung naay motion, mobalik dayon sa ACTIVE
    if (motionDetected) {
        return SystemState::ACTIVE;
    }
    
    // Kung ACTIVE pero mo-exceed na sa 15 seconds (15000 ms) nga walay motion, moadto sa INACTIVE
    if (isActive && timeSinceMotion >= 15000) {
        return SystemState::INACTIVE;
    }
    
    // Kung INACTIVE na daan ug walay motion, magpabilin nga INACTIVE
    if (!isActive) {
        return SystemState::INACTIVE;
    }
    
    // Default: magpabilin nga ACTIVE
    return SystemState::ACTIVE;
}