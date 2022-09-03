is_digit(char*):
        pushq   %rbp  			;сохранили указатель кадра программы, вызвавшей подпрограмму
					;(поместили в стек значение в регистре rbp)
        movq    %rsp, %rbp 		;rsp - указатель на вершину стека, в rsp поместили rbp 
        movq    %rdi, -24(%rbp)		;(смещаем вершину стека на 24 байта вперед - резервируем место под локальные переменные).
					;s лежит на rdi, rdi кладем в rbp
	movq    $0, -8(%rbp)		;присвоение 0 в i
        jmp     .L2			;переходим к L2
.L6:
        movq    -24(%rbp), %rdx		;на rdx кладем s
        movq    -8(%rbp), %rax		;на rax 0
        addq    %rdx, %rax		;на rax s[0]
        movzbl  (%rax), %eax		
        cmpb    $47, %al		;проверка s[0] >= '0'
        jle     .L3			;->L3
        movq    -24(%rbp), %rdx
        movq    -8(%rbp), %rax
        addq    %rdx, %rax
        movzbl  (%rax), %eax
        cmpb    $57, %al		;проверка s[0] <= '9'
        jle     .L4			;->L4
.L3:
        movl    $0, %eax		;кладем 0 в eax
        jmp     .L5			;->L5
.L4:
        addq    $1, -8(%rbp)		;i++	
.L2:
        movq    -24(%rbp), %rdx		;на rdx кладем s
        movq    -8(%rbp), %rax		;на rax кладем i
        addq    %rdx, %rax		;получили указатель (s+i) в rax
        movzbl  (%rax), %eax		;читаем по адресу rax расширяет в длинное слово и кладет результат в регистр eax.
        testb   %al, %al		;проверка на s[i] != '\0'
        jne     .L6			;проверка пройдена -> L6
        movl    $1, %eax		;не пройдена -> кладем на eax 1
.L5:
        popq    %rbp			;извлечение из стека места продолжения программы
				 
        ret
exhibitor(double, unsigned long):
        pushq   %rbp
        movq    %rsp, %rbp
        movsd   %xmm0, -40(%rbp) 	;поместить содержимое регистра xmm0 по адресу rbp-40(x)
        movq    %rdi, -48(%rbp)		;на rbp n
        movsd   .LC0(%rip), %xmm0	;помещаем 1 в xmm0
        movsd   %xmm0, -8(%rbp)		;записываем 1 на -8(%rbp)	
        movsd   .LC0(%rip), %xmm0	;помещаем 1 в xmm0
        movsd   %xmm0, -16(%rbp)	;записываем 1 на -16(%rbp)
        movq    $1, -24(%rbp)		;i = 1
        jmp     .L8
.L11:
        movq    -24(%rbp), %rax		;i in rax
        testq   %rax, %rax		;сравнение i
        js      .L9
        pxor    %xmm0, %xmm0
        cvtsi2sdq       %rax, %xmm0
        jmp     .L10
.L9: 					;делаем положительное
        movq    %rax, %rdx		;i на rdx
        shrq    %rdx			;i>>1 (i/2)
        andl    $1, %eax		;0 или 1
        orq     %rax, %rdx
        pxor    %xmm0, %xmm0		;затираем
        cvtsi2sdq       %rdx, %xmm0	;convert single int to single double
        addsd   %xmm0, %xmm0		
.L10:
        movsd   -40(%rbp), %xmm1
        divsd   %xmm0, %xmm1		;x / i
        movsd   -8(%rbp), %xmm0
        mulsd   %xmm1, %xmm0		;a*x
        movsd   %xmm0, -8(%rbp)		;a = a * x
        movsd   -16(%rbp), %xmm0	;s+=a;
        addsd   -8(%rbp), %xmm0	
        movsd   %xmm0, -16(%rbp)
        addq    $1, -24(%rbp)		;i++
.L8:
        movq    -24(%rbp), %rax 	;записываем i в rax
        cmpq    -48(%rbp), %rax 	;сравниваем i <= n
        jbe     .L11
        movsd   -16(%rbp), %xmm0	
        movq    %xmm0, %rax		;1 in rax
        movq    %rax, %xmm0		
        popq    %rbp			;извлечение из стека места продолжения программы
        ret
foo():
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $0, -4(%rbp)
        jmp     .L14
.L15:
        addl    $1, -4(%rbp)
.L14:
        cmpl    $99999, -4(%rbp)
        jle     .L15
        nop
        nop
        popq    %rbp
        ret
.LC1:
        .string "incorect data!"
.LC2:
        .string "%lf"
main:
        pushq   %rbp
        movq    %rsp, %rbp
        subq    $32, %rsp		;выделение места под локальные переменные
        movl    %edi, -20(%rbp)		;argc
        movq    %rsi, -32(%rbp)		;argv
        movq    -32(%rbp), %rax		;rax = argv
        addq    $8, %rax		;rax = argv[1]
        movq    (%rax), %rax
        movq    %rax, %rdi
        call    is_digit(char*)		;проверка argv[1] на число
        xorl    $1, %eax
        testb   %al, %al
        jne     .L17			;-> L17 if not equal
        movq    -32(%rbp), %rax
        addq    $16, %rax
        movq    (%rax), %rax		;???
        movq    %rax, %rdi
        call    is_digit(char*)		;проверка argv[2] на число
        xorl    $1, %eax		;???
        testb   %al, %al
        jne     .L17			;-> L17 if not equal
        cmpl    $3, -20(%rbp) 		;проверка argc != 3 
        je      .L18			;->L18 if equal
.L17:		
        movl    $1, %eax
        jmp     .L19
.L18:
        movl    $0, %eax
.L19:
        testb   %al, %al
        je      .L20
        movl    $.LC1, %edi
        call    puts			;incorrect data
        movl    $0, %eax
        jmp     .L21
.L20:
        call    foo()
        movq    -32(%rbp), %rax
        addq    $8, %rax
        movq    (%rax), %rax
        movq    %rax, %rdi
        call    atoll
        movq    %rax, -8(%rbp) 		;n = atoll
        movq    -32(%rbp), %rax
        addq    $16, %rax
        movq    (%rax), %rax
        movq    %rax, %rdi
        call    atoll
        movq    %rax, -16(%rbp)		;x = atoll
        movq    -8(%rbp), %rdx
        pxor    %xmm1, %xmm1
        cvtsi2sdq -16(%rbp), %xmm1 	;преобразование rbp к типу
        movq    %xmm1, %rax
        movq    %rdx, %rdi
        movq    %rax, %xmm0
        call    exhibitor(double, unsigned long) ;xmm0 содержит название программы exhibitor
						 ;xmm0 = exhibitor result

        movq    %xmm0, %rax
        movq    %rax, %xmm0
        movl    $.LC2, %edi
        movl    $1, %eax
        call    printf
        movl    $0, %eax
.L21:
        leave
        ret
.LC0:
        .long   0
        .long   1072693248