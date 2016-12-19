/** 
 * \file 
 * \brief Este arquivo tem a finalidade de implementar de inicializar a GDT
 */
#include "include/Gdt.h"

/**
 * \brief Estrutura que guarda a GDT.
 * \attention Toda GDT deve no minimo ter 3 entradas, uma que sera usada pelo pelo processador, outra para guardar o seguimento da memória onde esta o 
 * código do programa e a ultima para guardar para guardar segmineto de Dados. No nosso caso usaremos mais duas entradas na lista, uma para guardar
 * a Tss do ecalonador de processos e outra para a Tss dos processos. 
 */
struct gdt_entry gdt[5];
struct gdt_ptr gp;
struct s_TSS tss;
struct s_TSS tss1;

/**
 * \brief Retorna o valor do registrado eip da TSS1
 */ 
T_32 getTSS1eip() {
	return tss1.eip;
}
/**
 * \brief Define valores para TSS1
 */ 
void setTSSvalues(ulong_t novo_eip, ulong_t novo_esp) {
	tss1.cs = baseCS;
	tss1.ds = baseDS;
	tss1.ss = baseDS;
	tss1.es = baseDS;
	tss1.eflags = 0x00000202;
	tss1.eip = novo_eip;
	tss1.esp = novo_esp;
}
/**
 * 
 * \brief Define valores para os registradores da TSS1.
 * \attention Ebp e esp no momento não estã sendo usados, mas 
 * por isso estão comentados.
 */ 
void setTSSregs(T_32 eax, T_32 ebx, T_32 ecx, T_32 edx, T_32 esi, T_32 edi,	T_32 ebp, T_32 esp){
	tss1.eax = eax;
	tss1.ebx = ebx;
	tss1.ecx = ecx;
	tss1.edx = edx;
	tss1.esi = esi;
	tss1.edi = edi;
	tss1.ebp = ebp;
	tss1.esp = esp;
}
/**
 * \brief Inicializa a TSS e TSS1
 */
void init_basicTSS() {
	tss1.cr3 = 0;
	tss.cr3 = 0;
}
/**
 * \brief Inicializa as variaveis da TSS0.
 */
void initTSS_SO(ulong_t eip_scheduller) {
	tss.cs = baseCS;
	tss.ds = baseDS;
	tss.ss = baseDS;
	tss.es = baseDS;
	tss.eflags = 0x00000202;
	tss.eip = eip_scheduller;
}
/**
 * \brief Define uma entrada na GDT.
 */
void gdt_set_gate(int num, unsigned long base, unsigned long limit,
		unsigned char access, unsigned char gran) {
	
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;
	
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granulalidade = ((limit >> 16) & 0x0F);

	gdt[num].granulalidade |=  (gran & 0xF0);
	gdt[num].access = access;
}

/*
 *Seta a GDT a partir da especificações Intel.
 *1. Primeira linha da GDT é reservada para uso da Intel.
 *2. Segunda linha: CODE BASE e CODE LIMIT
 *3. Terceira linha: DATA BASE e DATA LIMIT
 *4. Quarta e quinta linha : TSS
 *Como iremos trabalhar no modo Protected Flat Model o CS = DS
 *Ou seja o sistema não trabalha com segmentação.
 *[OBS] Os parametros adicionais especificam o tipo de segmento
 *e outras propriedades. Ver Intel [N]
 * Depois 
 */
/**
 *\brief inicializa a GDT, em modo FLAT PROTEGIDO. 
 **/
void gdt_install() {
	gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
	gp.base = (unsigned int) &gdt;
	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3,(unsigned long int)&tss, sizeof(tss),0x89, 0x0F);
	gdt_set_gate(4,(unsigned long int)&tss1,sizeof(tss1),0x89, 0x0F);
	gdt_flush();
}
/**
 * \brief Reinicia o valor da TSS armazenado na GDT.
 * \param num Inidica qual TSS vai ser reinicializada.
 */  
void reset_tss_gate(int num) {
	if (num == 1) {
		gdt_set_gate(4, (unsigned long int)&tss1, sizeof(tss1),0x89, 0x0F);
	}
	else if (num == 0) {
		gdt_set_gate(3,(unsigned long int)&tss,sizeof(tss),0x89, 0x0F);
	}
}

///Deve executar depois do GDT INSTALL...
/**
 * \brief Atualiza os valores das estrutura de TSS.
 **/
void tss_install() {
	puts("Instalando TSS ! v0.5 \n");
	init_basicTSS();
	atualizar_TSS0_STACK();
	atualizar_TSS1_STACK();
	puts("Valores atualizados! \n");
	load_TSS();
	puts("TSS instalada! \n");
}

