include ../configs/config.mk

asm_obj = osloader.o
c_ojb = load.o



.PHONY : all obj 

all : osloader.bin osloader.debug

osloader.bin : osloader.elf
	${OBJCOPY} ${OBJCOPY_GENBIN_FLAGS} $< $@

osloader.debug : osloader.elf
	${OBJCOPY} ${OBJCOPY_GENSYM_FLAGS} $< $@
	cp osloader.debug ../debug/

osloader.elf : ${asm_obj} ${c_ojb} osloader.ld
	${LD} ${LINK_FLAGS} -T osloader.ld -o $@ ${asm_obj} ${c_ojb}


${asm_obj} : %.o : %.asm
	${AS} ${AS_ELF_FLAGS} $< -o $@ 

${c_ojb} : %.o : %.c
	${CC} -c $< -o $@ ${CC_FLAGS}

obj : ${asm_obj} ${c_ojb}


