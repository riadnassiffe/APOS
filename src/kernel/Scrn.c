/**
 * \file
 * \brief Implementação das funções do Vídeo.
 **/
#include "include/Scrn.h"
/** Guarda o valor para memória de vídeo*/
unsigned short *textmemptr;
/** Define as cores da tela*/
int attrib = 0x0F; 
/** Representam as cordenadas do curso*/
int csr_x = 0, csr_y = 0; 


/**
 * \brief Faz a tela rolar, para baixo.
 **/
void scroll(void) {
	unsigned blank, temp;

	blank = 0x20 | (attrib << 8);
	if (csr_y >= 25) {
		temp = csr_y - 25+ 1;
		memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80* 2);
		memsetw (textmemptr + (25 - temp) * 80, blank, 80);
		csr_y = 25 - 1;
	}
}

/**
 * \brief Atualiza a posição do cursor do teclado na tela 
 **/
void move_csr(void) {
	unsigned temp;

	temp = csr_y * 80+ csr_x;

	Out_Byte(0x3D4, 14);
	Out_Byte(0x3D5, temp >> 8);
	Out_Byte(0x3D4, 15);
	Out_Byte(0x3D5, temp);
}

/**
 *  \brief Limpa a tela.
 **/
void cls() {
	unsigned blank;
	int i;

	blank = 0x20 | (attrib << 8);
	for (i = 0; i < 25; i++)
		memsetw (textmemptr + i * 80, blank, 80);
	csr_x = 0;
	csr_y = 0;
	move_csr();
}

/**
 * \brief Coloca apenas um unico carater na tela
 **/
void putch(unsigned char c) {
	unsigned short *where;
	unsigned att = attrib << 8;

	if (c == 0x08) {
		if (csr_x != 0)
			csr_x--;
	} else if (c == 0x09) {
		csr_x = (csr_x + 8) & ~(8 - 1);
	} else if (c == '\r') {
		csr_x = 0;
	} else if (c == '\n') {
		csr_x = 0;
		csr_y++;
	} else if (c >= ' ') {
		where = textmemptr + (csr_y * 80+ csr_x);
		*where = c | att; /*atribui cor a um caracter*/
		csr_x++;
	}

	if (csr_x >= 80) {
		csr_x = 0;
		csr_y++;
	}

	scroll();
	move_csr();
}

/**
 * \brief Função feita para imprimir na tela, feita em ASSEMBLER.
 */
void puts(const char *text) {
	int i;
	for (i = 0; i < strlen(text); i++) {
		putch(text[i]);
	}
}

/**
 * \brief Função feita para imprimir na tela.
 */
void printf(unsigned char *text) {
	int i=0;
	while (text[i] != '%') {
		putch(text[i]);
		i++;
	}
}

/**
 * \brief Seleciona uma cor para a letra e para o funda da tela.
 **/
void settextcolor(unsigned char forecolor, unsigned char backcolor) {
	attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/**
 * \brief Aciona o modo texto VGA.
 **/
void init_video(void) {
	textmemptr = (unsigned short *)0xB8000;
	cls();
}

/**
 * \brief Imprime na tela um inteiro.
 **/
void printfInt(int num) {
	char string[10];
	itoa(num, string, 10);
	puts(string);
}

/** 
 * \brief Responsável por imprimir mensagens de debug na tela. 
 **/
void APOSDEBUG(int debug, char *text) {
	int i;
	if (debug == 1) {
		for (i = 0; i < strlen(text); i++) {
			putch(text[i]);
		}
	}
}
