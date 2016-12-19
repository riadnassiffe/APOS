/**
 * Arquivo responsavel por prover todos os extern fornecidos pelo arquivo start.asm
 */

#ifndef KISTART_H_
#define KISTART_H_

extern void load_TSS();
extern void store_TSS();
extern void atualizar_TSS0_STACK();
extern void atualizar_TSS1_STACK();
extern void turnoff();///Fun��o implementada em assembly que possibilita desligar a m�quina.
extern void gdt_flush();///Fun��o implementada em assembly que possibilita carregar um novo segmento de registradores.

#endif
