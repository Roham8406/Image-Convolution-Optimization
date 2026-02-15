; void convolve_asm(int cell, unsigned char* image,
;                   int channels, int xOffset, int yOffset)
section .note.GNU-stack
global convolve_asm

section .text
; rdi = int cell (kernel value)
; rsi = unsigned char* image (pointer to current 8-pixel window)
; rdx = channels (3)
; rcx = xOffset (unused)
; r8  = yOffset (unused)
; ymm0, ymm1, ymm2 = R, G, B accumulators (preserved across calls)

convolve_asm:
    ; 1. Load 24 bytes (8 pixels * 3 channels)
    vmovdqu xmm3, [rsi]         ; Load first 16 bytes (Pixels 0-4, part of 5)
    vmovq   xmm4, [rsi + 16]    ; Load remaining 8 bytes (Part of 5, Pixels 6-7)

    ; 2. Extract Red Channel (8 bytes)
    vpshufb xmm5, xmm3, [rel mask_r_in_low]   ; Get R from first 16 bytes
    vpshufb xmm6, xmm4, [rel mask_r_in_high]  ; Get R from last 8 bytes
    vpor    xmm5, xmm5, xmm6                  ; Combine: xmm5 now has 8 R bytes

    ; 3. Extract Green Channel (8 bytes)
    vpshufb xmm6, xmm3, [rel mask_g_in_low]
    vpshufb xmm8, xmm4, [rel mask_g_in_high]
    vpor    xmm6, xmm6, xmm8                  ; xmm6 now has 8 G bytes

    ; 4. Extract Blue Channel (8 bytes)
    vpshufb xmm8, xmm3, [rel mask_b_in_low]
    vpshufb xmm9, xmm4, [rel mask_b_in_high]
    vpor    xmm8, xmm8, xmm9                  ; xmm8 now has 8 B bytes

    ; 5. Convert to 32-bit Integers & Multiply
    vmovd xmm7, edi
    vpbroadcastd ymm7, xmm7                   ; Broadcast kernel cell value

    ; Red
    vpmovzxbd ymm4, xmm5
    vpmulld ymm4, ymm4, ymm7
    vpaddd ymm0, ymm0, ymm4

    ; Green
    vpmovzxbd ymm4, xmm6
    vpmulld ymm4, ymm4, ymm7
    vpaddd ymm1, ymm1, ymm4

    ; Blue
    vpmovzxbd ymm4, xmm8
    vpmulld ymm4, ymm4, ymm7
    vpaddd ymm2, ymm2, ymm4

    ret

section .rodata align=32

; LOW MASKS: Look at first 16 bytes. Indices: 0, 3, 6, 9, 12, 15
mask_r_in_low:  db 0, 3, 6, 9, 12, 15, -1, -1,  -1,-1,-1,-1,-1,-1,-1,-1
mask_g_in_low:  db 1, 4, 7, 10, 13, -1, -1, -1, -1,-1,-1,-1,-1,-1,-1,-1
mask_b_in_low:  db 2, 5, 8, 11, 14, -1, -1, -1, -1,-1,-1,-1,-1,-1,-1,-1

; HIGH MASKS: Look at last 8 bytes (offset 16). 
; Pixel 5 G is at index 0, Pixel 5 B is at index 1, Pixel 6 R is at index 2...
mask_r_in_high: db -1, -1, -1, -1, -1, -1, 2, 5,  -1,-1,-1,-1,-1,-1,-1,-1
mask_g_in_high: db -1, -1, -1, -1, -1, 0, 3, 6,  -1,-1,-1,-1,-1,-1,-1,-1
mask_b_in_high: db -1, -1, -1, -1, -1, 1, 4, 7,  -1,-1,-1,-1,-1,-1,-1,-1