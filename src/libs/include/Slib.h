/*
 * Definição da biblioteca de tratamento de String
 */
#include "../../include/Defs.h"
#ifndef __SLIB_H
#define __SLIB_H

extern BOOL strcpy( char *dest,  char *source);

extern BOOL strcpyn( char *dest,  char *source, int size);

extern BOOL strcmp(const char *op1, const char *op2);

extern T_SIZE strlenght(const char *source);

extern BOOL strstart( char *op1,  char *op2);

extern int searchchar( char *op1, char op2);

extern int atoi( char *string);

extern void itoa(int value, char* str, int base);

void *memcpy(void *dest, const void *src, size_t count);

void *memset(void *dest, char val, size_t count);

unsigned char *getfirststr(char *op1);

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);

size_t strlen(const char *str);
#endif
