section .note.GNU-stack
section .text
global judge

; rdi=out, rsi=threshold, rdx=size,

judge:
    push rbx
    xor rax, rax

    dec rsi       
    movzx   ecx, sil
    vmovd   xmm2, ecx
    vpbroadcastb ymm2, xmm2

    vpcmpeqd ymm3, ymm3, ymm3
    vpsllw   ymm3, ymm3, 7 

loop:
    vmovdqu ymm1, [rdi]

    vpxor   ymm4, ymm1, ymm3
    vpxor   ymm5, ymm2, ymm3
    vpcmpgtb ymm6, ymm4, ymm5

    vpmovmskb ebx, ymm6
    popcnt   ebx, ebx
    mov      ebx, ebx
    add      rax, rbx 

    add rdi, 32
    sub rdx, 32
    jg loop

    pop rbx
    ret