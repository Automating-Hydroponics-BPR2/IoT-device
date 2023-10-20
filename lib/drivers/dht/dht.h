#ifndef DHT11_H_
#define DHT11_H_

#include <stdint.h>
// #include "driver/gpio.h"

typedef enum
{
    DHT11_CRC_ERROR = -2,
    DHT11_TIMEOUT_ERROR,
    DHT11_OK
} DHT11_STATUS;

struct dht11_reading
{
    int status;
    float temperature;
    float humidity;
};

struct dht11_reading dht11_read_data();
void dht11_init();

#endif