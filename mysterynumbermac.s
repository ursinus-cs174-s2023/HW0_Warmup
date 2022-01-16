### --------------------------------------------------------------------
### mysterynumbermac.s
### Author: Chris Tralie
### --------------------------------------------------------------------
	.section	__RODATA__,__rodata
.fmtStr:
	.string	"%i\n"

### --------------------------------------------------------------------
	.section   __DATA__,__data

### --------------------------------------------------------------------
	.section   __BSS__,__bss

### --------------------------------------------------------------------
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main

_main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$3, %esi
	movl    $1, %edi
.loop:
	add     %edi, %esi 
	add     $1, %edi
	cmp     $19, %edi
	jl      .loop
.endloop:
	leaq	.fmtStr(%rip), %rdi
	movl	$0, %eax
	callq	_printf
	movl	$0, %eax
	pop		%rbp
	retq
