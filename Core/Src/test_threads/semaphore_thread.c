/**
 * @file test2.c
 * @author Krzysztof Bortnowski (krzysztof.bortnowski@sesto.pl)
 * @brief
 * @version 0.1
 * @date 2024-04-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "threads_inc.h"
#include "main.h"

void semaphoreThread(void const *argument)
{
    const uint8_t data = ((uint8_t *)argument)[0]; // Task number
    const int max = ((uint8_t *)argument)[1];      // Number of measurements per task
    int i = 0;

    while (!start_flag)
    {
        osThreadYield(); // Forcing task switch so lower priority has a chance to take context
    }

    while (1)
    {
        osMutexWait(mutexHandle, osWaitForever);
        values[data][i++] = __HAL_TIM_GetCounter(&htim2);
        osThreadYield(); // Forcing task switch
        osMutexRelease(mutexHandle);
        osThreadYield(); // Forcing task switch

        if (i == max)
            break;
    }

    while (1)
    {
        osDelay(10000); // Forcing delay so that main_thread has a chance to take context
    }
}
