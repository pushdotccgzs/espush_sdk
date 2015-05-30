/*
 * driver.h
 *
 *  Created on: 2015Äê5ÔÂ16ÈÕ
 *      Author: Sunday
 */

#ifndef APP_INCLUDE_PUSH_H_
#define APP_INCLUDE_PUSH_H_

typedef void(*msg_cb)(uint8* pdata, uint32 len);

void ICACHE_FLASH_ATTR push_register(uint32 appid, uint8* appkey, msg_cb msgcb);


#endif /* APP_INCLUDE_PUSH_H_ */
