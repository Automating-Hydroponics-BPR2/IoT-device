#ifndef WIFI_DRIVER_H_
#define WIFI_DRIVER_H_

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_http_server.h"
#include "esp_http_client.h"
#include "freertos/event_groups.h"

#define WIFI_SSID_AP "esp32_ap1"
#define WIFI_PWD "A_pwd_is_needed_here"
#define WIFI_SSID "str"
#define WIFI_PASS "str"
#define WIFI_CHANNEL 11
#define MAX_CONN_CNT 1
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
#define MAX_RETRY 10

enum wifi_mode
{
    AP = 0,
    STA = 1
};

void start_webserver(void);
esp_err_t handle_http_get(httpd_req_t *req);
esp_err_t handle_http_post(httpd_req_t *req);
void handle_wifi_events(void *, esp_event_base_t, int32_t, void *);
void handle_wifi_connection(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void init_wifi(int mode);
void init_wifi_ap();

#endif