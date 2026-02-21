#!/bin/bash
gcc -O3 -mavx -c asm/sum_matrix.s -o build/sum.o
nasm -f elf64 asm/convolve.s -o build/convolve.o
#nasm -f elf64 -g -F dwarf asm/convolve.s -o build/convolve.o


gcc -O0 -mavx -no-pie runner.c build/sum.o build/convolve.o -o build/2 -lm
build/2 Assets/Garcia.jpg Assets/matrix.txt
# build/2


