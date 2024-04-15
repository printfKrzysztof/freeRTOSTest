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
#include "threads.h"
#include "main.h"
#include "frame_controller.h"

void mainThread(void const *argument)
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
                    HAL_UART_Transmit(&huart2, buffer, 10, 100);
                    uint8_t times_task1[10];
                    uint8_t times_task2[10];
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

void mainTask()
{
}