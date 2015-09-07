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

#include "push.h"
#include "dht.h"

void ICACHE_FLASH_ATTR msg_recv_cb(uint8* pdata, uint32 len)
{
	os_printf("\n\nMSG_RECV_CB: [%d], [%s]\n\n", len, pdata);
}

void ICACHE_FLASH_ATTR user_rf_pre_init()
{

}


void system_init_over()
{
	ESP_DBG("system load completed.\n");
}


void push_wsd(void* param)
{
	struct sensor_reading *dht = readDHT(0);
	if(!dht->success) {
		ESP_DBG("reading failed.\n");
		return;
	}
	char buf[32] = { 0 };

	uint32 temperature = dht->temperature * 100;
	uint32 timestamp = get_timestamp();
	os_sprintf(buf, "wd,%d", temperature);

	espush_msg_plan(buf, os_strlen(buf), timestamp);
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
	ESP_DBG("RST => reason: [%d], exccause: [%d], epc1: [%d], epc2: [%d], epc3: [%d], excvaddr: [%d], depc: [%d]\n",
			rstInfo->reason, rstInfo->exccause, rstInfo->epc1, rstInfo->epc2, rstInfo->epc3, rstInfo->excvaddr, rstInfo->depc);

	struct station_config config;
	os_strcpy(config.ssid, "ChinaNet-966");
	os_strcpy(config.password, "Jchen0406");

	wifi_set_opmode(STATION_MODE);
	wifi_station_set_config(&config);
	wifi_station_set_auto_connect(1);
	wifi_station_dhcpc_start();

	espush_register(15192, "987d4a76556011e5b2bd002288fc6d2b", "", VER_SDK, NULL);
	DHTInit(SENSOR_DHT11, 5000);

	static os_timer_t sensor_timer;
	os_timer_disarm(&sensor_timer);
	os_timer_setfn(&sensor_timer, push_wsd, NULL);
	os_timer_arm(&sensor_timer, 60000, 1);
}

