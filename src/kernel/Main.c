#include "include/Main.h"

void msgError() {
	puts("\n Error: Saida invalida! \n");
	__asm__ __volatile__
("hlt");
}

struct teste {
	int part1;
	int part2;
};

void (*ponteiro)();

void myFuncao1() {
	puts("Iniciou programa 1 \n");
	while (1) {
		printfAPOS((unsigned char *)"     programa 1 \n");
	}
}

void myFuncao2() {
	puts("Iniciou programa 2 \n");
	while (1) {
		printfAPOS((unsigned char *)"                   programa 2 \n");
	}
}

void myFuncao3() {
	puts("Iniciou programa 3 \n");
	while (1) {
		printfAPOS((unsigned char *)"                                     programa 3 \n");
	}
}

void myFuncao4() {
	puts("Iniciou programa 4 \n");
	while (1) {
		printfAPOS((unsigned char *)"                                                     programa 4 \n");
	}
}

void myFuncao5() {
	puts("Iniciou programa 5 \n");
	printfAPOS((unsigned char *)"                                                   programa 5 \n");
	puts("Terminou programa 5 \n");
	exit();
}


//__asm__ __volatile__ ("hlt");
void _main(multiboot_info_t *boot_info){

	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	init_video();
	initIDE();
	initMemBitMap(boot_info);
	loadFatIDEDrivers();
	timer_install();
	keyboard_install();
	puts("Iniciando inst. TSS \n");
	tss_install();
	puts("Iniciando S.O : Versao 0.1.9  \n");
	puts("Iniciando tabelas do gerenciador de memoria \n");
	puts("Iniciando estruturas do gerenciador de processos \n");
	init_gProc();
	reset_tss_gate(0);
	timer_install();
	initInterfaceShare();
	puts("Estruturas internas iniciadas \n");
	puts("Sistema iniciado... \n");

	__asm__ __volatile__
("sti");

		ponteiro=myFuncao1;
	if (createProcess(1, "proc1", 0, ponteiro)==0){
		puts("\n >>> Processo criado.. \n");
	}

	ponteiro=myFuncao2;
	if (createProcess(1, "proc2", 0, ponteiro)==0) {
		puts("\n >>> Processo criado.. \n");
	}

	ponteiro=myFuncao3;
	if (createProcess(1, "proc3", 0, ponteiro)==0)
		puts("\n >>> Processo criado.. \n");

	ponteiro=myFuncao4;
	if (createProcess(1, "proc4", 0, ponteiro)==0)
		puts("\n >>> Processo criado.. \n");

	ponteiro=myFuncao5;
	if (createProcess(1, "proc5", 0, ponteiro)==0)
		puts("\n >>> Processo criado.. \n");

	ponteiro = command_interface;
	if (createProcess(1, "interface", 0, ponteiro)==0)
		puts("\n >>> Processo interface de comandos criado.. \n");
	puts("\n Carregou procs! \n");	
	
	while (1) {
		//puts("SO main 0.1.4.6 \n");
	}
}
