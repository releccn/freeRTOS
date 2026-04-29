#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void blinkLed(void *pvParameters) {
    while (1) {
        // gpio_set_level(2 , 1); // turn on gpio2 light
        // printf("On\n");
        // vTaskDelay(pdMS_TO_TICKS(5000)); // half a second delay
        // gpio_set_level(2, 0); // turn off gpio2 light
        // printf("Off\n");

        gpio_set_level(2, 1); // hi
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(2, 0); // lo
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    gpio_set_direction(2, GPIO_MODE_OUTPUT); // Required for the onboard LED to blink.
    xTaskCreate(blinkLed, "blinkLed", 2048, NULL, 5, NULL);
}
