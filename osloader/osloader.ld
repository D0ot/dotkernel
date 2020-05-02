OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_osloader_start)

when location of gdt changed, it can be running.

SECTIONS
{
    . = 0x7e00;

    .text :
    {
        osloader.o(.text)
    }

    . = ALIGN(0x10);
    .gdt_data : 
    {
        osloader.o(.gdt_data)
    }

    /* must align it */
    . = ALIGN(16);
    .text2 :
    {
        load.o(.text)
    }

    . = ALIGN(16);
    .text3 :
    {
        uart.o(.text)
    }

    . = ALIGN(4);
    .data :
    {
        *(.data)
    }

    . = ALIGN(4);
    .rodata :
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    . = ALIGN(4);
    .bss :
    {
        *(.bss)
    }


    /DISCARD/ : 
    {
        *(.eh_frame)
    }

   

}