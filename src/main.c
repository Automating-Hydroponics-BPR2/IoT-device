#include "application.h"
#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    init_hw();
    xTaskCreate(connect_to_wifi, "wifi", 4096, NULL, 8, NULL);
    // xTaskCreate(connect_to_mqtt, "MQTT", 4096, NULL, 9, NULL);
    xTaskCreate(measure_temperature_humidity, "Temp-Hum", 4096, NULL, 10, NULL);
}