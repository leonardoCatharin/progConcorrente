	.file	"main.c"
	.comm	caixas,8,8
	.globl	numeroDeCaixas
	.data
	.align 4
	.type	numeroDeCaixas, @object
	.size	numeroDeCaixas, 4
numeroDeCaixas:
	.long	2
	.globl	numeroDeClientes
	.align 4
	.type	numeroDeClientes, @object
	.size	numeroDeClientes, 4
numeroDeClientes:
	.long	100000
	.globl	contagemClientes
	.bss
	.align 4
	.type	contagemClientes, @object
	.size	contagemClientes, 4
contagemClientes:
	.zero	4
	.globl	mutex
	.align 32
	.type	mutex, @object
	.size	mutex, 40
mutex:
	.zero	40
	.text
	.globl	delay
	.type	delay, @function
delay:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	jmp	.L2
.L3:
	addl	$1, -4(%rbp)
.L2:
	movl	-4(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.L3
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	delay, .-delay
	.globl	menorFila
	.type	menorFila, @function
menorFila:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, -8(%rbp)
	movl	$1, -4(%rbp)
	jmp	.L5
.L7:
	movq	caixas(%rip), %rcx
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	movl	(%rax), %ecx
	movq	caixas(%rip), %rsi
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rsi, %rax
	movl	(%rax), %eax
	cmpl	%eax, %ecx
	jge	.L6
	movl	-4(%rbp), %eax
	movl	%eax, -8(%rbp)
.L6:
	addl	$1, -4(%rbp)
.L5:
	movl	numeroDeCaixas(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L7
	movl	-8(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	menorFila, .-menorFila
	.globl	verificaTamanhoFilas
	.type	verificaTamanhoFilas, @function
verificaTamanhoFilas:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, -4(%rbp)
	jmp	.L10
.L13:
	movq	caixas(%rip), %rcx
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jle	.L11
	movl	-4(%rbp), %eax
	jmp	.L12
.L11:
	addl	$1, -4(%rbp)
.L10:
	movl	numeroDeCaixas(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L13
	movl	$-1, %eax
.L12:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	verificaTamanhoFilas, .-verificaTamanhoFilas
	.globl	novoCliente_thread
	.type	novoCliente_thread, @function
novoCliente_thread:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L15
.L16:
	movl	$mutex, %edi
	call	pthread_mutex_lock
	movl	$0, %eax
	call	menorFila
	movl	%eax, -4(%rbp)
	movq	caixas(%rip), %rcx
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	movl	(%rax), %edx
	addl	$1, %edx
	movl	%edx, (%rax)
	movl	$mutex, %edi
	call	pthread_mutex_unlock
	movq	caixas(%rip), %rcx
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	addq	$16, %rax
	movq	%rax, %rdi
	call	sem_post
	addl	$1, -8(%rbp)
	movl	$100, %edi
	call	delay
.L15:
	movl	numeroDeClientes(%rip), %eax
	cmpl	%eax, -8(%rbp)
	jl	.L16
	movl	$0, %edi
	call	pthread_exit
	.cfi_endproc
.LFE5:
	.size	novoCliente_thread, .-novoCliente_thread
	.globl	caixa_thread
	.type	caixa_thread, @function
caixa_thread:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movl	%eax, -8(%rbp)
	jmp	.L18
.L23:
	movl	$mutex, %edi
	call	pthread_mutex_lock
	movq	caixas(%rip), %rcx
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jle	.L19
	movq	caixas(%rip), %rcx
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	movl	(%rax), %edx
	subl	$1, %edx
	movl	%edx, (%rax)
	movl	contagemClientes(%rip), %eax
	subl	$1, %eax
	movl	%eax, contagemClientes(%rip)
	movl	$mutex, %edi
	call	pthread_mutex_unlock
	movq	caixas(%rip), %rcx
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	addq	$16, %rax
	movq	%rax, %rdi
	call	sem_wait
	jmp	.L20
.L19:
	movl	$mutex, %edi
	call	pthread_mutex_unlock
.L20:
	movl	$mutex, %edi
	call	pthread_mutex_lock
	movl	$0, %eax
	call	verificaTamanhoFilas
	movl	%eax, -4(%rbp)
	cmpl	$-1, -4(%rbp)
	je	.L21
	movq	caixas(%rip), %rcx
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	movl	(%rax), %edx
	subl	$1, %edx
	movl	%edx, (%rax)
	movl	contagemClientes(%rip), %eax
	subl	$1, %eax
	movl	%eax, contagemClientes(%rip)
	movl	$mutex, %edi
	call	pthread_mutex_unlock
	movq	caixas(%rip), %rcx
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	addq	$16, %rax
	movq	%rax, %rdi
	call	sem_wait
	jmp	.L22
.L21:
	movl	$mutex, %edi
	call	pthread_mutex_unlock
.L22:
	movl	$1000, %edi
	call	delay
.L18:
	movl	contagemClientes(%rip), %eax
	testl	%eax, %eax
	jg	.L23
	movl	$0, %edi
	call	pthread_exit
	.cfi_endproc
.LFE6:
	.size	caixa_thread, .-caixa_thread
	.globl	start
	.type	start, @function
start:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	numeroDeClientes(%rip), %eax
	movl	%eax, contagemClientes(%rip)
	movl	numeroDeCaixas(%rip), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, caixas(%rip)
	movl	$0, -80(%rbp)
	jmp	.L25
.L26:
	movl	$0, -64(%rbp)
	leaq	-64(%rbp), %rax
	addq	$16, %rax
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	sem_init
	movq	caixas(%rip), %rcx
	movl	-80(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	movq	-64(%rbp), %rdx
	movq	%rdx, (%rax)
	movq	-56(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-48(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-40(%rbp), %rdx
	movq	%rdx, 24(%rax)
	movq	-32(%rbp), %rdx
	movq	%rdx, 32(%rax)
	movq	-24(%rbp), %rdx
	movq	%rdx, 40(%rax)
	addl	$1, -80(%rbp)
.L25:
	movl	numeroDeCaixas(%rip), %eax
	cmpl	%eax, -80(%rbp)
	jl	.L26
	leaq	-72(%rbp), %rax
	movl	$0, %ecx
	movl	$novoCliente_thread, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create
	movl	$0, -76(%rbp)
	jmp	.L27
.L28:
	movl	-76(%rbp), %eax
	cltq
	movq	%rax, %rsi
	movq	caixas(%rip), %rcx
	movl	-76(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	addq	$8, %rax
	movq	%rsi, %rcx
	movl	$caixa_thread, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create
	addl	$1, -76(%rbp)
.L27:
	movl	numeroDeCaixas(%rip), %eax
	cmpl	%eax, -76(%rbp)
	jl	.L28
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L29
	call	__stack_chk_fail
.L29:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	start, .-start
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$0, %eax
	call	start
	movl	$0, -4(%rbp)
	jmp	.L31
.L32:
	movq	caixas(%rip), %rcx
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	movq	8(%rax), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join
	addl	$1, -4(%rbp)
.L31:
	movl	numeroDeCaixas(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L32
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
