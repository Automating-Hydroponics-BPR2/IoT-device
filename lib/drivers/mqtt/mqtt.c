#include "mqtt.h"
#include <stdlib.h>

esp_mqtt_client_handle_t client = NULL;
bool enabled = false;
void publish_reading(int temp, int hum)
{
    char buffer[5];

    if (client != NULL)
    {
        esp_mqtt_client_publish(client, TEMP_TOPIC, itoa(temp, buffer, 10), 0, 2, 0);
        esp_mqtt_client_publish(client, HUM_TOPIC, itoa(hum, buffer, 10), 0, 2, 0);
    }
}

void handle_mqtt_events(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "mqtt broker connected");
        esp_mqtt_client_subscribe(client, ENABLE_TOPIC, 2);
        esp_mqtt_client_subscribe(client, TEMP_TOPIC, 2);
        break;
    case MQTT_EVENT_DATA:
        if (!strncmp(event->topic, ENABLE_TOPIC, event->topic_len))
        {
            enabled = event->data[0] - '0';
            ESP_LOGI(TAG, "Enabled");
            printf("Enabled is: %d\n", enabled);
            // ESP_LOGI("data: %d", event->data[0]);
        }
        else if (!strncmp(event->topic, TEMP_TOPIC, event->topic_len))
        {
            ESP_LOGI(TAG, "TEMP");
            float temperature = atof(event->data);
            printf("Temp is: %f\n", temperature);
            // ESP_LOGI("data: %d", event->data[0]);
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "errtype: %d", event->error_handle->error_type);
        break;
    default:
        // ESP_LOGI(TAG, "event: %d", event_id);
        break;
    }
}

void handle_wifi_connect(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {

        //.broker.address.uri = "mqtt://3a77f38482f24f338a9040308154fbd5.s2.eu.hivemq.cloud",
        .broker.address.hostname = "3a77f38482f24f338a9040308154fbd5.s2.eu.hivemq.cloud",
        .broker.address.port = 8883,
        .broker.address.path = "mqtt",
        .broker.address.transport = MQTT_TRANSPORT_OVER_SSL,
        .credentials.username = "hydroponics",
        .credentials.authentication.password = "Hydroponics123",
        .credentials.client_id = "client-1"};
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, handle_mqtt_events, NULL);
    esp_mqtt_client_start(client);
    // apptemp_init(publish_reading);
}

void handle_wifi_failed(void)
{
    ESP_LOGE(TAG, "wifi failed");
}

/*void app_main()
{
    esp_event_loop_create_default();

    connect_wifi_params_t cbs = {
        .on_connected = handle_wifi_connect,
        .on_failed = handle_wifi_failed};
    appwifi_connect(cbs);
}*/