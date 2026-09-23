#include "input.h"
#include "config.h"
#include "display_mode.h"
#include "rtos_objects.h"
#include "driver/gpio.h"

void input_init() {
    gpio_config_t io = {};
    io.pin_bit_mask = (1ULL << PIN_ENC_CLK) | (1ULL << PIN_ENC_DT);
    io.mode = GPIO_MODE_INPUT;
    io.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io);
}

void InputTask(void *) {
    printf("--> InputTask started\n");
    TickType_t lastWake = xTaskGetTickCount();
    DisplayMode mode = DisplayMode::TEMPERATURE;
    int prevClk = gpio_get_level((gpio_num_t)PIN_ENC_CLK);
    for (;;) {
        int clk = gpio_get_level((gpio_num_t)PIN_ENC_CLK);
        if (prevClk == 1 && clk == 0) {
            if (xEventGroupGetBits(systemEvents) & EVENT_ACTIVE) {
                bool cw = gpio_get_level((gpio_num_t)PIN_ENC_DT) == 1;
                mode = cw ? nextDisplayMode(mode) : previousDisplayMode(mode);
                xQueueSend(modeQueue, &mode, 0);
                log_line("Input", "Mode -> %d", (int)mode);
            }
        }
        prevClk = clk;
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(2));
    }
}