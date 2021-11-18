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
    // printf("%c\n", _dest);

    unsigned int z = 0;
    while (z < 5)
    {
        printf("%d\n", _dest[z]);
        ++z;
    }
}