#!/bin/bash

for i in {3..15}
do
    dd if=boot/boot.bin of=build/img.bin bs=512 count=1 conv=notrunc seek=${i}
done
