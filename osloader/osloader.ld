OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_osloader_start)

SECTIONS
{
    . = 0x7e00;

    .text :
    {
        osloader.o(.text)
    }

    .data :
    {
        osloader.o(.data)
    }

    .gdt_data : 
    {
        osloader.o(.gdt_data)
    }



}