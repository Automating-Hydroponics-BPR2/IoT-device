#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Wifi/Wifi_driver.h"

void app_main()
{
    init_wifi_ap();
}