#include "dht/dht.h"
#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi/wifi.h"

struct dht11_reading dht11;
SemaphoreHandle_t xSemaphore = NULL;

void init_hw()
{
    dht11_init();
}

void connect_to_wifi(void *arg)
{
    init_wifi();
}

void measure_temperature_humidity(void *arg)
{
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
        dht11 = dht11_read_data();
        printf("Status: %d\n", dht11.status);
        printf("Humidity: %f\n", dht11.humidity);
        printf("Temperature: %f\n", dht11.temperature);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}