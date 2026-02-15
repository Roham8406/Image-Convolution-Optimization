; rdi = out pointer
; rsi = ch (assume 3)
section .note.GNU-stack
global store_convolve_result

section .text
; rdi = out pointer
; rsi = ch (3)
; rdx = sum
; ymm0/1/2 = R/G/B (32-bit ints)
store_convolve_result:
    ; 1. Normalization & Clamping
    vmovd xmm7, edx
    vcvtsi2ss xmm7, xmm7, edx
    vbroadcastss ymm7, xmm7

    vcvtdq2ps ymm0, ymm0
    vcvtdq2ps ymm1, ymm1
    vcvtdq2ps ymm2, ymm2

    vdivps ymm0, ymm0, ymm7
    vdivps ymm1, ymm1, ymm7
    vdivps ymm2, ymm2, ymm7

    vcvtps2dq ymm0, ymm0
    vcvtps2dq ymm1, ymm1
    vcvtps2dq ymm2, ymm2

    vpxor ymm7, ymm7, ymm7
    vpmaxsd ymm0, ymm0, ymm7
    vpmaxsd ymm1, ymm1, ymm7
    vpmaxsd ymm2, ymm2, ymm7
    vmovdqa ymm7, [rel clamp255]
    vpminsd ymm0, ymm0, ymm7
    vpminsd ymm1, ymm1, ymm7
    vpminsd ymm2, ymm2, ymm7

    ; 2. Pack to 8-bit bytes (8 values per XMM register)
    vpackusdw ymm0, ymm0, ymm0 ; 32->16 bit
    vpackuswb ymm0, ymm0, ymm0 ; 16->8 bit
    vpackusdw ymm1, ymm1, ymm1
    vpackuswb ymm1, ymm1, ymm1
    vpackusdw ymm2, ymm2, ymm2
    vpackuswb ymm2, ymm2, ymm2

    ; Extract the low 128 bits (8 bytes of data in each)
    vmovdqa xmm0, xmm0
    vmovdqa xmm1, xmm1
    vmovdqa xmm2, xmm2

    ; 3. Interleave first 16 bytes (Pixels 0-4 and part of 5)
    vpshufb xmm3, xmm0, [rel mask_r_low]
    vpshufb xmm4, xmm1, [rel mask_g_low]
    vpshufb xmm5, xmm2, [rel mask_b_low]
    vpor xmm3, xmm3, xmm4
    vpor xmm3, xmm3, xmm5
    vmovdqu [rdi], xmm3       ; Store first 16 bytes

    ; 4. Interleave remaining 8 bytes (Pixels 5-7)
    vpshufb xmm0, xmm0, [rel mask_r_high]
    vpshufb xmm1, xmm1, [rel mask_g_high]
    vpshufb xmm2, xmm2, [rel mask_b_high]
    vpor xmm0, xmm0, xmm1
    vpor xmm0, xmm0, xmm2
    vmovq [rdi+16], xmm0      ; Store last 8 bytes (vmovq stores 64 bits)

    ret

section .rodata align=32
clamp255: dd 255,255,255,255,255,255,255,255

; Low masks (for first 16 bytes)
; Mapping: P0:0,1,2 P1:3,4,5 P2:6,7,8 P3:9,10,11 P4:12,13,14 P5:15
mask_r_low: db 0,-1,-1, 1,-1,-1, 2,-1,-1, 3,-1,-1, 4,-1,-1, 5
mask_g_low: db -1,0,-1, -1,1,-1, -1,2,-1, -1,3,-1, -1,4,-1, -1
mask_b_low: db -1,-1,0, -1,-1,1, -1,-1,2, -1,-1,3, -1,-1,4, -1

; High masks (for next 8 bytes starting at offset 16)
; Mapping: P5:0,1 P6:2,3,4 P7:5,6,7
mask_r_high: db -1, 5,-1,-1, 6,-1,-1, 7, -1,-1,-1,-1,-1,-1,-1,-1
mask_g_high: db 5,-1,-1, 6,-1,-1, 7,-1, -1,-1,-1,-1,-1,-1,-1,-1
mask_b_high: db -1,-1, 5,-1,-1, 6,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1