/**
 * \file
 * \brief Este arquivo, guarda as definições que podem ser usadas, por qualquer parte do sistema.
 */
#ifndef __DEFS_H
#define __DEFS_H

#define baseCS  0x08
#define baseDS  0x10
#define baseTSS0 0x18
#define baseTSS1 0x20

#define T_16 unsigned int
#define T_32 unsigned long int
#define ushort_t unsigned short   //16bits 
#define ulong_t  unsigned long    //64bits 
#define uint_t   unsigned int     //32bits
#define uchar_t  unsigned char    //8bits
#define BOOL int
#define T_SIZE unsigned int

/**
 * \brief
 */
#define TRUE 1
/**
 * \brief
 */
#define FALSE 0
/**
 * \brief
 */
#define NULL 0x00
/**
 * \brief
 */
typedef int size_t;

/** \brief Failing exit status.  */
#define    EXIT_FAILURE    1   
/** \brief Successful exit status.  */
#define    EXIT_SUCCESS    0   

/**
 * 
 */
struct regsT {
	T_16 cs,ds,ss,gs,fs,es;
	T_32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
	T_32 int_no, err_code;
	T_32 eip, eflags, useresp;
};
/**
 * \brief
 */
struct regs {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};
#endif
