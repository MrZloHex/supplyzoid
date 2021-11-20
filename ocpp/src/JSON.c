#include "JSON.h"

bool
check_json
(
    char *str,
    size len
)
{
    int res = mjson(str, len, NULL, NULL);
    if (res > 0)
        return true; 
    else
        return false;
}