.text
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $8, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	movl $0, %r12d
	movl $1, %ebx
	movl $10, %r13d
.Lbb2:
	addl %r12d, %ebx
	negl %r12d
	addl %ebx, %r12d
	movl %r12d, %esi
	leaq fmtw(%rip), %rdi
	movl $0, %eax
	callq printf
	subl $1, %r13d
	cmpl $0, %r13d
	jg .Lbb2
	movl $0, %eax
	popq %r13
	popq %r12
	popq %rbx
	leave
	ret
.type main, @function
.size main, .-main
/* end function main */

.data
.balign 8
fmtw:
	.ascii "%d\n"
	.byte 0
/* end data */

.data
.balign 8
fmtd:
	.ascii "%lf\n"
	.byte 0
/* end data */

.section .note.GNU-stack,"",@progbits
