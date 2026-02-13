#!/bin/bash
gcc -O3 -mavx -c asm/sum_matrix.s -o build/sum_matrix.o
gcc -O3 -mavx -no-pie runner.c build/sum_matrix.o -o build/2 -lm
build/2 Assets/Amazon2.jpg Assets/matrix.txt


