#include "stringg.h"

void
strcpyy
(
    char *_dest,
    char *_src
)
{
    unsigned int i = 0;
    while (_src[i] != 0)
    {
        _dest[i] = _src[i];
        ++i;
    }
    _dest[i] = '\0';
}



unsigned int
strlenn
(
    char *_str
)
{
    unsigned int len = 0;

    while (_str[len] != 0)
    {
        ++len;
    }

    return len;
}