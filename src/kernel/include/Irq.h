/** 
 * \file 
 * \brief Este arquivo realiza o tratamento de interrupções
 */
#include "../../include/Defs.h"
#include "../interface/KIio.h"
#include "../interface/KIidt.h"
/**
 * Carrega as variáveis declaradas no arquivo de boot.asm
 **/
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();


void irq_install_handler(int irq, void (*handler)(struct regs *r));

void irq_uninstall_handler(int irq);

/*
 Remapeando IRC.
 -> Setar corretamente antes de se comunicar com dispositivos.
 IRQ 0-7 -> INT 20h-27h
 IRQ 8-15 -> INT 28h-2Fh

 -> Usamos um algoritmo de dominio público.
 */
void irq_remap(void);
void irq_install();

/* 
 -> Handlers de interrupcao devem sinalizar o termino da 
 operacao para o chip. Caso nao seja dado o aviso, o sistema
 nao ira enviar mais sinais.
 
 Veja em: [1]
 *Handlers for IRQs must issue End Of Interrupt (EOI) to one or both PIC chips. For IRQ 0-7: 
 outportb(0x20, 0x20);
 *For IRQ 8-15, both 8259 chips must get EOI: 
 outportb(0xA0, 0x20);
 outportb(0x20, 0x20);

 */
void irq_handler(struct regs *r);
/*
 Este handler sera utilizado apenas para o clock!
 */
void irq_handler_clock();
