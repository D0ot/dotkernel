include configs/config.mk
.PHONY : all clean debug

all : 
	cd boot && make 
	cd osloader && make
	cd kernel && make

	# loader consists of mbr and osloader
	dd if=/dev/zero of=${IMG_BIN} bs=1024 count=1024 conv=notrunc
	dd if=boot/boot.bin of=${IMG_BIN} bs=512 count=1 conv=notrunc seek=0


debug:
	cp boot/boot.debug debug/
	gnome-terminal --geometry=80x14+0+1200 -- ./scripts/test.sh
	gnome-terminal --geometry=130x80+1600+0 -- ./scripts/debugp.sh 

clean:
	cd boot && make clean
	cd osloader && make clean
	cd kernel && make clean

