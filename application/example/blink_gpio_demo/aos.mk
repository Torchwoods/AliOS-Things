NAME := blink_gpio_demo

$(NAME)_MBINS_TYPE := app
$(NAME)_VERSION := 1.0.2
$(NAME)_SUMMARY := blink_gpio_demo
$(NAME)_SOURCES := blink_demo.c maintask.c

$(NAME)_COMPONENTS += osal_aos yloop

ifeq ($(HOST_MCU_FAMILY),mcu_esp8266)
GLOBAL_DEFINES += ESP8266_CHIPSET
endif

GLOBAL_DEFINES += AOS_NO_WIFI

$(NAME)_INCLUDES += ./
