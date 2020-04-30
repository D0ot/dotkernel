OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(osloader_main)

SECTIONS
{
    . = 0x8000;

    .entry :
    {
        preboot/jumper.o(.text) /* it will  call main() */
    }

    . = ALIGN(4);
    .text :
    {
        *(.text .stub .text.* .gnu.linkonce.t.*)
    }

    .rodata :
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }


    . = ALIGN(4);
    .bss :
    {
        *(.bss)
    }


    . = ALIGN(4);
    .data :
    {
        *(.data)
    }


    .check :
    {
        . = .;
        BYTE(0xcc);
    }

    /DISCARD/ : 
    {
        *(.eh_frame)
    }

}