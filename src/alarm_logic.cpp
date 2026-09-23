#include "alarm_logic.h"
#include "config.h"

AlarmState evaluateTemperature(float temperature) {
    if (temperature <= TEMP_LOW_LIMIT_C)  return AlarmState::LOW_TEMPERATURE;
    if (temperature >= TEMP_HIGH_LIMIT_C) return AlarmState::HIGH_TEMPERATURE;
    return AlarmState::NORMAL;
}