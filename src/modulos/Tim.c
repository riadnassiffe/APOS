/*
 Universidade Salvador - UNIFACS

 Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
 Aluno: Thiago Rocha de Assis        thiagoassis1984@hotmail.com

 Handler do Timer
 - Tratador da interrupção do Clock
 
 */
#include "include/Tim.h"

int timer = 0;
int timerlimit = 10;

/* 
 Handler do Timer.
 Frequencia default: 18.222Hz
 */
void timer_handler() {
	timer++;
	/*
	 A cada 20 ticks executar o escalonador
	 Depois..substituir por uma variável QUANTUM e criar métodos de 
	 manipulação. 
	 O controle é mais efetivo se manipularmos diretamente a taxa de inter.
	 diretamente do Hardware, em vez de fazer o tratamento via software.
	 */
	if (timer == timerlimit) {
		//puts(".");
		//puts("\n                    *******  [start] Escalonando  *******  \n");
		resched();
		timer = 0;
		//puts("\n                    *******  [fim] Escalonando  *******  \n");
	}
	//puts("\n Retornando do timer! \n");
}

void timer_wait(int ticks) {
	unsigned long eticks;

	eticks = timer + ticks;
	while (timer < (int) eticks)
		;
}

/* *
 *Instala o handler que ira tratar a interrupção do relógio.
 *Handler do Timer deve ficar no IRQ0.
 */
void timer_install() {
	irq_install_handler(0, timer_handler);
}

int getTimerLimit() {
	return timerlimit;
}

void setTimerLimit(int time) {
	timerlimit=time;
}
