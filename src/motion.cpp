#include "motion.h"
#include "config.h"
#include "rtos_objects.h"
#include "driver/gpio.h"

void motion_init() {
    gpio_config_t io = {};
    io.pin_bit_mask = 1ULL << PIN_PIR;
    io.mode = GPIO_MODE_INPUT;
    io.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io);
}

void MotionTask(void *) {
    printf("--> MotionTask started\n");
    TickType_t lastWake = xTaskGetTickCount();
    bool prev = false;
    for (;;) {
        bool m = gpio_get_level((gpio_num_t)PIN_PIR) == 1;
        if (m != prev) {
            if (m) xEventGroupSetBits(systemEvents, EVENT_MOTION);
            else   xEventGroupClearBits(systemEvents, EVENT_MOTION);
            log_line("Motion", "PIR %s", m ? "HIGH" : "LOW");
        }
        prev = m;
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(50));
    }
}