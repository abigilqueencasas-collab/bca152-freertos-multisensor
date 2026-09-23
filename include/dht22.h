#pragma once
#include "driver/gpio.h"
#include "esp_err.h"
void dht22_init(gpio_num_t pin);
esp_err_t dht22_read(gpio_num_t pin, float *temp, float *hum);