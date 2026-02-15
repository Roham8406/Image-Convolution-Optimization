section .note.GNU-stack
global store_convolve_result

section .text
store_convolve_result:
    ; rdi = out pointer
    ; rdx = sum (normalization)

    vmovd xmm4, edx
    vpbroadcastd ymm4, xmm4
    vcvtdq2ps ymm4, ymm4

    ; Normalize: Int -> Float -> Div -> Int
    vcvtdq2ps ymm0, ymm0
    vcvtdq2ps ymm1, ymm1
    vcvtdq2ps ymm2, ymm2
    vcvtdq2ps ymm3, ymm3

    vdivps ymm0, ymm0, ymm4
    vdivps ymm1, ymm1, ymm4
    vdivps ymm2, ymm2, ymm4
    vdivps ymm3, ymm3, ymm4

    vcvtps2dq ymm0, ymm0 ; R (8x 32-bit)
    vcvtps2dq ymm1, ymm1 ; G
    vcvtps2dq ymm2, ymm2 ; B
    vcvtps2dq ymm3, ymm3 ; A

    ; --- THE FIX: INTERLEAVING ---
    ; Interleave R and G
    vpunpckldq ymm4, ymm0, ymm1 ; [R0 G0 R1 G1 | R4 G4 R5 G5]
    vpunpckhdq ymm5, ymm0, ymm1 ; [R2 G2 R3 G3 | R6 G6 R7 G7]

    ; Interleave B and A
    vpunpckldq ymm6, ymm2, ymm3 ; [B0 A0 B1 A1 | B4 A4 B5 A5]
    vpunpckhdq ymm7, ymm2, ymm3 ; [B2 A2 B3 A3 | B6 A6 B7 A7]

    ; Combine into RGBA 32-bit pixels
    vpunpcklqdq ymm0, ymm4, ymm6 ; Pixel 0, 1 (Lower 128) | Pixel 4, 5 (Upper 128)
    vpunpckhqdq ymm1, ymm4, ymm6 ; Pixel 2, 3 (Lower 128) | Pixel 6, 7 (Upper 128)
    
    ; Pack to 8-bit
    ; Note: Since we are writing to a 4-channel image, we pack 32->16->8
    vpackusdw ymm0, ymm0, ymm1
    vpackuswb ymm0, ymm0, ymm0 ; This is a simplified pack for the first 8 pixels
    
    ; Write the final 32 bytes (8 RGBA pixels)
    vmovdqu [rdi], ymm0
    ret