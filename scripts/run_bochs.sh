#!/bin/bash

bochs -f ${DOTKERNEL_PATH}/scripts/bochsrc

# used for debug serial port 
# qemu-system-i386 -hda build/img.bin -gdb tcp::1234 -S -serial stdio 