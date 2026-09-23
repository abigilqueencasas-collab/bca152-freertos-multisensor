#include "display.h"
#include "display_mode.h"
#include "oled_ssd1306.h"
#include "rtos_objects.h"
#include "sensors.h"
#include <cstdio>

void display_init() { oled_init(); }

void DisplayTask(void *) {
    printf("--> DisplayTask started\n");
    SensorData d = {};
    bool haveData = false, wasActive = true, lastMotion = false, lastAlarm = false;
    DisplayMode mode = DisplayMode::TEMPERATURE;

    for (;;) {
        bool dirty = false;
        if (xQueueReceive(displayQueue, &d, pdMS_TO_TICKS(200)) == pdTRUE) { haveData = true; dirty = true; }
        DisplayMode m;
        while (xQueueReceive(modeQueue, &m, 0) == pdTRUE) { mode = m; dirty = true; }

        EventBits_t bits = xEventGroupGetBits(systemEvents);
        bool active   = (bits & EVENT_ACTIVE) != 0;
        bool motionOn = (bits & EVENT_MOTION) != 0;
        bool alarmOn  = (bits & EVENT_ALARM) != 0;

        if (!active) {
            if (wasActive) { oled_clear(); oled_flush(); oled_power(false); wasActive = false; }
            continue;
        }
        if (!wasActive) { oled_power(true); wasActive = true; dirty = true; }
        if (motionOn != lastMotion || alarmOn != lastAlarm) dirty = true;
        lastMotion = motionOn; lastAlarm = alarmOn;
        if (!dirty) continue;

        char title[16], value[16];
        switch (mode) {
            case DisplayMode::TEMPERATURE:
                snprintf(title, sizeof title, "Temperature"); snprintf(value, sizeof value, "%.1f C", d.temperature); break;
            case DisplayMode::HUMIDITY:
                snprintf(title, sizeof title, "Humidity");    snprintf(value, sizeof value, "%.1f %%", d.humidity); break;
            case DisplayMode::LIGHT:
                snprintf(title, sizeof title, "Light");       snprintf(value, sizeof value, "%d %%", d.lightLevel); break;
            case DisplayMode::MOTION:
                snprintf(title, sizeof title, "Motion");      snprintf(value, sizeof value, "%s", motionOn ? "DETECTED" : "NONE"); break;
        }
        oled_clear();
        oled_text(0, 0, "ROOM MONITOR", 1);
        oled_text(0, 20, title, 1);
        oled_text(0, 34, (haveData || mode == DisplayMode::MOTION) ? value : "WAITING", 2);
        if (alarmOn) oled_text(0, 56, "ALARM!", 1);
        oled_flush();
    }
}