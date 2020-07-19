#include "aos/kernel.h"
#include "aos/hal/i2c.h"
#include "aos/hal/gpio.h"
#include "board.h"
#include "hal_dht11.h"

#define DHT11_DATA_GPIO 1
#define DHT_MAXTIMINGS	100
#define DHT_BREAKTIME	4
#define DHT_MAXCOUNT	100

gpio_dev_t dht11;

#define GPIO_DIR_INPUT	0
#define GPIO_DIR_OUTPUT	1

#define GPIO_SET_DIR(pin,dir) do{ \
	dht11.port = pin ;	\
	if(dir)dht11.config = OUTPUT_PUSH_PULL;	\
	else dht11.config = INPUT_PULL_UP; \
	hal_gpio_init(&dht11); \
}while(0);

#define GPIO_SET_DATA(x) do { \
	if(x) hal_gpio_output_high(&dht11); \
	else hal_gpio_output_low(&dht11); \
}while(0)

uint8_t GPIO_GET_DATA(void)
{
	uint32_t values = 0;
	hal_gpio_input_get(&dht11,&values);
	return (values & 0xff);
}

uint8_t hal_dht11_read(dht11_sensor_data_t *output)	
{
	int counter = 0;
	int laststate = 1;
	int i = 0;
	int j = 0;
	int checksum = 0;
	int data[100];
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;
	
	GPIO_SET_DIR(DHT11_DATA_GPIO,GPIO_DIR_OUTPUT);
	// Wake up device, 250ms of high
	GPIO_SET_DATA(1);
	DelayMs(20);

	// Hold low for 20ms
	GPIO_SET_DATA(0);
	DelayMs(20);

	// High for 40ns
	GPIO_SET_DATA(1);
	DelayUs(18);//≈40us
	// Set DHT_PIN pin as an input

	GPIO_SET_DIR(DHT11_DATA_GPIO,GPIO_DIR_INPUT);

	// wait for pin to drop?
	while (GPIO_GET_DATA() == 1 && i < DHT_MAXCOUNT) {
		DelayUs(4);//≈10.78us
		i++;
	}

	if(i == DHT_MAXCOUNT)
	{
		printf("DHT_MAXCOUNT");
		return 0;
	}

	// read data
	for (i = 0; i < DHT_MAXTIMINGS; i++)
	{
		// Count high time (in approx us)
		counter = 0;
		while (GPIO_GET_DATA() == laststate)
		{
			counter++;
			DelayUs(4);//≈10.78us
			if (counter == 1000){
				break;
			}
		}
		
		laststate = GPIO_GET_DATA();
		if (counter == 1000){
			break;
		}

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
	dht11.port = DHT11_DATA_GPIO;
	dht11.config = INPUT_PULL_UP;
	hal_gpio_init(&dht11);
}

