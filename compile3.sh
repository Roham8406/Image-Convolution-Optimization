#!/bin/bash
gcc -O3 -mavx -c asm/sum_matrix.s -o build/sum.o
nasm -f elf64 asm/convolve.s -o build/convolve.o
#nasm -f elf64 -g -F dwarf asm/convolve.s -o build/convolve.o

g++ -g circleDetect.cpp -o build/24 `pkg-config --cflags --libs opencv4`

build/24



