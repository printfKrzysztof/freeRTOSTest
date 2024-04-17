/**
 * @file main_thread.c
 * @author Krzysztof Bortnowski (krzysztof.bortnowski@sesto.pl)
 * @brief
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "stdlib.h"
#include "inttypes.h"
#include "threads_inc.h"
#include "main.h"
#include "frame_controller.h"
#include "cmsis_os.h"
// Threads definitions

osThreadId task1;
osThreadId task2;

osThreadDef(Task1Thread, task1Thread, osPriorityNormal, 0, 128);
osThreadDef(Task2Thread, task2Thread, osPriorityNormal, 0, 128);
void mainTaskThread(void const *argument)
{

    (void)(argument);

    while (1)
    {

        uint8_t buffer[FRAME_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        HAL_UART_Receive(&huart2, buffer, 10, 100);
        commands_em command;
        uint8_t arg_count, args[4];

        if (DecodeFrame(buffer, &command, &arg_count, args) == 0)
        {
            switch (command)
            {
            case TASK_SWITCH:
                // Open new tasks and get this one on hold so it will be
                {

                    task1 = osThreadCreate(osThread(Task1Thread), NULL);
                    task2 = osThreadCreate(osThread(Task2Thread), NULL);

                    HAL_TIM_Base_Start(&htim2);
                    osDelay(10); // 1 milisecond block for main task
                                 // osThreadSuspend(osThread(Task1Thread));
                                 // osThreadSuspend(osThread(Task2Thread));

                    osThreadTerminate(task1);
                    osThreadTerminate(task2);
                    HAL_TIM_Base_Stop(&htim2);
                    __HAL_TIM_SET_COUNTER(&htim2, 0);

                    float value = values1[0] / (values2[0] + 1);
                    // float value = 2;
                    uint8_t my_tab[4];
                    // Pakowanie floatów
                    uint8_t *ptr = (uint8_t *)&value;
                    for (int i = 0; i < 4; i++)
                    {
                        my_tab[i] = *(ptr + i);
                    }
                    CodeFrame(buffer, TASK_SWITCH, 4, my_tab);
                    HAL_UART_Transmit(&huart2, buffer, 10, 100);
                }
                break;

            case SEMAPHORE_TIME:
            {
            }
            break;
            case COMMAND_NO:
            {
                if (CodeFrame(buffer, COMMAND_NO, 0, args) == 0)
                    HAL_UART_Transmit(&huart2, buffer, 10, 100);
            }
            break;
            default:
                break;
            }
        }
    }
}
