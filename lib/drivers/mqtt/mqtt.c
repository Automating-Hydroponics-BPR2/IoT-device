#include "mqtt.h"
#include <stdlib.h>
#include "../extensions/string_extensions.h"

esp_mqtt_client_handle_t client = NULL;
bool enabled = false;

void publish_reading(float temp, float hum)
{
    char temp_buffer[10];
    char hum_buffer[10];
    ftoa(temp, temp_buffer, 3);
    ftoa(hum, hum_buffer, 3);

    char res[70] = "";
    sprintf(res, "{\"deviceId\": %s, \"temperature\": %s, \"humidity\": %s}", DEVICE_ID, temp_buffer, hum_buffer);
    if (client != NULL)
    {
        esp_mqtt_client_publish(client, TOPIC, res, 0, 2, 0);
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
        esp_mqtt_client_subscribe(client, TOPIC, 2);
        break;
    case MQTT_EVENT_DATA:
        if (!strncmp(event->topic, ENABLE_TOPIC, event->topic_len))
        {
            enabled = event->data[0] - '0';
            ESP_LOGI(TAG, "Enabled");
            printf("Enabled is: %d\n", enabled);
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "errtype: %d", event->error_handle->error_type);
        break;
    default:
        ESP_LOGI(TAG, "event: %d", (int)event_id);
        break;
    }
}

void handle_wifi_connect(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.hostname = MQTT_BROKER_HOSTNAME,
        .broker.address.port = MQTT_BROKER_PORT,
        .broker.address.path = MQTT_BROKER_PATH,
        .broker.address.transport = MQTT_TRANSPORT_OVER_SSL,
        .credentials.username = MQTT_BROKER_USERNAME,
        .credentials.authentication.password = MQTT_BROKER_PASSWORD};
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, handle_mqtt_events, NULL);
    esp_mqtt_client_start(client);
}

void handle_wifi_failed(void)
{
    ESP_LOGE(TAG, "wifi failed");
}
