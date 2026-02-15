section .note.GNU-stack
section .text
global store_convolve_result

store_convolve_result:
    ; rdi = destination pointer
    ; rdx = normalization divisor

    vmovd           xmm4, edx
    vpbroadcastd    ymm4, xmm4
    vcvtdq2ps       ymm4, ymm4

    vcvtdq2ps       ymm0, ymm0
    vdivps          ymm0, ymm0, ymm4

    vcvttps2dq      ymm0, ymm0 
    vpackusdw       ymm0, ymm0, ymm0
    vextracti128    xmm1, ymm0, 1
    
    vpackuswb       xmm0, xmm0, xmm1
    
    vmovq [rdi], xmm0
    ret