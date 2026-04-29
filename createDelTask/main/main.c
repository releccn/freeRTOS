/* blink and Manager Task. 
Blink - blink LED every 0.5s, self delete task after 10 blinks
Manager - deletes Blink Task after 5 seconds if still running.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

TaskHandle_t blinkTask = NULL;

void blink(void *pvParameters) {
    printf("Running Blink...\n");
    while (1) {
        for (int i = 0; i < 15; ++i) {
            gpio_set_level(2, 1); // hi - on
            vTaskDelay(pdMS_TO_TICKS(500)); // 0.5 second delay
            gpio_set_level(2, 0); // lo - off
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        printf("Killing Task...\n");
        vTaskDelete(NULL);
    }
}

void manager(void *pvParameters) {
    printf("Running Manager...\n");
    vTaskDelay(pdMS_TO_TICKS(5000)); // wait 5 seconds
    if (blinkTask != NULL) {
        printf("Killing blink\n");
        vTaskDelete(blinkTask); // delete blink
    }
    printf("Killing manager (self)\n");
    gpio_set_level(2, 0); // turn off led
    vTaskDelete(NULL); // then delete self
}

void app_main(void)
{
    gpio_set_direction(2, GPIO_MODE_OUTPUT);
    xTaskCreate(blink, "blink", 2048, NULL, 5, &blinkTask);
    xTaskCreate(manager, "manager", 2048, NULL, 4, NULL);
}