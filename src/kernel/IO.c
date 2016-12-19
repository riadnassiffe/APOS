#include "include/Io.h"


/**
 * ESTA FUN플O FOI RETIRADA DO geekOS DA Maryland CollegePark
 * Escreve um byte na porta de I/O.
 */
void Out_Byte(ushort_t port, uchar_t value)
{
    __asm__ __volatile__ ("outb %b0, %w1": : "a" (value), "Nd" (port));
}

/**
 * ESTA FUN플O FOI RETIRADA DO geekOS DA Maryland CollegePark
 * Ler um byte da porta de I/O.
 */
uchar_t In_Byte(ushort_t port)
{
    uchar_t value;

    __asm__ __volatile__ (
	"inb %w1, %b0"
	: "=a" (value)
	: "Nd" (port)
    );

    return value;
}

/**
 * ESTA FUN플O FOI RETIRADA DO geekOS DA Maryland CollegePark
 * Escreve uma word na porta de I/O.
 */
void Out_Word(ushort_t port, ushort_t value)
{
    __asm__ __volatile__ (
	"outw %w0, %w1"
	:
	: "a" (value), "Nd" (port)
    );
}

/**
 * ESTA FUN플O FOI RETIRADA DO geekOS DA Maryland CollegePark
 * Escreve uma word da porta de I/O.
 */
ushort_t In_Word(ushort_t port)
{
    ushort_t value;

    __asm__ __volatile__ (
	"inw %w1, %w0"
	: "=a" (value)
	: "Nd" (port)
    );

    return value;
}
