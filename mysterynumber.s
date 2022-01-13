### --------------------------------------------------------------------
### mystery.s
### Author: Chris Tralie
### --------------------------------------------------------------------
	.section	.rodata
fmtStr:
	.string	"%i\n"

### --------------------------------------------------------------------
	.section        .data

### --------------------------------------------------------------------
	.section        .bss

### --------------------------------------------------------------------
	.text
	.globl	main
	.type	main, @function
main:
	movl	$3, %esi
        movl    $1, %edi
loop:
        add     %edi, %esi 
        add     $1, %edi
        cmp     $19, %edi
        jl      loop
endloop:
	leaq	fmtStr(%rip), %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	ret
