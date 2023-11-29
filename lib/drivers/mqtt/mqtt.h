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
#define TAG_MQTT "mqtt"

void publish_reading(char *res);
void handle_wifi_connect(void);
void handle_wifi_failed(void);

#endif