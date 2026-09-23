#pragma once
struct SensorData {
    float temperature;
    float humidity;
    int   lightLevel;      // 0-100 %
    bool  motionDetected;
};
void sensors_init();
void SensorTask(void *pv);