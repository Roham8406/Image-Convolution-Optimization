#!/bin/bash
#gcc -O3 -mavx -c asm/sum_matrix.s -o build/sum.o
nasm -f elf64 asm/sum_matrix.s -o build/sum.o
nasm -f elf64 asm/convolve.s -o build/convolve.o
g++ -O3 -mavx -no-pie circleDetect.cpp build/sum.o build/convolve.o -o build/24 `pkg-config --cflags --libs opencv4`
build/24



