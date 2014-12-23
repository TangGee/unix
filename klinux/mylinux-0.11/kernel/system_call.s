/*
 *  linux/kernel/system_call.s
 *
 *  (C) 1991 linus Torvalds
 */

/*
 * system_call.s countains the system-call low-level handling routines 
 * This also contains the timer-interrupt handler, as some of the code is
 * the same. The hd- and floppy-interrupts are also hear.
 *
 * Note : this code handles signal-recognition, whic happens every time
 * after a timer interrupt and after each system_call. Oradinary interrupts
 * don't handle signal-recognition. as that world clutter the up totally
 * unnecessarily
 *
 * Stack layout in 'ret_from_sys_call'
 *         
 *         0(%esp) --%eax
 *          4(%esp) --%ebx
 *          8(%esp) --%ecx
 *          C(%esp) --%edx
 *         10(%esp) --%fs
 *         14(%esp) --%es
 *         18(%esp) --%ds
 *         1C(%esp) --%eip
 *         20(%esp) --%cs
 *         24(%esp) --%eflags
 *         28(%esp) --%oldesp
 *         2C(%esp) --%oldss
 */

SIG_CHLD  = 17

EAX       = 0x00
EBX       = 0x04
ECX       = 0x08
EDX       = 0x0C
FS        = 0x10
ES        = 0x14
DS        = 0x18
EIP       = 0x1c
CS        = 0x20
EFLAGS    = 0x24
OLDESP    = 0x28
OLDSS     = 0x2c

state     = 0
counter   = 4
priority  = 8
signal    = 12
sigaction = 16
blocked   = (33*16)

#offsets withi sigaction
sa_handler = 0
sa_mask    = 4
sa_flags   = 8
sa_resrorer = 12

nr_system_calls =72

/*
 * OK , I get parallel printer interrupts while using the floopy for some
 * strange reason. Urgel. Now I just ignore them.
 */
.globl _system_call,_sys_fork,_timer_interrupt,_sys_execve
.globl _hd_interrupt,_floppy_interrupt,_parallel_interrupt
.globl _device_not_available,_coprocessor_error

.align 4
bad_sys_call:
   movl $-1,%eax
   iret
.aglign 4
reschedule:
   pushl $ret_from_sys_call
   jmp _schedule
.aglign 4
_system_call:
    cmpl $nr_system_calls-1,%eax
    ja bad_sys_call 
    push %ds
    push %es
    push %fs
    pushl %edx
    pushl %ecx
    pushl %ebx
    movl $0x10,%edx     #0x0001 0000 内核数据段
    mov %dx,%ds
    mov %dx,%es
    movl $0x17,%edx
    mov  %dx,%fs
    call _sys_call)table(,%eax,4)
    pushl %eax
    movl _current,%eax
    cmpl $0,state(%eax)
    jne reschedule
    cmpl $0,counter(%eax) 
    je reschedule
ret_from_sys_call:
    movl _current,%eax
    cmpl _task,%eax
    je 3f
    cmpw $0x0f,CS(%esp)
    jne 3f
    cmpw $0x17,OLDSS(%esp)
    jne 3f
    mov signal(%eax),%ebx
    movl blocked(%eax,),%ecx
    notl %ecx
    andl %ebx,%ecx
    bsfl %ecx,%ecx
    je 3f
    btrl %ecx,%ebx
    movl %ebx,signal(%eax)
    incl %ecx
    pushl %ecx
    call _do_signal
    popl %eax
3:  popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    pop  %fs
    pop  %es
    pop  %ds
    iret

.align 4
_coprocessor_error:
    push %ds
    push %es
    push %fs
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    movl $0x10,%eax
    mov %ax,%ds
    mov %ax,%es
    movl $0x17,%eax
    mov %ax,%fs
    pushl $ret_from_sys_call
    jmp _math_error

.align 4
_device_not_available:
    push %ds
    push %es
    push %fs
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    movl $0x10,%eax
    mov %ax,%ds
    mov %ax,%es
    movl $0x17,%eax
    mov %ax,%fs
    pushl $ret_from_sys_call
    clts
    movl $cr0,%eax
    testl $0x4,%eax
    je _math_state_restore
    pushl %ebp
    pushl %esi
    pushl %edi
    call _math_emulate
    popl %edi
    popl %esi
    popl %ebp
    ret

.align 2
_timer_interrupt:
    push %ds
    push %es
    push %fs
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    movl $0x10,%eax
    mov %ax,%ds
    mov %ax,%es
    movl $0x17,%eax
    mov %ax,%fs
    incl _jiffies
    movb $0x20,%al
    outb %al,$0x20
    movl CS(%esp),%eax
    andl $3,%eax
    pushl %eax
    call _do_timer
    andl $4,%sp
    jmp ret_from_sys_call

.align 4
_sys_execve:
    lea EIP(%esp),%eax
    pushl %eax
    call _do_execve  
    addl $4,%esp
    ret

.align 4
_sys_fork:
    call _find_empty_process
    testl %eax,%eax
    js 1f
    push %gs
    pushl %esi
    pushl %edi
    pushl %ebp
    pushl %eax
    call _copy_process
    addl %0x20,%esp
1:  ret

_hd_interrupt:
    pushl %eax
    pushl %ecx
    pushl %edx
    push %ds
    push %es
    push %fs
    movl $0x10,%eax
    mov  %ax,%ds
    mov %ax,%es
    movl $0x17,%eax
    mov %ax,%fs
    movb $0x20,%al
    outb %al,$0xA0
    jmp 1f
1:  jmp 1f
1:  xorl %edx,%edx
    xchgl _do_hd,%edx
    testl %edx,%edx
    jne 1f
    movl $_unexpected_hd_interrupt,%edx
1:  outb %al,$0x20
    call %edx
    pop %fs
    pop %es
    pop %ds
    popl %edx
    popl %ecx
    popl %eax
    iret

_floppy_interrupt:
    pushl %eax
    pushl %ecx
    pushl %edx
    push %ds
    push %es
    push %fs
    movl $0x10,%eax
    mov %ax,%ds
    mov %ax,%es
    movl $0x17,%eax
    mov %ax,%fs
    movb $0x20,%al
    outb %al,$0x20
    xorl %eax,%eax
    xchgl _do_floppy,%eax
    testl %eax,%eax
    jne 1f
    movl $_unexpected_floppy_interrupt,%eax
1:  call *%eax
    pop %fs
    pop %es
    pop %ds
    popl %edx
    popl %ecx
    popl %eax
    iret

_parallel_interrupt:
    pushl %eax
    movb $0x20,%al
    outb %al,$0x20
    popl %eax
    iret
    
    
    

    
