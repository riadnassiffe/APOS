/** 
 * \file 
 * \brief Este arquivo implementada a GDT, a partir das especificões da INTEL.
 */
#include "../interface/KIgdt.h"
#include "../interface/KIstart.h"
#include "../interface/KIscrn.h"



T_32 getTSS1eip();

void setTSSvalues(ulong_t novo_eip, ulong_t novo_esp);

void setTSSregs(T_32 eax, T_32 ebx, T_32 ecx, T_32 edx, T_32 esi, T_32 edi,	T_32 ebp, T_32 esp);

void init_basicTSS();

void initTSS_SO(ulong_t eip_scheduller);

void gdt_set_gate(int num, unsigned long base, unsigned long limit,	unsigned char access, unsigned char gran);

void gdt_install();

void reset_tss_gate(int num);

void tss_install();

/**
 * \brief Estrutura de uma TSS
 */
struct s_TSS {
	ushort_t link;
	// +0
	ushort_t reserved1;
	// +2
	ulong_t esp0;
	// +4
	ushort_t ss0;
	// +12
	ushort_t reserved2;
	// +14
	ulong_t esp1;
	// +16
	ushort_t ss1;
	// +24
	ushort_t reserved3;
	// +26
	ulong_t esp2;
	// +28
	ushort_t ss2;
	// +36
	ushort_t reserved4;
	// +38
	ulong_t cr3;
	// +40
	ulong_t eip;
	// +48
	ulong_t eflags;
	// +56
	ulong_t eax;
	ulong_t ecx;
	ulong_t edx;
	ulong_t ebx;
	ulong_t esp;
	ulong_t ebp;
	ulong_t esi;
	ulong_t edi;
	ushort_t es;
	ushort_t reserved5;
	ushort_t cs;
	ushort_t reserved6;
	ushort_t ss;
	ushort_t reserved7;
	ushort_t ds;
	ushort_t reserved8;
	ushort_t fs;
	ushort_t reserved9;
	ushort_t gs;
	ushort_t reserved10;
	ushort_t ldt;
	ushort_t reserved11;
	uint_t debugTrap : 1;
	uint_t reserved12 : 15;
	ushort_t ioMapBase;
};

/** 
 * \brief Descritor da GDT, guarda as informações contidas em uma entrada da GDT.
 */
struct gdt_entry {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granulalidade;
	unsigned char base_high;
}__attribute__((packed));
/**
 * \brief gdt_ptr guarda a informação, do tamanho minimo e maximo que uma GDT pode ter, o seu tamanho minino é 1.
 */
struct gdt_ptr {
	/** \brief tamanho maximo.*/
	unsigned short limit;
	/** \brief endereço do começo da GDT.*/
	unsigned int base;
}__attribute__((packed));

