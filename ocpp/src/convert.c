#include "convert.h"

int
int_to_charset
(
    int  value,
    char set[],
    int  digits
)
{
    int i = 0;
    while (value)
    {
        set[i++] = (value % 10) + '0';
        value /= 10;
    }

    while (i < digits)
    {
        set[i++] = '0';
    }

    reverse(set, i);
    set[i] = '\0';
    return i;
}


void
reverse
(
    char *set,
    int len
)
{
    int i = 0;
    int j = len - 1;
    int temp;
    
    while (i < j)
    {
        temp = set[i];
        set[i] = set[j];
        set[j] = temp;
        
        i++;
        j--;
    }
}
