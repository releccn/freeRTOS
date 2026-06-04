#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

StaticTask_t task_buffer;

StackType_t task_stack[3072];


void taskDyna(void *pvArgs) {
    while (1) {
        ESP_LOGI("Dynamic", "Running dynamic task: free stack (bytes); %u",
                (unsigned)uxTaskGetStackHighWaterMark2(NULL));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void taskStat(void *pvArgs) {
    while (1) {
        ESP_LOGI("Static", "Running static task: free stack (bytes); %u",
             (unsigned)uxTaskGetStackHighWaterMark2(NULL));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{   
    ESP_LOGI("Main", "Heap before task creation: %zu (bytes)", xPortGetFreeHeapSize());

    xTaskCreate(taskDyna, "Dynamic", 2048, NULL, 3, NULL);
    ESP_LOGI("Main", "Heap after dynamic task creation: %zu (bytes)", xPortGetFreeHeapSize());

    xTaskCreateStatic(taskStat, "Static", 3072, NULL, 3, task_stack, &task_buffer);
    ESP_LOGI("Main", "Heap after dynamic and task creation: %zu (bytes)", xPortGetFreeHeapSize());
}
