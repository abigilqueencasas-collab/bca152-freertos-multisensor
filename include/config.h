#pragma once
#include <cstdint>

// --- Pins ---
constexpr int PIN_DHT22    = 15;
constexpr int PIN_PIR      = 26;
constexpr int PIN_ENC_CLK  = 18;
constexpr int PIN_ENC_DT   = 19;
constexpr int PIN_OLED_SDA = 21;
constexpr int PIN_OLED_SCL = 22;
constexpr int PIN_BUZZER   = 25;
// LDR analog = GPIO34 (ADC1_CHANNEL_6)

// --- Limits ---
constexpr float    TEMP_LOW_LIMIT_C  = 18.0f;
constexpr float    TEMP_HIGH_LIMIT_C = 30.0f;
constexpr uint32_t INACTIVITY_TIMEOUT_MS = 15000;