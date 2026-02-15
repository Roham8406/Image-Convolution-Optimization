section .note.GNU-stack
section .text
global store_convolve_result

store_convolve_result:
    ; rdi = destination pointer
    ; rdx = normalization divisor

    ; 1. Convert divisor to float and broadcast
    vmovd xmm4, edx
    vpbroadcastd ymm4, xmm4
    vcvtdq2ps ymm4, ymm4

    ; 2. Convert accumulated sums in ymm0 to float and divide
    vcvtdq2ps ymm0, ymm0
    vdivps ymm0, ymm0, ymm4

    ; 3. Convert back to 32-bit integers (rounding to nearest)
    vcvtps2dq ymm0, ymm0 

    ; 4. Pack 32-bit -> 16-bit (Unsigned Saturate)
    ; This converts 8x 32-bit ints to 8x 16-bit ints in the lower half of each 128-bit lane
    vpackusdw ymm0, ymm0, ymm0
    
    ; 5. Move the high 128 bits to a temporary xmm register to prepare for final pack
    vextracti128 xmm1, ymm0, 1
    
    ; 6. Pack 16-bit -> 8-bit (Unsigned Saturate)
    ; This is the fix! 'us' allows values up to 255.
    vpackuswb xmm0, xmm0, xmm1
    
    ; 7. Store the 8 bytes (2 pixels) to memory
    vmovq [rdi], xmm0
    ret