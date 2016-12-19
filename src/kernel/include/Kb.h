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
 buffer[]: mantém valores de entrada
 
 */
struct s_kb_buffer {
	unsigned int size;
	unsigned int mark;
	char buffer[256][2];
};


/*
 função: init_kb_buffer
 descricao: inicializa buffer do teclado.
 */
void init_kb_buffer();

/*
 função: getKbSize
 descrição: retorna numero de bytes utilizados no buffer!
 */
int get_Kb_Size();

/*
 função: clear_kb_Buffer
 descrição: limpa buffer do teclado.
 */
void clear_kb_Buffer();

/*
 função: inc_kb_mark
 descrição: incrementa marcador do buffer do keyboard!
 obs: se estiver na ultima posicao, a proxima é a primeira          
 */
void inc_kb_mark();

/*
 função: add_kb_buffer
 descrição: adiciona leitura no buffer
 */
void add_kb_buffer(unsigned char valor);
/*
 função: get_kb_buffer
 descrição: adiciona leitura no buffer
 */
unsigned char get_kb_buffer();
/* 
 Função: keyborad_handler
 Handler que trata a recepção de dados do teclado.
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
