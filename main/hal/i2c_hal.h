/*
 * Copyright (c) 2017-2018 Mika Tuupola
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

#ifndef _TINKERMETRY_HAL_I2C_H
#define	_TINKERMETRY_HAL_I2C_H

typedef struct {
    uint8_t number;
    uint8_t address;
    uint8_t scl_io_number;
    uint8_t sda_io_number;
} hal_i2c_slave_config_t;

typedef struct {
    uint8_t number;
    uint8_t scl_io_number;
    uint8_t sda_io_number;
} hal_i2c_master_config_t;

void hal_i2c_slave_init(const hal_i2c_slave_config_t *i2c_slave_config);
void hal_i2c_master_init(const hal_i2c_master_config_t *i2c_master_config);

uint8_t hal_i2c_slave_write(uint8_t number, uint8_t *buffer, uint16_t length);
uint8_t hal_i2c_master_probe(uint8_t number, uint8_t address);
uint8_t hal_i2c_master_write(uint8_t number, uint8_t address, uint8_t *buffer, uint16_t length);
uint8_t hal_i2c_master_read(uint8_t number, uint8_t address, uint8_t *buffer, uint16_t length);

#endif
