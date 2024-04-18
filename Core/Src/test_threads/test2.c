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

void semaphoreTask1Thread(void const *argument)
{
    (void)(argument);
    int i = 0;
    while (1)
    {

        osSemaphoreWait(semaphoreHandle, osWaitForever);
        values1[i] = __HAL_TIM_GetCounter(&htim2);
        osSemaphoreRelease(semaphoreHandle);
        osDelay(1); // Adjust delay as needed
        i++;
    }
}

void semaphoreTask2Thread(void const *argument)
{
    (void)(argument);
    int i = 0;
    while (1)
    {

        osSemaphoreWait(semaphoreHandle, osWaitForever);
        values2[i] = __HAL_TIM_GetCounter(&htim2);
        osSemaphoreRelease(semaphoreHandle);
        osDelay(1); // Adjust delay as needed
        i++;
    }
}