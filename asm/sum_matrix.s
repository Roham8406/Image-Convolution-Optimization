.section .note.GNU-stack,"",@progbits
.intel_syntax noprefix

.data
one:    .float 1.0         
.text
.global sum_matrix
sum_matrix:

    vxorps  ymm0, ymm0, ymm0
    vxorps  ymm1, ymm1, ymm1
    vxorps  ymm2, ymm2, ymm2
    vxorps  ymm3, ymm3, ymm3

loop:
    vmovups ymm2, [rdi]
    vmovups ymm3, [rdi+32]
    vaddps  ymm0, ymm0, ymm2
    vaddps  ymm1, ymm1, ymm3
    add     rdi, 64
    sub     esi, 16
    jg      loop

    vaddps  ymm0, ymm0, ymm1

    vextractf128 xmm1, ymm0, 1  
    vaddps xmm0, xmm0, xmm1
    vhaddps xmm0, xmm0, xmm0
    vhaddps xmm0, xmm0, xmm0

    ret
