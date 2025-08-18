/*******************************************************************************
**                                                                            **
**  文件名称:  uMenu.c                                                         **
**  版权所有:  CopyRight @ Actec Medical Technology Co.,Ltd 2015               **
**  文件描述:  菜单处理接口函数                                                 **
**  ========================================================================  **
**  创建信息:  | 2021-9-20 | l | 创建本模块                                     **
**  ========================================================================  **
**  修改信息:  单击此处添加....                                               **
*******************************************************************************/
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_newlib.h"
#include "esp_log.h"
#include "esp_err.h"
#include "time.h"
#include "hal_key.h"

static const char *TAG = "uMenu";

/******************************************************************************/
/*                           全局定义区域                                     */
/******************************************************************************/
static QueueHandle_t key_msg_evt = NULL;

/******************************************************************************/
/*                           函数声明                                     */
/******************************************************************************/
void menu_thread_entry(void *parameter);
/*******************************************************************************
**  函数名称:  menu_set_key_event
**  功能描述:发送按键值
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
static esp_err_t menu_set_key_event(uint32_t keyid, uint8_t keyevt)
{
	keyid++;
	printf("send keyid %ld, keyevt %d \n", keyid , keyevt);
	uint32_t tempval = SET_KEYID_KEYEVT(keyid, keyevt);
	xQueueSendFromISR(key_msg_evt, &tempval,NULL);
	return 0;
}
/*******************************************************************************
**  函数名称:  menu_get_key_event
**  功能描述:  获取按键值
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
uint32_t menu_get_key_event(uint32_t timeout_ms)
{
	uint32_t tempval=0;
	if( xQueueReceive(key_msg_evt, &tempval, (TickType_t)timeout_ms/portTICK_PERIOD_MS) == pdPASS )
	{
		return tempval;
	}
	else
	{
		return 0;
	}
}
/*******************************************************************************
**  函数名称:  menu_thread_init
**  功能描述:  菜单线程任务初始化
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void menu_thread_init(void)
{
    ESP_LOGI(TAG, "menu_thread_init");
	key_msg_evt = xQueueCreate(20, sizeof(uint32_t));
    xTaskCreate(menu_thread_entry, "uMenu", 4096, NULL, 10, NULL);
}
/*******************************************************************************
**  函数名称:  menu_thread_entry
**  功能描述:  菜单线程任务
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void menu_thread_entry(void *parameter)
{
    uint32_t keyidevt;
	uint16_t keyid, keyevt;
    uint32_t timeout=portMAX_DELAY;

	KeySetCb(menu_set_key_event);

	while(1)
	{
        keyidevt = menu_get_key_event(timeout);
		keyid    = GET_KEYID(keyidevt);
		keyevt   = GET_KEYEVT(keyidevt);
		printf("receive keyid %d, keyevt %d.\r\n", keyid,keyevt);
	}
}
