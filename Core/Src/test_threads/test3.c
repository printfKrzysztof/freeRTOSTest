/**
 * @file test3.c
 * @author Krzysztof Bortnowski (krzysztof.bortnowski@sesto.pl)
 * @brief
 * @version 0.1
 * @date 2024-04-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "threads_inc.h"
#include "main.h"

void queueTaskThread(void const *argument)
{
    (void)(argument);
    osEvent evt;
    int i = 0;
    while (1)
    {
        osMessagePut(queueHandle, __HAL_TIM_GetCounter(&htim2), osWaitForever);

        // Wait for a message from the queue
        evt = osMessageGet(queueHandle, osWaitForever);
        if (evt.status == osEventMessage && i < 10)
        {
            // Calculate the queue time
            values1[i] = __HAL_TIM_GetCounter(&htim2) - evt.value.v;
        }

        i++;
    }
}
