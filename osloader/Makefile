include ../configs/config.mk

asm_obj = osloader.o
c_obj = load.o ../libs/uart.o ../libs/kustd.o ../libs/disk.o ../libs/log.o \
		../libs/paging.o ../libs/utils.o

.PHONY : all obj 

all : osloader.bin osloader.debug

osloader.bin : osloader.elf
	${OBJCOPY} ${OBJCOPY_GENBIN_FLAGS} $< $@

osloader.debug : osloader.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@
	cp $@ ../debug/

osloader.elf : ${asm_obj} ${c_obj} osloader.ld
	${LD} ${LINK_FLAGS} -T osloader.ld -o $@ ${asm_obj} ${c_obj}

${asm_obj} : %.o : %.s
	${AS} ${AS_ELF_FLAGS} $< -o $@ 

${c_obj} : %.o : %.c
	${CC} -c $< -o $@ ${CC_FLAGS}

obj : ${asm_obj} ${c_obj}


