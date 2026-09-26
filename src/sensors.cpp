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
    SensorData d = {};
    TickType_t lastWakeTime = xTaskGetTickCount();
    printf("[SensorTask] Initialized - sampling every 2 seconds\n");

    for (;;) {
        // --- DHT22 Reading ---
        float t, h;
        esp_err_t dht_status = dht22_read((gpio_num_t)PIN_DHT22, &t, &h);

        if (dht_status == ESP_OK) {
            d.temperature = t;
            d.humidity = h;
            printf("[SensorTask] DHT22 OK | Temperature: %.1f C | Humidity: %.1f %%\n", t, h);
        } else {
            printf("[SensorTask] DHT22 read failed (err %d)\n", dht_status);
        }

        // --- LDR Reading ---
        int raw;
        adc_oneshot_read(s_adc, ADC_CHANNEL_6, &raw);
        d.lightLevel = 100 - ((raw * 100) / 4095);

        // --- Motion status (para kompleto ang SensorData nga gi-publish) ---
        d.motionDetected = (xEventGroupGetBits(systemEvents) & EVENT_MOTION) != 0;

        // --- Publish to Queues ---
        
        xQueueSend(displayQueue, &d, 0);
        xQueueSend(alarmQueue, &d, 0);
        printf("[SensorTask] Published | Temperature: %.1f C | Humidity: %.1f %% | Light: %d %% | Motion: %d\n",
               d.temperature, d.humidity, d.lightLevel, d.motionDetected);

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(2000));
    }
}