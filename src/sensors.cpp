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
    TickType_t lastWake = xTaskGetTickCount();
    for (;;) {
        SensorData d = {};
        int raw = 0;
        adc_oneshot_read(s_adc, ADC_CHANNEL_6, &raw);
        d.lightLevel = raw * 100 / 4095;                       // 0-100 %, DILI lux

        if (dht22_read((gpio_num_t)PIN_DHT22, &d.temperature, &d.humidity) == ESP_OK) {
            d.motionDetected = (xEventGroupGetBits(systemEvents) & EVENT_MOTION) != 0;
            xQueueOverwrite(displayQueue, &d);
            xQueueOverwrite(alarmQueue, &d);
            log_line("Sensor", "Temperature: %.2f C | Humidity: %.2f %% | Light: %d %%",
                     d.temperature, d.humidity, d.lightLevel);
        } else {
            log_line("Sensor", "DHT22 read failed");
        }
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(2000));
    }
}