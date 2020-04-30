# manuallly set
DIR_PATH=/home/doot/sysmaking/dotkernel

TOOLS_PREFIX = /usr/local/cross/bin/i686-elf-

IMG_BIN = build/img.bin
KERNEL_ELF = build/kernel.elf


CC = ${TOOLS_PREFIX}gcc
CXX = ${TOOLS_PREFIX}g++
LD = ${TOOLS_PREIFX}ld
AS = nasm 
OBJCOPY = ${TOOLS_PREIFX}objcopy

AS_ELF_FLAGS = -f elf32 -F dwarf -g
AS_BIN_FLAGS = -f bin
LINK_FLAGS = -nostdlib -g
CC_FLAGS = -std=gnu11 -ffreestanding -m32 -masm=intel -Wall -Wextra -Wno-address-of-packed-member -g -O0

OBJCOPY_GENBIN_FLAGS = -O binary -S
OBJCOPY_GENSYM_FLAGS = --only-keep-debug

CLEAN_CMD = find . -maxdepth 1 | grep -e  "\.o" -e "\.elf" -e "\.bin" -e "\.debug" | xargs rm -f