_estack = 0x20008000;

MEMORY
{
    FLASH ( rx )      : ORIGIN = 0x08000000, LENGTH = 128K
    RAM ( rxw )       : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS
{
    .text : { obj/main.asm.o }
}   