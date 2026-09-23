#include "system_state.h"
#include "state_logic.h"
#include "config.h"
#include "rtos_objects.h"
#include "esp_timer.h"

static uint32_t now_ms() { return (uint32_t)(esp_timer_get_time() / 1000); }

void StateTask(void *) {
    TickType_t lastWake = xTaskGetTickCount();
    SystemState state = SystemState::ACTIVE;
    uint32_t lastMotion = now_ms();
    for (;;) {
        bool motion = (xEventGroupGetBits(systemEvents) & EVENT_MOTION) != 0;
        if (motion) lastMotion = now_ms();

        SystemState next = evaluateSystemState(state, motion, now_ms() - lastMotion, INACTIVITY_TIMEOUT_MS);
        if (next != state) {
            state = next;
            if (state == SystemState::ACTIVE) xEventGroupSetBits(systemEvents, EVENT_ACTIVE);
            else                              xEventGroupClearBits(systemEvents, EVENT_ACTIVE);
            log_line("State", "-> %s", state == SystemState::ACTIVE ? "ACTIVE" : "INACTIVE");
        }
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(100));
    }
}