section .note.GNU-stack
section .text
global convolve_asm

convolve_asm:
    ; rdi = cell weight (int)
    ; rsi = image pointer (unsigned char*)
    
    ; 1. Load 8 bytes (2 pixels: R1G1B1A1 R2G2B2A2)
    ; we use xmm to load, then expand to ymm
    vmovq xmm4, [rsi] 
    
    ; 2. Expand 8 unsigned bytes to 8 32-bit integers
    vpmovzxbd ymm4, xmm4 

    ; 3. Broadcast the kernel cell multiplier (the weight)
    vmovd xmm9, edi
    vpbroadcastd ymm9, xmm9

    ; 4. Multiply each channel by the weight and add to accumulator (ymm0)
    vpmulld ymm4, ymm4, ymm9
    vpaddd ymm0, ymm0, ymm4

    ret