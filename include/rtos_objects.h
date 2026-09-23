#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#define EVENT_ACTIVE (1U << 0)
#define EVENT_MOTION (1U << 1)
#define EVENT_ALARM  (1U << 2)

extern QueueHandle_t displayQueue, alarmQueue, modeQueue;
extern SemaphoreHandle_t logMutex;
extern EventGroupHandle_t systemEvents;

void rtos_objects_init();
void log_line(const char *tag, const char *fmt, ...);