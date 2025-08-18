/*******************************************************************************
**                                                                            **
**  文件名称:  cmd_file.c                                                        **
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
// #include "type.h"
#include "esp_log.h"
#include "esp_console.h"
#include "ff.h"
#include "diskio.h"
#include "esp_system.h"
#include "unistd.h"
/******************************************************************************/
/*                           内部相关定义                                     */
/******************************************************************************/

/******************************************************************************/
/*                           内部相关定义                                     */
/******************************************************************************/
 
/*******************************************************************************
**  函数名称:  cmd_cat
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
static int cmd_cat(int argc, char **argv)
{
    FILE* fd;
    int res;
    int offset=0; 
    int ulen;
    char tmpbuf[16]; 
    uint32_t filelen;
    char fileneme[32];

    if(argv[1] == NULL){
        return -1; 
    }
    sprintf(fileneme,"/littlefs/%s",argv[1]);
    printf("\r\n");
    fd = fopen(fileneme ,  "rb+");
    if( fd == NULL ){
        printf("Unable to open %s \n",fileneme);
        return -1;
    }
    fseek(fd, 0, SEEK_END);

    filelen = ftell(fd);
    
    printf("fileneme %s filelen %ld \n",fileneme,filelen );
    if(filelen == 0){
        fclose(fd);
        return -2;
    }
    offset=0;
    fseek(fd, 0, SEEK_SET);
    for(; ; ){
        ulen = filelen-offset;
        if(ulen > 16){
            ulen = 16;
        }
        memset(tmpbuf,0,sizeof(tmpbuf));
        res = fread( tmpbuf,ulen,1,fd);
        //ESP_LOGI(TAG,"res %d ulen %d",res, ulen);
        // printf("%08x :%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x ",
        //             offset,
        //             tmpbuf[0],tmpbuf[1],tmpbuf[2],tmpbuf[3],
        //             tmpbuf[4],tmpbuf[5],tmpbuf[6],tmpbuf[7],
        //             tmpbuf[8],tmpbuf[9],tmpbuf[10],tmpbuf[11],
        //             tmpbuf[12],tmpbuf[13],tmpbuf[14],tmpbuf[15]);        
        printf("%s", tmpbuf);
        if(res <= 0 || ulen < 16){
            break;
        }
        offset+=16;
    }
    fclose(fd); 

    return 0;
}
/*******************************************************************************
**  函数名称:  register_cmd_cat
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void register_cmd_cat(void)
{
    const esp_console_cmd_t cmd = {
        .command = "cat",
        .help = "console utest parm ",
        .hint = NULL,
        .func = &cmd_cat,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}
/*******************************************************************************
**  函数名称:  cmd_touch
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无   
*******************************************************************************/
static int cmd_touch(int argc, char **argv)
{
    FILE* fd;
    uint32_t filelen;
    char fileneme[32];
    printf("\r\n");
    if (argc < 2 ) {
        printf("Please input '<path>' %d " ,argc);
        return 0;
    }
    
    sprintf(fileneme,"/littlefs/%s",argv[1]);

    fd = fopen(fileneme ,  "wb+");
    if( fd == NULL ){
        printf("Unable to open %s ",fileneme);
        return -1;
    }
    if(argv[2] == NULL){
        fclose(fd);
        return -1; 
    }

    filelen = strlen( argv[2]); 

    fwrite( argv[2],filelen,1,fd);
    printf("filelen %ld ",filelen );
 
    fclose(fd);
    return 0;
}
/*******************************************************************************
**  函数名称:  register_cmd_touch
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void register_cmd_touch(void)
{
    const esp_console_cmd_t cmd = {
        .command = "touch",
        .help = "console touch path text ",
        .hint = NULL,
        .func = &cmd_touch,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}  
/*******************************************************************************
**  函数名称:  cmd_rm
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
static int cmd_rm(int argc, char **argv)
{
    char fileneme[32];
    
    if (argc < 1 ) {
        printf("Please input '<path>' %d " ,argc);
        return 0;
    }
    
    sprintf(fileneme,"/littlefs/%s",argv[1]);
    unlink(fileneme);
    return 0;
}
/*******************************************************************************
**  函数名称:  register_cmd_rm
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void register_cmd_rm(void)
{
    const esp_console_cmd_t cmd = {
        .command = "rm",
        .help = "console rm path ",
        .hint = NULL,
        .func = &cmd_rm,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}
/*******************************************************************************
**  函数名称:  cmd_mv
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
static int cmd_mv(int argc, char **argv)
{
    char newname[32];
    char oldname[32];
    
    if (argc < 1 ) {
        printf("Please input oldFile newFile %d" ,argc);
        return 0;
    }
    
    sprintf(newname,"/littlefs/%s",argv[1]);
    sprintf(oldname,"/littlefs/%s",argv[1]);
    rename(oldname,newname);
    return 0;
}
/*******************************************************************************
**  函数名称:  register_cmd_rm
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void register_cmd_mv(void)
{
    const esp_console_cmd_t cmd = {
        .command = "mv",
        .help = "console mv path ",
        .hint = NULL,
        .func = &cmd_mv,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}
/*******************************************************************************
**  函数名称:  cmd_ls
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
#if 0
int cmd_ls(int argc, char **argv)
{
    char * p_para=argv[1];
    FF_DIR target_dir; 
    FILINFO dir_entry;
 
    if ( p_para == NULL ) 
    {
         
        if ( FR_OK != f_opendir(&target_dir, ".") ){
            printf("error open dir /data");    
            return 0;
        } 
         printf("f_opendir /data");    
        while( (f_readdir(&target_dir, &dir_entry) == FR_OK)  && dir_entry.fname[0] != 0)
        {
             printf("/%s", dir_entry.fname);    
            #if 0
            if ( dir_entry.fname[0] != '/data' ) // ignore . and .. entry
            {
                TCHAR const * const p_name =  dir_entry.fname;
                if ( dir_entry.fattrib & AM_DIR ) // directory
                {
                     printf("/%s", p_name);
                }else{
                    printf("%-40s%d KB", p_name, dir_entry.fsize / 1000);
                }
                putchar('\n');
            }
            #endif
        }

        (void) f_closedir(&target_dir);
    }else{
        puts("ls only supports list current directory only, try to cd to that folder first");
        return 0;
    }

  return 0;
}
#else
int cmd_ls(int argc, char **argv)
{
    return 0;
}
#endif
 /*******************************************************************************
**  函数名称:  register_cmd_rm
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void register_cmd_ls(void)
{
    const esp_console_cmd_t cmd = {
        .command = "ls",
        .help = "console ls path ",
        .hint = NULL,
        .func = &cmd_ls,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}
/*******************************************************************************
**  函数名称:  register_cmd_fatfs
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void register_cmd_file(void)
{
    register_cmd_touch();

    register_cmd_cat();

    register_cmd_rm();

    register_cmd_ls();

    register_cmd_mv();
}