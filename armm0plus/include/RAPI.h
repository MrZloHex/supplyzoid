#ifndef RAPI_H_
#define RAPI_H_

#include "stdtypes.h"
#include "stringg.h"
#include "convert.h"

#define RAPI_BUF_LEN 32
#define RAPI_SOC '$'
#define RAPI_EOC 0xD
#define RAPI_SOSI ':'

#define RAPI_MAX_TOKENS 10


typedef struct
{
	char buf_cmd[RAPI_BUF_LEN];
	size buf_index;

	char *tokens[RAPI_MAX_TOKENS];
	size token_index;
} RAPI;

void
rapi_reset(RAPI *rapi);

void
rapi_update(RAPI *rapi);

bool
rapi_analyze(RAPI *rapi);

void
rapi_process_cmd(RAPI *rapi);


#endif /* RAPI_H_ */