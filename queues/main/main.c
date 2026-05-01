#include <stdio.h>
#include <stdlib.h> // for rand()
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// This program sends a random number from 0-99 to the queue, and prints it and determines whether it is odd or even.

QueueHandle_t queue; 
UBaseType_t QueueLength = 3;
TickType_t ProducerTicksToWait = pdMS_TO_TICKS(100);
TickType_t ConsumerTicksToWait = pdMS_TO_TICKS(1000);

TaskHandle_t producerHandle = NULL;

void producer(void *pv) {
    while (1) {
        int num = rand() % 100; // 0-99
        if (xQueueSend(queue, &num, ProducerTicksToWait) == pdPASS) {
            printf("Producer sent %d to queue\n", num);
        }
        else {
            printf("Producer failed: Queue FULL!\n");
            vTaskSuspend(NULL); // suspend itself
        }
        vTaskDelay(pdMS_TO_TICKS(50)); // every 50ms send a random number to queue
    }
}

void consumer(void *pv) {
    int num;
    while (1) {
        if (xQueueReceive(queue, &num, ConsumerTicksToWait) == pdPASS) {
            printf("Consumer received %d from queue\n", num);
            if (num % 2 == 0) {
                printf("Num is even.\n"); 
            }
            else {
                printf("Num is odd\n");
            }
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        else {
            printf("Consumer failed: Queue EMPTY!\n");
            vTaskResume(producerHandle);
        }
    }
}

void app_main() {
    queue = xQueueCreate(QueueLength, sizeof(int));
    if (queue == NULL) {
        printf("Failed to create queue\n"); 
        return;
    }
    xTaskCreate(producer, "producer", 2048, NULL, 3, &producerHandle);
    xTaskCreate(consumer, "consumer", 2048, NULL, 3, NULL);
}