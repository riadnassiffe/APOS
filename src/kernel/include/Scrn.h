/**
 * \file
 * \brief Definição das funções do Vídeo.
 **/

#include "../../libs/include/Slib.h" 
#include "../interface/KIio.h" 



extern int getInterfaceState(int pid);
extern int getMyPid();

/**
 * \brief Faz a tela rolar, para baixo.
 **/
void scroll(void);

/**
 * \brief Atualiza a posição do cursor do teclado na tela 
 **/
void move_csr(void);

/**
 *  \brief Limpa a tela.
 **/
void cls();

/**
 * \brief Coloca apenas um unico carater na tela
 **/
void putch(unsigned char c);

/**
 * \brief Usa uma rotina para imprimi uma STRING
 **/
void puts(const char *text);

/**
 * \brief Função feita para imprimir na tela.
 */
extern void printf(unsigned char *text);

/**
 * \brief Seleciona uma cor para a letra e para o funda da tela.
 **/
void settextcolor(unsigned char forecolor, unsigned char backcolor);

/**
 * \brief Aciona o modo texto VGA.
 **/
void init_video(void);

/**
 * \brief Imprime na tela um inteiro.
 **/
void printfInt(int num);

/** 
 * \brief Responsável por imprimir mensagens de debug na tela. 
 **/
void APOSDEBUG(int debug, char *text);
