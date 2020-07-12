OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(kmain)

SECTIONS
{
    . = 0x80000000;

    .text : 
    {
        *(.text)
    }
    
    .rodata : 
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
        *(.common)
    }

    /DISCARD/ : 
    {
        *(.eh_frame)
    }

}