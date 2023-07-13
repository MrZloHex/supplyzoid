#include "logger.h"

#include "serial.h"
#include "stdio.h"

void
logger_init(Logger *log, RTC_HandleTypeDef *rtc)
{
    log->rtc = rtc;
    log->log_index = 0;
}



void
logger_log_task_set
(
    Logger *log, Controller_TaskSet_Result res,
    char *ok_comment, char *file, int line
)
{
	HAL_RTC_GetTime
	(
		log->rtc,
		&(log->logs[log->log_index].time),
		RTC_FORMAT_BIN
	);

    log->logs[log->log_index].file = file;
    log->logs[log->log_index].line = line;
    log->logs[log->log_index].comment = ok_comment;
    log->logs[log->log_index].type = res == CTRL_SET_OK ? LT_INFO : LT_ERROR;
    log->logs[log->log_index].err_code = (int)res;
    log->log_index++;
}

void
logger_log_protocol
(
    Logger *log, Controller_Protocol_Result res,
    char *ok_comment, char *file, int line
)
{
	HAL_RTC_GetTime
	(
		log->rtc,
		&(log->logs[log->log_index].time),
		RTC_FORMAT_BIN
	);

    log->logs[log->log_index].file = file;
    log->logs[log->log_index].line = line;
    log->logs[log->log_index].comment = ok_comment;
    log->logs[log->log_index].type = res == CTRL_PTCL_OK || res == CTRL_PTCL_RESPONSE ? LT_INFO : LT_ERROR;
    log->logs[log->log_index].err_code = (int)res;
    log->log_index++;
}

void
logger_log
(
    Logger *log, LogType type,
    char *comment, char *file, int line
)
{
	HAL_RTC_GetTime
	(
		log->rtc,
		&(log->logs[log->log_index].time),
		RTC_FORMAT_BIN
	);

    log->logs[log->log_index].file = file;
    log->logs[log->log_index].line = line;
    log->logs[log->log_index].comment = comment;
    log->logs[log->log_index].type = type;
    log->logs[log->log_index].err_code = -1;
    log->log_index++;
}

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

static const char *k_LOG_TYPES[] =
{
    "INFO", "ERROR", "FATAL", "WARNING", "DEBUG", "TRACE"
};


void
logger_dump(Logger *log)
{
    for (size_t i = 0; i < log->log_index; ++i)
    {
        char time[9];
        time_to_str(log->logs[i].time, time);
        char source[256];
        sprintf(source, "%s:%d", log->logs[i].file, log->logs[i].line);
        #warning ADD CUSTOM FUNCTION
        uprintf
        (
            DBUG_UART, 100, 512, "%s\t%s\t%-100s %-50s %d\r\n",
            time, k_LOG_TYPES[log->logs[i].type],
            source, log->logs[i].comment, log->logs[i].err_code
        );
    }

    log->log_index = 0;
}
