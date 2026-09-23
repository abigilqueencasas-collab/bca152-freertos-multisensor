#include "alarm_logic.h"
#include "config.h"

AlarmState evaluateTemperature(float t) {
    if (t < TEMP_LOW_LIMIT_C)  return AlarmState::LOW_TEMPERATURE;
    if (t > TEMP_HIGH_LIMIT_C) return AlarmState::HIGH_TEMPERATURE;
    return AlarmState::NORMAL;
}