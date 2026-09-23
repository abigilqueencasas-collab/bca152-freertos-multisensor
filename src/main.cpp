#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include "motion.h"
#include "system_state.h"
#include "sensors.h"
#include "display.h"

extern "C" void app_main(void) {
    printf("BCA152 FreeRTOS Multisensor\nSystem starting...\n");

    rtos_objects_init();
    motion_init();
    sensors_init();
    display_init();

    xTaskCreate(MotionTask,  "MotionTask",  3072, nullptr, 3, nullptr);
    xTaskCreate(StateTask,   "StateTask",   3072, nullptr, 2, nullptr);
    xTaskCreate(SensorTask,  "SensorTask",  4096, nullptr, 2, nullptr);
    xTaskCreate(DisplayTask, "DisplayTask", 4096, nullptr, 1, nullptr);
}