makefile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
project_path := $(dir $(makefile_path))
export

include configs/config.mk

.PHONY : all clean debug test testconf

all : config
	cd boot && make 
	cd osloader && make
	cd kernel && make

	# loader consists of mbr and osloader
	dd if=/dev/zero of=${IMG_BIN} bs=512 count=20480 conv=notrunc
	dd if=boot/boot.bin of=${IMG_BIN} bs=512 count=1 conv=notrunc seek=0
	dd if=osloader/osloader.bin of=${IMG_BIN} bs=512 conv=notrunc seek=1

	# kernel elf file
	dd if=kernel/kernel.elf of=${IMG_BIN} bs=512 conv=notrunc seek=1024

reall : 
	make clean
	make all

test :
	cd test/build && make

testconf :
	cd test && mkdir -p build && cd build && cmake .. -DPROJECT_PATH=${project_path}

config : defs/defs.conf
	python3 scripts/mkdefs.py defs/defs.conf defs/defs.s defs/defs.h

qdebug: export DOTKERNEL_PATH = $(project_path)
qdebug: 
	gnome-terminal --geometry=80x14+0+1200 -- ./scripts/run_qemu.sh
	gnome-terminal --geometry=130x80+1600+0 -- ./scripts/debugp.sh 

bdebug: export DOTKERNEL_PATH = $(project_path)
bdebug: 
	gnome-terminal --geometry=80x14+0+1200 -- ./scripts/run_bochs.sh

gdebug: export DOTKERNEL_PATH = $(project_path)
gdebug: 
	gnome-terminal --geometry=80x14+0+1200 -- ./scripts/run_bochs-gdb.sh
	gnome-terminal --geometry=130x80+1600+0 -- ./scripts/debugp.sh 


clean:
	${CLEAN_CMD}

