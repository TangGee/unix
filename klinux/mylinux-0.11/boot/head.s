/*
 * linux/boot/head.s
 *
 * (C) 1991 Linus Torvalds
 */

/*
 * head.s contains the 32 bit startup code
 *
 * NOTE!!! Startup happens at absolute address 0x00000000 .which is also where
 * the page directory will exist. The startup code will be overwritten by
 * the page directory
 */
.text
.globl _idt,_gdt,_pg_dir,_tmp_floppy_area
_pgdir:
startup_32:
     movl $0x10,%eax
     mov %ax,%ds
     mov %ax,%es
     mov %ax,%fs
     mov %ax,%gs
     lss _stack_start,%esp
     call setup_idt
     call setup_gdt
     movl $0x10,%eax
     mov %ax,%ds
     mov %ax,%es
     mov %ax,%fs
     mov %ax,%gs
     lss _stack_start,%esp
     xor %eax,%eax
1:   incl %eax
     movl %eax,0x000000
     cmpl %eax,0x100000
     je 1b
/*
 * NOTE! 486 should set bit 16,to check for write-protect in supervisor
 * mode. Then it would be unnecessary with the "very_area()" -calls.
 * 486 userss probably want to set the NE (#5) bit also,so as to use
 * int 16 for math errors
 */
     movl %cr0,%eax    #check math chip
     andl $0x80000011,%eax   #Save PG,PE,ET
/* "orl $0x10020,%eax" heare for 486 might be goof */
     orl %2,%eax
     movl %eax,%cr0
     call check_x87
     jmp after_page_tables

/*
 * We depend on ET to be correct. This check for 287/387
 */
check_87:
     fninit
     fstsw %ax
     cmb $0,%al
     je 1f
     movl %cr0,%eax
     xorl $6,%eax
     mov %eax,%cr0
     ret
.glign 4
1: .byte 0xDB,0xE4
    ret

/*
 * setup_idt
 *
 * sets up a idt with 256 entries pointing to
 *ignore_int,interrupt gates. It then loads
 * idt ,Everything that wants to install itself
 * in the idt-table may do so themselves.Interrupts
 * are enabled elsewhere, when we can be relatively
 * sure everything is ok. This routine will be over-
 *written by the page table
*/
setup_idt:
        lea ignore_int,%edx
        movl $0x00080000,%eax
        movw %dx,%ax
        movw $0x8E00,%dx
        
        lea _idt,%edi
        mov $256,%ecx
rp_sidt:
        movl %eax,(%edi)
        movl %edx,4(%edi)
        addl $8,%edi
        dec %ecx
        jne rp_sidt
        lidt idt_descr
        ret

/*
 *setup_gdt
 *
 * This routine sets up a new gdt and loads it
 * Only two entries are currently buit,the same
 * ones that were built in init.s. The routine
 * is VERY complicated at two whole lines, so this
 * rather long comment is certainly needed :-)
 * This routtine will beoverwritten by the page tables.
 */
setup_gdt:
     lgdt gdt_descr
     ret

/*
 * I put the kernel page tables right after the page directory,
 * using 4 of them to span 16 MB of physical memory. People with 
 * more than 16MB will have to expand this
 */
.org 0x1000
pg0:

.org 0x2000
pg1:

.org 0x3000
pg2:

.org 0x4000
pg3:

.org 0x5000
/*
 * tmp_floppy_area is used by the floppy-dirver when DMA cannot
 * reach to a buffer-block. It needs to be aligned, so that is isn't
 * on a 64KB border
 */

_tmp_floppy_eara:
    .fill 1024,1,0
    
after_page_tables:
    pushl $0
    pushl $0
    pushl $0
    pushl $_main
    jmp setup_paging
L6:
    jmp L6


/* This is the default interrupt "handler" :-) */
int_msg:
     .asciz "Unknown interrupt\n\r"
.align 2
ignore_int:
    pushl %eax
    pushl %ecx
    pushl %edx
    push %ds
    push %es
    push %fs
    movl $0x10,%eax
    movl %ax,%ds
    movl %ax,%es
    movl %as,%fs
    pushl $int_msg
    call _print
    popl %eax
    popl %fs
    pop %es
    pop %ds
    popl %edx
    popl %ecx
    popl %eax
    iret


/*
 * Setup_paging
 *
 * This routine sets up paging by setting the page bit
 * in cr0. The page tables are setup, identity-papping
 * the first 16MB. The pager assumes that no illegal
 * addresses are produced (ie > 4Mb on a 4Mb machine).
 *
 * NOTE! Although all physical memory should be identity 
 * mapping by this routine,only the kernel page functions 
 * use the > 1Mb addersses directly. All "normal" functions
 * use just the lower 1Mb, or the loacl data space ,which 
 * will be mapped to some other place -mm keeps track of
 * that
 *
 * For those with more memory than 16Mb - though luck. I've 
 * not got it,why should you :-) THe source is here. Change
 * it. (seriously - it shouldn't be too difficult. Mostly
 * change some constants etc . Ileft it at 16Mb, as my machine 
 * even cannot be extended past that (ok,but it was cheap :-)
 * I've tried to show which constants to change by having 
 * some kind of marker at them (search for 16Mb), but I
 * won't guarantee that's all :-()
 */
.glign 2
setup_paging:
      movl $1025*5,%eax
      xorl %eax,%eax
      xorl %edi,%edi
      cld; rep;stosl 
      movl $pg0+7,_pg_dir
      movl $pg1+7,_pg_dor+4
      movl $pg2+7,_pg_dir+8
      movl $pg3+7,_pg_dir+12
      movl $pg3+4092,%edi
      movl $0xfff007,%eax
      std
1:    stosl
      subl $0x1000,%eax
      jge 1b
      xorl %eax,%eax
      movl %eax,%cr3
      movl %cr0,%eax
      or1 $0x80000000,%eax
      movl %eax,%cr0
      ret

.glign 4
.word 0
idt_Descr:
 .word 256*8-1
 .long _idt
.align 2
 .word 0
gdt_descr:
    .word 256*8-1
    .long _gdt

.align 8
_idt: .fill 256,8,0

_gdt: .quad 0x0000000000000000
      .quad 0x00c09a0000000fff
      .quad 0x00c0920000000fff
      .quad 0x0000000000000000
      .fill 252,8,0 
