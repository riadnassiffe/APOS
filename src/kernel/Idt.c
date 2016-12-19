/*
 Universidade Salvador - UNIFACS

 Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
 Aluno: Thiago Rocha de Assis        thiagoassis1984@hotmail.com

 IDT
 Tabela de descritores de interrupção
  
*/
#include "include/Idt.h"


/**
 * Define a IDT com 257 posições, apesar de nem todas serem utilizadas.
 */
struct idt_entry idt[256];
/**
 */
struct idt_ptr idtp;


/**
 * Define uma entrada da IDT.
 */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/**
 * Define uma entrada na GDT de uma processo.
 */
void idt_set_TaskGate(unsigned char num, unsigned short sel, unsigned char flags)
{
    idt[num].sel = sel;
    idt[num].flags = flags;
}
/**
 * Inicializa a IDT.
 */
void idt_install()
{
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int) &idt;
    memset(&idt, 0, sizeof(struct idt_entry) * 256);
    idt_load();
}
