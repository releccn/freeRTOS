#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

TimerHandle_t oneshot;
TimerHandle_t periodic;

void oneshot_callback(TimerHandle_t Timer) {
    printf("Oneshot timer expired! (5s has elapsed)\n");
}

void periodic_callback(TimerHandle_t Timer) {
    static int count = 0;
    count++;
    printf("The periodic timer has called this function %d times!\n", count);

    if (count == 5) {
        printf("The periodic timer has been stopped! (5 iterations completed)\n");
        xTimerStop(periodic, 0);
        // If a TimerStop is called within a callback function, the block time must be 0. 
        // If it is not 0, it causes a system deadlock.
    }
}

void app_main(void)
{
    oneshot = xTimerCreate("Oneshot", pdMS_TO_TICKS(5000), pdFALSE, NULL, oneshot_callback);
    periodic = xTimerCreate("Periodic", pdMS_TO_TICKS(500), pdTRUE, NULL, periodic_callback);

    xTimerStart(oneshot, 0);
    xTimerStart(periodic, 0);
}
