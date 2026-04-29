/* Print a message every 2 seconds, count how many times the task runs (i.e. how many times the message prints)*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void message(void *pvParameters) {
    // Increment count
    unsigned int *c = (unsigned int *)pvParameters;
    while (1) {
        printf("(%u) This is a message from freeRTOS! Yay!\n", *c);
        *c+= 1;
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2 second Delay
    }
}

void app_main(void)
{
    static unsigned int count = 0; // This needs to be static, because when app_main is done executing, count becomes dangling. 
                                   // Message lives, but app_main goes out of scope, and since count is local, it is dangling.
    xTaskCreate(message, "messageFunc", 2048, &count, 5, NULL);
}