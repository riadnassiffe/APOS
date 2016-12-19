/*
 Interface do Sistema Operacional
 */
#include "If.h"

extern void turnoff();

struct s_shareScreen listInterface;

/**
 * Inicializa a tela.
 */
void initInterfaceShare() {
	int cont;
	for (cont=0; cont<11; cont++)
		listInterface.listAllow[cont]=0;
}

/**
 * Troca o estado da interface.
 */
void changeInterfaceMode(int pid) {
	if (listInterface.listAllow[pid] == 0)
		listInterface.listAllow[pid] = 1;
	else
		listInterface.listAllow[pid] = 0;
}
/**
 * Seta a interface como visivel
 */
void setInterfaceFree(int pid) {
	listInterface.listAllow[pid] = 1;
}

/**
 * Seta a interface como não visivel
 */
void setInterfaceNotFree(int pid) {
	listInterface.listAllow[pid] = 0;
}

/**
 * Retornar o status da interface.
 */
int getInterfaceState(int pid) {
	return listInterface.listAllow[pid];
}
/**
 * Lista os commandos disponiveis
 */
void command_list() {
	puts(" *** Comandos disponiveis: \n");
	puts(" ***  Use proc, para visualizar processos\n");
	puts(" *** \n");
	puts(" ***  help \n");
	puts(" ***  getreadylist \n");
	puts(" ***  seetree \n");
	puts(" ***  showdisk \n");
	puts(" ***  exit \n");
	puts(" ***  kill PID \n");
	puts(" ***  gettimerlimit \n");
	puts(" ***  settimerlimit \n");
	puts(" ***  createfile \n");
	puts(" ***  testread / testwrite / rdiskend \n");
}

/**
 * Sai do SO e desliga a máquina
 */
void command_exit() {
	puts(" ************************************************** \n");
	puts(" *** APOS : Academic Purpose Operating System  ***\n");
	puts(" ***              FINALIZANDO 				***\n");
	puts(" ************************************************** \n");
	turnoff();
}

/**
 *Fun&ccedil;ão principal da interface do S.O, ela é responsavel por capturar os dados digitados pelo usuário
 */
void command_interface() {
	unsigned char command[256];
	puts("\n");
	puts(" ****************************************************** \n");
	puts(" ***      APOS : Academic Purpose Operating System  ***\n");
	puts(" ***                    Bem Vindo ao novo           ***\n");
	puts(" ****************************************************** \n");
	puts(" * Duvidas digite: help ");
	while (1) {
		puts("\n x:\\ ");
		strcpy((char *)command, (char *)eGetString());
		puts("\n");
		command_analyzer(command);

	}
}

/**
 * Identifica se o parametro é um commando ou não
 *   Retorno:  -1 = Nao encontrado em command
 *             n = Valor do parametro npar
 */
int get_parm(unsigned char *command){
 unsigned char parm[256];   
 int num;
 int ret = 0;
 num = searchchar((char *)command,' ');
 strcpyn((char *)parm,(char *)getfirststr((char *)(command+num)),(strlenght((const char*)command)-num));
 ret = atoi((char *)parm);
 return ret;  
}

/**
 * Analiza os comandos digitados na interface, e executa caso eles estejam implementados. 
 */
