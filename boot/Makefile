include ../configs/config.mk

objects = boot.o 

.PHONY : all obj 

all : boot.bin boot.debug

boot.bin : boot.elf
	${OBJCOPY} ${OBJCOPY_GENBIN_FLAGS} $< $@

boot.debug : boot.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@
	cp boot.debug ../debug/

boot.elf : ${objects} boot.ld
	${LD} ${LINK_FLAGS} -T boot.ld -o $@ ${objects}


${objects} : %.o : %.s
	${AS} ${AS_ELF_FLAGS} $< -o $@ 


obj : ${objects}


