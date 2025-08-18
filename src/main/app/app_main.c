/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "console.h"
#include "esp_log.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "hal_key.h"
#include "uMenu.h"
#include "oam_thread.h"


static const char *TAG = "app_main";

void printf_chip_information(void)
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;

    esp_chip_info(&chip_info);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%ldMB %s flash\n", flash_size / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %ld bytes\n", esp_get_minimum_free_heap_size());
}


void board_init(void)
{
    ESP_LOGI(TAG, "%s",__func__);
	nvs_init();
    console_init();
    hal_key_init();
}

void app_init(void)
{
    ESP_LOGI(TAG, "%s",__func__);
    oam_thread_Init(); 
    menu_thread_init();
}

void app_main(void)
{
    ESP_LOGI(TAG, "%s",__func__);
    printf_chip_information();
    board_init();
    app_init();
    for(;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
