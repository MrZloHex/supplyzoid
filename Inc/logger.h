#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "controller_types.h"
#include "time.h"

#include "main.h"

#define LOGGER_LOG(L, R, C) _Generic((R), \
	Controller_TaskSet_Result:  logger_log_task_set,        \
	Controller_Protocol_Result: logger_log_protocol,        \
    HAL_StatusTypeDef:  logger_log_hal,  \
    Controller_Result: logger_log_fatal, \
	default:  logger_log \
	)(L, R, C, __FILE__, __LINE__)


typedef enum LogType_E
{
    LT_FATAL,
    LT_ERROR,
    LT_INFO,
    LT_WARN,
    LT_DEBUG,
    LT_TRACE
} LogType;

typedef struct
{
    RTC_TimeTypeDef time;
    char *file;
    int line;
    char *comment;
    LogType type;
    int err_code;
} Log;

#define MAX_LOGS 64

typedef struct Logger_S
{
    RTC_HandleTypeDef *rtc;

    LogType level;
    Log logs[MAX_LOGS];
    uint8_t log_index;
    Controller_Result fatal_error;
} Logger;

void
logger_init(Logger *log, RTC_HandleTypeDef *rtc, LogType level);

void
logger_log_task_set
(
    Logger *log, Controller_TaskSet_Result res,
    char *ok_comment, char *file, int line
);

void
logger_log_protocol
(
    Logger *log, Controller_Protocol_Result res,
    char *ok_comment, char *file, int line
);

void
logger_log_hal
(
    Logger *log, HAL_StatusTypeDef res,
    char *ok_comment, char *file, int line
);

void
logger_log_fatal
(
    Logger *log, Controller_Result res,
    char *comment, char *file, int line
);

void
logger_log
(
    Logger *log, LogType type,
    char *comment, char *file, int line
);

void
logger_dump(Logger *log);


#endif /* __LOGGER_H__ */