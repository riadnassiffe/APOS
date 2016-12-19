#ifndef KIIDT_H_
#define KIIDT_H_

extern void idt_set_gate(unsigned char num, unsigned long base,unsigned short sel, unsigned char flags);

extern void idt_install();

extern void idt_set_TaskGate(unsigned char num, unsigned short sel, unsigned char flags);

#endif /*KIDT_H_*/
