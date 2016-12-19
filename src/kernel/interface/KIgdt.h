#ifndef KIGDT_H_
#define KIGDT_H_

#include "../../include/Defs.h"

extern void gdt_install();
extern void setTSSvalues(ulong_t novo_eip, ulong_t novo_esp);
void setTSSregs(T_32 eax, T_32 ebx, T_32 ecx, T_32 edx, T_32 esi, T_32 edi,	T_32 ebp, T_32 esp);
extern void initTSS_SO(ulong_t eip_scheduller);
extern void reset_tss_gate();
extern void tss_install();

#endif /*KGDT_H_*/
