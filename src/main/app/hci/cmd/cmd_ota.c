/*******************************************************************************
**                                                                            **
**  文件名称:  cmd_ota.c                                                       **
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

#include "esp_log.h"
#include "esp_console.h"

/******************************************************************************/
/*                           内部相关定义                                     */
/******************************************************************************/
//static const char *TAG = "ota";
/******************************************************************************/
/*                           内部相关定义                                     */
/******************************************************************************/
/******************************************************************************/
/*                           函数定义区域                                     */
/******************************************************************************/
static char *pfilename;
/*******************************************************************************
**  函数名称:  ota_icon_updata
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void ota_icon_updata(void)
{
    //https_event_notes("picture.bin",ota_picture_upgrade); 
}
/*******************************************************************************
**  函数名称:  ota_font_updata
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void ota_font_updata(void)
{
   // https_event_notes("font.bin",ota_font_upgrade);
}
/*******************************************************************************
**  函数名称:  ota_app_updata
**  功能描述:   
**  输入参数:  无
**  输出参数:  无 
**  返回参数:  无
*******************************************************************************/
void ota_app_updata(void)
{
    //https_event_notes("app.bin",ota_app_upgrade);
}
/*******************************************************************************
**  函数名称:  ota_app_updata
**  功能描述:   
**  输入参数:  无
**  输出参数:  无 
**  返回参数:  无
*******************************************************************************/
void ota_bsdiff_updata(void)
{
    //unlink(pfilename);
    printf("ota_bsdiff_updata\n");
 
}
/*******************************************************************************
**  函数名称:  ota_app_updata
**  功能描述:   
**  输入参数:  无
**  输出参数:  无 
**  返回参数:  无
*******************************************************************************/
void ota_bsdiff_download(void)
{
    pfilename ="picon.bin";
    //https_event_notes(pfilename,ota_bsdiff_updata);
}
/*******************************************************************************
**  函数名称:  ota_icon_cb
**  功能描述:   
**  输入参数:  无
**  输出参数:  无 
**  返回参数:  无
*******************************************************************************/
void ota_icon_cb(void)
{
    //unlink(pfilename);
    printf("ota_icon_cb\n");
 
}
/*******************************************************************************
**  函数名称:  ota_icon_download
**  功能描述:  bspatch picture.bin picture1.bin picon.bin
**  输入参数:  无
**  输出参数:  无 
**  返回参数:  无
*******************************************************************************/
void ota_icon_download(void)
{
    pfilename ="picture.bin";
   // https_event_notes(pfilename,ota_icon_cb);
}
/*******************************************************************************
**  函数名称:  cmd_ota
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
#if 1
static int cmd_ota(int argc, char **argv)
{
    char *url_1 = "http://192.168.5.164/UpgradeFile/5888_charge.json";
    char *url_2 = "http://actec.oss-cn-hangzhou.aliyuncs.com/admin/20220914/6321b63b2c7b7.json?md5=7942efdbbf93d414c7f77e984f959d79";
    char *url_3 = "http://192.168.5.164/UpgradeFile/5888_chargeE10.json";

    char *file = "5888_charge.json";
    char *url=NULL;
    if( argc < 2 ) return 0;

    if( strcmp(argv[1],"url1")==0 ){
        url = url_1;
    }else if( strcmp(argv[1],"url2") == 0 ){
        url = url_2;
    }else if( strcmp(argv[1], "url_3")==0 ){
        url = url_3;
    }
    else{
        return 0;
    }
    printf("url %s, file %s\n", url,file);
    //https_event_notes(url,file,NULL,NULL);
    return 0;
}
#else
static int cmd_ota(int argc, char **argv)
{
    struct optparse_long longopts[] = {
        {"up",   'u', OPTPARSE_NONE},
        {"cp",   'c', OPTPARSE_NONE},
        {"s", 's', OPTPARSE_OPTIONAL}, 
        {"w", 'w', OPTPARSE_OPTIONAL}, 
        {"f", 'f', OPTPARSE_NONE}, 
        {"bs", 'b', OPTPARSE_NONE}, 
        {"app", 'a', OPTPARSE_NONE}, 
        {0}
    };
    int  longindex;
    int  cmdid = 0;
    char *pfilename;
    char *flashname;
    char *arg;
    int option;
    struct optparse options;

    pfilename="/data/picture.bin";
    flashname="picture";

    optparse_init(&options, argv);

    while ((option = optparse_long(&options, longopts, &longindex)) != -1) { 
        switch (option) {
            case 'a':
                cmdid = 7;
                break;
            case 'u':
                cmdid = 1;
                break;
            case 'c':
                cmdid = 2;
                break;
            case 's':
                cmdid = 3;
                break;
            case 'w'://下載圖庫
                cmdid = 4;
                break;
            case 'f':
                cmdid = 5;
                break;
            case 'b'://下载字库
                cmdid = 6;
                break; 
            case 'o':
                pfilename="/data/ota.bin";
                flashname="ota";
                break;
            case '?':
                printf("%s: %s\n", argv[0], options.errmsg);
                return 0;
        }
    }
    switch (cmdid) {
        case 1: // 
            printf("otafilecopytoflash\n");
            otafilecopytoflash(flashname,pfilename); 
            break;
        case 2:
            printf("otaflashcopytofile\n");
            otaflashcopytofile(pfilename,flashname);
            break;
        case 3:;
            ota_icon_updata(); 
            break;
        case 4:
            ota_icon_download(); 
            break;
        case 5:
            ota_font_updata(); 
            break;
        case 6:
            // ota_bsdiff_download(); 
            break;
        case 7:
             ota_app_updata(); 
            break;
        default:
            break;
    }    
    /* Print remaining arguments. */
    while ((arg = optparse_arg(&options)))
        printf("%s\n", arg);

    return 0;
}
#endif
/*******************************************************************************
**  函数名称:  register_cmd_ota
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void register_cmd_ota(void)
{
    const esp_console_cmd_t cmd = {
        .command = "ota",
        .help = "console ota parm ",
        .hint = NULL,
        .func = &cmd_ota,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}


/*******************************************************************************
**  函数名称:  cmd_bspatch
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
static int cmd_bspatch(int argc, char **argv)
{ 
    #if 0
    const char *oldfile ="/data/picture.bin";
    const char * newfile ="/data/pictur2.bin";
    const char *patchfile ="/data/picon.bin";
    ota_bspatch(oldfile , newfile ,  patchfile);
    #endif
    return 0;
}
/*******************************************************************************
**  函数名称:  register_cmd_bspatch
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void register_cmd_bspatch(void)
{
    const esp_console_cmd_t cmd = {
        .command = "bspatch",
        .help = "console bspatch -h ",
        .hint = NULL,
        .func = &cmd_bspatch,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}