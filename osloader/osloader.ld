OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(osloader_start)

SECTIONS
{
    . = 0x8000;

    .entry :
    {
        osloader.o(.entry) /* it will  call main() */
    }

    .text :
    {
        osloader.o(.text)
    }

    .data :
    {
        osloader.o(.data)
    }


}