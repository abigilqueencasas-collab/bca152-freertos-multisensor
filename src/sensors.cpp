#include "sensors.h"
#include "config.h"
#include "dht22.h"
#include "rtos_objects.h"
#include "esp_adc/adc_oneshot.h"

static adc_oneshot_unit_handle_t s_adc;

void sensors_init() {
    dht22_init((gpio_num_t)PIN_DHT22);
    adc_oneshot_unit_init_cfg_t u = {};
    u.unit_id = ADC_UNIT_1;
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&u, &s_adc));
    adc_oneshot_chan_cfg_t c = {};
    c.atten = ADC_ATTEN_DB_12;
    c.bitwidth = ADC_BITWIDTH_12;
    ESP_ERROR_CHECK(adc_oneshot_config_channel(s_adc, ADC_CHANNEL_6, &c));   // GPIO34
}

void SensorTask(void *) { 
    printf("--> SensorTask: Started\n");
    SensorData d = {};
    TickType_t lastWakeTime = xTaskGetTickCount();
    
    for (;;) {
        printf("--> SensorTask: Reading DHT22...\n");
        float t, h;
        if (dht22_read((gpio_num_t)PIN_DHT22, &t, &h) == ESP_OK) {
            d.temperature = t;
            d.humidity = h;
        }
        
        printf("--> SensorTask: Reading LDR...\n");
        int raw;
        adc_oneshot_read(s_adc, ADC_CHANNEL_6, &raw);
        d.lightLevel = (raw * 100) / 4095; // Convert to 0-100%
        
        printf("--> SensorTask: Sending to Queue...\n");
        xQueueSend(displayQueue, &d, 0);
        xQueueSend(alarmQueue, &d, 0);
        
        printf("--> SensorTask: Delaying...\n");
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(2000));
    }
}