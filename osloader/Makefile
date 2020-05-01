include ../configs/config.mk

objects = osloader.o 

.PHONY : all obj 

all : osloader.bin osloader.debug

osloader.bin : osloader.elf
	${OBJCOPY} ${OBJCOPY_GENBIN_FLAGS} $< $@

osloader.debug : osloader.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@
	cp osloader.debug ../debug/

osloader.elf : ${objects} osloader.ld
	${LD} ${LINK_FLAGS} -T osloader.ld -o $@ ${objects}


${objects} : %.o : %.asm
	${AS} ${AS_ELF_FLAGS} $< -o $@ 


obj : ${objects}


