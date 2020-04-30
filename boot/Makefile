include ../configs/config.mk

objects = boot.o 

.PHONY : all obj clean 

all : boot.bin boot.debug

boot.bin : boot.elf
	${OBJCOPY} ${OBJCOPY_GENBIN_FLAGS} $< $@

boot.debug : boot.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@

boot.elf : ${objects} boot.ld
	${LD} ${LINK_FLAGS} -T boot.ld -o $@ ${objects}


${objects} : %.o : %.asm
	${AS} ${AS_ELF_FLAGS} $< -o $@ 


obj : ${objects}

clean : 
	${CLEAN_CMD}

