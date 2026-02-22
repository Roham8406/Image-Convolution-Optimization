#!/bin/bash
gcc -O3 -mavx -c asm/sum_matrix.s -o build/sum.o
nasm -f elf64 asm/convolve.s -o build/convolve.o
#nasm -f elf64 -g -F dwarf asm/convolve.s -o build/convolve.o

#gcc -O0 -g -mavx -no-pie rect.c build/sum.o build/convolve.o build/judge.o -o build/23 -lm
gcc -O0 -mavx -no-pie rect.c build/sum.o build/convolve.o build/judge.o -o build/23 -lm
#gdb build/23
build/23


