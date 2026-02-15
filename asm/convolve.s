section .note.GNU-stack
section .text
global convolve_asm

convolve_asm:
    vmovq           xmm4,	[rsi] 
    vpmovzxbd       ymm4,	xmm4 

    vmovd           xmm9,	edi
    vpbroadcastd    ymm9,	xmm9

    vpmulld         ymm4,	ymm4,	ymm9
    vpaddd          ymm0,	ymm0,	ymm4

    ret