include ../configs/config.mk

.PHONY : all main.h

objs = \
	../libs/log.o \
	../libs/kustd.o \
	../libs/uart.o \
	../libs/utils.o \
	../libs/paging.o \
	../arch/x86/interrupt.o \
	../arch/x86/pic8259a.o \
	../arch/x86/pit8254.o \
	bitmap.o buddy.o main.o


asm_objs = start.o ../arch/x86/interrupt_asm.o 

all : kernel.elf kernel.debug kernel.dis

CC_FLAGS_INT = -mno-red-zone -mgeneral-regs-only

kernel.elf : ${objs} ${asm_objs} int_handle.o kernel.ld
	${LD} ${LINK_FLAGS} -T kernel.ld -o $@ ${objs} int_handle.o ${asm_objs}
	cp $@ ../build/

kernel.debug : kernel.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@
	cp $@ ../debug/

kernel.dis : kernel.elf
	${OBJDUMP} ${OBJDUMP_GENDIS_FLAGS} $< > $@

${objs} : %.o : %.c %.h
	${CC} -c $< -o $@ ${CC_FLAGS}

int_handle.o : int_handle.c int_handle.h
	${CC} -c $< -o $@ ${CC_FLAGS} ${CC_FLAGS_INT}

${asm_objs} : %.o : %.s
	${AS} ${AS_ELF_FLAGS} $< -o $@
