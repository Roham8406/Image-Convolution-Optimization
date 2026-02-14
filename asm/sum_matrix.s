.section .note.GNU-stack,"",@progbits
.intel_syntax noprefix

.text
.global sum_matrix
sum_matrix:

    vpxor  ymm0, ymm0, ymm0
    vpxor  ymm1, ymm1, ymm1
    vpxor  ymm2, ymm2, ymm2
    vpxor  ymm3, ymm3, ymm3

loop:
    vmovdqu ymm2, [rdi]
    vmovdqu ymm3, [rdi+32]
    vpaddd  ymm0, ymm0, ymm2
    vpaddd  ymm1, ymm1, ymm3
    add     rdi, 64
    sub     esi, 16
    jg      loop

    vpaddd  ymm0, ymm0, ymm1

    vextracti128 xmm1, ymm0, 1
    vpaddd  xmm0, xmm0, xmm1

    movhlps xmm1, xmm0 
    paddd   xmm0, xmm1
    pshufd  xmm1, xmm0, 1 
    paddd   xmm0, xmm1 
    movd    eax, xmm0

    
    ret
