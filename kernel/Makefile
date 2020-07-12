include ../configs/config.mk

.PHONY : all 

objs = main.o 


all : kernel.elf kernel.debug

kernel.elf : ${objs} kernel.ld
	${LD} ${LINK_FLAGS} -T kernel.ld -o $@ ${objs} 
	cp $@ ../build/

kernel.debug : kernel.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@
	cp $@ ../debug/

${objs} : %.o : %.c
	${CC} -c $< -o $@ ${CC_FLAGS}


