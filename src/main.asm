.386
.MODEL flat, stdcall
option casemap:none


	.file	"main.c"
	.text
	.section .rdata,"dr"
LC0:
	.ascii "Buttons\0"
	.text
	.globl	_WinMain@16
	.def	_WinMain@16;	.scl	2;	.type	32;	.endef
_WinMain@16:
LFB678:
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	pushl	%ebx
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x78,0x6
	.cfi_escape 0x10,0x3,0x2,0x75,0x7c
	addl	$-128, %esp
	movl	%ecx, %ebx
	pxor	%xmm0, %xmm0
	movups	%xmm0, -76(%ebp)
	movups	%xmm0, -60(%ebp)
	movups	%xmm0, -52(%ebp)
	movl	$LC0, -40(%ebp)
	movl	(%ebx), %eax
	movl	%eax, -60(%ebp)
	movl	$15, (%esp)
	movl	__imp__GetSysColorBrush@4, %eax
	call	*%eax
	subl	$4, %esp
	movl	%eax, -48(%ebp)
	movl	$_WndProc@16, -72(%ebp)
	movl	$32512, 4(%esp)
	movl	$0, (%esp)
	movl	__imp__LoadCursorA@8, %eax
	call	*%eax
	subl	$8, %esp
	movl	%eax, -52(%ebp)
	leal	-76(%ebp), %eax
	movl	%eax, (%esp)
	movl	__imp__RegisterClassA@4, %eax
	call	*%eax
	subl	$4, %esp
	movl	-40(%ebp), %eax
	movl	$0, 44(%esp)
	movl	(%ebx), %edx
	movl	%edx, 40(%esp)
	movl	$0, 36(%esp)
	movl	$0, 32(%esp)
	movl	$150, 28(%esp)
	movl	$230, 24(%esp)
	movl	$150, 20(%esp)
	movl	$150, 16(%esp)
	movl	$282001408, 12(%esp)
	movl	$LC0, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$0, (%esp)
	movl	__imp__CreateWindowExA@48, %eax
	call	*%eax
	subl	$48, %esp
	jmp	L2
L3:
	leal	-36(%ebp), %eax
	movl	%eax, (%esp)
	movl	__imp__TranslateMessage@4, %eax
	call	*%eax
	subl	$4, %esp
	leal	-36(%ebp), %eax
	movl	%eax, (%esp)
	movl	__imp__DispatchMessageA@4, %eax
	call	*%eax
	subl	$4, %esp
L2:
	movl	$0, 12(%esp)
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	leal	-36(%ebp), %eax
	movl	%eax, (%esp)
	movl	__imp__GetMessageA@16, %eax
	call	*%eax
	subl	$16, %esp
	testl	%eax, %eax
	jne	L3
	movl	-28(%ebp), %eax
	leal	-8(%ebp), %esp
	popl	%ecx
	.cfi_restore 1
	.cfi_def_cfa 1, 0
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret	$16
	.cfi_endproc
LFE678:
	.section .rdata,"dr"
LC1:
	.ascii "../res/2.bmp\0"
LC2:
	.ascii "LoadImage failed\0"
LC3:
	.ascii "Beep\0"
LC4:
	.ascii "button\0"
LC5:
	.ascii "CreateWindow failed\0"
LC6:
	.ascii "SendMessage failed\0"
	.text
	.globl	_WndProc@16
	.def	_WndProc@16;	.scl	2;	.type	32;	.endef
_WndProc@16:
LFB679:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$72, %esp
	cmpl	$273, 12(%ebp)
	je	L6
	cmpl	$273, 12(%ebp)
	ja	L7
	cmpl	$1, 12(%ebp)
	je	L8
	cmpl	$2, 12(%ebp)
	je	L9
	jmp	L7
L8:
	movl	$16, 20(%esp)
	movl	$64, 16(%esp)
	movl	$64, 12(%esp)
	movl	$0, 8(%esp)
	movl	$LC1, 4(%esp)
	movl	$0, (%esp)
	movl	__imp__LoadImageA@24, %eax
	call	*%eax
	subl	$24, %esp
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	L10
	movl	$LC2, (%esp)
	call	_printf
	jmp	L7
L10:
	movl	$0, 44(%esp)
	movl	$0, 40(%esp)
	movl	$1, 36(%esp)
	movl	8(%ebp), %eax
	movl	%eax, 32(%esp)
	movl	$100, 28(%esp)
	movl	$80, 24(%esp)
	movl	$100, 20(%esp)
	movl	$20, 16(%esp)
	movl	$1342177408, 12(%esp)
	movl	$LC3, 8(%esp)
	movl	$LC4, 4(%esp)
	movl	$0, (%esp)
	movl	__imp__CreateWindowExA@48, %eax
	call	*%eax
	subl	$48, %esp
	movl	%eax, -16(%ebp)
	cmpl	$0, -16(%ebp)
	jne	L11
	movl	$LC5, (%esp)
	call	_printf
	jmp	L7
L11:
	movl	-12(%ebp), %eax
	movl	%eax, 12(%esp)
	movl	$0, 8(%esp)
	movl	$247, 4(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	movl	__imp__SendMessageA@16, %eax
	call	*%eax
	subl	$16, %esp
	movl	%eax, -20(%ebp)
	cmpl	$0, -20(%ebp)
	jne	L16
	movl	$LC6, (%esp)
	call	_printf
	jmp	L7
L6:
	movl	16(%ebp), %eax
	cmpw	$1, %ax
	jne	L13
	movl	$50, 4(%esp)
	movl	$40, (%esp)
	movl	__imp__Beep@8, %eax
	call	*%eax
	subl	$8, %esp
L13:
	movl	16(%ebp), %eax
	cmpw	$2, %ax
	jne	L17
	movl	$0, (%esp)
	movl	__imp__PostQuitMessage@4, %eax
	call	*%eax
	subl	$4, %esp
	jmp	L17
L9:
	movl	$0, (%esp)
	movl	__imp__PostQuitMessage@4, %eax
	call	*%eax
	subl	$4, %esp
	jmp	L7
L16:
	nop
	jmp	L7
L17:
	nop
L7:
	movl	20(%ebp), %eax
	movl	%eax, 12(%esp)
	movl	16(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	movl	__imp__DefWindowProcA@16, %eax
	call	*%eax
	subl	$16, %esp
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret	$16
	.cfi_endproc
LFE679:
	.ident	"GCC: (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders) 12.1.0"
	.def	_printf;	.scl	2;	.type	32;	.endef
