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

#include <driver/i2c.h>
#include <esp_log.h>

#include "unwindoze.h"
#include "spektrum.h"
#include "i2c.h"

static const char* TAG = "tinkermetry";

void i2c_slave_init()
{
    int i2c_slave_port = I2C_SLAVE_NUM;
    ESP_LOGD(TAG, "Starting I2C slave at port %d.", i2c_slave_port);

    i2c_config_t conf;
    conf.sda_io_num = I2C_SLAVE_SDA;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_SLAVE_SCL;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.mode = I2C_MODE_SLAVE;
    conf.slave.addr_10bit_en = 0;
    conf.slave.slave_addr = TELE_DEVICE_AIRSPEED;

    ESP_ERROR_CHECK(i2c_param_config(i2c_slave_port, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(
        i2c_slave_port,
        conf.mode,
        I2C_SLAVE_RX_BUF_LEN,
        I2C_SLAVE_TX_BUF_LEN,
        0
    ));
}

void i2c_master_init()
{
    int i2c_master_port = I2C_MASTER_NUM;
    ESP_LOGD(TAG, "Starting I2C master at port %d.", i2c_master_port);

    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;

    ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(
        i2c_master_port,
        conf.mode,
        I2C_MASTER_RX_BUF_LEN,
        I2C_MASTER_TX_BUF_LEN,
        0
    ));
}