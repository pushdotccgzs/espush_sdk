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

	//启动完成后从flash读入资料
	espush_cfg_s cfg;
	if(!read_espush_cfg(&cfg)) {
		//读取失败，开启配网模式
		ESP_DBG("read flash cfg info failed. \n");
		espush_local_init("ESP_AT", "espush.cn");
	} else {
		//读取成功，直接开启espush_register即可
		//system_init_done后会自动连接
		ESP_DBG("read flash cfg info success. \n");
		regist_info_s reg_info;
		reg_info.second_boot = 0;
		reg_info.boot_app = 0;
		reg_info.flashmap = system_get_flash_size_map();

		espush_init_regist_info(&reg_info);
		espush_register(cfg.app_id, cfg.appkey, cfg.devid, VER_SDK, msg_recv_cb);
	}

	system_init_done_cb(system_init_over);
}

