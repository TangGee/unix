# head.s包含32位保护模式初始化设置代码，时钟中断代码。系统调用终端代码和两个任务的代码。
# 在初始化完成之后程序移动到任务0开始执行，并且在时钟终端控制下进行任务0和1之间的切换。

LATCH = 11930            #定时器出书画技术值 每隔10毫秒发送一次中断请求
SCRN_SEL = 0x18          #屏幕显示内存段选择符
TSS0_SEL = 0X20          #任务0的TSS断选择符
LDT0_SEL = 0x28          #任务0的LDT选择符
TSS1_SEL = 0X30          #任务1的TSS 选择符
LDT1_SEL = 0x38          #任务1的LDT段选择符

.text
startup_32:
#首先加载数据段寄存器DS 堆栈寄存器SS和堆栈指针ESP。所有段的线性基地址都是0
      mov $0x10 ,%eax         #0x10是GDT中数据段选择符
      mov %ax   , %ds
      lss init_stack,%esp 
#在新的位置重新设置IDT和GDT表
      call setup_idt
      call setup_gdt
      mov  $0x10,%eax         #在重新初始化gdt idt之后重新加载所有的段寄存器
      mov  %ax, %ds
      mov  %ax, %es
      mov  %ax, %fs
      mov, %ax, %gs
      lss init_stack,%esp
#设置8253定时芯片。把计数器通道0设置成每个10毫秒向终端控制器发送一个终端请求信号
      movb $0x36,%al          #控制字：设置通道0工作在3，，计数初值采用二进制
      movl $0x43,%edx         #8253芯片控制字寄存器的写端口
      outb %al,%dx
      movl $LATCH,%eax        #初始计数指设置为LATCH 即频率100HZ
      movl $0x40,%edx         #通道0的端口
      outb %al,%dx            #分两次把计数值写入通道
      movb %ah,%al
      outb %al,%dx
#在IDT表第8和第128（0x80）项处分别设置定时中断门描述符和系统调用们描述副
      movl  $0x00080000,%eax      #终端程序属于内核，即EAX高字是内核代码段选择符0x0008
      mov  $timer_inputerupt,%ax  #设置定时中断描述符。取定时终端处理程序
#eax中高地址是0x0008 为内核代码段的选择符  ax为timer_inputerrupt的地址
      movw $0x8E00,%dx            #中断门类型是14（屏蔽终端），特权级0或硬件使用       0x8E00  1000 1110 0000 0000 
# 1000 1110 0000 0000 
      movl $0x08,ecx             #开机时BIOS设置的始终终端向量号8,这里直接使用它
      lea idt(,%eax,8),%esi    #把IDT描述符0x08地址放入ESI中，然后设置描述符
      movl %eax,(%esi)         # 0000 0000 0000 1000 
      movl %edx,4(%esi)
      movw $system_inputerrupt, %ax  #设置系统调用陷阱描述副。取系统调用处理程序
      movw $0xef00,%dx             #陷阱们类型是15,特权级3的程序可执行
      movl $0x80,%ecx              #系统调用向量号是0x80
      lea idt(,%ecx,8),%esi     #把IDT描述副项0x80地址放入ESI中，然后设置该描述
      movl %eax,(%esi)
      movl %edx,4(%esi)

#好了，现在我们为移动到任务0（任务A）中执行来操作对战内容，在对战中人工建立终端
#返回时的场景信息
     pushfl          
     andl $0xffffbfff,(%esp)
     popfl
     movl $TSS0_SEL,%eax
     ltr %ax
     movl $0,current
     sti
     pushl $0x17
     push $init_stack
     pushfl
     pushl $0x0f
     pushl $task0
     iret

setup_gdt:
      lgdt    lgdt_opcode
      ret
#这段代码暂时设置IDT表中所有256个中断描述符都为同一个默认值，均使用默认的终端处理过程ignore_int，设置的具体方法是： 首先在eax和edx寄存器中分别设置好默认终端描述符0-3字节和4-7字节，然后利用该寄存器对循环往IDT表中填充默认的终端描述副内容。
setup_idt:                      #把所有256终端们描述副都设置成默认处理过程
      lea ignore_int,%edx       #设置方法与设置定时终端门描述副
      movl $0x00080000,%eax     #选择副为0x008
      movw %dx,%ax
      movw $0x8E00,%dx          #终端类型 特权级为0
      lea idt,%edi          
      mov $256,ecx              #循环设置所有256个们描述副项
rp_idt: mov %eax, (edi)
      mov %edx,4(%edi)
      addl $8,%edi
      dec %ecx
      jne rp_idt
      lidt lidt_opcode
      ret 


write_char:
     push %gs         #首先保存要用到的寄存器，EAX由调用者负责保存
     pushl %ebx
     mov $SCRN_SEL, %ebx 
     mov %bx, %gs 
     movl scr_loc,%bx      #再从变量src_loc中取目前字符显示位置
     shl $1,%ebx    #因为在屏幕上每个字符还有一个属性字节，因此字符
     movb %al,%gs:(%ebx)   #实际显示位置对应显示内存便宜要x2 
     shr $1,%ebx          #把字符显示到内存后把位置处以2加1,此时的位置值对应
     incl %ebx            #下一个位置。如果该位置大于2000 则复位为0
     cmpl $2000,%ebx
     jb 1f
     mov $0,%eax
 1:  movl %ebx,src_loc
     popl %ebx
     pop  %gs
     ret
    
