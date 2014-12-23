!
!  setup.s          (C)1991 Linus Torvalds
!
!setup.s is responsible for getting the system data from BIOS,
!and putting them into teh appropriate placese in system
!both setup.s and system has been loaded by the bootblock
!
!This code disk the bios for memory/disk/other,and 
!puts them in a safe place : 0x90000-0x901FF,ie where the bootblock
!used to be.It is then up to the protected mode system to read them from 
!there before the area is overwritten for buffer-block
!
!
!
!NOTE!These had better be the same as in bootsect.s!

INITSEG  = 0x9000
SYSSEG   = 0x1000
SETUPSEG = 0x9020

.globl begtext,begdata,begbss,endtext,enddata,endbss
.text
begtext
.data
begdata
.bss
begbss
.text

entry start
start:

!ok,the read went well so we get current cursor position and save it for
!posterity

    mov ax,#INITSEG
    mov ds,ax
    mov ax,#0x0300
    mov bx,#0x0000
    int 0x10
    mov [0],dx

!get memory size(extended mem,kB)    

    mov ah,#0x88
    int 0x15
    mov [2],ax

!get video-card data
    
    mov ah,#0x0f
    int 0x10
    mov [4],bx   !bh = display page
    mov [6],ax   !ax = vedio mode, ah=window width

! check for EGA/VGA and some config parameters
    
    mov ah,#0x12
    mov bl,#0x10
    int 0x10
    mov [8],ax  !?? Linus is error
    mov [10],bx !bl=display memory  -x---64k 0x01-128k 0x02-192k 0x03-256k
    mov [12],cx !bh 0x00--彩色模式I/O端口0x3bx 0x01--单色 0x3dx

!Get hd0 data

    mov ax,#0x0000
    mov ds,ax
    lds si,[4*0x41]
    mov ax,#INITSEG
    mov es,ax
    mov di,#0x0080
    mov cs,0x10
    rep
    movsb

!Get hd1 data

    mov ax,#0x0000
    mov ds,ax
    mov si,[4*0x46]
    mov ax,#INITSEG
    mov es,#INITSEG
    mov di,#0x0090
    mov cx,#0x10
    rep
    movb

!Check that there IS a hd1 :-)
    
    mov ax,0x1500
    mov dl,#0x81
    int 0x13
    jc  no_disk1
    cmp ah,#3
    je is_disk1
no_disk:
    mov ax,#INITSEG
    mov es,ax
    mov di,#0x0090
    mov cx,0x10
    mov ax,#0x00
    rep 
    stosb
is_disk:
   
!now we want to move to protected mode ...

    cli

!first we move the system to it's rightful place
    
    mov ax, #0x0000
    cld                 !direction=0 ,movs moves forward
do_move:
    mov es,ax
    add ax,#0x1000
    cmp ax,#0x9000
    jz end_move
    mov ds,ax
    sub di,di
    sub si,si
    mov cx,0x8000
    rep
    movsw
    jmp do_move

!then we load the segment descriptors

end_move:
    mov  ax,#SETUPSEG  !right, forgot this at first. didn't work :-)
    mov  ds,ax
    lidt ldt_48
    lgdt lgdt_48

! that  was painless, now we enable A20

   call empty_8042
   mov al,#0xD1     ! 1101 0001
   out #0x64,al
  call empty_8042
   mov al,#0xDF
   mov #0x60,al
   call empty_8042

!well, that went ok,I hope. Now we have to reprogram the interrupts :-)
!we put them right after the intel-reserved hardware interrupt,at
!in 0x20-0x2F,There they won't mess up anything.Sadly IBM really
!messed this up with the original PC,and they heven't been able to
!rectigy it afterwards. Thus the bios puts inputterrupts at 0x08-0x0f
!which is used for the internal hardware interrupt as well.We just 
!have to reprogram the 8259's,and it isn't fun

   mov al,#0x11     !initialization sequence
   out #0x20,al     !send it to 8250A-1
   .word 0x00eb,0x00eb
   out #0xA0,al
   .word 0x00eb,0x00eb
   mov al,#0x20
   out #0x21,al
   .word 0x00eb,0x00eb
   mov al,#0x28
   out #0xA1,al
   .word 0x00eb,0x00eb
   mov al,#0x04
   out #0x21,al
    .word 0x00eb,0x00eb
   mov al,#0x02
   out #0xA1,al
   .word 0x00eb,0x00eb
   mov al,0x01
   out #0x21,al
   .word 0x00eb,0x00eb
   out #0xA1,al
   .word 0x00eb,0x00eb
   mov al,#0xFF
   out #0x21,al
   .word 0x00eb,0x00eb
   out #0xA1,al

!well, that certainly wasn,t fun :-( Hopefully it works, and we don't
!need no steenking BIOS anyway(except fot the initial loading :-))
!The BIOS-routine wants lots of unnecessary data,and it's less
!intersting anyway This is how real programmers do it
!
!Well,now's the time to actually move into protected mode.To make
!things as simple as possible ,we do go register set-up or anything,
!we let the gun-compiled 32-bite programs do that.We just jump to 
!absolute address 0x0000,in 32-bir protected mode

    mov ax,#0x0001   !protected mode (PE) bite
    lmsw ax          !This is it
    jmmpi 0,8        !jmp offset 0 of segment 8 (cs)

!this routine check that the key board commadn queue is empty
!No timeout is used - if this hangs there is somthing wrong with
!the machine,and we probably couldn't proceed anyway
    empty_8042:
    .word 0x00eb,0x00eb
    in al,#0x64
    test al,#2
    jnz empty_8042
    ret

gdt:
    .word 0,0,0,0      !dummy
    
    .word 0x07FF       !8Mb
    .word 0x0000       !base address=0
    .word 0x9A00       !code read/exec
    .word 0x00C0       !granularity=6096
    
    .word 0x07FF
    .word 0x0000
    .word 0x9200
    .word 0x00C0

idt_48:
    .word
    .word 0,0

gdt_48:
    .word 0x800 
    .word 512+gdt,0x9

.text
endtext
.data
enddata
.bss
endbss

