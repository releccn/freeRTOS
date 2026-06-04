#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/uart.h"
#include <string.h>

#include <time.h>

SemaphoreHandle_t uartMutex;

void uartLogger(const char *msg) {
    if (xSemaphoreTake(uartMutex, portMAX_DELAY)) {
        uart_write_bytes(UART_NUM_0, msg, strlen(msg));
        uart_write_bytes(UART_NUM_0, "\n", 1);

        xSemaphoreGive(uartMutex); // Give back mutex for use 
    }
}

void A(void *args) {
    while (1) {
        // Print a message to UART every 500ms.
        char messageA[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
        // uartLogger(messageA);
        uart_write_bytes(UART_NUM_0, messageA, strlen(messageA));
        vTaskDelay(pdMS_TO_TICKS(5)); // 500 ms delay
    }
}

void B(void *args) {
    while (1) {
        // Print a message to UART every 800ms
        char messageB[] = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n";
        // uartLogger(messageB);
        uart_write_bytes(UART_NUM_0, messageB, strlen(messageB));
        vTaskDelay(pdMS_TO_TICKS(10)); // 800 ms delay
    }
}

void C(void *args) {
    // Took this code from Gemini.
    while (1) {

    
        time_t now;
        time(&now);
        
        struct tm timeinfo;
        localtime_r(&now, &timeinfo);

        // Buffer to hold the formatted string
        char time_str[64];
        
        // Formats to: "2026-05-25 22:15:30\r\n"
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S\r\n", &timeinfo);

        // Send the string over UART
        // uartLogger(time_str);
        uart_write_bytes(UART_NUM_0, time_str, strlen(time_str));
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2 seconds delay
    }
    
}

void app_main(void)
{
    uartMutex = xSemaphoreCreateMutex();

    if (uartMutex == NULL) {
        printf("Failed to create mutex...");
        return;
    }

    uart_config_t uartcfg = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_driver_install(UART_NUM_0, 1024, 1024, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uartcfg);

    xTaskCreate(A, "A", 2048, NULL, 5, NULL);
    xTaskCreate(B, "B", 2048, NULL, 5, NULL);
    xTaskCreate(C, "C", 2048, NULL, 5, NULL);

}
