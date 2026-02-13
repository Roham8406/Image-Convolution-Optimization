.section .note.GNU-stack,"",@progbits
.intel_syntax noprefix

.data
one:    .float 1.0         # use .float instead of dd

.text
.global sum_matrix
sum_matrix:
    movss xmm0, [one]
    ret
