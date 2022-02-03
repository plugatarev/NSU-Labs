is_digit(char*):
        movzbl  (%rdi), %eax
        testb   %al, %al
        je      .L4
        addq    $1, %rdi
        jmp     .L3
.L8:
        movzbl  (%rdi), %eax
        addq    $1, %rdi
        testb   %al, %al
        je      .L4
.L3:
        subl    $48, %eax
        cmpb    $9, %al
        jbe     .L8
        xorl    %eax, %eax
        ret
.L4:
        movl    $1, %eax
        ret
exhibitor(double, unsigned long):
        testq   %rdi, %rdi
        je      .L14
        movsd   .LC0(%rip), %xmm3
        movl    $1, %eax
        movapd  %xmm3, %xmm2
.L13:
        pxor    %xmm1, %xmm1
        cvtsi2sdq       %rax, %xmm1
        testq   %rax, %rax
        jns     .L12
        movq    %rax, %rdx
        movq    %rax, %rcx
        pxor    %xmm1, %xmm1
        shrq    %rdx
        andl    $1, %ecx
        orq     %rcx, %rdx
        cvtsi2sdq       %rdx, %xmm1
        addsd   %xmm1, %xmm1
.L12:
        movapd  %xmm0, %xmm4
        addq    $1, %rax
        divsd   %xmm1, %xmm4
        mulsd   %xmm4, %xmm2
        addsd   %xmm2, %xmm3
        cmpq    %rax, %rdi
        jnb     .L13
        movapd  %xmm3, %xmm0
        ret
.L14:
        movsd   .LC0(%rip), %xmm3
        movapd  %xmm3, %xmm0
        ret
foo():
        ret
.LC1:
        .string "incorect data!"
.LC2:
        .string "%lf"
main:
        pushq   %rbp
        pushq   %rbx
        movq    %rsi, %rbx
        subq    $8, %rsp
        movq    8(%rsi), %r8
        movzbl  (%r8), %eax
        testb   %al, %al
        je      .L18
        leaq    1(%r8), %rdx
        jmp     .L20
.L41:
        movzbl  (%rdx), %eax
        addq    $1, %rdx
        testb   %al, %al
        je      .L18
.L20:
        subl    $48, %eax
        cmpb    $9, %al
        jbe     .L41
.L19:
        movl    $.LC1, %edi
        call    puts
.L38:
        addq    $8, %rsp
        xorl    %eax, %eax
        popq    %rbx
        popq    %rbp
        ret
.L18:
        movq    16(%rbx), %rdx
        movzbl  (%rdx), %eax
        addq    $1, %rdx
        testb   %al, %al
        jne     .L21
        jmp     .L22
.L42:
        movzbl  (%rdx), %eax
        addq    $1, %rdx
        testb   %al, %al
        je      .L22
.L21:
        subl    $48, %eax
        cmpb    $9, %al
        jbe     .L42
        jmp     .L19
.L22:
        cmpl    $3, %edi
        jne     .L19
        movq    %r8, %rdi
        movl    $10, %edx
        xorl    %esi, %esi
        call    strtoll
        movq    16(%rbx), %rdi
        movl    $10, %edx
        xorl    %esi, %esi
        movq    %rax, %rbp
        call    strtoll
        pxor    %xmm4, %xmm4
        movsd   .LC0(%rip), %xmm2
        cvtsi2sdq       %rax, %xmm4
        testq   %rbp, %rbp
        je      .L24
        movapd  %xmm2, %xmm1
        movl    $1, %eax
.L27:
        pxor    %xmm0, %xmm0
        cvtsi2sdq       %rax, %xmm0
        testq   %rax, %rax
        jns     .L26
        movq    %rax, %rdx
        movq    %rax, %rcx
        pxor    %xmm0, %xmm0
        shrq    %rdx
        andl    $1, %ecx
        orq     %rcx, %rdx
        cvtsi2sdq       %rdx, %xmm0
        addsd   %xmm0, %xmm0
.L26:
        movapd  %xmm4, %xmm3
        addq    $1, %rax
        divsd   %xmm0, %xmm3
        mulsd   %xmm3, %xmm1
        addsd   %xmm1, %xmm2
        cmpq    %rax, %rbp
        jnb     .L27
.L24:
        movapd  %xmm2, %xmm0
        movl    $.LC2, %edi
        movl    $1, %eax
        call    printf
        jmp     .L38
.LC0:
        .long   0
        .long   1072693248