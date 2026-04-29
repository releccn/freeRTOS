#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void unpinned(void *pvParameters) {
    while (1) {
        printf("The unpinned task is running on Core %d\n", xPortGetCoreID());
        vTaskDelay(pdMS_TO_TICKS(3000)); // 1 second delay
    }
}

void pinnedCore0(void *pvParameters) {
    while (1) {
        printf("Called Blink Task Running on Core %d\n", xPortGetCoreID());
        gpio_set_level(2, 1); // hi
        vTaskDelay(pdMS_TO_TICKS(1000)); // half second delay
        gpio_set_level(2, 0); // lo
        vTaskDelay(pdMS_TO_TICKS(1000)); // half sec delay
    }
}

void pinnedCore1(void *pvParameters) {
    // take a count parameter and modify it.
    
    unsigned int *count = (unsigned int *)pvParameters;
    while (1) {
        printf("Called Count Task Running on Core %d\n", xPortGetCoreID());
        printf("Count: %u\n", (*count)++);
        // *count++;
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void app_main(void)
{
    gpio_set_direction(2, GPIO_MODE_OUTPUT);
    static unsigned int c = 0;

    xTaskCreate(unpinned, "Unpinned", 2048, NULL, 5, NULL);
    xTaskCreatePinnedToCore(pinnedCore0, "pinned0", 2048, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(pinnedCore1, "pinned1", 2048, &c, 5, NULL, 1);
}