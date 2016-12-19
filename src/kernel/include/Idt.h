/** 
 * \file 
 * \brief Este guarda as definições da IDT(Tabela de descritores de interrupção)
 */

#include "../../libs/include/Slib.h" 

extern void idt_load();
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_set_TaskGate(unsigned char num, unsigned short sel, unsigned char flags);
void idt_install();

/**
 * \brief Define os valores de uma entrada da IDT.
 */
struct idt_entry {
	unsigned short base_lo;
	unsigned short sel;
	unsigned char always0;
	unsigned char flags;
	unsigned short base_hi;
}__attribute__((packed));

/**
 * \brief Guarda informação sobre o tamanho da IDT.
 */
struct idt_ptr {
	/** \brief Tamanho maximo da lista de interrupção*/
	unsigned short limit;
	/** \brief Endereço do começo da tabela de descritores de interrupção*/
	unsigned int base;
}__attribute__((packed));


