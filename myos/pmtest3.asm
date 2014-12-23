;==============================================
; pmtest3.asm
;=============================================

%include      "pm.inc"    ;常量，宏，以及一些说明

org            0100h
               jmp   LABEL_BEGIN

[SECTION .gdt]
;GDT
;
LABEL_GDT:          Descriptor   0  ,            0 ,        0         ;空描述符
LABEL_DESC_NORMAL:  Descriptor   0  ,            0ffffh,    DA_DRW     ; Normal
LABEL_DESC_CODE32:  Descriptor   0  ,      SegCode32Len -1, DAC_DA_32 ;非一致性代码
LABEL_DESC_CODE16:  Descriptor   0  ,            0ffffh,    DA_C      ;非一致性代码
LABEL_DESC_DATA:    Descriptor   0  ,       DataLen -1  ,  DA_DRW + DA_DPL1 ;
LABEL_DESC_STACK:   Descriptor   0  ,       TopOfStack  ,  DA_DRWA + DA_32  ;
LABEL_DESC_LDT :    Descriptor   0  ,       LDTLen -1 ,    DA_LDT   ; LDT
LABEL_DESC_VIDEO:   Descriptor   0B8000h,   0ffffh,        DA_DRW ;显存首地址
;
;GDT结束

GdLten    equ      $-LABEL_GDT  ;GDT长度
GdtPtr    dw       GdtLen - 1   ; GDT 界限
          dd        0           ; GDT 基地址

;GDT 选择子
SelectorNormal      equ     LABEL_DESC_NORMAL         - LABEL_GDT
SelectorCode32      equ     LABEL_DESC_CODE32         - LABEL_GDT  
SelectorCode16      equ     LABEL_DESC_CODE16         - LABEL_GDT
SelectorData        equ     LABEL_DESC_DATA           - LABEL_GDT
SelectorStack       equ     LABEL_DESC_Stack          - LABEL_GDT
SelectorLDT         equ     LABEL_DESC_LDT            - LABEL_GDT
SelectorVideo       equ     LABEL_DESC_VIDEO          - LABEL_GDT
; END of [SECTION .gdt]

[SECTION .data]         ;  数据段
ALING   32
[BITS 32]
LABEL_DATA:
SPAValueInRealMode  dw      0
;字符串
PMMessage:          dw      "In Protect Mode now. ^.^" , 0    ;进入保护模式后显示
;此字符串
OffsetPMMessage     equ      PMMessage - $$ 
StrTest:            db       "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0
OffsetStrTest       equ      StrTest - $$
DataLen             equ      $ - LABEL_DATA
;END OF [SECTION .DATA]

;全局堆栈段
[SECTION .gs]
ALING    32
[BITS 32]
LABEL_STACK:
    times 512 db 0
TopOfStack      equ           $-LABEL_STACK - 1

;END of [SECTION .GS]

[SECTION .S16]
[BITS 16]
LABEL_BEGIN:
    mov    ax , cs
    mov    ds , ax
    mov    es , ax
    mov    ss , ax
    mov    sp , 0100h
  
    mov    [LABEL_GO_BACK_TO_REAL+3], ax
    mov    [SPValueInRealMode] ,      sp
    
   ;初始化 16位代码段描述
    mov    ax , cs
    movzx  eax, ax
    shl    eax, 4
    add    exa, LABEL_SEG_CODE16
    mov    word [LABEL_DESC_CODE16 + 2] , ax
    shr    eax, 16
    mov    byte [LABEL_DESC_CODE16 + 4] . al
    mov    byte [LABEL_DESC_CODE16 + 7] , ah
    
;初始化32位代码段描述符
    xor    eax , eax
    mov    ax  , cs
    shl    eax , 4
    add    eax , LABEL_SEG_CODE32
    mov    word [LABEL_DESC_CODE32 + 2] , ax
    shr    exa ,  16
    mov    byte [LABEL_DESC_CODE32 + 4] , al
    mov    byte [LABEL_DESC_CODE32 + 7] , ah
    
;初始化数据段描述符
    xor    eax ,  eax
    mov    ax  ,  ds
    shl    eax ,  4
    add    eax ,  LABEL_DATA
    mov    word [LABEL_DESC_DATA + 2] , ax
    shr    eax , 16
    mov    byte [LABEL_DESC_DATA + 4] , al
    mov    byte [LABEL_DESC_DATA + 7] , ah

;初始化堆栈段
    xor    eax ,  eax
    mov    ax  ,  ds
    shl    eax ,  4
    add    eax ,  LABEL_STACK
    mov    word [LABEL_DESC_STACK + 2] , ax
    shr    eax , 16
    mov    byte [LABEL_DESC_STACK + 4] , al
    mov    byte [LABEL_DESC_STACK + 7] , ah
 
;初始化GDT 中的描述符
    xor    eax ,  eax
    mov    ax  ,  ds
    shl    eax ,  4
    add    eax ,  LABEL_LDT
    mov    word [LABEL_DESC_LDT + 2] , ax
    shr    eax , 16
    mov    byte [LABEL_DESC_LDT + 4] , al
    mov    byte [LABEL_DESC_LDT + 7] , ah

;初始化LDR中的描述副

    xor    eax ,  eax
    mov    ax  ,  ds
    shl    eax ,  4
    add    eax ,  LABEL_CODE_A
    mov    word [LABEL_DESC_CODEA + 2] , ax
    shr    eax , 16
    mov    byte [LABEL_DESC_CODEA + 4] , al
    mov    byte [LABEL_DESC_CODEA + 7] , ah

;为加载GDTR 作准备
    xor   eax ,  eax
    mov   ax  ,  ds
    shl   eax ,  4
    add   eax ,  LABEL_GDT
    mov   dword [GdtPtr + 2] ,  eax

;加载 GDTR
    lgdt   [GdtPtr]
   
;关中断
    cli

;打开地址线 a20
    in    al , 92h
    or    al , 00000010b
    out   92h, al

;住被切换到保护模式
    mov    eax ,  cr0
    or     eax ,  1
    mov    cr0 ,  eax

;真正进入保护模式
     jmp    dword  SelectorCode32:0  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
