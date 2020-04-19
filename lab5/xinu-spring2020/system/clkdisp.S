/* clkdisp.s - clkdisp (x86) */

/*------------------------------------------------------------------------
 * clkdisp  -  Interrupt dispatcher for clock interrupts (x86 version)
 *------------------------------------------------------------------------
 */
#include <icu.s>
		.text
		.globl	clkdisp		# Clock interrupt dispatcher
clkdisp:
		pushal			# Save registers
		cli			# Disable further interrupts
		movb	$EOI,%al	# Reset interrupt
		outb	%al,$OCW1_2

		call	clkhandler	# Call high level handler


    /* Obtain the address of the current procs entry */

    movl currpid, %eax      // Get the currpid value
    movl $proctab, %ebx     // Get the address of the proctab
    movl $76, %edx          // Move the sizeof(struct procent) into edx
    mul  %edx               // eax = currpid * 76
    addl %eax, %ebx         // ebx = eax + ebx  -- get the address of the currpid procent

    /* Ensure that the process has a cb function */

    movl %ebx, %edx         // edx = &proctab[currpid]
    addl $56, %ebx          // ebx = ebx + 56  -- (the offset of prcbvalid)
    movl (%ebx), %eax      // eax should now contain the value of prcbvalid
    cmp  $1, %eax           // check to see if eax is 1
    jne _endclkdisp         // Jump to the end of clkdisp if there
                            // is not a registered callback function

    /* Ensure that the process has a message waiting for it */

    addl $16, %ebx          // ebx = 16 + ebx (offset for prtmpvalid)
                            // ebx = address of prtmpvalid
    movl (%ebx), %eax       // eax = *ebx
    cmp $1, %eax
    jne _endclkdisp         // If prtmpvalid is 0, jump to the end of the function
                            // This means that message has not been received


    /* Reset prtmpvalid to 0, move prtmpbuf to prmbufptr

    movl $0, (%ebx)         // Change prtmpvalid to be 0
    subl $4, %ebx           // ebx = &proctab[currpid].prtmpbuf
    movl (%ebx), %eax       // eax = prtmpbuf
    subl $4, %ebx           // ebx = &proctab[currpid].prmbufptr
    movl %eax, (%ebx)       // *ebx = eax


    /* Manipulate the stack to arrange for the correct return addresses */


    movl $0, %ecx
    movl (%esp), %eax
    pushl %eax

    movl $1, %ecx
    movl 8(%esp), %eax
    movl %eax, 4(%esp)

    movl $2, %ecx
    movl 12(%esp), %eax
    movl %eax, 8(%eax)

    movl $3, %ecx
    movl 16(%esp), %eax
    movl %eax, 12(%esp)

    movl $4, %ecx
    movl 20(%esp), %eax
    movl %eax, 16(%eax)

    movl $5, %ecx
    movl 24(%esp), %eax
    movl %eax, 20(%eax)

    movl $6, %ecx
    movl 28(%esp), %eax
    movl %eax, 24(%eax)

    movl $7, %ecx
    movl 32(%esp), %eax
    movl %eax, 28(%esp)

    // shift EIP, CS, and EFLAGS
    movl $8, %ecx
    movl 36(%esp), %eax
    movl %eax, 32(%eax)   // EIP

    movl $9, %ecx
    movl 40(%esp), %eax

_endclkdisp:
		sti			# Restore interrupt status
		popal			# Restore registers
		iret			# Return from interrupt


