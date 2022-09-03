is_digit(char*):
        movl    $0, %edx
        jmp     .L2
.L7:
        addq    $1, %rdx
.L2:
        movzbl  (%rdi,%rdx), %eax
        testb   %al, %al
        je      .L6
        subl    $48, %eax
        cmpb    $9, %al
        jbe     .L7
        movl    $0, %eax
        ret
.L6:
        movl    $1, %eax
        ret
exhibitor(double, unsigned long):
        movl    $1, %eax
        movsd   .LC0(%rip), %xmm3
        movapd  %xmm3, %xmm2
        jmp     .L9
.L10:
        movq    %rax, %rdx
        shrq    %rdx
        movq    %rax, %rcx
        andl    $1, %ecx
        orq     %rcx, %rdx
        pxor    %xmm1, %xmm1
        cvtsi2sdq       %rdx, %xmm1
        addsd   %xmm1, %xmm1
.L11:
        movapd  %xmm0, %xmm4
        divsd   %xmm1, %xmm4
        mulsd   %xmm4, %xmm2
        addsd   %xmm2, %xmm3
        addq    $1, %rax
.L9:
        cmpq    %rdi, %rax
        ja      .L13
        testq   %rax, %rax
        js      .L10
        pxor    %xmm1, %xmm1
        cvtsi2sdq       %rax, %xmm1
        jmp     .L11
.L13:
        movapd  %xmm3, %xmm0
        ret
foo():
        movl    $0, %eax
        jmp     .L15
.L16:
        addl    $1, %eax
.L15:
        cmpl    $99999, %eax
        jle     .L16
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
        testb   %al, %al
        je      .L18
        cmpl    $3, %ebp
        je      .L19
.L18:
        movl    $.LC1, %edi
        call    puts
.L20:
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
        jmp     .L20
.LC0:
        .long   0
        .long   1072693248