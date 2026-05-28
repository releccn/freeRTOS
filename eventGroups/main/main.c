#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"

EventGroupHandle_t event_group;

#define WIFI_BIT BIT0
#define MQTT_BIT BIT1
#define READY_BIT BIT2

void wifi_task(void *args) {
    vTaskDelay(pdMS_TO_TICKS(3000)); // Simulate connecting to WiFi
    ESP_LOGI("WIFI", "WiFi Connected!");

    xEventGroupSetBits(event_group, WIFI_BIT);
    vTaskDelete(NULL);
}

void mqtt_task(void *args) {
    vTaskDelay(pdMS_TO_TICKS(4000)); // Simulate connecting to MQTT
    ESP_LOGI("MQTT", "MQTT Connected!");

    xEventGroupSetBits(event_group, MQTT_BIT);
    vTaskDelete(NULL);
}

void ready_task(void *args) {
    vTaskDelay(pdMS_TO_TICKS(6000)); // Set BIT2 after 6 seconds
    ESP_LOGI("READY", "Ready Bit Set");

    xEventGroupSetBits(event_group, READY_BIT);
    vTaskDelete(NULL);
}


void controller_task(void *pvParameters) {
    ESP_LOGI("CONTROLLER", "Waiting for Wi-Fi + MQTT + Ready");
    
    // Wait for both WIFI_BIT, MQTT_BIT and READY_BIT to be set
    xEventGroupWaitBits(
        event_group,
        WIFI_BIT | MQTT_BIT | READY_BIT,
        pdTRUE,   // clear bits on exit
        pdFALSE,   // wait for ALL bits
        portMAX_DELAY
    );
    ESP_LOGI("CONTROLLER", "System is ready. Starting application logic...");
    for (;;) {
        ESP_LOGI("CONTROLLER", "Controller: running...");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    event_group = xEventGroupCreate();

    xTaskCreate(controller_task, "controller", 2048, NULL, 6, NULL);
    xTaskCreate(wifi_task, "wifi", 2048, NULL, 5, NULL);
    xTaskCreate(mqtt_task, "mqtt", 2048, NULL, 5, NULL);
    xTaskCreate(ready_task, "ready", 2048, NULL, 5, NULL);
}