.align 2
ignore_int:
       push  %ds
       pushl %eax
       movl  $0x10,%eax     #首先让DS指向内核数据段，因为终端程序属于内核。
       mov  %ax, %ds
       movl $67,%eax
       call write_char
       popl %eax
       pop  %ds
       iret

#这里是定时中断处理程序
.align 2
time_interrupt:
  push %ds
  pushl %eax
  movl $0x10,%eax
  mov  %ax,%ds
  movb $0x20,%al
  outb %al,$0x20
  movl $1,%eax
  cmpl %eax,current
  je 1f
  movl %eax,current
  ljmp $TSS1_SEL ,$0
  jmp 2f
1:movl $0,current
  ljmp $TSS0_SEL,$0
2:popl %eax
  pop %ds
  iret
  
 
.align 2
lidt_opcode:
   .word 256*8-1
   .long idt
lgdt_opcode:
    .word (end_gdt-gdt)-1   #加载GDTR寄存器的6字节操作数：段长度和基地址
    .long gdt               #

.glign 3
idt: .fill 256,8,0

gdt:
   .quad  0x0000000000000000     #GDT表。第一个描述符不用
  
   .quad  0x00c09a00000007ff     #第二个内核代码描述符，其选择符是0x08
#g=1 单位4k D/B=1 avl=0 段限长7ff=8m P=1在内存中 DPL=0 内核 s=1内核代码 type=1010 代码 执行可读  基地址0x0 
#0000 0000 1100 0000 1001 1010 0000 0000 0000 0000 0000 0000 0000 0111 1111 1111
   .quad  0x00c09200000007ff     #第3个是内核数据段描述符。其选择符是0x10
#g=1 4k D/B=1 avl=0 p=1 dpl=0 s=1 type=0 数据段只读 基地址0 长度8M 
#0000 0000 1100 0000 1001 0010 0000 0000 0000 0000 0000 0000 0000 0111 1111 1111
   .quad  0x00c0920b80000002     #显示内存段描述符
#g=1 4K D/B=1 avl=0 p=1 存在 p=0 级别0 s=1 type=0010 数据 段限长8k 基地址0x800000 
#0000 0000 1100 0000 1001 0010 0000 1010 1000 0000 0000 0000 0000 0000 0000 0010

   .word  0x68, tss0,0xe900,0x0  #TSS0 0x20
   .word  0x40, ldt0,0xe200,0x0  #LDT0 0x28
   .word  0x68, tss1,0xe900,0x0 #TSS1 0x30
   .word  0x40, ldt1,0xe200,0x0  #0x38
end_gdt:
    .fill 128,4,0                #初始内核堆栈空间





init_stack:                      #初始内核堆栈空间。
     .long init_stack            #刚进入保护模式时用于加载SS：ESP堆栈指针，堆栈SS：堆栈段便宜位置       
     .word  0x10                 #堆栈段同内核数据段
     
#下面是任务0的LDT段表中的局部描述符
.align 3
ldt0:      .quad 0x0000000000000000
#g=1 4k B=1 p=1存在 特权级别3 S=1 数据或者代码 type=1010 代码段只读 描述符0000 0000 0000 1111= 0x0f  8M
#0000 0000 1100 0000 1111 1010 0000 0000 0000 0000 0000 0000 0000 0011 1111 1111
           .quad 0x00c0fa00000003ff
#g=1 4k P=1存在 特权3 type=0010 可读写数据 段长8M 描述符0000 0000 0001 0111=0x17
#0000 0000 1100 0000 1111 0010 0000 0000 0000 0000 0000 0000 0000 0011 1111 1111
           .quad 0x00c0f200000003ff 

tss0:      .long 0              /*back link */
           .long krn_stk0,0x10       /*esp ss0*/
           .long 0,0,0,0,0
           .long 0,0,0,0,0
           .long 0,0,0,0,0
           .long 0,0,0,0,0,0
           .long LDT0_SEL,0x8000000 

           .fill 128,4,0
krn_stk0:

.align 3
ldt1:      .quad 0x0000000000000000

#0000 0000 1100 1111
           .quad 0x00c0fa00000003ff
           .quad 0x00c0f200000003ff

tss1:      .long 0
           .long krn_tsk1,0x10
           .long 0,0,0,0,0
#0x0000 0000 0000 0000 0000 0010 0000 00000   DF=1
           .long task1,0x200        
           .long 0,0,0,0
           .long 0x17,0x0f,0x17,0x17,0x17,0x17
           .long LDT1_SEL,0x8000000

           .fill 128,4,0   #128x4 B 
krn_stk1:

#下面是任务0和任务1的程序，他们分别循环显示A和B
task0:
  movl $0x17,%eax
  movw %ax,%ds
  movl $65,%al            #A
  int $0x80
  movl $0xfff,%ecx       
1: loop 1b
  jmp task0

task1:
  movl $66,%al    #B
  int $0x80
  movl $0xfff,%ecx
1: loop 1b
  jmp task1

  .fill 128,4,0
usr_tsk1:

