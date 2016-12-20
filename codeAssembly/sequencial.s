	.file	"sequencial.c"
	.comm	caixas,8,8
	.globl	numeroDeCaixas
	.data
	.align 4
	.type	numeroDeCaixas, @object
	.size	numeroDeCaixas, 4
numeroDeCaixas:
	.long	16
	.globl	numeroDeClientes
	.align 4
	.type	numeroDeClientes, @object
	.size	numeroDeClientes, 4
numeroDeClientes:
	.long	10000000
	.text
	.globl	menorFila
	.type	menorFila, @function
menorFila:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, -8(%rbp)
	movl	$1, -4(%rbp)
	jmp	.L2
.L4:
	movq	caixas(%rip), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %edx
	movq	caixas(%rip), %rax
	movl	-8(%rbp), %ecx
	movslq	%ecx, %rcx
	salq	$2, %rcx
	addq	%rcx, %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jge	.L3
	movl	-4(%rbp), %eax
	movl	%eax, -8(%rbp)
.L3:
	addl	$1, -4(%rbp)
.L2:
	movl	numeroDeCaixas(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L4
	movl	-8(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	menorFila, .-menorFila
	.globl	atraso
	.type	atraso, @function
atraso:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	jmp	.L7
.L8:
	addl	$1, -4(%rbp)
.L7:
	movl	-4(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.L8
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	atraso, .-atraso
	.globl	adicionaCliente
	.type	adicionaCliente, @function
adicionaCliente:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, -8(%rbp)
	jmp	.L10
.L11:
	movl	$0, %eax
	call	menorFila
	movl	%eax, -4(%rbp)
	movq	caixas(%rip), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %edx
	addl	$1, %edx
	movl	%edx, (%rax)
	addl	$1, -8(%rbp)
	movl	$100, %edi
	call	atraso
.L10:
	movl	numeroDeClientes(%rip), %eax
	cmpl	%eax, -8(%rbp)
	jl	.L11
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	adicionaCliente, .-adicionaCliente
	.globl	verificaTamanhoFilas
	.type	verificaTamanhoFilas, @function
verificaTamanhoFilas:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, -4(%rbp)
	jmp	.L13
.L16:
	movq	caixas(%rip), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jle	.L14
	movl	-4(%rbp), %eax
	jmp	.L15
.L14:
	addl	$1, -4(%rbp)
.L13:
	movl	numeroDeCaixas(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L16
	movl	$-1, %eax
.L15:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	verificaTamanhoFilas, .-verificaTamanhoFilas
	.globl	atenderClientes
	.type	atenderClientes, @function
atenderClientes:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	jmp	.L18
.L21:
	movl	$0, -4(%rbp)
	jmp	.L19
.L20:
	movq	caixas(%rip), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %edx
	subl	$1, %edx
	movl	%edx, (%rax)
	movl	$1000, %edi
	call	atraso
	addl	$1, -4(%rbp)
.L19:
	movl	numeroDeCaixas(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L20
.L18:
	movl	$0, %eax
	call	verificaTamanhoFilas
	cmpl	$-1, %eax
	jne	.L21
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	atenderClientes, .-atenderClientes
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movl	numeroDeCaixas(%rip), %eax
	cltq
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, caixas(%rip)
	movl	$0, -4(%rbp)
	jmp	.L23
.L24:
	movl	$0, -16(%rbp)
	movq	caixas(%rip), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rax, %rdx
	movl	-16(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -4(%rbp)
.L23:
	movl	numeroDeCaixas(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L24
	movl	$0, %eax
	call	adicionaCliente
	movl	$0, %eax
	call	atenderClientes
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
