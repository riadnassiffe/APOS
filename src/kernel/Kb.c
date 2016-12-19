#include "include/Kb.h"

/**
 * \brief Estruturas que definem o Buffer do KeyBoard
 */
struct s_kb_buffer keyboard;

unsigned char scancode;

/**
 * \brief Inicializa buffer do teclado.
 *
 */
void init_kb_buffer() {
	keyboard.size=0;
	keyboard.mark=0;
}

/**
 * \brief Retorna numero de bytes utilizados no buffer!
 */
int get_Kb_Size() {
	return keyboard.size;
}

/**
 * \brief Limpa buffer do teclado.
 */
void clear_kb_Buffer() {
	keyboard.size=0;
	keyboard.mark=0;
}

/**
 * \brief Incrementa marcador do buffer do keyboard!
 * \attention se estiver na ultima posicao, a proxima é a primeira          
 */
void inc_kb_mark() {
	if (keyboard.mark == 255)
		keyboard.mark=0;
	else
		keyboard.mark++;
}

/**
 * \brief Adiciona leitura no buffer
 */
void add_kb_buffer(unsigned char valor) {
	keyboard.buffer[keyboard.mark][0]=valor;
	keyboard.size++;
	inc_kb_mark();
}

/**
 * \brief Adiciona leitura no buffer
 */
unsigned char get_kb_buffer() {
	unsigned char enc;
	int indice;
	indice=keyboard.mark-keyboard.size;
	enc=keyboard.buffer[indice][0];
	//if (keyboard.buffer[indice][1] != 0) {
	//	f_screens(keyboard.buffer[indice][1]);
	//}
	keyboard.size--;
	keyboard.mark--;
	return enc;
}

/** 
 * \brief MAPEAMENTO DOS SINAIS DO TECLADO: LAYOUT: US Keyboard
 */
unsigned char kbdus[128][2] = {{0, 0},
							   {7, 0},
							   {'1',0},
							   {'2', 0},
							   {'3', 0},
							   {'4', 0},
							   {'5', 0},
							   {'6', 0},
							   {'7', 0},
							   {'8', 0},
						/* 9 */{'9', 0},
							   {'0', 0},
							   {'-', 0},
							   {'=', 0},
							   {'\b', 0},
				/* Backspace */{'\t', 0}, /* Tab */
							   {'q', 0},
							   {'w', 0},
							   {'e', 0},
							   {'r', 0},
					   /* 19 */{'t', 0},
					   		   {'y', 0},
					   		   {'u', 0},
					   		   {'i', 0},
					   		   {'o', 0},
					   		   {'p', 0},
					   		   {'[', 0},
					   		   {']', 0},
					   		   {'\n', 0}, 
				/* Enter key */{0, 0}, 
		   /* 29   - Control */{'a', 0},
							   {'s', 0}, 
			   				   {'d', 0}, 
							   {'f', 0}, 
			   				   {'g', 0},
							   {'h', 0},
			   				   {'j', 0},
							   {'k', 0}, 
			   				   {'l', 0},
							   {';', 0}, 
					   /* 39 */{'\'', 0}, 
							   {'`', 0},
					   	       {0, 0},  /* Left shift */
							   {'\\', 0},
					   	       {'z', 0},
							   {'x', 0}, 
					   	       {'c', 0},
							   {'v', 0},
					   	       {'b', 0},
							   {'n', 0}, /* 49 */
					   	       {'m', 0},
							   {',', 0},
					   	       {'.', 0},
							   {'/', 0},
					   	       {0, 0}, /* Right shift */
							   {'*', 0},
					   	       {0, 0}, /* Alt */
							   {' ', 0}, /* Space bar */
					   	       {0, 0}, /* Caps lock */
     						   {0, '1'}, /* 59 - F1 key  */
							   {0, '2'}, /* 60 - F2 key  */
							   {0, '3'}, /* 61 - F3 key  */
							   {0, '4'}, /* 62 - F4 key  */
							   {0, '5'}, /* 63 - F5 key  */
							   {0, '6'}, /* 64 - F6 key  */
							   {0, '7'}, /* 65 - F7 key  */
							   {0, '8'}, /* 66 - F8 key  */
							   {0, '9'}, /* 67 - F9 key  */
							   {0, '0'}, /* 68 ... F10 */
							   {0, 0}, /* 69 - Num lock*/
							   {0, 0}, /* Scroll Lock */
							   {0, 0}, /* Home key */
							   {0, 0}, /* Up Arrow */
							   {0, 0}, /* Page Up */
							   {'-', 0},
							   {0, 0}, /* Left Arrow */
							   {0, 0},
							   {0, 0}, /* Right Arrow */
							   {'+', 0},
							   {0, 0}, /* 79 - End key*/
							   {0, 0}, /* Down Arrow */
							   {0, 0}, /* Page Down */
							   {0, 0}, /* Insert Key */
							   {0, 0}, /* Delete Key */
							   {0, 0},
							   {0, 0},
							   {0, 0},
							   {0, 0}, /* F11 Key */
							   {0, 0}, /* F12 Key */
							   {0, 0} /* All other keys are undefined */
};

/** 
 * \brief Trata a recepção de dados do teclado.
 */
/*
 * Ele precisa ter o throuput maximizado, com intuito de diminuir a
 * quantidade de tempo que o sistema fica interrompido.
 */
void keyboard_handler(struct regs *r) {
	scancode = In_Byte(0x60);
	if (scancode & 0x80) {
		//TECLA LIBERADA
	} else {
		keyboard.buffer[keyboard.mark][0]=kbdus[scancode][0];
		keyboard.buffer[keyboard.mark][1]=kbdus[scancode][1];
		keyboard.size++;
		if (keyboard.mark == 255)
			keyboard.mark=0;
		else
			keyboard.mark++;
	}
}

/**
 * \brief Retorna um caracter lido do teclado!
 */
char eGetChar() {
	char val;
	while (get_Kb_Size() == 0) {
		puts("");
	}
	val=get_kb_buffer();
	return val;
}

/**
 * \brief Retorna um caracter lido do teclado, com echo no video
 */
unsigned char *eGetString() {
	unsigned char strmax[256]; //0--255
	char valor;
	int pointer=0;
	valor=eGetChar();
	while (valor != '\n') {
		if (valor == '\b') {
			strmax[pointer]=valor;
			putch(valor);
			putch(' ');
			putch(valor);
			pointer--;
		} else {
			strmax[pointer]=valor;
			putch(valor);
			pointer++;
		}
		valor=eGetChar();
	}
	strmax[pointer]='\0';
	return strmax;
}

/**
 * \brief Retorna um caracter lido do teclado, sem echo no video
 */
unsigned char *GetString() {
	unsigned char strmax[256]; //0--255
	char valor;
	int pointer=0;
	valor=eGetChar();
	while (valor != '\n') {
		strmax[pointer]=valor;
		pointer++;
		valor=eGetChar();
	}
	strmax[pointer]='\0';
	return strmax;
}

/** 
 * \brief Inicializa  o telcado na porta IRQ1
 */
void keyboard_install() {
	init_kb_buffer();
	irq_install_handler(1, keyboard_handler);
}
