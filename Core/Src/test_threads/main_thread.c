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
osThreadDef(SemaphoreTask1, semaphoreTask1Thread, osPriorityNormal, 0, 128);
osThreadDef(SemaphoreTask2, semaphoreTask2Thread, osPriorityNormal, 0, 128);
osThreadDef(QueueTask, queueTaskThread, osPriorityNormal, 0, 128);
osThreadDef(Context1Thread, contextTask1Thread, osPriorityHigh, 0, 128);
osThreadDef(Context2Thread, contextTask2Thread, osPriorityLow, 0, 128);
osSemaphoreDef(semaphore);
osMessageQDef(queue, 16, uint32_t);

void resetValues()
{
    for (size_t i = 0; i < 10; i++)
    {
        values1[i] = 0;
        values2[i] = 0;
    }
    __HAL_TIM_SET_COUNTER(&htim2, 0);
}

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

                    resetValues();
                    task1 = osThreadCreate(osThread(Task1Thread), NULL);
                    task2 = osThreadCreate(osThread(Task2Thread), NULL);

                    HAL_TIM_Base_Start(&htim2);
                    osDelay(10); // 1 milisecond block for main task

                    osThreadTerminate(task1);
                    osThreadTerminate(task2);
                    HAL_TIM_Base_Stop(&htim2);

                    float value = 0;
                    for (size_t i = 0; i < 10; i++)
                    {
                        value += values2[i] - values1[i];
                    }
                    value = value / 10;
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
                resetValues();
                semaphoreHandle = osSemaphoreCreate(osSemaphore(semaphore), 1);
                task1 = osThreadCreate(osThread(SemaphoreTask1), NULL);
                task2 = osThreadCreate(osThread(SemaphoreTask2), NULL);
                HAL_TIM_Base_Start(&htim2);
                osDelay(10); // 1 milisecond block for main task
                osThreadTerminate(task1);
                osThreadTerminate(task2);
                HAL_TIM_Base_Stop(&htim2);

                float value = 0;
                for (size_t i = 0; i < 10; i++)
                {
                    value += values2[i] - values1[i];
                }
                value = value / 10;

                uint8_t my_tab[4];
                // Pakowanie floatów
                uint8_t *ptr = (uint8_t *)&value;
                for (int i = 0; i < 4; i++)
                {
                    my_tab[i] = *(ptr + i);
                }
                CodeFrame(buffer, SEMAPHORE_TIME, 4, my_tab);
                HAL_UART_Transmit(&huart2, buffer, 10, 100);
            }
            break;

            case QUEUE_TIME:
            {
                resetValues();
                queueHandle = osMessageCreate(osMessageQ(queue), NULL);
                task1 = osThreadCreate(osThread(QueueTask), NULL);

                HAL_TIM_Base_Start(&htim2);
                osDelay(10); // 1 milisecond block for main task
                osThreadTerminate(task1);

                HAL_TIM_Base_Stop(&htim2);
                float value = 0;
                for (size_t i = 0; i < 10; i++)
                {
                    value += values1[i];
                }
                value = value / 10;
                uint8_t my_tab[4];
                // Pakowanie floatów
                uint8_t *ptr = (uint8_t *)&value;
                for (int i = 0; i < 4; i++)
                {
                    my_tab[i] = *(ptr + i);
                }
                CodeFrame(buffer, QUEUE_TIME, 4, my_tab);
                HAL_UART_Transmit(&huart2, buffer, 10, 100);
            }
            break;

            case CONTEXT_SWITCH: // From higher to Lower
            {
                resetValues();
                task1 = osThreadCreate(osThread(Context1Thread), NULL);
                task2 = osThreadCreate(osThread(Context2Thread), NULL);

                HAL_TIM_Base_Start(&htim2);
                osDelay(10); // 1 milisecond block for main task

                osThreadTerminate(task1);
                osThreadTerminate(task2);
                HAL_TIM_Base_Stop(&htim2);

                float value = 0;
                for (size_t i = 0; i < 10; i++)
                {
                    value += values2[i] - values1[i];
                }
                value = value / 10;
                uint8_t my_tab[4];
                // Pakowanie floatów
                uint8_t *ptr = (uint8_t *)&value;
                for (int i = 0; i < 4; i++)
                {
                    my_tab[i] = *(ptr + i);
                }
                CodeFrame(buffer, CONTEXT_SWITCH, 4, my_tab);
                HAL_UART_Transmit(&huart2, buffer, 10, 100);
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
