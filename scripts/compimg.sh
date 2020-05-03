#!/bin/bash

xxd dump.bin > build/dump.bin.txt
xxd build/img.bin > build/img.bin.txt
diff build/dump.bin.txt build/img.bin.txt -y | less