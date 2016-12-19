#include "../../include/Defs.h"

#ifndef KIIO_H_
#define KIIO_H_


extern void Out_Byte(ushort_t port, uchar_t value);

extern uchar_t In_Byte(ushort_t port);

extern void Out_Word(ushort_t port, ushort_t value);

extern ushort_t In_Word(ushort_t port);

#endif /*KIIO_H_*/
