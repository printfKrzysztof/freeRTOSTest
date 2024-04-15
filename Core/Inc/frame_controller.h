/**
 * @file frame_controller.h
 * @author Krzysztof Bortnowski (krzysztof.bortnowski@sesto.pl)
 * @brief
 * @version 0.1
 * @date 2024-04-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "stdlib.h"
#include "inttypes.h"

#define MAX_ARGS 4
#define FRAME_SIZE (5 + MAX_ARGS)
typedef enum
{
    TASK_SWITCH = 0,
    SEMAPHORE_TIME,
    QUEUE_TIME,
    CONTEXT_SWITCH,
    RESEND_LAST_FRAME,
    COMMAND_NO
} commands_em;

#define ERR_FRM_NO_START -1
#define ERR_FRM_CRC -2
#define ERR_FRM_WRONG_COMMAND -3
#define ERR_FRM_TOO_MANY_ARGS -4

/**
 * @brief Decodes with CRC Check
 *
 * @param frame
 * @param command
 * @param arg_count
 * @param args
 * @return int
 */
int DecodeFrame(uint8_t *frame, uint8_t *command, uint8_t *arg_count, uint8_t *args);

/**
 * @brief Codes frame with CRC
 *
 * @param frame
 * @param command
 * @param arg_count
 * @param args
 * @return ERR_FRM
 */
int CodeFrame(uint8_t *frame, uint8_t command, uint8_t arg_count, uint8_t *args);