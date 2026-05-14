#include <stdio.h>
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

void app_main(void)
{
    // Let ESP32 wake by timer for now (every 5 seconds)
    // esp_err_t wk = esp_sleep_enable_timer_wakeup(5000000); // 5 microseconds
    // if (wk != ESP_OK) {
    //     ESP_LOGI("WK", "Could not enable sleep timer");
    // }

    // xTaskCreate(sampleTask, "sampleTask", 2048, NULL, 5, NULL);


    // esp_sleep_enable_ext0_wakeup(GPIO_NUM_9, 1); // GPIO 0 - Low

    // while (1) {
    //     ESP_LOGI("WK", "Entering Deep Sleep");

    //     esp_deep_sleep_start();


    //     // Should be RTC Timer
    //     esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    //     switch (cause) {
    //         case ESP_SLEEP_WAKEUP_TIMER:
    //             ESP_LOGI("WK", "Woke up: RTC timer");
    //             break;
    //         case ESP_SLEEP_WAKEUP_UNDEFINED:
    //             ESP_LOGW("WK", "Woke up: cause undefined");
    //             break;
    //         case ESP_SLEEP_WAKEUP_EXT0:
    //             ESP_LOGI("WK", "Woke up: Button");
    //             break;
    //         default:
    //             ESP_LOGI("WK", "Woke up: cause=%d", (int)cause);
    //             break;
    //     }
    // }
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    switch (cause) {
        case ESP_SLEEP_WAKEUP_TIMER:
            ESP_LOGI("WK", "Woke up: RTC timer");
            break;
        case ESP_SLEEP_WAKEUP_EXT0:
            ESP_LOGI("WK", "Woke up: Button (GPIO9)");
            break;
        case ESP_SLEEP_WAKEUP_UNDEFINED:
            ESP_LOGW("WK", "First boot or reset (cause undefined)");
            break;
        default:
            ESP_LOGI("WK", "Woke up: cause=%d", (int)cause);
            break;
    }
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_9, 1); // Wake on GPIO9 HIGH 


    ESP_LOGI("WK", "Entering Deep Sleep");
    esp_deep_sleep_start();
}
