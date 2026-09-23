#include "rtos_objects.h"
#include "sensors.h"
#include "display_mode.h"
#include <cstdarg>
#include <cstdio>

QueueHandle_t displayQueue, alarmQueue, modeQueue;
SemaphoreHandle_t logMutex;
EventGroupHandle_t systemEvents;

void rtos_objects_init() {
    displayQueue = xQueueCreate(1, sizeof(SensorData));
    alarmQueue   = xQueueCreate(1, sizeof(SensorData));
    modeQueue    = xQueueCreate(8, sizeof(DisplayMode));
    logMutex     = xSemaphoreCreateMutex();
    systemEvents = xEventGroupCreate();
    xEventGroupSetBits(systemEvents, EVENT_ACTIVE);
}

void log_line(const char *tag, const char *fmt, ...) {
    char msg[128];
    va_list ap; va_start(ap, fmt); vsnprintf(msg, sizeof msg, fmt, ap); va_end(ap);
    xSemaphoreTake(logMutex, portMAX_DELAY);
    fputs("[", stdout); fputs(tag, stdout); fputs("] ", stdout);
    fputs(msg, stdout); fputs("\n", stdout);
    xSemaphoreGive(logMutex);
}