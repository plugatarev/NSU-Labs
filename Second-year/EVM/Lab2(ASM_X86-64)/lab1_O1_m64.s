is_digit(char*):
        movzbl  (%rdi), %eax
        testb   %al, %al
        je      .L4
        addq    $1, %rdi
.L3:
        subl    $48, %eax
        cmpb    $9, %al
        ja      .L5
        addq    $1, %rdi
        movzbl  -1(%rdi), %eax
        testb   %al, %al
        jne     .L3
        movl    $1, %eax
        ret
.L4:
        movl    $1, %eax
        ret
.L5:
        movl    $0, %eax
        ret
exhibitor(double, unsigned long):
        testq   %rdi, %rdi
        je      .L12
        movl    $1, %eax
        movsd   .LC0(%rip), %xmm3
        movapd  %xmm3, %xmm2
        jmp     .L11
.L9:
        movq    %rax, %rdx
        shrq    %rdx
        movq    %rax, %rcx
        andl    $1, %ecx
        orq     %rcx, %rdx
        pxor    %xmm1, %xmm1
        cvtsi2sdq       %rdx, %xmm1
        addsd   %xmm1, %xmm1
.L10:
        movapd  %xmm0, %xmm4
        divsd   %xmm1, %xmm4
        mulsd   %xmm4, %xmm2
        addsd   %xmm2, %xmm3
        addq    $1, %rax
        cmpq    %rax, %rdi
        jb      .L7
.L11:
        testq   %rax, %rax
        js      .L9
        pxor    %xmm1, %xmm1
        cvtsi2sdq       %rax, %xmm1
        jmp     .L10
.L12:
        movsd   .LC0(%rip), %xmm3
.L7:
        movapd  %xmm3, %xmm0
        ret
foo():
        movl    $100000, %eax
.L15:
        subl    $1, %eax
        jne     .L15
        ret
.LC1:
        .string "incorect data!"
.LC2:
        .string "%lf"
main:
        pushq   %r12
        pushq   %rbp
        pushq   %rbx
        movl    %edi, %ebp
        movq    %rsi, %rbx
        movq    8(%rsi), %r12
        movq    %r12, %rdi
        call    is_digit(char*)
        testb   %al, %al
        je      .L18
        movq    16(%rbx), %rdi
        call    is_digit(char*)
        cmpl    $3, %ebp
        jne     .L18
        cmpb    $1, %al
        je      .L19
.L18:
        movl    $.LC1, %edi
        call    puts
.L21:
        movl    $0, %eax
        popq    %rbx
        popq    %rbp
        popq    %r12
        ret
.L19:
        movl    $10, %edx
        movl    $0, %esi
        movq    %r12, %rdi
        call    strtoll
        movq    %rax, %rbp
        movq    16(%rbx), %rdi
        movl    $10, %edx
        movl    $0, %esi
        call    strtoll
        pxor    %xmm0, %xmm0
        cvtsi2sdq       %rax, %xmm0
        movq    %rbp, %rdi
        call    exhibitor(double, unsigned long)
        movl    $.LC2, %edi
        movl    $1, %eax
        call    printf
        jmp     .L21
.LC0:
        .long   0
        .long   1072693248