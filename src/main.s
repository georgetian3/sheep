	.file	"main.c"
	.text
	.section .rdata,"dr"
.LC0:
	.ascii "Buttons\0"
	.text
	.globl	WinMain
	.def	WinMain;	.scl	2;	.type	32;	.endef
	.seh_proc	WinMain
WinMain:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$224, %rsp
	.seh_stackalloc	224
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	%r8, 32(%rbp)
	movl	%r9d, 40(%rbp)
	pxor	%xmm0, %xmm0
	movaps	%xmm0, -128(%rbp)
	movaps	%xmm0, -112(%rbp)
	movaps	%xmm0, -96(%rbp)
	movaps	%xmm0, -80(%rbp)
	movq	%xmm0, -64(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, -64(%rbp)
	movq	16(%rbp), %rax
	movq	%rax, -104(%rbp)
	movl	$15, %ecx
	movq	__imp_GetSysColorBrush(%rip), %rax
	call	*%rax
	movq	%rax, -80(%rbp)
	leaq	WndProc(%rip), %rax
	movq	%rax, -120(%rbp)
	movl	$32512, %edx
	movl	$0, %ecx
	movq	__imp_LoadCursorA(%rip), %rax
	call	*%rax
	movq	%rax, -88(%rbp)
	leaq	-128(%rbp), %rax
	movq	%rax, %rcx
	movq	__imp_RegisterClassA(%rip), %rax
	call	*%rax
	movq	-64(%rbp), %rax
	movq	$0, 88(%rsp)
	movq	16(%rbp), %rdx
	movq	%rdx, 80(%rsp)
	movq	$0, 72(%rsp)
	movq	$0, 64(%rsp)
	movl	$150, 56(%rsp)
	movl	$230, 48(%rsp)
	movl	$150, 40(%rsp)
	movl	$150, 32(%rsp)
	movl	$282001408, %r9d
	leaq	.LC0(%rip), %r8
	movq	%rax, %rdx
	movl	$0, %ecx
	movq	__imp_CreateWindowExA(%rip), %rax
	call	*%rax
	jmp	.L2
.L3:
	leaq	-48(%rbp), %rax
	movq	%rax, %rcx
	movq	__imp_TranslateMessage(%rip), %rax
	call	*%rax
	leaq	-48(%rbp), %rax
	movq	%rax, %rcx
	movq	__imp_DispatchMessageA(%rip), %rax
	call	*%rax
.L2:
	leaq	-48(%rbp), %rax
	movl	$0, %r9d
	movl	$0, %r8d
	movl	$0, %edx
	movq	%rax, %rcx
	movq	__imp_GetMessageA(%rip), %rax
	call	*%rax
	testl	%eax, %eax
	jne	.L3
	movq	-32(%rbp), %rax
	addq	$224, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC1:
	.ascii "LoadImage failed\0"
.LC2:
	.ascii "button\0"
.LC3:
	.ascii "CreateWindow failed\0"
.LC4:
	.ascii "SendMessage failed\0"
	.text
	.globl	createButton
	.def	createButton;	.scl	2;	.type	32;	.endef
	.seh_proc	createButton
createButton:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	addq	$-128, %rsp
	.seh_stackalloc	128
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movl	%r8d, 32(%rbp)
	movl	%r9d, 40(%rbp)
	movq	24(%rbp), %rax
	movl	$16, 40(%rsp)
	movl	$64, 32(%rsp)
	movl	$64, %r9d
	movl	$0, %r8d
	movq	%rax, %rdx
	movl	$0, %ecx
	movq	__imp_LoadImageA(%rip), %rax
	call	*%rax
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L6
	leaq	.LC1(%rip), %rax
	movq	%rax, %rcx
	call	printf
.L6:
	movq	16(%rbp), %rax
	movl	$-6, %edx
	movq	%rax, %rcx
	movq	__imp_GetWindowLongPtrA(%rip), %rax
	call	*%rax
	movq	$0, 88(%rsp)
	movq	%rax, 80(%rsp)
	movq	$0, 72(%rsp)
	movq	16(%rbp), %rax
	movq	%rax, 64(%rsp)
	movl	$64, 56(%rsp)
	movl	$64, 48(%rsp)
	movl	40(%rbp), %eax
	movl	%eax, 40(%rsp)
	movl	32(%rbp), %eax
	movl	%eax, 32(%rsp)
	movl	$1342177408, %r9d
	movl	$0, %r8d
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdx
	movl	$0, %ecx
	movq	__imp_CreateWindowExA(%rip), %rax
	call	*%rax
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jne	.L7
	leaq	.LC3(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$0, %eax
	jmp	.L8
.L7:
	movq	-8(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %r9
	movl	$0, %r8d
	movl	$247, %edx
	movq	%rax, %rcx
	movq	__imp_SendMessageA(%rip), %rax
	call	*%rax
	movq	%rax, -24(%rbp)
	cmpq	$0, -24(%rbp)
	jne	.L9
	leaq	.LC4(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$0, %eax
	jmp	.L8
.L9:
	movq	-16(%rbp), %rax
.L8:
	subq	$-128, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC5:
	.ascii "../res/1.bmp\0"
.LC6:
	.ascii "../res/2.bmp\0"
.LC7:
	.ascii "%d\12\0"
	.text
	.globl	WndProc
	.def	WndProc;	.scl	2;	.type	32;	.endef
	.seh_proc	WndProc
WndProc:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	%edx, 24(%rbp)
	movq	%r8, 32(%rbp)
	movq	%r9, 40(%rbp)
	cmpl	$273, 24(%rbp)
	je	.L11
	cmpl	$273, 24(%rbp)
	ja	.L12
	cmpl	$1, 24(%rbp)
	je	.L13
	cmpl	$2, 24(%rbp)
	je	.L14
	jmp	.L12
.L13:
	movq	16(%rbp), %rax
	movl	$10, %r9d
	movl	$10, %r8d
	leaq	.LC5(%rip), %rdx
	movq	%rax, %rcx
	call	createButton
	movq	%rax, -8(%rbp)
	movq	16(%rbp), %rax
	movl	$20, %r9d
	movl	$20, %r8d
	leaq	.LC6(%rip), %rdx
	movq	%rax, %rcx
	call	createButton
	movq	%rax, -16(%rbp)
	jmp	.L12
.L11:
	movq	32(%rbp), %rax
	movzwl	%ax, %eax
	movl	%eax, %edx
	leaq	.LC7(%rip), %rax
	movq	%rax, %rcx
	call	printf
	jmp	.L12
.L14:
	movl	$0, %ecx
	movq	__imp_PostQuitMessage(%rip), %rax
	call	*%rax
	nop
.L12:
	movq	40(%rbp), %r8
	movq	32(%rbp), %rcx
	movl	24(%rbp), %edx
	movq	16(%rbp), %rax
	movq	%r8, %r9
	movq	%rcx, %r8
	movq	%rax, %rcx
	movq	__imp_DefWindowProcA(%rip), %rax
	call	*%rax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders) 12.1.0"
	.def	printf;	.scl	2;	.type	32;	.endef
