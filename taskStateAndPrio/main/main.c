#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"  // Add this (AI suggestion)

#define NOW_MS() (esp_timer_get_time() / 1000) // AI Suggestion for keeping track of which tasks are running at a point in time.

void lowPrio(void *pvParameters) {
    while (1) {
        printf("[%6lld ms] - LOW    Core: %d    Stack: %d\n", NOW_MS(), xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
        // printf("This is the LOW priority running on core %d\n", xPortGetCoreID());
        // printf("This is how much I use on the stack: %d\n", uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second
    }
}

void medPrio(void *pvParameters) {
    while (1) {
        printf("[%6lld ms] - MED    Core: %d    Stack: %d\n", NOW_MS(), xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
        // printf("This is the MEDIUM priority running on core %d\n", xPortGetCoreID());
        // printf("This is how much I use on the stack: %d\n", uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(pdMS_TO_TICKS(500)); // 0.5 second
    }
}
void highPrio(void *pvParameters) {
    for (int i = 0; i < 5; ++i) {
        printf("[%6lld ms] - HIGH    Core: %d    Stack: %d\n", NOW_MS(), xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
        // printf("(%d) This is the HIGH priority running on core %d\n", i, xPortGetCoreID());
        // printf("This is how much I use on the stack: %d\n", uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(pdMS_TO_TICKS(500)); // 0.5 second
    }
    printf("[%6lld ms]  Changing highPrio to 0\n", NOW_MS());
    vTaskPrioritySet(NULL, 1); // Set to lowest prio after 5 iterations.

    while (1) { // Keep in infinite loop, or else it returns.
        printf("[%6lld ms] - HIGH    Core: %d    Stack: %d\n", NOW_MS(), xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));
        vTaskDelay(pdMS_TO_TICKS(1500)); // 1.5 second
    }
}

void app_main(void)
{
    xTaskCreate(lowPrio, "low", 2048, NULL, 2, NULL);
    xTaskCreate(medPrio, "med", 2048, NULL, 3, NULL);
    xTaskCreate(highPrio, "high", 2048, NULL, 4, NULL);
}
