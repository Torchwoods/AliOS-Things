/*
 * Copyright (C) 2017 YunOS Project. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "aos/kernel.h"
#include "aos/hal/i2c.h"
#include "aos/hal/gpio.h"
#include "board.h"
#include "ssd1306.h"

#define SSD1306_ADDRS	0x3C
#define SSD1306_WR_CMD	0x00
#define SSD1306_WR_DATA	0x40


static i2c_dev_t oled_i2c_dev = {0};

int32_t ssd1306_write(uint8_t data, SSD1306_WR_MODE mode)
{
	uint8_t buf[2] = {0x00,data};

	if(mode == SSD1306_CMD) {
		buf[0] = SSD1306_WR_CMD;
	} else if(mode == SSD1306_DATA) {
		buf[0] = SSD1306_WR_DATA;
	}

	hal_i2c_master_send(&oled_i2c_dev, SSD1306_ADDRS, buf,2, 1);
	
	return 0;
}

void ssd1306_init(void)
{

	oled_i2c_dev.port                 = PORT_I2C_2;
    oled_i2c_dev.config.address_width = I2C_HAL_ADDRESS_WIDTH_7BIT;
    oled_i2c_dev.config.freq          = I2C_BUS_BIT_RATES_100K;
    oled_i2c_dev.config.mode          = I2C_MODE_MASTER;

 	hal_i2c_init(&oled_i2c_dev);

	//os_delay_us(5000);

#if 1
	//config oled
	ssd1306_write(SSD1306_DISPLAYOFF, SSD1306_CMD);
	ssd1306_write(SSD1306_SETDISPLAYCLOCKDIV, SSD1306_CMD);
	ssd1306_write(80, SSD1306_CMD);
	ssd1306_write(SSD1306_SETMULTIPLEX, SSD1306_CMD);
	ssd1306_write(0x3F, SSD1306_CMD);
	ssd1306_write(SSD1306_SETDISPLAYOFFSET, SSD1306_CMD);
	ssd1306_write(0x00, SSD1306_CMD);
	ssd1306_write(SSD1306_SETSTARTLINE, SSD1306_CMD);
	ssd1306_write(SSD1306_ENABLE_CHARGE_PUMP, SSD1306_CMD);
	ssd1306_write(0x14, SSD1306_CMD);
	ssd1306_write(SSD1306_MEMORYMODE, SSD1306_CMD);
	ssd1306_write(0x02, SSD1306_CMD);
	ssd1306_write(0xA1, SSD1306_CMD);
	ssd1306_write(SSD1306_COMSCANINC, SSD1306_CMD);
	ssd1306_write(SSD1306_SETCOMPINS, SSD1306_CMD);
	ssd1306_write(0X12, SSD1306_CMD);
	ssd1306_write(SSD1306_SETCONTRAST, SSD1306_CMD);
	ssd1306_write(0x1F, SSD1306_CMD); //set brightness
	ssd1306_write(SSD1306_SETPRECHARGE, SSD1306_CMD);
	ssd1306_write(0xF1, SSD1306_CMD);
	ssd1306_write(SSD1306_SETVCOMDETECT, SSD1306_CMD);
	ssd1306_write(0x30, SSD1306_CMD);
	ssd1306_write(SSD1306_DISPLAYALLON_RESUME, SSD1306_CMD);
	ssd1306_write(SSD1306_NORMALDISPLAY, SSD1306_CMD);
	ssd1306_write(SSD1306_DISPLAYON, SSD1306_CMD);
	oled_clear_screen();
#endif 
	return 0;
}

void ssd1306_deinit(void)
{
	hal_i2c_finalize(&oled_i2c_dev);
}

void ssd1306_set_brightness(uint8_t brightness)
{
	ssd1306_write(SSD1306_SETCONTRAST, SSD1306_CMD);
	ssd1306_write(brightness, SSD1306_CMD); //set brightness
}

void ssd1306_reset(void)
{
	
}
