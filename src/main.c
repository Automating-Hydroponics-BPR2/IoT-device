#include "application.h"
#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    init_hw();
    xTaskCreate(connect_to_wifi, "wifi", 4096, NULL, 1, NULL);
    xTaskCreate(send_data, "Publish data", 4096, NULL, 2, NULL);
    xTaskCreate(measure_temperature_humidity, "Temp-Hum", 4096, NULL, 3, NULL);
}