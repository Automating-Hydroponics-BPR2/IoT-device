#include "dht/dht.h"
#include <stdlib.h>
#include <stdio.h>

void app_main()
{
    dht11_init();
    struct dht11_reading reading = dht11_read_data();
    printf("Status: %d\n", reading.status);
    printf("Humidity: %f\n", reading.humidity);
    printf("Temperature: %f\n", reading.temperature);
}