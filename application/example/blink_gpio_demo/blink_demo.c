/*
 * Copyright (C) 2015-2020 Alibaba Group Holding Limited
 */
#include <stdio.h>
#include "aos/kernel.h"
#include "ulog/ulog.h"
#include "board.h"
#include "aos/hal/gpio.h"

#define GPIO_KEY_IO 12
#define GPIO_LED_IO 6

gpio_dev_t led;
gpio_dev_t key;

void key_handler(void *arg)
{
    hal_gpio_output_toggle(&led);
}

int application_start(int argc, char *argv[])
{
    int ret = -1;

    led.port = GPIO_LED_IO;
    led.config = OUTPUT_PUSH_PULL;

    ret = hal_gpio_init(&led);
    if (ret != 0) {
        printf("gpio init error !\n");
    }

    key.port = GPIO_KEY_IO;
    key.config = INPUT_PULL_UP;

    hal_gpio_init(&key);
    hal_gpio_enable_irq(&key, IRQ_TRIGGER_FALLING_EDGE, key_handler, NULL);

    while(1)
    aos_msleep(100);

    return 0;
}
