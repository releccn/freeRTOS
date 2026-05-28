#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

TaskHandle_t button_task_handle = NULL;

static void IRAM_ATTR isr(void *arg) {
    BaseType_t xHigherPrioTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(button_task_handle, &xHigherPrioTaskWoken); // 

    if (xHigherPrioTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

void gpio0_task(void *arg) {
    uint32_t count = 0;
    while (1) {
        // ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        count += ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        ESP_LOGI("GPIO0", "Task unblocked by ISR");
        ESP_LOGI("GPIO0", "GPIO0 was pressed %u times!", count);
    }
}

void app_main(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << 0),
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE
    };
    gpio_config(&io_conf);

    // Install ISR service and attach ISR to the button pin.
    gpio_install_isr_service(0);
    gpio_isr_handler_add(0, isr, NULL);

    xTaskCreate(gpio0_task, "gpio0", 2048, NULL, 5, &button_task_handle);
}
