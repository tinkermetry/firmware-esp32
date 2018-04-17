#include <stdint.h>

#include "i2c.h"
#include "i2c_esp32.h"

void hal_i2c_slave_init(const hal_i2c_slave_config_t *i2c_slave_config)
{
    hal_esp32_i2c_slave_init(i2c_slave_config);
};

uint16_t hal_i2c_slave_write(uint8_t port, uint8_t *data, uint16_t size)
{
    return hal_esp32_i2c_slave_write(port, data, size);
}
