#ifndef DHT11_H_
#define DHT11_H_

#include <stdint.h>

typedef enum
{
    /// @brief This status is returned if the data is corrupted.
    DHT11_CRC_ERROR = -2,
    /// @brief This status is returned when the sensor fails to read the data.
    DHT11_TIMEOUT_ERROR,
    /// @brief This status is returned when everything is OK.
    DHT11_OK
} DHT11_STATUS;

struct dht11_reading
{
    /// @brief The status that was returned.
    /// @param DHT11_STATUS
    int status;
    float temperature;
    float humidity;
};

/// @brief Reads data from DHT11 sensor and returns dht11_reading
/// @return dht11_reading
struct dht11_reading dht11_read_data();

/// @brief Initializes the DHT11 sensor
void dht11_init();

#endif