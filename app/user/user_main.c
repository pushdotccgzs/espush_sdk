/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/1/1, v1.0 create this file.
*******************************************************************************/
#include <os_type.h>
#include <c_types.h>
#include <osapi.h>
#include <driver/uart.h>
#include <user_interface.h>
#include <mem.h>
#include <espconn.h>

#include "push.h"


void ICACHE_FLASH_ATTR user_rf_pre_init()
{
	os_printf("user_rf_pre_init.\n");
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_printf("\n\nready\n\n;");
	struct rst_info* rstInfo = system_get_rst_info();

	struct station_config config;
	os_strcpy(config.ssid, "TEST8266");
	os_strcpy(config.password, "1jcsxeastl");

	wifi_set_opmode(STATION_MODE);
	wifi_station_set_config(&config);
	wifi_station_set_auto_connect(1);
	wifi_station_dhcpc_start();

	//uint32 appid, char appkey[32], char devid[32], enum VERTYPE type, msg_cb msgcb
	espush_register(1234, "25b28f0ffb9711e4a96d446d579b49a1", "", VER_SDK, NULL);
}
