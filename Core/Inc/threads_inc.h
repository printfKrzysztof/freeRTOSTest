/**
 * @file threads.h
 * @author Krzysztof Bortnowski (krzysztof.bortnowski@sesto.pl)
 * @brief
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef THREADS_INC_H_
#define THREADS_INC_H_
#include "cmsis_os.h"
void mainTaskThread(void const *argument);
void task1Thread(void const *argument);
void task2Thread(void const *argument);

#endif // THREADS_INC_H_
