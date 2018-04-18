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

#include <stdint.h>
#include <driver/i2c.h>
#include <esp_log.h>

#include "../unwindoze.h"
#include "i2c_hal.h"
#include "i2c_esp32.h"

static const char* TAG = "i2c_esp32";

void hal_esp32_i2c_slave_init(const hal_i2c_slave_config_t *i2c_slave_config)
{
    ESP_LOGD(TAG, "Starting I2C slave at port %d.", i2c_slave_config->number);

    i2c_config_t esp32_i2c_config;
    esp32_i2c_config.sda_io_num = i2c_slave_config->sda_io_number;
    esp32_i2c_config.sda_pullup_en = GPIO_PULLUP_DISABLE;
    esp32_i2c_config.scl_io_num = i2c_slave_config->scl_io_number;
    esp32_i2c_config.scl_pullup_en = GPIO_PULLUP_DISABLE;
    esp32_i2c_config.mode = I2C_MODE_SLAVE;
    esp32_i2c_config.slave.addr_10bit_en = 0;
    esp32_i2c_config.slave.slave_addr = i2c_slave_config->address;

    ESP_ERROR_CHECK(i2c_param_config(i2c_slave_config->number, &esp32_i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(
        i2c_slave_config->number,
        esp32_i2c_config.mode,
        I2C_SLAVE_RX_BUF_LEN,
        I2C_SLAVE_TX_BUF_LEN,
        0
    ));
};

uint8_t hal_esp32_i2c_slave_write(uint8_t port, uint8_t *buffer, uint16_t size)
{
    return i2c_slave_write_buffer(
        port,
        buffer,
        size,
        500 / portTICK_RATE_MS
    );
}