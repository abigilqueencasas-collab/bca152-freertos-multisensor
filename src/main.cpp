#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include "motion.h"
#include "system_state.h"
#include "sensors.h"
#include "display.h"
#include "input.h"
#include "alarm.h"

extern "C" void app_main(void) {
    printf("BCA152 FreeRTOS Multisensor\nSystem starting...\n");

    printf("1. Starting init...\n");
    rtos_objects_init();
    printf("2. RTOS objects initialized\n");
    
    motion_init();
    printf("3. Motion initialized\n");
    
    sensors_init();
    printf("4. Sensors initialized\n");
    
    display_init();
    printf("5. Display initialized\n");
    
    input_init();
    printf("6. Input initialized\n");
    
    alarm_init();
    printf("7. Alarm initialized\n");

    xTaskCreate(MotionTask,  "MotionTask",  8192, nullptr, 3, nullptr);
    xTaskCreate(InputTask,   "InputTask",   8192, nullptr, 3, nullptr);
    xTaskCreate(StateTask,   "StateTask",   8192, nullptr, 2, nullptr);
    xTaskCreate(SensorTask, "SensorTask", 12288, nullptr, 2, nullptr);    
    xTaskCreate(AlarmTask,   "AlarmTask",   8192, nullptr, 2, nullptr);
    xTaskCreate(DisplayTask, "DisplayTask", 8192, nullptr, 1, nullptr); 
    
    printf("8. All tasks created! System running.\n");
}