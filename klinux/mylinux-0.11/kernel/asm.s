/*
 * linux/kernel/asm.s
 *
 * (C) 1992 Linus Torvalds
 */
/*
 * asm.s contains the low-level code for most hadware faults.
 * page_exception is handled by the mm, so that isn't here .This
 * file also handles (hopefully) fpu-exceptions due to TS-bit, as 
 * the fpu must be properly saved/resored. This hasn't been tested.
 */


.globl _divide_error,_debug,_nmi,_int3,_overflow,_bounds,_invalid_op
.globl _double_fault,coprocessor_segment_overrun
.globl _invalid_TSS,_segment_not_present,_stack_segment
.globl _general_protection,coprocessor_error,_reserved

_divide_error:
     pushl $_do_divide_error
no_error_code:
     xohgl %edx,(%esp)

