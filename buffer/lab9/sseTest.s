	.file	"sseTest.c"
	.text
	.section	.rodata
.LC5:
	.string	"|%g %g| * |%g %g| = |%g %g|\n"
.LC6:
	.string	"|%g %g|   |%g %g|   |%g %g|\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB505:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$448, %rsp
	movl	%edi, -436(%rbp)
	movq	%rsi, -448(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -112(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -104(%rbp)
	movsd	.LC2(%rip), %xmm0
	movsd	%xmm0, -96(%rbp)
	movsd	.LC3(%rip), %xmm0
	movsd	%xmm0, -88(%rbp)
	movsd	.LC2(%rip), %xmm0
	movsd	%xmm0, -80(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -72(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -64(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -56(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -48(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -40(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -32(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	leaq	-48(%rbp), %rax
	movq	%rax, -416(%rbp)
	movq	-416(%rbp), %rax
	movupd	(%rax), %xmm0
	movaps	%xmm0, -352(%rbp)
	leaq	-48(%rbp), %rax
	addq	$16, %rax
	movq	%rax, -424(%rbp)
	movq	-424(%rbp), %rax
	movupd	(%rax), %xmm0
	movaps	%xmm0, -336(%rbp)

	; loop iterator initialization
	movl	$0, -428(%rbp)
	jmp	.L4
.L14:
	; increment loop iterator

	; load a
	movl	-428(%rbp), %eax
	addl	%eax, %eax
	cltq
	leaq	0(,%rax,8), %rdx
	leaq	-112(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -376(%rbp)
	movq	-376(%rbp), %rax
	movupd	(%rax), %xmm0
	movaps	%xmm0, -320(%rbp)

	; load b1
	movl	-428(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	leaq	-80(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -392(%rbp)
	movq	-392(%rbp), %rax
	movsd	(%rax), %xmm0
	movsd	%xmm0, -384(%rbp)
	movsd	-384(%rbp), %xmm0
	unpcklpd	%xmm0, %xmm0
	nop
	movaps	%xmm0, -304(%rbp)

	; load b2
	movl	-428(%rbp), %eax
	cltq
	addq	$2, %rax
	leaq	0(,%rax,8), %rdx
	leaq	-80(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -408(%rbp)
	movq	-408(%rbp), %rax
	movsd	(%rax), %xmm0
	movsd	%xmm0, -400(%rbp)
	movsd	-400(%rbp), %xmm0
	unpcklpd	%xmm0, %xmm0
	nop
	movaps	%xmm0, -288(%rbp)
	movapd	-320(%rbp), %xmm0
	movaps	%xmm0, -176(%rbp)
	movapd	-304(%rbp), %xmm0
	movaps	%xmm0, -160(%rbp)
	movapd	-176(%rbp), %xmm0

	; First multiplication
	mulpd	-160(%rbp), %xmm0
	movapd	-352(%rbp), %xmm1
	movaps	%xmm1, -208(%rbp)
	movaps	%xmm0, -192(%rbp)
	movapd	-208(%rbp), %xmm0

	; First add
	addpd	-192(%rbp), %xmm0
	movaps	%xmm0, -352(%rbp)
	movapd	-320(%rbp), %xmm0
	movaps	%xmm0, -240(%rbp)
	movapd	-288(%rbp), %xmm0
	movaps	%xmm0, -224(%rbp)
	movapd	-240(%rbp), %xmm0

	; Second multiplication
	mulpd	-224(%rbp), %xmm0
	movapd	-336(%rbp), %xmm1
	movaps	%xmm1, -272(%rbp)
	movaps	%xmm0, -256(%rbp)
	movapd	-272(%rbp), %xmm0

	; First add
	addpd	-256(%rbp), %xmm0
	movaps	%xmm0, -336(%rbp)
	addl	$1, -428(%rbp)
.L4:
	; loop header check
	cmpl	$1, -428(%rbp)
	; backward branch
	jle	.L14

	leaq	-48(%rbp), %rax
	movq	%rax, -360(%rbp)
	movapd	-352(%rbp), %xmm0
	movaps	%xmm0, -128(%rbp)
	movapd	-128(%rbp), %xmm0
	movq	-360(%rbp), %rax
	movups	%xmm0, (%rax)
	nop
	leaq	-48(%rbp), %rax
	addq	$16, %rax
	movq	%rax, -368(%rbp)
	movapd	-336(%rbp), %xmm0
	movaps	%xmm0, -144(%rbp)
	movapd	-144(%rbp), %xmm0
	movq	-368(%rbp), %rax
	movups	%xmm0, (%rax)
	nop
	movsd	-32(%rbp), %xmm4
	movsd	-48(%rbp), %xmm3
	movsd	-64(%rbp), %xmm2
	movsd	-80(%rbp), %xmm1
	movsd	-96(%rbp), %xmm0
	movq	-112(%rbp), %rax
	movapd	%xmm4, %xmm5
	movapd	%xmm3, %xmm4
	movapd	%xmm2, %xmm3
	movapd	%xmm1, %xmm2
	movapd	%xmm0, %xmm1
	movq	%rax, %xmm0
	leaq	.LC5(%rip), %rdi
	movl	$6, %eax
	call	printf@PLT
	movsd	-24(%rbp), %xmm4
	movsd	-40(%rbp), %xmm3
	movsd	-56(%rbp), %xmm2
	movsd	-72(%rbp), %xmm1
	movsd	-88(%rbp), %xmm0
	movq	-104(%rbp), %rax
	movapd	%xmm4, %xmm5
	movapd	%xmm3, %xmm4
	movapd	%xmm2, %xmm3
	movapd	%xmm1, %xmm2
	movapd	%xmm0, %xmm1
	movq	%rax, %xmm0
	leaq	.LC6(%rip), %rdi
	movl	$6, %eax
	call	printf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L16
	call	__stack_chk_fail@PLT
.L16:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE505:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1072693248
	.align 8
.LC1:
	.long	0
	.long	1073741824
	.align 8
.LC2:
	.long	0
	.long	1074266112
	.align 8
.LC3:
	.long	0
	.long	1074790400
	.ident	"GCC: (Arch Linux 9.3.0-1) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
