#!/bin/bash

xxd dump.bin > dump.bin.txt
xxd build/img.bin > build/img.bin.txt
diff dump.bin.txt build/img.bin.txt -y | less