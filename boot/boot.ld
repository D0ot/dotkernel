OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
    . = 0x7c00;

    entry : AT(0x0)
    {
        boot.o(mbr)
    }

    .text :
    {
        boot.o(.text)
    }

    .data : 
    {
        boot.o(.data)
    }
    
    . = 0x7c00 + 510;
    .mbrcheck :
    {
        BYTE(0x55)
        BYTE(0xaa)
    }



}