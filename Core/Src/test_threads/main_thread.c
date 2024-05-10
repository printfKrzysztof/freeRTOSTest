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

osThreadId tasks[MAX_THREADS];

osThreadDef(Task1Thread, task1Thread, osPriorityNormal, 0, 128);
osThreadDef(SemaphoreTask1, semaphoreTask1Thread, osPriorityNormal, 0, 128);
osThreadDef(SemaphoreTask2, semaphoreTask2Thread, osPriorityNormal, 0, 128);
osThreadDef(QueueTask, queueTaskThread, osPriorityNormal, 0, 128);
osThreadDef(Context1Thread, contextTask1Thread, osPriorityHigh, 0, 128);
osThreadDef(Context2Thread, contextTask2Thread, osPriorityLow, 0, 128);
osSemaphoreDef(semaphore);
osMessageQDef(queue, 16, uint32_t);

void resetValues(uint8_t *buffer_tx, uint8_t *buffer_rx)
{
    for (size_t i = 0; i < MAX_THREADS; i++)
    {
        for (size_t j = 0; j < MAX_TEST_PER_THREAD; j++)
        {
            values[i][j] = 0;
            values[i][j] = 0;
        }
    }
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    start_flag = 0;

    for (size_t i = 0; i < COMMAND_FRAME_SIZE; i++)
    {
        buffer_rx[i] = 0;
    }
    for (size_t i = 0; i < SCORE_FRAME_SIZE; i++)
    {
        buffer_tx[i] = 0;
    }
}

void mainTaskThread(void const *argument)
{

    (void)(argument);
    uint8_t buffer_rx[COMMAND_FRAME_SIZE];
    uint8_t buffer_tx[SCORE_FRAME_SIZE];
    while (1)
    {

        if (HAL_UART_Receive(&huart2, buffer_rx, COMMAND_FRAME_SIZE, 100) == HAL_OK)
        {

            commands_em command;
            uint8_t arg_count, args[4];
            if (DecodeCommandFrame(buffer_rx, &command, &arg_count, args) == 0)
            {
                switch (command)
                {
                case TASK_SWITCH:

                {
                    resetValues(buffer_tx, buffer_rx); // Reseting all values

                    // Argument 1 - Number of threads
                    // Argument 2 - Number of measurements per task

                    uint8_t task_args[args[0]][2];
                    for (size_t i = 0; i < args[0]; i++)
                    {
                        task_args[i][0] = i;
                        task_args[i][1] = args[1];
                        tasks[i] = osThreadCreate(osThread(Task1Thread), task_args[i]);
                    }

                    HAL_TIM_Base_Start(&htim2);
                    start_flag = 1;
                    osDelay(10); // 10 milisecond block for main task

                    HAL_TIM_Base_Stop(&htim2);

                    for (size_t i = 0; i < args[0]; i++) // For each task
                    {
                        osThreadTerminate(tasks[i]);
                    }

                    for (size_t i = 0; i < args[0]; i++) // For each task
                    {
                        if (CodeScoreFrame(buffer_tx, TASK_SWITCH, (uint16_t)(args[1] * 4), (uint8_t *)(values[i])) == 0)
                        {
                            HAL_UART_Transmit(&huart2, buffer_tx, SCORE_FRAME_SIZE, 1000);
                        }
                        // osDelay(10);
                    }
                }
                break;

                case SEMAPHORE_TIME:
                {
                }
                break;

                case QUEUE_TIME:
                {
                }
                break;

                case CONTEXT_SWITCH: // From higher to Lower
                {
                }
                break;

                case COMMAND_NO:
                {
                    if (CodeScoreFrame(buffer_tx, COMMAND_NO, 0, args) == 0)
                        HAL_UART_Transmit(&huart2, buffer_tx, 10, 100);
                }
                break;
                default:
                    break;
                }
            }
        }
    }
}
