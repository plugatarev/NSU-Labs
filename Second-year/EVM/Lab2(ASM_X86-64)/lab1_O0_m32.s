is_digit(char*):
        pushl   %ebp
        movl    %esp, %ebp
        subl    $16, %esp
        movl    $0, -4(%ebp)
        jmp     .L2
.L6:
        movl    8(%ebp), %edx
        movl    -4(%ebp), %eax
        addl    %edx, %eax
        movzbl  (%eax), %eax
        cmpb    $47, %al
        jle     .L3
        movl    8(%ebp), %edx
        movl    -4(%ebp), %eax
        addl    %edx, %eax
        movzbl  (%eax), %eax
        cmpb    $57, %al
        jle     .L4
.L3:
        movl    $0, %eax
        jmp     .L5
.L4:
        addl    $1, -4(%ebp)
.L2:
        movl    8(%ebp), %edx
        movl    -4(%ebp), %eax
        addl    %edx, %eax
        movzbl  (%eax), %eax
        testb   %al, %al
        jne     .L6
        movl    $1, %eax
.L5:
        leave
        ret
exhibitor(double, unsigned int):
        pushl   %ebp
        movl    %esp, %ebp
        subl    $48, %esp
        movl    8(%ebp), %eax
        movl    %eax, -40(%ebp)
        movl    12(%ebp), %eax
        movl    %eax, -36(%ebp)
        fld1
        fstpl   -8(%ebp)
        fld1
        fstpl   -16(%ebp)
        movl    $1, -20(%ebp)
        jmp     .L8
.L9:
        movd    -20(%ebp), %xmm0
        movq    %xmm0, -48(%ebp)
        fildq   -48(%ebp)
        fldl    -40(%ebp)
        fdivp   %st, %st(1)
        fldl    -8(%ebp)
        fmulp   %st, %st(1)
        fstpl   -8(%ebp)
        fldl    -16(%ebp)
        faddl   -8(%ebp)
        fstpl   -16(%ebp)
        addl    $1, -20(%ebp)
.L8:
        movl    -20(%ebp), %eax
        cmpl    16(%ebp), %eax
        jbe     .L9
        fldl    -16(%ebp)
        leave
        ret
foo():
        pushl   %ebp
        movl    %esp, %ebp
        subl    $16, %esp
        movl    $0, -4(%ebp)
        jmp     .L12
.L13:
        addl    $1, -4(%ebp)
.L12:
        cmpl    $99999, -4(%ebp)
        jle     .L13
        nop
        nop
        leave
        ret
.LC2:
        .string "incorect data!"
.LC3:
        .string "%lf"
main:
        leal    4(%esp), %ecx
        andl    $-16, %esp
        pushl   -4(%ecx)
        pushl   %ebp
        movl    %esp, %ebp
        pushl   %ebx
        pushl   %ecx
        subl    $16, %esp
        movl    %ecx, %ebx
        movl    4(%ebx), %eax
        addl    $4, %eax
        movl    (%eax), %eax
        pushl   %eax
        call    is_digit(char*)
        addl    $4, %esp
        xorl    $1, %eax
        testb   %al, %al
        jne     .L15
        movl    4(%ebx), %eax
        addl    $8, %eax
        movl    (%eax), %eax
        pushl   %eax
        call    is_digit(char*)
        addl    $4, %esp
        xorl    $1, %eax
        testb   %al, %al
        jne     .L15
        cmpl    $3, (%ebx)
        je      .L16
.L15:
        movl    $1, %eax
        jmp     .L17
.L16:
        movl    $0, %eax
.L17:
        testb   %al, %al
        je      .L18
        subl    $12, %esp
        pushl   $.LC2
        call    puts
        addl    $16, %esp
        movl    $0, %eax
        jmp     .L19
.L18:
        call    foo()
        movl    4(%ebx), %eax
        addl    $4, %eax
        movl    (%eax), %eax
        subl    $12, %esp
        pushl   %eax
        call    atoll
        addl    $16, %esp
        movl    %eax, -12(%ebp)
        movl    4(%ebx), %eax
        addl    $8, %eax
        movl    (%eax), %eax
        subl    $12, %esp
        pushl   %eax
        call    atoll
        addl    $16, %esp
        movl    %eax, -16(%ebp)
        movl    -12(%ebp), %eax
        fildl   -16(%ebp)
        subl    $4, %esp
        pushl   %eax
        leal    -8(%esp), %esp
        fstpl   (%esp)
        call    exhibitor(double, unsigned int)
        addl    $16, %esp
        subl    $4, %esp
        leal    -8(%esp), %esp
        fstpl   (%esp)
        pushl   $.LC3
        call    printf
        addl    $16, %esp
        movl    $0, %eax
.L19:
        leal    -8(%ebp), %esp
        popl    %ecx
        popl    %ebx
        popl    %ebp
        leal    -4(%ecx), %esp
        ret