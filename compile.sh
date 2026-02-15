#!/bin/bash
gcc -O3 -mavx -c asm/sum_matrix.s -o build/sum_matrix.o
nasm -f elf64 asm/convolve.s -o build/convolve.o
nasm -f elf64 asm/store_convolve.s -o build/store_convolve.o

gcc -g -O3 -mavx -no-pie runner.c build/sum_matrix.o build/convolve.o build/store_convolve.o -o build/2 -lm
#gdb build/2
build/2 Assets/Amazon2.jpg Assets/matrix.txt



