.text
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	movl $0, %edx
	movl $1, %eax
	movl $5, %ecx
.Lbb2:
	addl %edx, %eax
	negl %edx
	addl %eax, %edx
	subl $1, %ecx
	cmpl $0, %ecx
	jg .Lbb2
	movl %eax, %esi
	leaq fmt(%rip), %rdi
	movl $0, %eax
	callq printf
	movl $0, %eax
	leave
	ret
.type main, @function
.size main, .-main
/* end function main */

.data
.balign 8
fmt:
	.ascii "%d\n"
	.byte 0
/* end data */

.section .note.GNU-stack,"",@progbits
