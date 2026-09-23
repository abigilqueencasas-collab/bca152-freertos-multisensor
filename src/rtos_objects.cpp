#include "rtos_objects.h"
#include "config.h"
#include "sensors.h"
#include "display_mode.h"
#include <cstdio>
#include <cstdarg>

QueueHandle_t displayQueue, alarmQueue, modeQueue;
SemaphoreHandle_t logMutex;
EventGroupHandle_t systemEvents;

void rtos_objects_init() {
    // 1. I-initialize ang mga Queues
    displayQueue = xQueueCreate(5, sizeof(SensorData));
    alarmQueue   = xQueueCreate(5, sizeof(SensorData));
    modeQueue    = xQueueCreate(5, sizeof(DisplayMode));

    // 2. I-initialize ang Mutex
    logMutex = xSemaphoreCreateMutex();

    // 3. I-initialize ang Event Group ug i-set dayon ang ACTIVE bit
    systemEvents = xEventGroupCreate();
    xEventGroupSetBits(systemEvents, EVENT_ACTIVE); 
}

void log_line(const char *tag, const char *fmt, ...) {
    char msg[128];
    va_list ap; va_start(ap, fmt); vsnprintf(msg, sizeof msg, fmt, ap); va_end(ap);
    
    // Siguraduha nga dili NULL ang logMutex bag-o mo-take
    if (logMutex != NULL) {
        xSemaphoreTake(logMutex, portMAX_DELAY);
    }
    
    fputs("[", stdout); fputs(tag, stdout); fputs("] ", stdout);
    fputs(msg, stdout); fputs("\n", stdout);
    
    if (logMutex != NULL) {
        xSemaphoreGive(logMutex);
    }
}