#include "dht22.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"

static portMUX_TYPE s_mux = portMUX_INITIALIZER_UNLOCKED;

// hulat samtang ang pin naa pa sa "level"; balik ang gidugayon (us) o -1 kung timeout
static int wait_while(gpio_num_t pin, int level, int timeout_us) {
    int64_t start = esp_timer_get_time();
    while (gpio_get_level(pin) == level)
        if (esp_timer_get_time() - start > timeout_us) return -1;
    return (int)(esp_timer_get_time() - start);
}

void dht22_init(gpio_num_t pin) {
    gpio_config_t io = {};
    io.pin_bit_mask = 1ULL << pin;
    io.mode = GPIO_MODE_INPUT_OUTPUT_OD;   // open-drain: pwede mo-read ug mo-write
    io.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io);
    gpio_set_level(pin, 1);
}

esp_err_t dht22_read(gpio_num_t pin, float *temp, float *hum) {
    uint8_t d[5] = {0};
    gpio_set_level(pin, 0);
    esp_rom_delay_us(2000);                // start signal: low >= 1 ms
    esp_err_t err = ESP_OK;

    // TANGGANGA ANG portENTER_CRITICAL DINHI
    
    gpio_set_level(pin, 1);
    if (wait_while(pin, 1, 100) < 0 || wait_while(pin, 0, 120) < 0 || wait_while(pin, 1, 120) < 0)
        err = ESP_ERR_TIMEOUT;
    for (int i = 0; i < 40 && err == ESP_OK; i++) {
        if (wait_while(pin, 0, 100) < 0) { err = ESP_ERR_TIMEOUT; break; }
        int high = wait_while(pin, 1, 120);
        if (high < 0) { err = ESP_ERR_TIMEOUT; break; }
        d[i / 8] <<= 1;
        if (high > 40) d[i / 8] |= 1;      // >40us = bit 1
    }
    
    // TANGGANGA ANG portEXIT_CRITICAL DINHI

    if (err != ESP_OK) return err;
    if (((d[0] + d[1] + d[2] + d[3]) & 0xFF) != d[4]) return ESP_ERR_INVALID_CRC;
    *hum = ((d[0] << 8) | d[1]) / 10.0f;
    float t = (((d[2] & 0x7F) << 8) | d[3]) / 10.0f;
    *temp = (d[2] & 0x80) ? -t : t;
    return ESP_OK;
}