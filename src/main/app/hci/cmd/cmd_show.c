/*******************************************************************************
**                                                                            **
**  文件名称:  show.c                                                           **
**  版权所有:  CopyRight @ AcTec(XiaMen) Medical Technology Co.,Ltd 2015       **
**  文件描述:                                                                  **
**  ========================================================================  **
**  创建信息:  | 2015-6-11 | lwp | 创建本模块                                   **
**  ========================================================================  **
**  修改信息:  单击此处添加....                                                 **
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_newlib.h"
#include "time.h"

/******************************************************************************/
/*                           内部相关定义                                     */
/******************************************************************************/
static const char *TAG = "show";
/******************************************************************************/
/*                           内部相关定义                                     */
/******************************************************************************/
 
 
/******************************************************************************/
/*                           函数定义区域                                     */
/******************************************************************************/
/*******************************************************************************
**  函数名称:  show_base
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
static void show_base(void)
{
    printf("%s\n", __func__);
}
/*******************************************************************************
**  函数名称:  show
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void show_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts); 
    printf("clock_gettime : tv_sec=%lld, tv_nsec=%ld\n", ts.tv_sec, ts.tv_nsec);
    struct tm t;
    char date_time[64];
    //strftime(date_time, sizeof(date_time), "%Y-%m-%d %H:%M:%S", localtime_r(&ts.tv_sec, &t));
    strftime(date_time, sizeof(date_time), "%H:%M", localtime_r(&ts.tv_sec, &t));
    strftime(date_time, sizeof(date_time), "%l:%M", localtime_r(&ts.tv_sec, &t));
    printf("clock_gettime : date_time=%s, tv_nsec=%ld\n", date_time, ts.tv_nsec);
    strftime(date_time, sizeof(date_time), "%a", localtime_r(&ts.tv_sec, &t));
    printf("clock_gettime : date_time=%s, tv_nsec=%ld\n", date_time, ts.tv_nsec);
    strftime(date_time, sizeof(date_time), "%A", localtime_r(&ts.tv_sec, &t));
    printf("clock_gettime : date_time=%s, tv_nsec=%ld\n", date_time, ts.tv_nsec);
}

/*******************************************************************************
**  函数名称:  show
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
static int show(int argc, char **argv)
{
    if (argc != 2 ) {
        ESP_LOGI(TAG, "Please input '<time>' \n");
        return -1;
    }
 
    if(strcmp(argv[1], "time") == 0) {
        show_time();
    }else if(strcmp(argv[1], "base") == 0) {
        show_base();
    }else if( strcmp(argv[1], "dbread") == 0){
        
    }

    return 0;
}
/*******************************************************************************
**  函数名称:  register_show
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void register_cmd_show(void)
{
    const esp_console_cmd_t cmd = {
        .command = "show",
        .help = "console show parm ",
        .hint = NULL,
        .func = &show,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}