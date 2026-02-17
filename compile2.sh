#!/bin/bash
gcc -O3 -mavx -c asm/sum_matrix.s -o build/sum.o
nasm -f elf64 asm/convolve.s -o build/convolve.o

gcc -g -O0 -mavx -no-pie judge.c build/sum.o build/convolve.o -o build/22 -lm
build/22



