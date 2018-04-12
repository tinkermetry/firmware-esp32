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

#ifndef _I2C_H
#define	_I2C_H

#define SPEKTRUM_DATA_LENGTH   16
#define I2C_SLAVE_1_SCL        26 /* yellow */
#define I2C_SLAVE_1_SDA        25 /* green */
#define I2C_SLAVE_1_NUM        I2C_NUM_0

#define I2C_SLAVE_2_SCL        19 /* yellow */
#define I2C_SLAVE_2_SDA        18 /* green */
#define I2C_SLAVE_2_NUM        I2C_NUM_1

#define I2C_SLAVE_TX_BUF_LEN   (64 * SPEKTRUM_DATA_LENGTH)
#define I2C_SLAVE_RX_BUF_LEN   (64 * SPEKTRUM_DATA_LENGTH)

#define ACK_CHECK_ENABLE       0x1 /* Master will require ack from slave */
#define ACK_CHECK_DISABLE      0x0
#define ACK_VAL                0x0
#define NACK_VAL               0x1

void i2c_slave_1_init();
void i2c_slave_2_init();

#endif

