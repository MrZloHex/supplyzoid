#include "mathh.h"

unsigned long
upower
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