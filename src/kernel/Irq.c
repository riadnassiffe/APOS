/*
 IRQ  
*/
#include "include/Irq.h"

/**
 * 
 */
void *irq_routines[16] = {0, 0, 0, 0, 0, 0, 0, 0,
						  0, 0, 0, 0, 0, 0, 0, 0};
/**
 * 
 */
void irq_install_handler(int irq, void (*handler)(struct regs *r)){
	
    irq_routines[irq] = handler;
}
/**
 * 
 */
void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}


/*
   Remapeando IRC.
   -> Setar corretamente antes de se comunicar com dispositivos.
       IRQ 0-7 -> INT 20h-27h
       IRQ 8-15 -> INT 28h-2Fh
   -> Usamos um algoritmo de dominio público.
*/
void irq_remap(void){
	
    Out_Byte(0x20, 0x11);
    Out_Byte(0xA0, 0x11);
    Out_Byte(0x21, 0x20);
    Out_Byte(0xA1, 0x28);
    Out_Byte(0x21, 0x04);
    Out_Byte(0xA1, 0x02);
    Out_Byte(0x21, 0x01);
    Out_Byte(0xA1, 0x01);
    Out_Byte(0x21, 0x0);
    Out_Byte(0xA1, 0x0);
}

/**
 * 
 */
void irq_install(){
	
    irq_remap();
    idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);

    idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}

/** 
  -> Handlers de interrupcao devem sinalizar o termino da 
  operacao para o chip. Caso nao seja dado o aviso, o sistema
  nao ira enviar mais sinais.
  
  Veja em: [1]
  *Handlers for IRQs must issue End Of Interrupt (EOI) to one or both PIC chips. For IRQ 0-7: 
        Out_Byte(0x20, 0x20);
  *For IRQ 8-15, both 8259 chips must get EOI: 
        Out_Byte(0xA0, 0x20);
        Out_Byte(0x20, 0x20);

*/
void irq_handler(struct regs *r){
    void (*handler)(struct regs *r);
 
    handler = irq_routines[r->int_no - 32];   
    if (handler)
    {
        handler(r);
    }
    if (r->int_no >= 40)
    {
        Out_Byte(0xA0, 0x20);
    }
    Out_Byte(0x20, 0x20);
}

/*
   Este handler sera utilizado apenas para o clock!
*/
void irq_handler_clock(){
	void (*handler)();
   
	handler = irq_routines[0];
    if (handler)
    {
      handler();
    }
    Out_Byte(0x20, 0x20);
}


/*
  Referencias:        
  [1] http://my.execpc.com/~geezer/osd/intr/index.htm
  [2] http://my.execpc.com/~geezer/osd/intr/8259.c
*/
