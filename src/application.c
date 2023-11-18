#include "dht/dht.h"
#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi/wifi.h"
#include "mqtt/mqtt.h"

struct dht11_reading dht11;
SemaphoreHandle_t xSemaphore = NULL;

void init_hw()
{
    dht11_init();
}

void connect_to_wifi(void *arg)
{
    esp_event_loop_create_default();

    connect_wifi_params_t cbs = {
        .on_connected = handle_wifi_connect,
        .on_failed = handle_wifi_failed};
    init_wifi(cbs);
}

void measure_temperature_humidity(void *arg)
{
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
        dht11 = dht11_read_data();
        // printf("Status: %d\n", dht11.status);
        // printf("Humidity: %f\n", dht11.humidity);
        // printf("Temperature: %f\n", dht11.temperature);
        publish_reading(dht11.temperature, dht11.humidity);
        vTaskDelay(pdMS_TO_TICKS(50000));
    }
}