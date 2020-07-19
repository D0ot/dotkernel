include ../configs/config.mk

.PHONY : all 

objs = main.o ../libs/log.o ../libs/common.o ../libs/uart.o bitmap.o

asm_objs = start.o

all : kernel.elf kernel.debug

kernel.elf : ${objs} ${asm_objs} kernel.ld
	${LD} ${LINK_FLAGS} -T kernel.ld -o $@ ${objs} ${asm_objs}
	cp $@ ../build/

kernel.debug : kernel.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@
	cp $@ ../debug/

${objs} : %.o : %.c
	${CC} -c $< -o $@ ${CC_FLAGS}

${asm_objs} : %.o : %.s
	${AS} ${AS_ELF_FLAGS} $< -o $@
