#ifndef MQTT_H_
#define MQTT_H_

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "mqtt_client.h"

#ifndef MQTT_BROKER_URL
#define MQTT_BROKER_URL "3a77f38482f24f338a9040308154fbd5.s2.eu.hivemq.cloud:8883/mqtt"
#endif

#define SENSOR_NO "1"
#define ENABLE_TOPIC "home/" SENSOR_NO "/enable"
#define TEMP_TOPIC "home/temperature/" SENSOR_NO
#define HUM_TOPIC "home/humidity/" SENSOR_NO
#define TAG "app"

/*void publish_reading(int temp, int hum);

void handle_mqtt_events(void *handler_args,
                        esp_event_base_t base,
                        int32_t event_id,
                        void *event_data);*/
void publish_reading(int temp, int hum);
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