#include <unity.h>
#include "alarm_logic.h"
#include "display_mode.h" // I-include ni
#include "state_logic.h"  // I-include ni

// --- Temperature Tests (5 total) ---
void test_temperature_low(void) {
    TEST_ASSERT_EQUAL(AlarmState::LOW_TEMPERATURE, evaluateTemperature(17.9));
}
void test_temperature_low_exact(void) { // Bag-o
    TEST_ASSERT_EQUAL(AlarmState::LOW_TEMPERATURE, evaluateTemperature(18.0)); // Boundary
}
void test_temperature_normal(void) {
    TEST_ASSERT_EQUAL(AlarmState::NORMAL, evaluateTemperature(25.0));
}
void test_temperature_high_exact(void) { // Bag-o
    TEST_ASSERT_EQUAL(AlarmState::HIGH_TEMPERATURE, evaluateTemperature(30.0)); // Boundary
}
void test_temperature_high(void) {
    TEST_ASSERT_EQUAL(AlarmState::HIGH_TEMPERATURE, evaluateTemperature(30.1));
}

// --- Display Navigation Tests (4 total) ---
void test_display_next_normal(void) {
    TEST_ASSERT_EQUAL(DisplayMode::HUMIDITY, nextDisplayMode(DisplayMode::TEMPERATURE));
}
void test_display_prev_normal(void) {
    TEST_ASSERT_EQUAL(DisplayMode::TEMPERATURE, previousDisplayMode(DisplayMode::HUMIDITY));
}
void test_display_next_wrap(void) { // Wrap-around gikan sa last padung sa first
    TEST_ASSERT_EQUAL(DisplayMode::TEMPERATURE, nextDisplayMode(DisplayMode::MOTION));
}
void test_display_prev_wrap(void) { // Wrap-around gikan sa first padung sa last
    TEST_ASSERT_EQUAL(DisplayMode::MOTION, previousDisplayMode(DisplayMode::TEMPERATURE));
}

// --- System State Tests (4 total) ---
void test_state_active_no_timeout(void) {
    TEST_ASSERT_EQUAL(SystemState::ACTIVE, evaluateSystemState(true, true, 1000)); 
}
void test_state_active_timeout(void) { // Mo-timeout human sa 15s (15000ms)
    TEST_ASSERT_EQUAL(SystemState::INACTIVE, evaluateSystemState(true, false, 16000));
}
void test_state_inactive_no_motion(void) {
    TEST_ASSERT_EQUAL(SystemState::INACTIVE, evaluateSystemState(false, false, 5000));
}
void test_state_inactive_motion(void) { // Mobalik sa ACTIVE kung naay motion
    TEST_ASSERT_EQUAL(SystemState::ACTIVE, evaluateSystemState(false, true, 5000));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    // Temperature
    RUN_TEST(test_temperature_low);
    RUN_TEST(test_temperature_low_exact);
    RUN_TEST(test_temperature_normal);
    RUN_TEST(test_temperature_high_exact);
    RUN_TEST(test_temperature_high);
    // Display
    RUN_TEST(test_display_next_normal);
    RUN_TEST(test_display_prev_normal);
    RUN_TEST(test_display_next_wrap);
    RUN_TEST(test_display_prev_wrap);
    // System State
    RUN_TEST(test_state_active_no_timeout);
    RUN_TEST(test_state_active_timeout);
    RUN_TEST(test_state_inactive_no_motion);
    RUN_TEST(test_state_inactive_motion);
    return UNITY_END();
}