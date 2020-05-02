OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_osloader_start)

SECTIONS
{
    . = 0x7e00;

    .text1 :
    {
        osloader.o(.text)
    }

    . = ALIGN(0x10);
    .text2 :
    {
        protected.o(.text)
        load.o(.text)
    }

    .data :
    {
        osloader.o(.data)

        protected.o(.data)
        load.o(.data)
    }

    . = ALIGN(0x200);
    .gdt_data : 
    {
        osloader.o(gdt_data)
    }



}