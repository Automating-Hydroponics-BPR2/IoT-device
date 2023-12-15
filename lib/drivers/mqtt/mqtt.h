#ifndef MQTT_H_
#define MQTT_H_

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "mqtt_client.h"
/// MQ Broker Credentials, Port and Host
#define MQTT_BROKER_HOSTNAME "3a77f38482f24f338a9040308154fbd5.s2.eu.hivemq.cloud"
#define MQTT_BROKER_PATH "mqtt"
#define MQTT_BROKER_USERNAME "hydroponics"
#define MQTT_BROKER_PASSWORD "Hydroponics123"
#define MQTT_BROKER_PORT 8883

/// Device ID
#define DEVICE_ID "ID-1"

/// MQ topics to send/receive data
#define ENABLE_TOPIC "home/enable"
#define READINGS_TOPIC "home/readings"

/// Logging Tag
#define TAG_MQTT "mqtt"

/// @brief Send the readings in a string format to the MQ Broker
/// @param resource
void publish_readings(char *data);
/// @brief Called if the connection to WiFi was successful
void handle_wifi_connect(void);
/// @brief Called if the connection to WiFi was not successful
void handle_wifi_failed(void);

#endif