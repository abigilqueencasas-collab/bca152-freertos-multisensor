#include "rtos_objects.h"
#include "config.h"
#include "sensors.h"
#include "display_mode.h"
#include "esp_log.h"   // <--- GAMITA NI IMBES <cstdio>
#include <cstdarg>

QueueHandle_t displayQueue, alarmQueue, modeQueue;
SemaphoreHandle_t logMutex;
EventGroupHandle_t systemEvents;

void rtos_objects_init() {
    displayQueue = xQueueCreate(5, sizeof(SensorData));
    alarmQueue   = xQueueCreate(5, sizeof(SensorData));
    modeQueue    = xQueueCreate(5, sizeof(DisplayMode));
    logMutex     = xSemaphoreCreateMutex();
    systemEvents = xEventGroupCreate();
    xEventGroupSetBits(systemEvents, EVENT_ACTIVE); 
}

void log_line(const char *tag, const char *fmt, ...) {
    char msg[128];
    va_list ap; 
    va_start(ap, fmt); 
    vsnprintf(msg, sizeof msg, fmt, ap); 
    va_end(ap);
    
    if (logMutex != NULL) {
        xSemaphoreTake(logMutex, portMAX_DELAY);
    }
    
    
    ESP_LOGI(tag, "%s", msg); 
    
    if (logMutex != NULL) {
        xSemaphoreGive(logMutex);
    }
}