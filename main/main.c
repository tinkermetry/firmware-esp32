/*
 * Copyright (c) 2017 Mika Tuupola
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <esp_log.h>
#include <driver/i2c.h>

#include "sdkconfig.h"
#include "unwindoze.h"
#include "spektrum.h"
#include "byteswap.h"
#include "hal/i2c.h"
#include "hal/i2c_esp32.h"     /* This should be conditional. */

#define I2C_SLAVE_0_SCL        19 /* yellow */
#define I2C_SLAVE_0_SDA        18 /* white!! green */
#define I2C_SLAVE_1_SCL        26 /* yellow */
#define I2C_SLAVE_1_SDA        25 /* white!! green */

static const char* TAG = "main";

void airspeed_task(void *params)
{
    spektrum_tele_speed_t airspeed;
    airspeed.identifier = SPEKTRUM_AIRSPEED;

    /* Spektrum telemetry message is always 16 bytes even when actual data */
    /* size might be smaller. Initialize extra bytes as null. */
    uint8_t buffer[SPEKTRUM_DATA_LENGTH] = { 0 };
    memcpy(buffer, &airspeed, sizeof(airspeed));

    uint16_t current_speed = 0;
    uint16_t maximum_speed = 0;

    while(1) {
        size_t data_size = hal_i2c_slave_write(
            I2C_SLAVE_0_PORT,
            buffer,
            SPEKTRUM_DATA_LENGTH
        );

        /* Stops logging if buffer is full. */
        if (0 != data_size) {
            ESP_LOG_BUFFER_HEXDUMP(TAG, (uint8_t *) buffer, data_size, ESP_LOG_DEBUG);
        }

        current_speed += 1;
        maximum_speed = 100;

        /* Spektrum expects big-endian / MSB first data. */
        airspeed.airspeed = bswap_16(current_speed);
        airspeed.maxAirspeed =  bswap_16(maximum_speed);
        memcpy(buffer, &airspeed, sizeof(airspeed));

        vTaskDelay(500 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

void altitude_task(void *params)
{
    spektrum_tele_altitude_t altitude;
    altitude.identifier = SPEKTRUM_ALTITUDE;

    /* Spektrum telemetry message is always 16 bytes even when actual data */
    /* size might be smaller. Initialize extra bytes as null. */
    uint8_t buffer[SPEKTRUM_DATA_LENGTH] = { 0 };
    memcpy(buffer, &altitude, sizeof(altitude));

    uint16_t current_altitude = 0;
    uint16_t maximum_altitude = 0;

    while(1) {
        size_t data_size = hal_i2c_slave_write(
            I2C_SLAVE_1_PORT,
            buffer,
            SPEKTRUM_DATA_LENGTH
        );

        /* Stops logging if buffer is full. */
        if (0 != data_size) {
            ESP_LOG_BUFFER_HEXDUMP(TAG, (uint8_t *) buffer, data_size, ESP_LOG_DEBUG);
        }

        current_altitude += 1;
        maximum_altitude = 120;

        /* Spektrum expects big-endian / MSB first data. */
        altitude.altitude = bswap_16(current_altitude);
        altitude.maxAltitude = bswap_16(maximum_altitude);
        memcpy(buffer, &altitude, sizeof(altitude));

        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

void app_main()
{
    hal_i2c_slave_config_t i2c_config_0;
    hal_i2c_slave_config_t i2c_config_1;

    i2c_config_0.port = I2C_SLAVE_0_PORT;
    i2c_config_0.address = SPEKTRUM_AIRSPEED;
    i2c_config_0.scl_io_number = I2C_SLAVE_0_SCL;
    i2c_config_0.sda_io_number = I2C_SLAVE_0_SDA;

    i2c_config_1.port = I2C_SLAVE_1_PORT;
    i2c_config_1.address = SPEKTRUM_ALTITUDE;
    i2c_config_1.scl_io_number = I2C_SLAVE_1_SCL;
    i2c_config_1.sda_io_number = I2C_SLAVE_1_SDA;

    hal_i2c_slave_init(&i2c_config_0);
    hal_i2c_slave_init(&i2c_config_1);

    xTaskCreatePinnedToCore(airspeed_task, "Airspeed", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(altitude_task, "Altitude", 2048, NULL, 1, NULL, 1);
}