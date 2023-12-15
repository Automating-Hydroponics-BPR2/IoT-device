#include "dht.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"

#define DHT11_PIN 17

static gpio_num_t dht_gpio;
static struct dht11_reading last_reading;

/// @brief Helper method to determine Timeout error
/// @param microSeconds
/// @param level
/// @return micros_ticks
static int wait_or_timeout(uint16_t microSeconds, int level)
{
    int micros_ticks = 0;
    while (gpio_get_level(dht_gpio) == level)
    {
        if (micros_ticks++ > microSeconds)
            return DHT11_TIMEOUT_ERROR;
        ets_delay_us(1);
    }
    return micros_ticks;
}

/// @brief Checks if there is CRC error
/// @param data[]
/// @return Status
static int check_CRC(uint8_t data[])
{
    if (data[4] == (data[0] + data[1] + data[2] + data[3]))
        return DHT11_OK;
    else
        return DHT11_CRC_ERROR;
}

/// @brief Prepare to read the data from the pin
static void send_start_signal()
{
    gpio_set_direction(dht_gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(dht_gpio, 0);
    ets_delay_us(20 * 1000);
    gpio_set_level(dht_gpio, 1);
    ets_delay_us(40);
    gpio_set_direction(dht_gpio, GPIO_MODE_INPUT);
}

static int check_response()
{
    /// Wait for next step ~80us
    if (wait_or_timeout(80, 0) == DHT11_TIMEOUT_ERROR)
        return DHT11_TIMEOUT_ERROR;

    /// Wait for next step ~80us
    if (wait_or_timeout(80, 1) == DHT11_TIMEOUT_ERROR)
        return DHT11_TIMEOUT_ERROR;

    return DHT11_OK;
}

/// @brief Prepare timeout error struct to be returned
static struct dht11_reading timeout_error()
{
    struct dht11_reading timeoutError = {DHT11_TIMEOUT_ERROR, -1, -1};
    return timeoutError;
}

/// @brief Prepare CRC error struct to be returned
static struct dht11_reading crc_Error()
{
    struct dht11_reading crcError = {DHT11_CRC_ERROR, -1, -1};
    return crcError;
}

void dht11_init()
{
    /// Wait 1 seconds to make the device pass its initial unstable status
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    dht_gpio = DHT11_PIN;
}

struct dht11_reading dht11_read_data()
{
    uint8_t data[5] = {0, 0, 0, 0, 0};

    send_start_signal();

    if (check_response() == DHT11_TIMEOUT_ERROR)
        return last_reading = timeout_error();

    /// Read response
    for (int i = 0; i < 40; i++)
    {
        /// Initial data
        if (wait_or_timeout(50, 0) == DHT11_TIMEOUT_ERROR)
            return last_reading = timeout_error();

        if (wait_or_timeout(70, 1) > 28)
        {
            /// Bit received was a 1
            data[i / 8] |= (1 << (7 - (i % 8)));
        }
    }

    if (check_CRC(data) != DHT11_CRC_ERROR)
    {
        last_reading.status = DHT11_OK;
        last_reading.temperature = data[2] + (float)data[3] * 0.1;
        last_reading.humidity = data[0] + (float)data[1] * 0.1;
        return last_reading;
    }
    else
    {
        last_reading = crc_Error();
        return last_reading;
    }
}