#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define GPIO2 2

void taskNoDrift(void *pvParameters) {
    // This task uses vTaskDelayUntil (to minimize drift) and prints sample sensor data (a float) every 200ms.
    float sample_data = 21.8; // Just for sample
    TickType_t last_wake = xTaskGetTickCount(); // Get last_wake so the task runs in accurate time interval.
    while (1) {
        printf("taskNoDrift running     Sample Sensor Data: %f\n", sample_data);
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(200)); // This task will run every 200ms.
    }
}

void taskWithDrift(void *pvParameters) {
    // This task blinks GPIO2 led on ESP32 every second. (with drift)
    while (1) {
        printf("taskWithDrift running\n");
        gpio_set_level(GPIO2, 1); // hi
        vTaskDelay(pdMS_TO_TICKS(500)); // 500ms between a blink
        gpio_set_level(GPIO2, 0); // lo
        vTaskDelay(pdMS_TO_TICKS(500)); // 500ms between a blink
        
        // Total 1 second task.
    
    }
}

void app_main(void)
{
    gpio_reset_pin(GPIO2); // Refresh PIN
    gpio_set_direction(GPIO2, GPIO_MODE_OUTPUT); // Set GPIO2 as OUTPUT
    xTaskCreate(taskNoDrift, "taskNoDrift", 2048, NULL, 1, NULL ); // Create task with prio 1
    xTaskCreate(taskWithDrift, "taskWithDrift", 2048, NULL, 1, NULL ); // Create task with prio 1
}
