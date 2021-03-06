;*************************************************************************
; Universidade Salvador - UNIFACS
;
; Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
; Aluno: Thiago Rocha de Assis        thiagoassis1984@hotmail.com
;
; Processador: Modo Protegido com segmento unico. =>  Flat Model
;                                         gdt => Limite 0xFFFF
;                                                Base   0x0000
;
; Funcionalidade: Ponto de entrada do kernel
;
;
; Sistema iniciado a partir do bootloader do GRUB.
;        1) Ambiente Modo Protegido (Cr0 setado)
;        2) A20 Gate (setado)
;
;
; Creditos:
;    - Parte da implementacao deste START foi baseado na solucao de
;         - F. Brandon. from OsDever community
;
;*************************************************************************
;*************************************************************************
; Faculdade Ruy Barbosa -FRB
;
; Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
; Aluno: R�ad Matto Nassiffe
;
;	Altera��o:
;	Acrescimo de coment�rios.
;	E modifica��es para que esse arquivo pudesse ser compilado como ELF.
;		-Na raiz da pasta boot existem dois arquivos al�m desse, o 
;			coff.txt e elf.txt o coff.txt � para caso o projeto seja compilado,
;	 		como coff e o elf caso ele seja compilado como ELF. 
;*************************************************************************


;Codigo em 32bits

[BITS 32]
SECTION .text

global start
start:
    mov esp, _sys_stack     ;STACK do kernel
    jmp stublet

; Proximas linhas sao usadas para inserir as assinaturas necessarias
; para que o .BIN seja interpretado corretamente pelo GRUB e possa ser
; inicializado pelo mesmo.
ALIGN 4
mboot:
    ;Macros de assinatura do GRUB
    MULTIBOOT_PAGE_ALIGN    equ 1<<0
    MULTIBOOT_MEMORY_INFO   equ 1<<1
    MULTIBOOT_AOUT_KLUDGE   equ 1<<16
    MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_CHECKSUM  equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
    EXTERN code, bss, end

    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM

    dd mboot
    dd code
    dd bss
    dd end
    dd start
; Fim da assinatura do GRUB
stublet:
    extern __main
    call __main
    jmp $

; Inicializa��o da GDT
;  - gdt implementada em C
;+Primeiro carregar a nova GDT, que mais tarde ser� usada no c�digo em C do kernel
;+Depois deve recarregar os registradores de segmento de dados.
;+E por ultimo apontar um novo endere�o para o CS(Code segment)
global _gdt_flush
extern _gp
_gdt_flush:
    lgdt [_gp]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2
flush2:
    ret

;Inicializacao da IDT
; - idt implementada em C
global _idt_load
extern _idtp
_idt_load:
    lidt [_idtp]
    ret

;Inicializacao da TSS

global _load_TSS
_load_TSS
   jmp 0x08:ref_tss
ref_tss:
   mov ax,0x10
   mov ds,ax
   mov es,ax
   mov fs,ax
   mov gs,ax
   mov ss,ax
   mov ax,0x18
   ltr ax
   ret

;salva o seletor   da TSS em AX
global _store_TSS
_store_TSS
   str ax
   ret


;Inicializacao
global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31

; ISR = Interrupt Service Rotine
;  0: Divide By Zero Exception
_isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

;  1: Debug Exception
_isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

;  2: Non Maskable Interrupt Exception
_isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

;  3: Int 3 Exception
_isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

;  4: INTO Exception
_isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

;  5: Out of Bounds Exception
_isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

;  6: Invalid Opcode Exception
_isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

;  7: Coprocessor Not Available Exception
_isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

;  8: Double Fault Exception (With Error Code!)
_isr8:
    cli
    push byte 8
    jmp isr_common_stub

;  9: Coprocessor Segment Overrun Exception
_isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Bad TSS Exception (With Error Code!)
_isr10:
    cli
    push byte 10
    jmp isr_common_stub

; 11: Segment Not Present Exception (With Error Code!)
_isr11:
    cli
    push byte 11
    jmp isr_common_stub

