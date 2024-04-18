/**
 * @file test4.c
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

void contextTask1Thread(void const *argument)
{
    (void)(argument);
    int i = 0;

    while (1)
    {

        values1[i] = __HAL_TIM_GetCounter(&htim2);
        i++;
        osDelay(1); // Force switching
    }
}

void contextTask2Thread(void const *argument)
{
    (void)(argument);
    int i = 0;
    while (1)
    {
        values2[i] = __HAL_TIM_GetCounter(&htim2);
        i++;
        osDelay(1); // Force switching
    }
}