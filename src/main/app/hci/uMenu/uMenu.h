/*******************************************************************************
**                                                                            **
**  文件名称:  uMenu.h                                                         **
**  版权所有:  CopyRight @ AcTec(XiaMen) Medical Technology Co.,Ltd 2015       **
**  文件描述:  uMenu头文件                                                      **
**  ========================================================================  **
**  创建信息:  | 2021-9-20 | l | 创建本模块                                     **
**  ========================================================================  **
**  修改信息:  单击此处添加....                                               **
*******************************************************************************/
#ifndef _UMENU_H_
#define _UMENU_H_
#include <stdint.h>

/******************************************************************************/
/*                           宏定义区域                                       */
/******************************************************************************/
#define MENU_DEFAULT_TIMEOUT     1000      //ms
#define MENU_CFGDEF_TIMEOUT       500   //ms

//菜单按键值
#define MENU_KEY_NONE           0
#define MENU_KEY_VAL1           1
#define MENU_KEY_VAL2           2
#define MENU_KEY_VAL3           3
#define MENU_KEY_VAL4           4
#define MENU_KEY_VAL5           5
#define MENU_KEY_VAL6           6
#define MENU_KEY_VAL7           7
#define MENU_KEY_VAL8           8
#define MENU_KEY_VAL9           9
#define MENU_KEY_VAL10          10

/******************************************************************************/
/*                           结构体定义                                       */
/******************************************************************************/
enum menu_lock {
    MENU_ULOCK  = 0 ,
    MENU_LOCK   = 1 ,
};
enum menu_ret {
    MENU_EOK  = 0 ,
    MENU_TIMEOUT   = 1 ,
};

enum menu_state_mode{
    NENU_WORK_NORMAL  = 0 ,
    NENU_WORK_FACTORY,
    MENU_WELCOME,
    MENU_LIST,
};

/******************************************************************************/
/*                           函数定义区域                                      */
/******************************************************************************/
void menu_thread_init(void);
uint32_t menu_get_key_event(uint32_t timeout_ms);
#endif /* UMENU_H_ */