void command_analyzer(unsigned char *command) {
	int parm = 0;
	int i = 0;
	char attribute;
	attribute = '1';

	parm = 0;
	i = 0;
	if (strcmp((const char *)command,(const char *)"help")==TRUE)
		command_list();
	else if (strcmp((const char *)command,(const char *)"ls")==TRUE)  
		command_list();
	else if (strcmp((const char *)command,(const char *)"exit"))  
		command_exit();
	else if (strcmp((const char *)command,(const char *)"gettimerlimit")){
		puts("*** Timer do scheduller: ");
		printfInt(getTimerLimit());
		puts("\n");     
	}
	else if (strstart((char *)command,(char *)"settimerlimit ")){
		parm=get_parm(command);
		setTimerLimit(parm);
		puts("*** Timer do scheduller: ");
		printfInt(parm);
		puts("\n");
	}
	else if (strcmp((const char *)command,(const char *)"getreadylist"))
		printBcpStat();  
	else if (strstart((char *)command,(char *)"kill ")){
		parm=get_parm(command);
		if (killProcess(parm) == 1){
		 puts("*** Processo: ");
		 printfInt(parm);
		 puts(" foi eliminado. \n");
		}
		else{
		 puts("*** Processo: ");
		 printfInt(parm);
		 puts(" nao foi eliminado. \n");
		}    
	}else if (strcmp((const char *)command, (const char *)"dir"))
		printDir(0);
	else if (strcmp((const char *)command, (const char *)"createfile")) {
		createFile(0, "teste1.txt\0", attribute, "Ola mundo lendo isso do HD");
	} else if (strcmp((const char *)command, (const char *)"createfiles")) {
		createFile(0, "teste1.txt\0", attribute, "Ola mundo");
		createFile(0, "teste2.txt\0", attribute, "Ola mundo");
		createFile(0, "teste3.txt\0", attribute, "Ola mundo");
		createFile(0, "teste4.txt\0", attribute, "Ola mundo");
		createFile(0, "teste5.txt\0", attribute, "Ola mundo");
		createFile(0, "teste6.txt\0", attribute, "Ola mundo");
		createFile(0, "teste7.txt\0", attribute, "Ola mundo");
		createFile(0, "teste8.txt\0", attribute, "Ola mundo");
		createFile(0, "teste9.txt\0", attribute, "Ola mundo");
		createFile(0, "teste10.txt\0", attribute, "Ola mundo");
		createFile(0, "teste11.txt\0", attribute, "Ola mundo");
		createFile(0, "teste12.txt\0", attribute, "Ola mundo");
		createFile(0, "teste13.txt\0", attribute, "Ola mundo");
		createFile(0, "teste14.txt\0", attribute, "Ola mundo");
		createFile(0, "teste15.txt\0", attribute, "Ola mundo");
		createFile(0, "teste16.txt\0", attribute, "Ola mundo");
		printDir(0);
	}else if (strcmp((const char *)command, (const char *)"deleteFile"))
		delFile(0, "teste\0", "txt\0",0);
	else if (strcmp((const char *)command, (const char *)"createDir"))
		createDIR(0, "testeDir\0", '1');
	else if (strcmp((const char *)command, (const char *)"deleteDir"))
		delDir(0, "testeDir");
	else if (strcmp((const char *)command, (const char *)"setDir"))
		refreshDirectoryEnter(0, "testeDir");
	else if (strcmp((const char *)command, (const char *) "p0")) {
		readFile(0, "teste2\0  ", "txt\0");
	} else if (strcmp((const char *)command,(const char *)"gettimerlimit")) {
		puts("*** Timer do scheduller: ");
		printfInt(getTimerLimit());
		puts("\n");
	} else if (strstart((char *)command, (char *)"settimerlimit ")) {
		parm=get_parm(command);
		setTimerLimit(parm);
		puts("*** Timer do scheduller: ");
		printfInt(parm);
		puts("\n");
	} else if (strstart((char *)command, (char *)"kill ")) {
		parm=get_parm(command);
		if (killProcess(parm) == 1) {
			puts("*** Processo: ");
			printfInt(parm);
			puts(" foi eliminado. \n");
		} else {
			puts("*** Processo: ");
			printfInt(parm);
			puts(" nao foi eliminado. \n");
		}
	}else if (strcmp((const char *)command, (const char *)"proc")) {
		puts("processos sendo mostrados \n");
		changeInterfaceMode(1);
		changeInterfaceMode(2);
		changeInterfaceMode(3);
		changeInterfaceMode(4);
		changeInterfaceMode(5);
		changeInterfaceMode(6);
		changeInterfaceMode(7);
		changeInterfaceMode(8);
		changeInterfaceMode(9);
		changeInterfaceMode(10);
		for (i=0; i < 1000000; ++i) {
			;
		}
		puts("escondendo processos \n");
		//coloca todos os processos para ficarem invisiveis na tela.
		changeInterfaceMode(1);
		changeInterfaceMode(2);
		changeInterfaceMode(3);
		changeInterfaceMode(4);
		changeInterfaceMode(5);
		changeInterfaceMode(6);
		changeInterfaceMode(7);
		changeInterfaceMode(8);
		changeInterfaceMode(9);
		changeInterfaceMode(10);
	} else {
		puts(" Comando Invalido! \n");
	}

}

/**
 * Imprimi mensagens na tela de acordo o estado da interface
 */
void printfAPOS(unsigned char *text) {
	if (getInterfaceState(getMyPid()) == 1) {
		puts((const char *)text);
	}
}
