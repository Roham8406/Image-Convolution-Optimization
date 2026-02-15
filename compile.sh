#!/bin/bash
gcc -O3 -mavx -c asm/sum_matrix.s -o build/sum_matrix.o
nasm -f elf64 asm/convolve.s -o build/convolve.o
nasm -f elf64 asm/store_convolve.s -o build/store_convolve.o

gcc -O0 -mavx -no-pie runner.c build/sum_matrix.o build/convolve.o build/store_convolve.o -o build/2 -lm
build/2 Assets/PM.png Assets/matrix.txt


