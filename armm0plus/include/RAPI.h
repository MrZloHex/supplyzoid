#ifndef RAPI_H_
#define RAPI_H_

#include "stdtypes.h"
#include "stringg.h"

#define RAPI_BUF_LEN 32
#define RAPI_SOC '$'
#define RAPI_EOC 0xD
#define RAPI_SOSI ':'

typedef struct
{
	char buf_cmd[RAPI_BUF_LEN];
	size buf_index;
} RAPI;

void
rapi_reset(RAPI *rapi);

void
rapi_update(RAPI *rapi);


#endif /* RAPI_H_ */
