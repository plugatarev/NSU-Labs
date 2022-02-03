is_digit(char*):
.L2:
        movb    (%rdi), %al
        testb   %al, %al
        je      .L7
        subl    $48, %eax
        incq    %rdi
        cmpb    $9, %al
        jbe     .L2
        xorl    %eax, %eax
        ret
.L7:
        movb    $1, %al
        ret
exhibitor(double, unsigned long):
        movaps  %xmm0, %xmm3
        movsd   .LC0(%rip), %xmm0
        movl    $1, %eax
        movaps  %xmm0, %xmm2
.L9:
        cmpq    %rdi, %rax
        ja      .L13
        testq   %rax, %rax
        js      .L10
        cvtsi2sdq       %rax, %xmm1
        jmp     .L11
.L10:
        movq    %rax, %rdx
        movq    %rax, %rcx
        shrq    %rdx
        andl    $1, %ecx
        orq     %rcx, %rdx
        cvtsi2sdq       %rdx, %xmm1
        addsd   %xmm1, %xmm1
.L11:
        movaps  %xmm3, %xmm4
        incq    %rax
        divsd   %xmm1, %xmm4
        mulsd   %xmm4, %xmm2
        addsd   %xmm2, %xmm0
        jmp     .L9
.L13:
        ret
foo():
        ret
.LC1:
        .string "incorect data!"
.LC2:
        .string "%lf"
main:
        pushq   %r12
        movl    %edi, %edx
        pushq   %rbp
        pushq   %rcx
        movq    8(%rsi), %r8
        movq    %r8, %rdi
        call    is_digit(char*)
        testb   %al, %al
        je      .L16
        movq    16(%rsi), %r12
        movq    %r12, %rdi
        call    is_digit(char*)
        cmpl    $3, %edx
        jne     .L16
        decb    %al
        je      .L17
.L16:
        movl    $.LC1, %edi
        call    puts
        jmp     .L19
.L17:
        movq    %r8, %rdi
        call    atoll
        movq    %r12, %rdi
        movq    %rax, %rbp
        call    atoll
        movq    %rbp, %rdi
        cvtsi2sdq       %rax, %xmm0
        call    exhibitor(double, unsigned long)
        movl    $.LC2, %edi
        movb    $1, %al
        call    printf
.L19:
        popq    %rdx
        xorl    %eax, %eax
        popq    %rbp
        popq    %r12
        ret
.LC0:
        .long   0
        .long   1072693248