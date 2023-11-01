#ifndef WIFI_DRIVER_H_
#define WIFI_DRIVER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#define MAX_RETRY 10

#define WIFI_SSID "Nokia 9"
#define WIFI_PASS "ec01a18d27fc"

void init_wifi();

#endif