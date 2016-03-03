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


static struct espconn user_conn;
static esp_tcp user_tcp;
static os_timer_t gl_timer;
static os_timer_t wifi_checker;


void init_done_cb(void* param);


void ICACHE_FLASH_ATTR user_rf_pre_init()
{
	os_printf("user_rf_pre_init.\n");
}


void reconnect_timer_cb(void* param)
{
	os_printf("reconnect_timer_cb\n");
	init_done_cb(NULL);
}


void reconnect()
{
	os_timer_disarm(&gl_timer);
	os_timer_setfn(&gl_timer, reconnect_timer_cb, NULL);
	os_timer_arm(&gl_timer, 1000, 0);
}


void disconnected_cb(void* args)
{
	os_printf("DISCONNECTED.\n");
	reconnect();
}


void recv_cb(void* args, char *pdata, unsigned short len)
{
	os_printf("RECV CB: [%d], [%d].\n", len, sizeof(unsigned short));
}


void sent_cb(void* args)
{
	os_printf("SENT.\n");
}


void connected_cb(void* args)
{
	os_printf("CONNECTED.\n");
    espconn_regist_disconcb(&user_conn, disconnected_cb);
    espconn_regist_recvcb(&user_conn, recv_cb);
    espconn_regist_sentcb(&user_conn, sent_cb);

    espconn_sent(&user_conn, "HELLO", os_strlen("HELLO"));
}


void reconnected_cb(void* args, sint8 err)
{
	os_printf("RECONNECTED [%d].\n", err);
	reconnect();
}


void init_done_cb(void* param)
{
	if(wifi_station_get_connect_status() != STATION_GOT_IP) {
		os_printf("WAIT...\n");
		return;
	} else {
		os_timer_disarm(&wifi_checker);
	}


	user_conn.type = ESPCONN_TCP;
	user_conn.state = ESPCONN_NONE;
	user_conn.proto.tcp = &user_tcp;
	user_conn.proto.tcp->remote_ip[0] = 192;
	user_conn.proto.tcp->remote_ip[1] = 168;
	user_conn.proto.tcp->remote_ip[2] = 0;
	user_conn.proto.tcp->remote_ip[3] = 103;
	user_conn.proto.tcp->local_port = espconn_port();
	user_conn.proto.tcp->remote_port = 12345;

	//regist event callback
    espconn_regist_connectcb(&user_conn, connected_cb);
    espconn_regist_reconcb(&user_conn, reconnected_cb);

    espconn_connect(&user_conn);
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
	os_strcpy(config.ssid, "ChinaNet-966");
	os_strcpy(config.password, "Jchen0406");

	wifi_set_opmode(STATION_MODE);
	wifi_station_set_config(&config);
	wifi_station_set_auto_connect(1);
	wifi_station_dhcpc_start();

	os_timer_disarm(&wifi_checker);
	os_timer_setfn(&wifi_checker, init_done_cb, NULL);
	os_timer_arm(&wifi_checker, 1000, 1);
}