; 12: Stack Fault Exception (With Error Code!)
_isr12:
    cli
    push byte 12
    jmp isr_common_stub

; 13: General Protection Fault Exception (With Error Code!)
_isr13:
    cli
    push byte 13
    jmp isr_common_stub

; 14: Page Fault Exception (With Error Code!)
_isr14:
    cli
    push byte 14
    jmp isr_common_stub

; 15: Reserved Exception
_isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: Floating Point Exception
_isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Alignment Check Exception
_isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

; 18: Machine Check Exception
_isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: Reserved
_isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Reserved
_isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

; 21: Reserved
_isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

; 22: Reserved
_isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

; 23: Reserved
_isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

; 24: Reserved
_isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

; 25: Reserved
_isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub

; 26: Reserved
_isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

; 27: Reserved
_isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

; 28: Reserved
_isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

; 29: Reserved
_isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

; 30: Reserved
_isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

; 31: Reserved
_isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub


extern _fault_handler

;
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, _fault_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

; 32: IRQ0
_irq0:  ;interrupcao do relogio!
    cli 
    jmp irq_clock_handler

; 33: IRQ1
_irq1:
    cli
    push byte 0
    push byte 33
    jmp irq_common_stub

; 34: IRQ2
_irq2:
    cli
    push byte 0
    push byte 34
    jmp irq_common_stub

; 35: IRQ3
_irq3:
    cli
    push byte 0
    push byte 35
    jmp irq_common_stub

; 36: IRQ4
_irq4:
    cli
    push byte 0
    push byte 36
    jmp irq_common_stub

; 37: IRQ5
_irq5:
    cli
    push byte 0
    push byte 37
    jmp irq_common_stub

; 38: IRQ6
_irq6:
    cli
    push byte 0
    push byte 38
    jmp irq_common_stub

; 39: IRQ7
_irq7:
    cli
    push byte 0
    push byte 39
    jmp irq_common_stub

; 40: IRQ8
_irq8:
    cli
    push byte 0
    push byte 40
    jmp irq_common_stub

; 41: IRQ9
_irq9:
    cli
    push byte 0
    push byte 41
    jmp irq_common_stub

; 42: IRQ10
_irq10:
    cli
    push byte 0
    push byte 42
    jmp irq_common_stub

; 43: IRQ11
_irq11:
    cli
    push byte 0
    push byte 43
    jmp irq_common_stub

; 44: IRQ12
_irq12:
    cli
    push byte 0
    push byte 44
    jmp irq_common_stub

; 45: IRQ13
_irq13:
    cli
    push byte 0
    push byte 45
    jmp irq_common_stub

; 46: IRQ14
_irq14:
    cli
    push byte 0
    push byte 46
    jmp irq_common_stub

; 47: IRQ15
_irq15:
    cli
    push byte 0
    push byte 47
    jmp irq_common_stub

extern _irq_handler
extern _irq_handler_clock

irq_common_stub:
    ;Salva todos os valores dos registradores, para recuperar mais tarde.
    pusha
    push ds
    push es
    push fs
    push gs


    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp

    push eax
    mov eax, _irq_handler ;Chama a fun��o irq_handler criada em C, no arquivo Irq.c
    call eax
    pop eax

    ;Recupera os valores dos registradores, salvo no in�cio dessa fun�c�o.
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret


