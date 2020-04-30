#include "aos/kernel.h"
#include "aos/hal/gpio.h"
#include "board.h"
#include "hal_dht11.h"
#include "esp8266/pin_mux_register.h"
#include "c_types.h"
#include "c_types.h"
#include "esp8266/ets_sys.h"
#include "gpio.h"
#include "esp_misc.h"

#define dht11_delay_us(x)	os_delay_us(x)
#define dht11_delay_ms(x)	do{ int idx = 0;for(idx=0;idx < x;idx++)dht11_delay_us(1000); }while(0)

#define sleepms(x)	dht11_delay_ms(x)

#define DHT11_DATA_GPIO 13
#define DHT_MAXTIMINGS	100
#define DHT_BREAKTIME	20
#define DHT_MAXCOUNT	100

uint8_t hal_dht11_read(dht11_sensor_data_t *output)	
{
	int counter = 0;
	int laststate = 1;
	int i = 0;
	int j = 0;
	int checksum = 0;
	int data[100];
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;
	uint8_t pin = DHT11_DATA_GPIO;
	
	// Wake up device, 250ms of high
	GPIO_OUTPUT_SET(pin, 1);
	sleepms(20);
	// Hold low for 20ms
	GPIO_OUTPUT_SET(pin, 0);
	sleepms(20);
	// High for 40ns
	GPIO_OUTPUT_SET(pin, 1);
	os_delay_us(40);
	// Set DHT_PIN pin as an input
	GPIO_DIS_OUTPUT(pin);

	// wait for pin to drop?
	while (GPIO_INPUT_GET(pin) == 1 && i < DHT_MAXCOUNT) {
		os_delay_us(1);
		i++;
	}

	if(i == DHT_MAXCOUNT)
	{
		return 0;
	}

	// read data
	for (i = 0; i < DHT_MAXTIMINGS; i++)
	{
		// Count high time (in approx us)
		counter = 0;
		while (GPIO_INPUT_GET(pin) == laststate)
		{
			counter++;
			os_delay_us(1);
			if (counter == 1000)
				break;
		}
		
		laststate = GPIO_INPUT_GET(pin);
		if (counter == 1000)
			break;
		// store data after 3 reads
		if ((i>3) && (i%2 == 0)) {
			// shove each bit into the storage bytes
			data[j/8] <<= 1;
			if (counter > DHT_BREAKTIME)
				data[j/8] |= 1;
			j++;
		}
	}

	if (j >= 39) {
		checksum = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
		if (data[4] == checksum) {
			// checksum is valid
			output->temperature = (float)data[2];
			output->humidity = (float)data[0];
		} else {
			return 0;
		}
	} else {		
		return 0;
	}
	
	return 1;
}



void hal_dht11_init(void)
{
	gpio_dev_t dht11;
	dht11.port = 13;
	dht11.config = INPUT_PULL_UP;
	hal_gpio_init(&dht11);
}

