#include "logger.h"

void
logger_init(Logger *log)
{
    log->log_index = 0;
}



void
logger_log_task_set
(
    Logger *log, Controller_TaskSet_Result res,
    char *ok_comment, char *file, int line
)
{}

void
logger_log_protocol
(
    Logger *log, Controller_Protocol_Result res,
    char *ok_comment, char *file, int line
)
{}

void
logger_error
(
    Logger *log, char *res,
    char *ok_comment, char *file, int line
)
{}


void
logger_fatal_error(Logger *log, Controller_Result err)
{
    log->fatal_error = err;

    // ADD SAVE TO FRAM
}
