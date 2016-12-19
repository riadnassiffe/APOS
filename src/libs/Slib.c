/*
 Universidade Salvador - UNIFACS

 Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
 Aluno: Thiago Rocha de Assis        thiagoassis1984@hotmail.com

 Biblioteca basica de STRINGS
 
 */
/*
 * Implementação baseada na versão anterior do APOS.
 * Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
 * Aluno: Ríad Mattos Nassiffe		   riad.nassiffe@gmail.com
 *
 * funções que recebiam unsigned char agora recebem somente
 * char. 
 */
#include "include/Slib.h"

//BOOL strcpy(unsigned char *dest,unsigned char *source);
//BOOL strcmp(unsigned char *op1,unsigned char *op2);
//T_SIZE strlenght(unsigned char *source);


/* =============================================================================
 Função: strcpy(unsigned char *dest,unsigned char *source)
 Retorno:  FALSE = ERROR   TRUE = SUCESSO
 
 OBS: Se SOURCE maior que destino a função retorna ERROR
 
 =============================================================================
 */
BOOL strcpy(char *dest,char *source) {
	int max;
	max=strlenght(source);
	while (max > 0) {
		*dest++=*source++;
		max--;
	}
	*dest='\0';
	return TRUE;
}

/* =============================================================================
 Função: strcpyn(unsigned char *dest,unsigned char *source)
 Retorno:  FALSE = ERROR   TRUE = SUCESSO
 
 OBS: Copia N bytes da string!
 
 =============================================================================
 */
BOOL strcpyn(char *dest,char *source,int size) {
	while (size > 0) {
		*dest++=*source++;
		size--;
	}
	*dest='\0';
	return TRUE;
}

/* =============================================================================
 Função: strcmp(unsigned char *op1,unsigned char *op2);
 Retorno:  FALSE = Diferentes   TRUE = Iguais
 
 =============================================================================
 */
/*
 BOOL strcmp(unsigned char *op1,unsigned char *op2){
 if (strlenght(op1)>strlenght(op2))
 return FALSE;
 else{
 while (*op2 != '\0'){
 if (*op1++ != *op2++)
 return FALSE;
 }
 }
 return TRUE;    
 }
 */

BOOL strcmp(const char *op1, const char *op2) {
	if (strlenght(op1) != strlenght(op2))
	return FALSE;
	else {
		while (*op2 != '\0' || *op1 != '\0') {
			if (*(op1++) != *(op2++))
			return FALSE;
		}
	}
	return TRUE;
}

/* =============================================================================
 Função: strlenght(unsigned char *source);
 Retorno:  Tamanho da string [source]
 
 =============================================================================
 */
T_SIZE strlenght(const char *source) {
	unsigned int tam=0;
	//	while (*source != '\0') {
	//		tam++;
	//		source++;
	//	}
	for(tam = 0; *source != '\0';tam++,source++);
	return tam;

}

/* =============================================================================
 Função: strstart(unsigned char *op1,unsigned char *op2);
 Retorno:  FALSE = Nao comeca com op2   TRUE = Comeca com op2
 
 =============================================================================
 */
BOOL strstart(char *op1,char *op2) {
	while (*op2 != '\0') {
		if (*op1++ != *op2++)
		return FALSE;
	}
	return TRUE;
}

/* =============================================================================
 Função: int searchstr(unsigned char *op1,unsigned char *op2);
 Retorno:  -1 = Nao encontrado op2 em op1   n = posicao na string
 
 =============================================================================
 */
int searchchar(char *op1, char op2) {
	int inc=0;
	while (*op1 != '\0') {
		if (*op1++ == op2)
			return ++inc;
		inc++;
	}
	return -1;
}

/* =============================================================================
 Função: unsigned char *getfirststr(unsigned char *op1);
 Retorno:  ponteiro para primeira sub-string
 
 =============================================================================
 */
unsigned char *getfirststr(char *op1) {
	unsigned char substr[256];
	int cont=0;
	while (*op1 != ' '&& *op1 != '\0') {
		substr[cont++]=*op1++;
	}
	substr[cont+1]='\0';
	return substr;
}

/* =============================================================================
 Função: int atoi(unsigned char *string);
 Retorno:  converte STRING para inteiro!
 
 =============================================================================
 */
int atoi(char *string) {
	int signal=1;
	int base=1;
	int ret=0;
	int cont;
	int size;
	char auxc;
	int auxi;
	size=strlenght(string);
	size--;
	if (*string == '-') {
		signal=-1;
		string++;
		size--;
	}
	for (cont=0; cont<size; cont++)
		base*=10;
	while (size >= 0) {
		auxc=*string++;
		auxi=(int)auxc-'0';
		auxi*=base;
		ret+=auxi;
		base/=10;
		size--;
	}
	ret*=signal;
	return ret;
}

/* =============================================================================
 Função: itoa(int value, char* str, int base);
 void strreverse(char* begin, char* end);
 Retorno:  converte INTEIRO para STRING!
 Fonte: Implementação baseada no Kernighan & Ritchie's "Ansi C"
 
 =============================================================================
 */
void strreverse(char* begin, char* end) {
	char aux;
	while (end>begin)
		aux=*end, *end--=*begin, *begin++=aux;
}

void itoa(int value, char* str, int base) {
	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char* wstr=str;
	int sign;
	// Validate base
	if (base<2|| base>35) {
		*wstr='\0';
		return;
	}
	// Take care of sign
	if ((sign=value) < 0)
		value = -value;
	// Conversion. Number is reversed.
	do
		*wstr++ = num[value%base];
	while (value/=base);
	if (sign<0)
		*wstr++='-';
	*wstr='\0';
	// Reverse string
	strreverse(str, wstr-1);
}
void *memcpy(void *dest, const void *src, size_t count) {
	const char *sp = (const char *)src;
	char *dp = (char *)dest;
	for (; count != 0; count--)
		*dp++ = *sp++;
	return dest;
}

void *memset(void *dest, char val, size_t count) {
	char *temp = (char *)dest;
	for (; count != 0; count--)
		*temp++ = val;
	return dest;
}

size_t strlen(const char *str) {
	size_t retval;
	for (retval = 0; *str != '\0'; str++)
		retval++;
	return retval;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count) {
	unsigned short *temp = (unsigned short *)dest;
	for (; count != 0; count--)
		*temp++ = val;
	return dest;
}

