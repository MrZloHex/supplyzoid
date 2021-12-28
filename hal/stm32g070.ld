
ENTRY(_start)

_estack = ORIGIN(RAM) + LENGTH(RAM);


MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 36K
  FLASH    (rx)    : ORIGIN = 0x8000000,   LENGTH = 128K
}


SECTIONS
{
    .text : { obj/main.asm.o }
}