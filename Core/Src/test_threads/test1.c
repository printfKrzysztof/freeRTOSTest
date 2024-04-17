/**
 * @file test1.c
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

void task1Thread(void const *argument)
{
    (void)(argument);
    int i = 0;

    while (1)
    {

        values1[i] = __HAL_TIM_GetCounter(&htim2);
        i++;
        osDelay(1);
    }
}

void task2Thread(void const *argument)
{
    (void)(argument);
    int i = 0;
    while (1)
    {
        values2[i] = __HAL_TIM_GetCounter(&htim2);
        i++;
        osDelay(1);
    }
}