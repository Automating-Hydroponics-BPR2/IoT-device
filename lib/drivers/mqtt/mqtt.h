#ifndef MQTT_H_
#define MQTT_H_

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "mqtt_client.h"

#define MQTT_BROKER_HOSTNAME "3a77f38482f24f338a9040308154fbd5.s2.eu.hivemq.cloud"
#define MQTT_BROKER_PATH "mqtt"
#define MQTT_BROKER_USERNAME "hydroponics"
#define MQTT_BROKER_PASSWORD "Hydroponics123"
#define MQTT_BROKER_PORT 8883
#define DEVICE_ID "ID-1"

#define ENABLE_TOPIC "home/enable"
#define TOPIC "home/readings"
#define TAG "app"

/*void publish_reading(int temp, int hum);

void handle_mqtt_events(void *handler_args,
                        esp_event_base_t base,
                        int32_t event_id,
                        void *event_data);*/

void publish_reading(float temp, float hum);
void handle_wifi_connect(void);

void handle_wifi_failed(void);

/*void app_main()
{
    esp_event_loop_create_default();

    connect_wifi_params_t cbs = {
        .on_connected = handle_wifi_connect,
        .on_failed = handle_wifi_failed};
    appwifi_connect(cbs);
}*/

#endif