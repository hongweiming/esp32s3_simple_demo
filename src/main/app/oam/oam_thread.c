/*******************************************************************************
**                                                                            **
**  文件名称:  oam_thread.c                                                       **
**  版权所有:  CopyRight @ Actec Medical Technology Co.,Ltd 2015               **
**  文件描述:                                                                  **
**  ========================================================================  **
**  创建信息:  | 2021-9-20 | l | 创建本模块                                     **
**  ========================================================================  **
**  修改信息:  单击此处添加....                                               **
*******************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "oam_thread.h"
#include <string.h>
#include "esp_timer.h"
#include "esp_log.h"

/******************************************************************************/
/*                           类型定义区域                                     */
/******************************************************************************/

struct oam_thread_msg {
    void (*process)(void *data, uint16_t data_len);
    void *data;
    uint16_t data_len;
};
/******************************************************************************/
/*                           相关定义区域                                     */
/******************************************************************************/

static const char *TAG = "oam";
static QueueHandle_t oam_msg_queue = NULL;
static esp_timer_handle_t oam_timer = NULL;
/******************************************************************************/
/*                           函數定義區域                                     */
/******************************************************************************/
/*******************************************************************************
**  函数名称:  oam_timeout_process
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
static void oam_tick_process(void *data, uint16_t len)
{
    //ESP_LOGI(TAG,"[%s %d] %d, %s.", __func__,__LINE__,len,(char *)data);
}
/*******************************************************************************
**  函数名称:  oam_process
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
static void oam_timer_cb(void* arg)
{
    char data[] = "timer_cb";
    uint16_t len = sizeof(data);
    //ESP_LOGI(TAG,"[%s %d] %d,%s.", __func__,__LINE__,len,(char *)data);
    oam_thread_post(oam_tick_process, data, len);
}
/*******************************************************************************
**  函数名称:  oam_init 
**  功能描述:  oam相关初始化 
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无  
*******************************************************************************/
static void oam_init(void)
{
    //oam_start();

    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &oam_timer_cb, 
        .name = "oam timer"
    };

    esp_timer_create(&periodic_timer_args, &oam_timer);

    esp_timer_start_periodic(oam_timer, 1000*1000);

    ESP_LOGI(TAG, "Entering sleep for 0.5s, time since boot: %lld us", esp_timer_get_time());
}
/*******************************************************************************
**  函数名称:  oam_thread_entry 
**  功能描述:  oam线程处理接口函数 
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无  
*******************************************************************************/
static void oam_thread_entry(void *arg)
{
    struct oam_thread_msg *thread_msg;
    uint32_t  wait_tick;
    (void)arg;
    
    wait_tick = 1000 / portTICK_PERIOD_MS;
    
    while (1) 
    {
        if(xQueueReceive(oam_msg_queue, &thread_msg, wait_tick))
        {
            if (thread_msg->process) 
            {
                thread_msg->process(thread_msg->data, thread_msg->data_len);
            }
            vPortFree(thread_msg);
        }
    }
}
/*******************************************************************************
**  函数名称:  oam_thread_post 
**  功能描述:  oam事件处理接口 
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无  
*******************************************************************************/
int oam_thread_post(void (*process)(void *data, uint16_t data_len), void *data, uint16_t data_len)
{
    struct oam_thread_msg *thread_msg;

    thread_msg = pvPortMalloc(sizeof(struct oam_thread_msg) + data_len);
    if (thread_msg == NULL) 
    {
        return -1;
    }
    thread_msg->data = (void *)(thread_msg + 1);
    thread_msg->process = process;
    memcpy(thread_msg->data, data, data_len);
    thread_msg->data_len = data_len;
    if(xQueueSendFromISR(oam_msg_queue, &thread_msg, NULL)!=pdPASS)
    {
        vPortFree(thread_msg);
    }

    return 0;
}
/*******************************************************************************
**  函数名称:  oam_thread_Init 
**  功能描述:  oam线程初始化 
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无  
*******************************************************************************/
int oam_thread_Init(void)
{   
    oam_init();
    
    oam_msg_queue = xQueueCreate(20, sizeof(uint32_t));
    
    xTaskCreate(oam_thread_entry, "oam_thread", 4096, NULL, 10, NULL);
    return 0;
}
