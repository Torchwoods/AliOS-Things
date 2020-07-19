#ifndef __HAL_DHT11_H__
#define __HAL_DHT11_H__
#include <stdint.h>

typedef struct{
	float temperature;
	float humidity;
}dht11_sensor_data_t;

extern uint8_t hal_dht11_read(dht11_sensor_data_t *output);
extern void hal_dht11_init(void);
//extern uint8_t hal_dht11_readData(dht11_sensor_data_t *output) ; 
#endif