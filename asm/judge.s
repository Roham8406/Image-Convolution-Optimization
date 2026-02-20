section .note.GNU-stack
section .text
global judge

; rdi=out, rsi=threshold, rdx=size,

judge:
    push rbx

    dec rsi       
    movzx   ecx, sil
    vmovd   xmm2, ecx
    vpbroadcastb ymm2, xmm2

    mov      eax, 0x80
    vmovd    xmm3, eax
    vpbroadcastb ymm3, xmm3
    xor rax, rax

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