/******************************************************************************
* @license Copyright (C) 2020,NBLINK TECHNOLOGY (XIAMEN) CO.,LTD.
* All rights reserved.
* @file    key.h
* @brief   
* @author  hongweiming
* @email   
* @date:   2020-08-07
* @note: 
* 
* -----------------------------------------------------------------
* Change Logs:
* Date				Author				Notes
* 2020-08-07		hongweiming		create this file
* 
******************************************************************************/
#ifndef _HAL_KEY_H_
#define _HAL_KEY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"

#define SET_KEYID_KEYEVT(X,Y)	((X<<16)+Y)

#define GET_KEYID(x)	((x>>16)&0x0000ffff)
#define GET_KEYEVT(x)	(x&0x0000ffff)


/*------------------------------- Includes ----------------------------------*/


/*---------------------- Constant / Macro Definitions -----------------------*/
enum
{
    KEY_SW1 = 0,
	KEY_SW2,
	KEY_SW3,
    KEY_NUM,
};

enum
{
    KEY_EVT_NULL = 0,
    KEY_EVT_LONG_PRESS,
    KEY_EVT_LONG_REPEAT,
    KEY_EVT_LONG_RELEASE,
    KEY_EVT_SINGLE_CLICK,
};

typedef void (*key_evt_func_t)(uint32_t key, uint8_t evt);

/*----------------------------- Global Typedefs -----------------------------*/

/*----------------------------- Global Defines ------------------------------*/

/*----------------------------- External Variables --------------------------*/

/*------------------------ Global Function Prototypes -----------------------*/
void hal_key_init(void);
uint8_t key_get_evt(uint8_t index);
void keys_handle(uint32_t key, uint8_t evt);

void KeySetCb(esp_err_t (*pCmdCb)(uint32_t keyeid, uint8_t keyevt));
void KeyClrCb(void);
void keycallback(uint32_t keyid,uint8_t evt);

#ifdef __cplusplus
}
#endif

#endif //_KEY_H_


