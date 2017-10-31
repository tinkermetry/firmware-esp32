/*
 * Copyright (c) 2017 Neil Kolban
 * Modifications copyright (C) 2017 Mika Tuupola
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

#include "i2c.h"

static const char* TAG = "tinkermetry";

void i2c_master_scan()
{
    ESP_LOGD(TAG, "Scanning I2C bus.");

	int i;
	esp_err_t result;
	printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
	printf("00:         ");
	for (i=3; i < 0x78; i++) {
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1 /* expect ack */);
		i2c_master_stop(cmd);

		result = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 10 / portTICK_PERIOD_MS);
		if (i % 16 == 0) {
			printf("\n%.2x:", i);
		}
		if (result == ESP_OK) {
			printf(" %.2x", i);
		} else {
			printf(" --");
		}
		i2c_cmd_link_delete(cmd);
	}
    printf("\n");
}
