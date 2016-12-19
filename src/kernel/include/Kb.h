/*
 Estruturas que definem o Buffer do KeyBoard
*/


// 
#include "../interface/KIio.h"
#include "../interface/KIscrn.h"
#include "../interface/KIirq.h"

#ifndef _KB_H
#define _KB_H


/*
 size: tamanho ocupado do buffer
 mark: marca o pr&oacute;ximo livre do buffer
 buffer[]: mant�m valores de entrada
 
 */
struct s_kb_buffer {
	unsigned int size;
	unsigned int mark;
	char buffer[256][2];
};


/*
 fun��o: init_kb_buffer
 descricao: inicializa buffer do teclado.
 */
void init_kb_buffer();

/*
 fun��o: getKbSize
 descri��o: retorna numero de bytes utilizados no buffer!
 */
int get_Kb_Size();

/*
 fun��o: clear_kb_Buffer
 descri��o: limpa buffer do teclado.
 */
void clear_kb_Buffer();

/*
 fun��o: inc_kb_mark
 descri��o: incrementa marcador do buffer do keyboard!
 obs: se estiver na ultima posicao, a proxima � a primeira          
 */
void inc_kb_mark();

/*
 fun��o: add_kb_buffer
 descri��o: adiciona leitura no buffer
 */
void add_kb_buffer(unsigned char valor);
/*
 fun��o: get_kb_buffer
 descri��o: adiciona leitura no buffer
 */
unsigned char get_kb_buffer();
/* 
 Fun��o: keyborad_handler
 Handler que trata a recep��o de dados do teclado.
 - Ele precisa ter o throuput maximizado, com intuito de diminuir a
 quantidade de tempo que o sistema fica interrompido.
 */
void keyboard_handler(struct regs *r);

char eGetChar();

// COM ECHO no video
unsigned char *eGetString();

void f_screens(char code);

//inicializa o teclado.
void keyboard_install();
#endif