irq_clock_handler:
    ;Salvando registradores na pilha
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
    push ds
    push es
    push fs   
    push gs
    ; BCP deve salvar estes valores
    mov eax,esp
    mov ebx,ebp
    mov [_reg_esp],eax
    mov [_reg_ebp],ebx

    mov esp,_sched_stack_t
    mov ebp,_sched_stack_t;verificar se � isso mesmo, pois esp � ponteiro para pilha e ebp � ponteiro para base da pilha.

    mov ax, 0x10
    mov ds, ax
    mov es, ax      ;No fim da execucao do scheduller
    mov fs, ax      ;_reg_esp e _reg_ebp com valores de retorno
    mov gs, ax
    mov eax, _irq_handler_clock 
    call eax  ;Chama a fun��o irq_handler_clock criada em C, no arquivo Irq.c
    ;Retornando do scheduller. Estamos usando a pilha do SO. Carregar do
    ;novo processo.
    mov eax,[_reg_esp]
    mov esp,eax
    mov eax,[_reg_ebp]
    mov ebp,eax
    ;Pilha do processo. Restaurar valores contidos na pilha de PROC!
    pop gs
    pop fs
    pop es
    pop ds
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
   iret

;Implementacao da funcao responsavel pela troca de contexto.
;Retorno da funcao changeContext leva o processador ao novo processo
;Algoritmo:
;   1. Salva contexto do processo antigo
;   2. Carrega novo contexto
;   3. seta valores de retorno na stack da funcao
;   4. Retorna da fun��o.

global _reg_eax
global _reg_ebx
global _reg_ecx
global _reg_edx
global _reg_edi
global _reg_esi
global _reg_esp
global _reg_ebp
global _reg_cs
global _reg_ds
global _reg_es
global _reg_ss
global _reg_gs
global _reg_fs
global _reg_eip ;IP esta na pilha da fun��o de chamada.
global _reg_ip
global _reg_eflags

global _flag_ctx
_flag_ctx db 0

tempWord0 dd 0
tempWord1 dw 0
tempWord dw 0
tempdouble dd 0
tempstack_t dd 0
tempstack_b dd 0

_reg_eax dd 0
_reg_ebx dd 0
_reg_ecx dd 0
_reg_edx dd 0
_reg_edi dd 0
_reg_ebp dd 0
_reg_esp dd 0
_reg_esi dd 0
_reg_cs dw 0
_reg_ds dw 0
_reg_es dw 0
_reg_ss dw 0
_reg_gs dw 0
_reg_fs dw 0
_reg_eip dd 0
_reg_ip dw 0
_reg_eflags dd 0


extern _tss
extern _tss1
global _atualizar_TSS0_STACK
global _atualizar_TSS1_STACK
_atualizar_TSS0_STACK:
  mov eax,esp
  mov [_tss+4],eax
  ret
_atualizar_TSS1_STACK:
  mov eax,esp
  mov [_tss1+4],eax
  ret


global _load_task
_load_task:
  jmp 0x20:0000
  ret

global _create_common_stack
_create_common_stack:  ;reg_esp\ebp e reg_eip devem estar com valores corretos
  ;Salvar stack do kernel!
  mov eax,esp
  mov [tempstack_t],eax
  mov eax,ebp
  mov [tempstack_b],eax
  ;eflags \ cs \ eip
  mov eax,[_reg_esp]
  mov ebx,[_reg_ebp]
  mov esp,eax
  mov ebp,ebx
  ;Setando pilha com valores de retorno.
  mov eax,0x00000202
  push eax
  mov ax,0x08
  push eax
  mov eax,[_reg_eip]
  push eax
;Carregando valores iniciais
  push eax
  push ebx
  push ecx
  push edx
  push esi
  push edi
  push ds
  push es
  push fs
  push gs
;Salvar ultimos valores dos ponteiros na bcp!
  mov eax,esp
  mov ebx,ebp
  mov [_reg_esp],eax
  mov [_reg_ebp],ebx
;Retornar para Stack do kernel!
  mov eax,[tempstack_t]
  mov esp,eax
  mov eax,[tempstack_b]
  mov ebp,eax
  ret

;Coloca o processado em modo de "espera", at� o proximo evento de IRQ.
global _turnoff
_turnoff:
    hlt
    jmp $
    

;
SECTION .bss
global _sys_stack

    resb 8192               ; STACK SIZE: 8KB
_sys_stack:

global _sched_stack

_sched_stack_base:
    resb 1024               ; STACK SIZE: 1KB
_sched_stack_t:
