#include "alarm.h"
#include "alarm_logic.h"
#include "config.h"
#include "rtos_objects.h"
#include "sensors.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

static void buzzer_set(bool on) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, on ? 512 : 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void alarm_init() {
    ledc_timer_config_t t = {};
    t.speed_mode = LEDC_LOW_SPEED_MODE;
    t.duty_resolution = LEDC_TIMER_10_BIT;
    t.timer_num = LEDC_TIMER_0;
    t.freq_hz = 2000;
    t.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&t);

    ledc_channel_config_t c = {};
    c.gpio_num = (gpio_num_t)PIN_BUZZER;
    c.speed_mode = LEDC_LOW_SPEED_MODE;
    c.channel = LEDC_CHANNEL_0;
    c.timer_sel = LEDC_TIMER_0;
    c.duty = 0;
    c.hpoint = 0;
    ledc_channel_config(&c);
}

void AlarmTask(void *) {
    SensorData d = {};
    bool haveData = false, buzzing = false;
    for (;;) {
        if (xQueueReceive(alarmQueue, &d, pdMS_TO_TICKS(200)) == pdTRUE) haveData = true;
        if (!haveData) continue;

        AlarmState st = evaluateTemperature(d.temperature);
        bool active = (xEventGroupGetBits(systemEvents) & EVENT_ACTIVE) != 0;

        if (st != AlarmState::NORMAL) xEventGroupSetBits(systemEvents, EVENT_ALARM);
        else                          xEventGroupClearBits(systemEvents, EVENT_ALARM);

        bool want = (st != AlarmState::NORMAL) && active;
        if (want != buzzing) {
            buzzing = want;
            buzzer_set(buzzing);
            log_line("Alarm", "Buzzer %s (state=%d)", buzzing ? "ON" : "OFF", (int)st);
        }
    }
}