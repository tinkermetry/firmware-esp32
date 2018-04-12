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

//#include <freertos/FreeRTOS.h>
//#include <freertos/task.h>

#include "sdkconfig.h"
#include "unwindoze.h"
#include "spektrum.h"
#include "i2c.h"

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
        size_t data_size = i2c_slave_write_buffer(
            I2C_SLAVE_NUM,
            buffer,
            SPEKTRUM_DATA_LENGTH,
            500 / portTICK_RATE_MS
        );

        /* Stops logging if buffer is full. */
        if (0 != data_size) {
            ESP_LOG_BUFFER_HEXDUMP(TAG, (uint8_t *) buffer, data_size, ESP_LOG_DEBUG);
        }

        current_speed += 1;
        maximum_speed = 100;

        /* Spektrum expects big-endian / MSB first data. */
        airspeed.airspeed = (current_speed >> 8) | (current_speed << 8);
        airspeed.maxAirspeed = (maximum_speed >> 8) | (maximum_speed << 8);
        memcpy(buffer, &airspeed, sizeof(airspeed));

        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

void i2c_read_task(void *params)
{
    uint8_t buffer[SPEKTRUM_DATA_LENGTH] = { 0 };

    ESP_LOGD(TAG, "Allocated %d bytes for reading.", SPEKTRUM_DATA_LENGTH);
    ESP_LOG_BUFFER_HEXDUMP(TAG, buffer, SPEKTRUM_DATA_LENGTH, ESP_LOG_DEBUG);

    while(1) {
        ESP_LOGD(TAG, "Reading from I2C slave.");
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        ESP_ERROR_CHECK(i2c_master_start(cmd));
        ESP_ERROR_CHECK(
            i2c_master_write_byte(
                cmd,
                (SPEKTRUM_AIRSPEED << 1) | I2C_MASTER_READ,
                ACK_CHECK_ENABLE
            )
        );
        if (SPEKTRUM_DATA_LENGTH > 1) {
            ESP_ERROR_CHECK(
                i2c_master_read(cmd, buffer, SPEKTRUM_DATA_LENGTH - 1, ACK_VAL)
            );
        }
        ESP_ERROR_CHECK(
            i2c_master_read_byte(cmd, buffer + SPEKTRUM_DATA_LENGTH - 1, NACK_VAL)
        );
        ESP_ERROR_CHECK(i2c_master_stop(cmd));
        ESP_ERROR_CHECK(
            i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS)
        );
        i2c_cmd_link_delete(cmd);
        ESP_LOG_BUFFER_HEXDUMP(TAG, buffer, SPEKTRUM_DATA_LENGTH, ESP_LOG_DEBUG);

        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

/* ESP32 application entry point */
void app_main()
{
    i2c_slave_init();
    // i2c_master_init();
    // i2c_master_scan();

    xTaskCreatePinnedToCore(airspeed_task, "Airspeed", 2048, NULL, 1, NULL, 1);
//    xTaskCreatePinnedToCore(i2c_read_task, "I2C read", 2048, NULL, 1, NULL, 1);
}