/*******************************************************************************
**                                                                            **
**  文件名称:  config.c                                                        **
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

//#include "esp_log.h"
#include "esp_console.h"

// #include "wifi_thread.h"
//#include "PPitems.h"
// #include "rtos_bsp_lib.h"


/******************************************************************************/
/*                           内部相关定义                                     */
/******************************************************************************/

/******************************************************************************/
/*                           函数声明区域                                     */
/******************************************************************************/

/*******************************************************************************
**  函数名称:  cmd_config
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无  value = atoi(argv[3]);
*******************************************************************************/
# if 0
static int cmd_config(int argc, char **argv)
{
    static uint8_t wifi_set_flag=0;
    char buff[128]={0};
    if (argc < 2 ) {
        printf("Please input '<wifi|demo|led>' \n");
        return -1;
    }
    printf("\r\n");

    if(strcmp(argv[1], "setwifi") == 0) {
		if(argc < 4) return 0;
        
        if(strcmp(argv[2], "ssid") == 0) {
            bsp_nvm_write( PP_WIFI_SSID, (uint8_t *)argv[3], strlen(argv[3]) );
            wifi_set_flag |= 0x01;
        }else if(strcmp(argv[2], "pwd") == 0) {
            bsp_nvm_write( PP_WIFI_PWD, (uint8_t *)argv[3],strlen(argv[3]));
            wifi_set_flag |= 0x02;
        }
        if( (wifi_set_flag&0X03)==0X03 ) {
            wifi_set_flag = 0x00;
            wifi_ctrl_event(WIFI_TOSTA_EVENT);
        }
    }else if( strcmp(argv[1], "getwifi") == 0 ) {
        if(argc < 3) return 0;

        if(strcmp(argv[2], "ssid") == 0) {
            uint8_t ssid[32]={0};
            bsp_nvm_read(PP_WIFI_SSID, ssid, 32);
            printf("PP_WIFI_SSID %s\n",ssid);
        }else if(strcmp(argv[2], "pwd") == 0) {
            uint8_t pwd[32]={0};
            bsp_nvm_read(PP_WIFI_PWD, pwd, 32);
            printf("PP_WIFI_PWD %s\n",pwd);
        }
    }else if( strcmp(argv[1],"getallpp") == 0 ) {
        uint32_t value = 0;
        uint8_t tempval[36]={0};
        for(uint8_t id=0; id<BSP_NVM_MAX_NUM; id++) {
            if( bsp_nvm_key_size(id) < 5 ){
                value = 0;
                bsp_nvm_read(id, (uint8_t *)&value, bsp_nvm_key_size(id));
                printf("id %d, value  [%d]\n", id,value);
            } else {
                memset(tempval,0,sizeof(tempval));
                bsp_nvm_read(id, (uint8_t *)tempval, bsp_nvm_key_size(id));
                printf("id %d, tempval [%s]\n", id,tempval);
            }

        }
    }else if(strcmp(argv[1], "clc") == 0){
        memset(buff,0,sizeof(buff));
        if(strcmp(argv[2], "ssid") == 0){
            bsp_nvm_write(PP_WIFI_SSID,(uint8_t *)buff,bsp_nvm_key_size(PP_WIFI_SSID));
            bsp_nvm_write(PP_WIFI_PWD,(uint8_t *)buff,bsp_nvm_key_size(PP_WIFI_PWD));
        }else if(strcmp(argv[2], "bind") == 0){
            bsp_nvm_write(PP_DEV_ID,(uint8_t *)buff,bsp_nvm_key_size(PP_DEV_ID));
            bsp_nvm_write(HTTP_UPL,(uint8_t *)buff,bsp_nvm_key_size(HTTP_UPL));
            bsp_nvm_write(PLACE_ID,(uint8_t *)buff,bsp_nvm_key_size(PLACE_ID));
            bsp_nvm_write(MQTT_URL,(uint8_t *)buff,bsp_nvm_key_size(MQTT_URL));
            bsp_nvm_write(MQTT_PORT,(uint8_t *)buff,bsp_nvm_key_size(MQTT_PORT));
            bsp_nvm_write(MQTT_CLIENTID,(uint8_t *)buff,bsp_nvm_key_size(MQTT_CLIENTID));
            bsp_nvm_write(MQTT_USERNAME,(uint8_t *)buff,bsp_nvm_key_size(MQTT_USERNAME));
            bsp_nvm_write(MQTT_PASSWD,(uint8_t *)buff,bsp_nvm_key_size(MQTT_PASSWD));
        }
    }
    else {
         printf("Please input '<get|set>' \n");
    }

    return 0;
}
#else
static int cmd_config(int argc, char **argv)
{
    return 0;
}
#endif
/*******************************************************************************
**  函数名称:  register_cmd_config
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void register_cmd_config(void)
{
    const esp_console_cmd_t cmd = {
        .command = "config",
        .help = "console config parm ",
        .hint = NULL,
        .func = &cmd_config,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}
