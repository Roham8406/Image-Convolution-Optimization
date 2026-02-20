#!/bin/bash
gcc -O3 -mavx -c asm/sum_matrix.s -o build/sum.o
nasm -f elf64 asm/convolve.s -o build/convolve.o
nasm -f elf64 asm/judge.s -o build/judge.o

gcc -O0 -mavx -no-pie judge.c build/sum.o build/convolve.o build/judge.o -o build/22 -lm
build/22



