/******************************************************************************
* @license Copyright (C) 2020,NBLINK TECHNOLOGY (XIAMEN) CO.,LTD.
* All rights reserved.
* @file    key.c
* @brief   
* @author  hongweiming
* @email   hongweiming@nblink-tech.com
* @date:   2020-08-07
* @note: 
* 
* -----------------------------------------------------------------
* Change Logs:
* Date				Author				Notes
* 2020-08-07		hongweiming		create this file
* 
******************************************************************************/


/*------------------------------- Includes ----------------------------------*/
#include <string.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_timer.h"
#include "hal_key.h"
#include "driver/gpio.h"

/*---------------------- Constant / Macro Definitions -----------------------*/
#define KEY_PRESS_STATUS(x)         (gpio_get_level(key_config_table[x].Pin) == key_config_table[x].active)

#define KEY_DOWM_TIME               (30*1000)//us
#define KEY_LONG_PRESS_TIME         (600*1000)//us
#define KEY_MULTI_TIMEOUT           (400*1000)//us


/*----------------------- Type Declarations ---------------------------------*/
typedef struct
{
    uint32_t Pin; 
    uint32_t pull;
    key_evt_func_t func;
    uint8_t active;
}key_config_table_t;

typedef struct
{
    uint8_t click:7;
    uint8_t long_press:1;
}key_evt_value_t;

typedef union
{
    uint8_t value;
    key_evt_value_t bits;
}key_evt_t;


typedef struct
{
    uint8_t is_pressed;
    int64_t press_start;
    int64_t press_end;
    key_evt_t evt;
    uint8_t evt_export;
}key_ctx_t;

typedef struct
{
    key_ctx_t key_ctx[KEY_NUM];
    QueueHandle_t gpio_evt_queue;
}key_control_t;

#define GPIO_KEY1_PIN        45
#define GPIO_KEY2_PIN        46
#define GPIO_KEY3_PIN        47


/*------------------- Global Definitions and Declarations -------------------*/
static const key_config_table_t key_config_table[KEY_NUM] = 
{
    {GPIO_KEY1_PIN, GPIO_PULLDOWN_ONLY, keys_handle, 1},
    {GPIO_KEY2_PIN, GPIO_PULLDOWN_ONLY, keys_handle, 1},
    {GPIO_KEY3_PIN, GPIO_PULLDOWN_ONLY, keys_handle, 1}
};

static key_control_t key_control;

static esp_err_t (*pKeyDoCb)(uint32_t keyeid, uint8_t keyevt);

/*----------------------- Variable Declarations -----------------------------*/



/*----------------------- Function Prototype --------------------------------*/
static void key_evt_process(uint8_t index);
static void key_process_task(void* arg);


/*----------------------- Function Implement --------------------------------*/
/******************************************************************************
* Name     	key_gpio_isr_handler 
* @brief  	
* @param  	
* @return 	
* @note   	
* -----------------------------------------------------------------
* Change Logs:
* Date				Author				Notes
* 2020-09-29		hongweiming		Create this function
 ******************************************************************************/
static void IRAM_ATTR key_gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(key_control.gpio_evt_queue, &gpio_num, NULL);
}

/******************************************************************************
* Name     	key_init 
* @brief  	
* @param  	
* @return 	
* @note   	
* -----------------------------------------------------------------
* Change Logs:
* Date				Author				Notes
* 2020-08-07		hongweiming		Create this function
 ******************************************************************************/
void hal_key_init(void)
{
    uint32_t i;
    gpio_config_t io_conf;

    memset(&key_control, 0, sizeof(key_control_t));
    key_control.gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(key_process_task, "key_process", 1024*3, NULL, 4, NULL);
	
	//config key gpio as pullup input mode.
    for(i=0; i<KEY_NUM; i++)
    {
        memset(&io_conf, 0, sizeof(gpio_config_t));
        io_conf.pin_bit_mask = ((1ULL<<key_config_table[i].Pin));
        io_conf.mode = GPIO_MODE_INPUT;
        if(key_config_table[i].pull == GPIO_FLOATING)
        {
            io_conf.pull_up_en = 0;
            io_conf.pull_down_en = 0;
        }
        else if(key_config_table[i].pull == GPIO_PULLUP_ONLY)
        {
            io_conf.pull_up_en = 1;
            io_conf.pull_down_en = 0;
        }
        else if(key_config_table[i].pull == GPIO_PULLDOWN_ONLY)
        {
            io_conf.pull_up_en = 0;
            io_conf.pull_down_en = 1;
        }
        else if(key_config_table[i].pull == GPIO_PULLUP_PULLDOWN)
        {
            io_conf.pull_up_en = 1;
            io_conf.pull_down_en = 1;
        }
        
        io_conf.intr_type = GPIO_INTR_ANYEDGE;
        gpio_config(&io_conf);
    }
	//install gpio isr service
	gpio_install_isr_service(0);
	//hook isr handler for specific gpio pin
	for(i=0; i<KEY_NUM; i++)
	{
		gpio_isr_handler_add(key_config_table[i].Pin, key_gpio_isr_handler, (void*) i);
	}
	
}

/******************************************************************************
* Name     	key_get_evt 
* @brief  	
* @param  	
* @return 	
* @note   	
* -----------------------------------------------------------------
* Change Logs:
* Date				Author				Notes
* 2020-08-07		hongweiming		Create this function
 ******************************************************************************/
