#include <unity.h>
#include "alarm_logic.h" // Importante!

// Tangtanga ang definition sa evaluateTemperature dinhi!
// Ang tests na lang ang ibilin:

void test_temperature_low(void) {
    TEST_ASSERT_EQUAL(AlarmState::LOW_TEMPERATURE, evaluateTemperature(17.9));
}

void test_temperature_normal(void) {
    TEST_ASSERT_EQUAL(AlarmState::NORMAL, evaluateTemperature(25.0));
}

void test_temperature_high(void) {
    TEST_ASSERT_EQUAL(AlarmState::HIGH_TEMPERATURE, evaluateTemperature(30.1));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_temperature_low);
    RUN_TEST(test_temperature_normal);
    RUN_TEST(test_temperature_high);
    return UNITY_END();
}