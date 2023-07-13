#include "logger.h"

#include "serial.h"
#include "stdio.h"

void
logger_init(Logger *log, RTC_HandleTypeDef *rtc)
{
    log->rtc = rtc;
    log->log_index = 0;

    uprintf(DBUG_UART, 100, 10, "\r\n\r\n");
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
    if (res == CTRL_OK || res == CTRL_PTCL_RESPONSE)
        log->logs[log->log_index].type = LT_INFO;
    else if (res == CTRL_PTCL_NO_SUCH_MSG || res == CTRL_PTCL_NON_VALID_MSG)
        log->logs[log->log_index].type = LT_WARN;
    else
        log->logs[log->log_index].type = LT_ERROR;

    log->logs[log->log_index].err_code = (int)res;
    log->log_index++;
}


void
logger_log_hal
(
    Logger *log, HAL_StatusTypeDef res,
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
    log->logs[log->log_index].type = res == HAL_OK ? LT_INFO : LT_ERROR;
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
logger_log_fatal
(
    Logger *log, Controller_Result res,
    char *comment, char *file, int line
)
{
    log->fatal_error = res;

	HAL_RTC_GetTime
	(
		log->rtc,
		&(log->logs[log->log_index].time),
		RTC_FORMAT_BIN
	);

    log->logs[log->log_index].file = file;
    log->logs[log->log_index].line = line;
    log->logs[log->log_index].comment = comment;
    log->logs[log->log_index].type = LT_FATAL;
    log->logs[log->log_index].err_code = -1;
    log->log_index++;
    // ADD SAVE TO FRAM

    logger_dump(log);
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

        if (log->logs[i].type == LT_FATAL)
        {
            uprintf
            (
                DBUG_UART, 100, 512, "\t\t\t\t\t\t\tTYPE\t%u\tTSET\t%u\tPTCL\t%u\r\n",
                log->fatal_error.type, log->fatal_error.errors.tset_err, log->fatal_error.errors.ocpp_err
            );
        }
    }

    log->log_index = 0;
}