uint8_t key_get_evt(uint8_t index)
{
    if(index >= KEY_NUM) return KEY_EVT_NULL;
    
    return key_control.key_ctx[index].evt_export;
}

/******************************************************************************
* Name     	keys_handle 
* @brief  	
* @param  	
* @return 	
* @note   	
* -----------------------------------------------------------------
 ******************************************************************************/
void keys_handle(uint32_t key, uint8_t evt)
{
	keycallback(key, evt);
}

/******************************************************************************
* Name     	key_evt_process 
* @brief  	
* @param  	
* @return 	
* @note   	
* -----------------------------------------------------------------
* Change Logs:
* Date				Author				Notes
* 2020-08-07		hongweiming		Create this function
 ******************************************************************************/
static void key_evt_process(uint8_t i)
{
    uint8_t key_is_pressed;
    int64_t diff;
    static int64_t diff_save=0;
	
	
    key_is_pressed = KEY_PRESS_STATUS(i);

    key_control.key_ctx[i].evt_export = KEY_EVT_NULL;
    
    if(key_control.key_ctx[i].is_pressed)//处于按下状态
    {
        diff = (esp_timer_get_time() - key_control.key_ctx[i].press_start) + 1;

		if(key_is_pressed)//按下状态
        {
            if(diff >= KEY_LONG_PRESS_TIME)
            {
                if(key_control.key_ctx[i].evt.value == 0)
                {	diff_save = diff;
                    key_control.key_ctx[i].evt.bits.long_press = 1;
                    key_control.key_ctx[i].evt_export = KEY_EVT_LONG_PRESS;//生成长按按下事件
                    if(key_config_table[i].func)
                    {
                        key_config_table[i].func(i, key_control.key_ctx[i].evt_export);
                    }
                }
				if((diff-diff_save) >= (100*1000)) 
				{	diff_save = diff;
					key_control.key_ctx[i].evt.bits.long_press = 1;
                    key_control.key_ctx[i].evt_export = KEY_EVT_LONG_REPEAT;//生成长按重复事件
                    if(key_config_table[i].func)
                    {
                        //key_config_table[i].func(i, key_control.key_ctx[i].evt_export);
                    }
				}
            }
			
        }
        else//松开
        {
//        	printf("key %d end be pressed!\n", i);
//			printf("diff  %lld\n", diff);
            key_control.key_ctx[i].is_pressed = 0;
            if(diff > KEY_DOWM_TIME)
            {
                if(key_control.key_ctx[i].evt.bits.long_press)
                {	
                    key_control.key_ctx[i].evt_export = KEY_EVT_LONG_RELEASE;//生成长按松开事件
                    if(key_config_table[i].func)
                    {
                        key_config_table[i].func(i, key_control.key_ctx[i].evt_export);
                    }
                    key_control.key_ctx[i].evt.value = 0;
                }
                else
                {
                    key_control.key_ctx[i].evt.bits.click++;
                    key_control.key_ctx[i].press_end = esp_timer_get_time();
                }
            }
            
        }
    }
    else//处于松开状态
    {
        if(key_is_pressed)//按键开始被按下
        {
//			printf("key %d start be pressed!\n", i);
			key_control.key_ctx[i].is_pressed = 1;
            key_control.key_ctx[i].press_start = esp_timer_get_time();
        }
        else//松开状态
        {
            diff = (esp_timer_get_time() - key_control.key_ctx[i].press_end) + 1;
            if(key_control.key_ctx[i].evt.value)//还在计算连击
            {
                if(diff > KEY_MULTI_TIMEOUT)
                {
                    key_control.key_ctx[i].evt_export = KEY_EVT_SINGLE_CLICK+key_control.key_ctx[i].evt.bits.click-1;
                    if(key_config_table[i].func)
                    {
                        key_config_table[i].func(i, key_control.key_ctx[i].evt_export);
                    }
                    key_control.key_ctx[i].evt.value = 0;
                }
            }
        }
    }
}

/******************************************************************************
* Name     	key_process_task 
* @brief  	
* @param  	
* @return 	
* @note   	
* -----------------------------------------------------------------
* Change Logs:
* Date				Author				Notes
* 2020-09-29		hongweiming		Create this function
 ******************************************************************************/
static void key_process_task(void* arg)
{
    uint32_t index,i;
    
    while(1) 
    {
        if(xQueueReceive(key_control.gpio_evt_queue, &index, pdMS_TO_TICKS(25))) 
        {
            key_evt_process(index);
        }
        else
        {
            for(i=0; i<KEY_NUM; i++)
            {
                key_evt_process(i);
            }
        }
    }
}

/*---------------------------------------------------------------------------*/
/*******************************************************************************
**  函数名称:  KeySetCb
**  功能描述:    
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void KeySetCb(esp_err_t (*pCmdCb)(uint32_t keyeid,uint8_t keyevt))
{
    pKeyDoCb=pCmdCb;
}

/*******************************************************************************
**  函数名称:  keycallback
**  功能描述:    
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void keycallback(uint32_t keyid,uint8_t evt)
{
	if(pKeyDoCb){
		pKeyDoCb(keyid,evt);
	}
}

/*******************************************************************************
**  函数名称:  KeyClrCb
**  功能描述:   
**  输入参数:  无
**  输出参数:  无
**  返回参数:  无
*******************************************************************************/
void KeyClrCb(void)
{
    pKeyDoCb=NULL;
}
