#include "dht/dht.h"
#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi/wifi.h"
#include "mqtt/mqtt.h"
#include "esp_log.h"
#include "string_extensions.h"

struct dht11_reading dht11;
SemaphoreHandle_t xSemaphore = NULL;

static void prepare_data(char *data)
{
    char temp_buffer[10];
    char hum_buffer[10];

    float_to_string(dht11.temperature, temp_buffer, 3);
    float_to_string(dht11.humidity, hum_buffer, 3);

    sprintf(data, "{\"deviceId\": %s, \"temperature\": %s, \"humidity\": %s}", DEVICE_ID, temp_buffer, hum_buffer);
}

void init_hw()
{
    dht11_init();
}

void connect_to_wifi(void *arg)
{
    esp_event_loop_create_default();

    connect_wifi_params_t params = {
        .on_connected = handle_wifi_connect,
        .on_failed = handle_wifi_failed};
    init_wifi(params);
}

void measure_temperature_humidity(void *arg)
{
    xSemaphore = xSemaphoreCreateMutex();
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
        if (xSemaphore != NULL)
        {
            if (xSemaphoreGive(xSemaphore) != pdTRUE)
            {
            }
            if (xSemaphoreTake(xSemaphore, (TickType_t)0))
            {
                dht11 = dht11_read_data();
                ESP_LOGI("DHT11", "Data read");
                xSemaphoreGive(xSemaphore);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50000));
    }
}

void send_data(void *arg)
{
    while (1)
    {
        if (xSemaphore != NULL)
        {
            if (xSemaphoreGive(xSemaphore) != pdTRUE)
            {
            }
            if (xSemaphoreTake(xSemaphore, (TickType_t)0))
            {
                char data[70] = "";
                prepare_data(data);
                publish_readings(data);
                ESP_LOGI(TAG_MQTT, "Data sent");
                xSemaphoreGive(xSemaphore);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(300000));
    }
}