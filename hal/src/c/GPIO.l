#include "byte.h"
#include "gports.h"
#include "GPIO.h"


extern void store(int address, int value);
extern int  load (int address);


byte
cfg_prt_pl_mode(byte port, byte pin, byte mode)
{
    int address;
    if      (port == PORTA)
    {
        address = GPIOA_CRL;
    }
    else if (port == PORTB)
    {
        address = GPIOB_CRL;
    }
    else if (port == PORTC)
    {
        address = GPIOC_CRL;
    }
    else if (port == PORTD)
    {
        address = GPIOD_CRL;
    }
    else if (port == PORTE)
    {
        address = GPIOE_CRL;
    }
    else if (port == PORTF)
    {
        address = GPIOF_CRL;
    }
    else if (port == PORTG)
    {
        address = GPIOG_CRL;
    }
    else
    {
        return 1;
    }
    store(address, 123);

    return 0;
}