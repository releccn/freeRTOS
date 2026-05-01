#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define TAG "Test"
#define btn1 20 // GPIO, ISR Trigger
#define btn2 19 // GPIO


QueueHandle_t buttonQueue;
UBaseType_t queueLen = 1; 

void IRAM_ATTR isr(void *gpio) {
    uint32_t gpioPin = (uint32_t)(gpio); // GPIO Pin passed

    BaseType_t HigherPrioFlag = pdFALSE; // Initialize pdFALSE

    xQueueSendFromISR(buttonQueue, &gpioPin, &HigherPrioFlag); // Send gpioPin to queue.

    if (HigherPrioFlag) {
        // Exit and attend to the higher task right away.
        portYIELD_FROM_ISR();
    }
}

void button_task(void *pv) {
    // Receive gpioPin from queue.
    uint32_t gpio;

    // Read from queue, wait till a button is pressed.
    while (1) {

        if (xQueueReceive(buttonQueue, &gpio, portMAX_DELAY) == pdPASS) { 
            // ESP_LOGI(TAG, "GPIO %d was pressed\n", gpio); // Prints information using ESP_LOGI
            if (gpio == btn1) ESP_LOGI(TAG, "Button 1 was pressed\n");
            else if (gpio == btn2) ESP_LOGI(TAG, "Button 2 was pressed\n");
        }
        else {
            printf("Queue full!\n");
        }
    }
}

void app_main(void)
{
    // Initialize buttons
    gpio_config_t btn1cfg = {
        .mode = GPIO_MODE_INPUT, // Read signal from Pin

        // Used pullup resistor on breadboard
        // .pull_down_en = GPIO_PULLDOWN_ENABLE, // 
        // .pull_up_en = GPIO_PULLUP_DISABLE,

        .intr_type = GPIO_INTR_NEGEDGE, // Negative Edge for Pullup Res
        .pin_bit_mask = (1ULL << btn1) // GPIO 20
    };
    gpio_config_t btn2cfg = {
        .mode = GPIO_MODE_INPUT, // Read signal from Pin

        // Used pullup resistor on breadboard
        // .pull_down_en = GPIO_PULLDOWN_ENABLE, // 
        // .pull_up_en = GPIO_PULLUP_DISABLE,
        
        .intr_type = GPIO_INTR_NEGEDGE, // Negative Edge for Pullup Res
        .pin_bit_mask = (1ULL << btn2) // GPIO 20
    };
    gpio_config(&btn1cfg);
    gpio_config(&btn2cfg);

    // Create queue
    buttonQueue = xQueueCreate(queueLen, sizeof(uint32_t));

    // Create task
    xTaskCreate(button_task, "Button Task", 2048, NULL, 3, NULL);

    // Initialize interrupt to GPIO
    gpio_install_isr_service(0); // GPIO Interrupt service for per-pin interrupt handlers
    gpio_isr_handler_add(btn1, isr, (void *)btn1); // ISR Handler for Button 1
    gpio_isr_handler_add(btn2, isr, (void *)btn2); // ISR Handler for Button 2

    printf("Done initialization\n");
}
