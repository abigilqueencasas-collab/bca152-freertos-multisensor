#pragma once
struct SensorData {
    float temperature;
    float humidity;
    int   lightLevel;
    bool  motionDetected;
};
void sensors_init();
void SensorTask(void *pv);