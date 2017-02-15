# x86 体系结构下 系统调用的汇编实现
	.section	.rodata
hello:
	.string "hello world\n"

	.globl	main
main:
	movl	$12, %edx      # length
	movl	$hello, %ecx   # addr
	movl	$1, %ebx       # fd
	movl	$4, %eax       # sys_write
	int	$0x80
	movl	$1, %eax       # sys_exit
	xorl	%ebx, %ebx     # ret num
	int	$0x80
	ret
