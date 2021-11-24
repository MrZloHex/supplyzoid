#include "mathh.h"

long
power
(
    int x,
    unsigned short y
)
{
    long res = 1;
    unsigned short exp = 0;
    while (exp < y)
    {
        res *= x;
        ++exp;
    }
    return res;
}