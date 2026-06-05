#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "freertos/task.h"

#define LED_GPIO 2

TimerHandle_t periodic_timer1 = NULL;
TimerHandle_t periodic_timer2 = NULL;

void periodic_callback(TimerHandle_t xTimer) {
    uint32_t periodSecond = (uint32_t)pvTimerGetTimerID(xTimer);
    ESP_LOGI("Daemon Task", "Daemon task: %u ms timer fired", (uint32_t)periodSecond);
}

void vApplicationIdleHook(void) {
    static int led_level = 0;
    static int64_t last_toggle_us = 0;

    int64_t now = esp_timer_get_time(); // microseconds since boot
    if ((now - last_toggle_us) >= 200000) {
        led_level ^= 1;
        gpio_set_level(LED_GPIO, led_level);
        last_toggle_us = now;
    }
}

void app_main(void)
{
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);

    periodic_timer1 = xTimerCreate("Daemon", pdMS_TO_TICKS(1000), pdTRUE, (void *)((uint32_t) 1), periodic_callback);
    periodic_timer2 = xTimerCreate("Daemon", pdMS_TO_TICKS(3000), pdTRUE, (void *)((uint32_t) 3), periodic_callback);

    xTimerStart(periodic_timer1, 0);
    xTimerStart(periodic_timer2, 0);
}
