section .note.GNU-stack
section .text
global convolve_asm

; System V AMD64 ABI:
; rdi=mat, rsi=in, rdx=out, rcx=w, r8=ch, r9=deg
; Stack: [rbp+16]=sum, [rbp+24]=centre, [rbp+32]=i, [rbp+40]=j

convolve_asm:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15

    mov r10d, [rbp+16]
    mov r11d, [rbp+24]
    mov r12d, [rbp+32]
    mov r13d, [rbp+40]
    
    vxorps ymm0, ymm0, ymm0
    ; movdqa xmm2, [mask8]

    mov r14d, r11d
    neg r14d
.loop_ki:
    mov r15d, r11d
    neg r15d
.loop_kj:

    ; Matrix Index: (centre + ki) * deg + (centre + kj)
    movsxd rax, r11d
    movsxd rbx, r15d
    add rax, rbx
    imul rax, r9
    
    movsxd rbx, r11d
    movsxd r10, r14d
    add rbx, r10
    add rax, rbx
    
    mov eax, [rdi + rax*4]  ; cell

    ; Image Pointer: ((j + kj) * w + (i + ki)) * ch
    movsxd rbx, r13d
    movsxd r10, r15d
    add rbx, r10
    imul rbx, rcx
    
    movsxd r10, r12d
    add rbx, r10
    movsxd r10, r14d
    add rbx, r10
    
    imul rbx, r8
    
    ; SIMD Math
    vmovq xmm4, [rsi + rbx]
    vpmovzxbd ymm4, xmm4
    vmovd xmm9, eax
    vpbroadcastd ymm9, xmm9
    vpmulld ymm4, ymm4, ymm9
    vpaddd ymm0, ymm0, ymm4

    inc r15d
    cmp r15d, r11d
    jle .loop_kj

    inc r14d
    cmp r14d, r11d
    jle .loop_ki

    ; Store
    mov r10d, [rbp+16]
    vmovd xmm4, r10d
    vpbroadcastd ymm4, xmm4
    vcvtdq2ps ymm4, ymm4
    vcvtdq2ps ymm0, ymm0
    vdivps ymm0, ymm0, ymm4
    vcvtps2dq ymm0, ymm0
    
    vpackusdw ymm0, ymm0, ymm0  
    vextracti128 xmm1, ymm0, 1
    vpackuswb xmm0, xmm0, xmm1
    ; movq rax, xmm0;
    ; movhlps xmm1, xmm0   ; xmm1 lower 64 = xmm0 upper 64
    ; movq rcx, xmm1

    ; Output: (j * w + i) * ch
    movsxd rax, r13d
    imul rax, rcx
    movsxd rbx, r12d
    add rax, rbx
    imul rax, r8 
    
    vpextrd [rdx + rax], xmm0, 0
    add rax, 4
    vpextrd [rdx + rax], xmm0, 2

    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret