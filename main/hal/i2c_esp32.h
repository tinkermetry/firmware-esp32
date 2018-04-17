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

#ifndef _TINKERMETRY_HAL_I2C_ESP32_H
#define	_TINKERMETRY_HAL_I2C_ESP32_H

#define I2C_SLAVE_TX_BUF_LEN   (8 * SPEKTRUM_DATA_LENGTH)
#define I2C_SLAVE_RX_BUF_LEN   (8 * SPEKTRUM_DATA_LENGTH)
#define I2C_SLAVE_0_PORT       I2C_NUM_0
#define I2C_SLAVE_1_PORT       I2C_NUM_1

#define I2C_ACK_CHECK_ENABLE   0x1 /* Master will require ack from slave */
#define I2C_ACK_CHECK_DISABLE  0x0
#define I2C_ACK_VALUE          0x0
#define I2C_NACK_VALUE         0x1

void hal_esp32_i2c_slave_init(const hal_i2c_slave_config_t* i2c_slave_config);
uint16_t hal_esp32_i2c_slave_write(uint8_t port, uint8_t *data, uint16_t size);

#endif

