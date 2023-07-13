#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "controller_types.h"
#include "time.h"


#define LOGGER_LOG(L, R, C) _Generic((R), \
	Controller_TaskSet_Result:  logger_log_task_set,        \
	Controller_Protocol_Result: logger_log_protocol,        \
	default:  logger_log \
	)(L, R, C, __FILE__, __LINE__)

typedef enum LogType_E
{
    LT_INFO,
    LT_ERROR,
    LT_FATAL,
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

    Log logs[MAX_LOGS];
    uint8_t log_index;
    Controller_Result fatal_error;
} Logger;

void
logger_init(Logger *log, RTC_HandleTypeDef *rtc);

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
logger_log
(
    Logger *log, LogType type,
    char *comment, char *file, int line
);


void
logger_error
(
    Logger *log, char *res,
    char *ok_comment, char *file, int line
);

void
logger_fatal_error(Logger *log, Controller_Result err);

void
logger_dump(Logger *log);


#endif /* __LOGGER_H__ */