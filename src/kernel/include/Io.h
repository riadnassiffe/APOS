
#ifndef IO_H_
#define IO_H_

#include "../../include/Defs.h"

/**
 * ESTA FUN플O FOI RETIRADA DO geekOS DA Maryland CollegePark
 * \brief  Escreve um byte na porta de I/O.
 */
void Out_Byte(ushort_t port, uchar_t value);
/**
 * ESTA FUN플O FOI RETIRADA DO geekOS DA Maryland CollegePark
 * \brief  Ler um byte da porta de I/O.
 */
uchar_t In_Byte(ushort_t port);
/**
 * ESTA FUN플O FOI RETIRADA DO geekOS DA Maryland CollegePark
 * \brief Escreve uma word na porta de I/O.
 */
void Out_Word(ushort_t port, ushort_t value);

/**
 * ESTA FUN플O FOI RETIRADA DO geekOS DA Maryland CollegePark
 * \brief Escreve uma word da porta de I/O.
 */
ushort_t In_Word(ushort_t port);

#endif /*IO_H_*/
