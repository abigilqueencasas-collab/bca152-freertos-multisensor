#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include "motion.h"
#include "system_state.h"

extern "C" void app_main(void) {
    printf("BCA152 FreeRTOS Multisensor\nSystem starting...\n");

    rtos_objects_init();     // KINAHANGLAN UNA NI (mohimo sa event group/mutex)
    motion_init();           // unya ang hardware init

    xTaskCreate(MotionTask, "MotionTask", 3072, nullptr, 3, nullptr);
    xTaskCreate(StateTask,  "StateTask",  3072, nullptr, 2, nullptr);
}