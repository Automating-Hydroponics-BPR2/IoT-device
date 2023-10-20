#include "Wifi_driver.h"
#include "string.h"

static const char *TAG = "ap-app";

static const char *HTML_FORM = "<html><form action=\"/\" method=\"post\">"
                               "<label for=\"ssid\">Local SSID:</label><br>"
                               "<input type=\"text\" id=\"ssid\" name=\"ssid\"><br>"
                               "<label for=\"pwd\">Password:</label><br>"
                               "<input type=\"text\" id=\"pwd\" name=\"pwd\"><br>"
                               "<input type=\"submit\" value=\"Submit\">"
                               "</form></html>";

static int retry_cnt = 0;
EventGroupHandle_t wifi_events;

// AP Mode
void init_wifi_ap()
{
    if (nvs_flash_init() != ESP_OK)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }

    esp_event_loop_create_default();
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &handle_wifi_events, NULL);

    esp_netif_init();
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID_AP,
            .ssid_len = strlen(WIFI_SSID_AP),
            .channel = WIFI_CHANNEL,
            .password = WIFI_PWD,
            .max_connection = MAX_CONN_CNT,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK},
    };
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);
    esp_wifi_start();

    start_webserver();
}

void start_webserver(void)
{
    httpd_uri_t uri_get = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = handle_http_get,
        .user_ctx = NULL};

    httpd_uri_t uri_post = {
        .uri = "/",
        .method = HTTP_POST,
        .handler = handle_http_post,
        .user_ctx = NULL};

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_post);
    }
}

esp_err_t handle_http_get(httpd_req_t *req)
{
    return httpd_resp_send(req, HTML_FORM, HTTPD_RESP_USE_STRLEN);
}

esp_err_t handle_http_post(httpd_req_t *req)
{
    char content[100];
    if (httpd_req_recv(req, content, req->content_len) <= 0)
    {
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "%.*s", req->content_len, content);
    char *andIndex = strchr(content, '&');
    int index = (int)(andIndex - content);
    char ssid[20], pass[20];
    strncpy(ssid, content + 5, index - 5);
    strncpy(pass, content + index + 5, strlen(content) - index);
    for (int i = 0; i < strlen(ssid); i++)
    {
        if (ssid[i] == '+')
        {
            ssid[i] = ' ';
        }
    }
    ESP_LOGI(TAG, "%s, %s", ssid, pass);

#undef WIFI_SSID
#undef WIFI_PASS
#define WIFI_SSID ssid
#define WIFI_PASS pass

    ESP_LOGI(TAG, "%s, %s", WIFI_SSID, WIFI_PASS);
    init_wifi_sta(ssid, pass);
    return httpd_resp_send(req, "received", HTTPD_RESP_USE_STRLEN);
}

// STA Mode
void init_wifi_sta(char ssid[20], char pass[20])
{
    if (nvs_flash_init() != ESP_OK)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }
    wifi_events = xEventGroupCreate();
    esp_event_loop_create_default();
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &handle_wifi_connection, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &handle_wifi_connection, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };
    esp_netif_init();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
    ESP_LOGI(TAG, "Connected to wifi: %s", ssid);
}

void handle_wifi_events(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        ESP_LOGI(TAG, "a station connected");
    }
}

void handle_wifi_connection(void *arg, esp_event_base_t event_base,
                            int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (retry_cnt++ < MAX_RETRY)
        {
            esp_wifi_connect();
            ESP_LOGI(TAG, "wifi connect retry: %d", retry_cnt);
        }
        else
        {
            xEventGroupSetBits(wifi_events, WIFI_FAIL_BIT);
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "ip: %d.%d.%d.%d", IP2STR(&event->ip_info.ip));
        retry_cnt = 0;
        xEventGroupSetBits(wifi_events, WIFI_CONNECTED_BIT);
    }
}