/*
 ESCALONADOR
 - Gerenciamento de Processos
 
 */

#include "include/Gpro.h"


/*
 Estruturas e variáveis globais
 */

struct s_list list_Ready;
struct s_list list_Block;

/*
 struct s_bcp
 Estrutura que define a BCP.
 */

struct s_bcp bcp;

//Identifica PID do PROCESSO ATUAL presente na CPU
int currentPid = 0;

/*
 int createProcess(..)  : Cria PROCESSO e retorna resultado da operacao  
 */

/**
* \brief Retorna o valor do pid do processo atual. 
*/
int getCurrentPid() {
	return currentPid;
}
/**
* \brief Imprimi o estado do contrle de processo. 
*/
void printBcpStat() {
	int cont;
	puts("\n PIDs ativos: ");
	for (cont=0; cont<11; cont++) {
		if (bcp.lin[cont].pa == 1) {
			printfInt(cont);
			puts(" ");
		}
	}
	puts("\n");
}
/**
* \brief Imprimi o tamanho da lista de processos prontos.
*/
void printSizeReady() {
	int tam;
	tam = size_listReady();
	puts("\n Tam. da lista de ready: ");
	printfInt(tam);
	puts("\n");
}
/**
* \brief Imprimi o valor do pid, de um processo.
* \param pid: valor do pid a ser impresso. 
*/
void showPid(int pid) {
	puts("\n Processo a carregar: ");
	printfInt(pid);
	puts("\n");
}
/**
* \brief Inicia bloco de controle de processo.. 
*/
void init_Bcp() {
	int cont;
	for (cont=0; cont<=MAX_PROC; cont++) {
		bcp.lin[cont].pa=0;
	}
}
/**
* \brief Pega o pid do processo que o executa.
* \return o valor do pid, do processo atual.
*/
int getMyPid() {
	return currentPid;
}
/**
* \brief Pega o pid do processo que o executa.
* \param pid: número que sera atribuido ai Pid atual.
* \return Retorna 0 se conseguir atribuir um pid ao processo atual e -1 se der algum erro.
*/
int setCurrentPid(int pid) {
	if (pid >= 0) {
		currentPid = pid;
		return 0;
	} else
		return -1;
}
/*Retorna o número da Particao Fixa do processo*/
int getMyPart() {
	return bcp.lin[currentPid].myPart;
}
/**
 * \brief  Retorna um valor de PID válido. 
 * \return Retorna um valor inteiro.
 */
int getFreePid() {
	int cont=1;
	while (cont<=MAX_PROC) {
		if (bcp.lin[cont].pa == 0) {
			bcp.lin[cont].pa = 1;
			return cont;
		}
		cont++;
	}
	return -1;
}
/**

 **/
void checkEsp() {
	int cont;
	int iguais=0;
	for (cont=0; cont<MAX_PROC; cont++) {
		if (bcp.lin[cont].esp == bcp.lin[cont+1].esp) {
			iguais++;
		}
	}
	puts("\n Iguais: ");
	printfInt(iguais);
	puts("\n");
}

/**
 * \brief Cria um processo.
 * \param pri Número de prioridade.
 * \param name Nome do processo.
 * \param numArg Número de argumentos.
 * \param pont Endereço do processo.
 * \return 0 se o processo for criado com sucesso e -1 se não conseguir.
 */
int createProcess(int pri, char name[MAX_NAMEPROC], unsigned int numArg, void (*pont)()) {
	int mypid;
	int mypart;
	__asm__ __volatile__
("cli");
 	mypid=getFreePid();
	if (mypid != -1) {
		mypart=alocPart(mypid);
	}
	if ((mypid != -1) && (mypart != -1)) {
		//tem PID e Partição válida.... 
		strcpyn(bcp.lin[mypid].pname,name,MAX_NAMEPROC);
		bcp.lin[mypid].pa = 1;
		bcp.lin[mypid].myPart = mypart;
		bcp.lin[mypid].prio = pri;
		bcp.lin[mypid].state = PROC_READY;
		bcp.lin[mypid].numArgs = numArg;
		bcp.lin[mypid].pont = pont; //Implementar método para carregar processo nessa particao
		bcp.lin[mypid].esp =(void *) getProcStackPointer(mypart);
		bcp.lin[mypid].ebp =(void *) bcp.lin[mypid].esp;
		reg_esp =(T_32) bcp.lin[mypid].esp;
		reg_ebp =(T_32) reg_esp;
		reg_eip =(T_32) pont;
		create_common_stack();
		bcp.lin[mypid].esp =(void *) reg_esp;
		bcp.lin[mypid].ebp =(void *) reg_ebp;
//		puts("\n >> Entrando em ready! \n");
		add_listReady(mypid);
//		puts("\n >>> Saindo de ready! \n");
		
		__asm__ __volatile__
(		"sti");
		return 0;
	}
	__asm__ __volatile__ ("sti");
	return -1;
}
/**
 * \brief Mata o processo com o ID do parâmetro
 * - Seta BCP line do processo PA=0
 * - Devolve particao.
 * - Remove o processo das filas de
 * - Ready
 * - Block
 * \param id: Valor do pid a ser encerrado
 * \return  Retornar 0 se não conseguir  1 se conseguir.
 */
int killProcess(int id) {
	//Se o processo estiver em execusão ele não pode ser finalizado.
	if (id == getMyPid()) {
		puts("\n *** Processo nao pode se matar! \n");
		return 0;
	}
	/*se processo estive na lista de finalizados, ele não pode ser finalizado de novo*/
	if (rem_procReadyList(id) == FALSE) {
		return 0;
	}
	disposePart(bcp.lin[id].myPart);
	bcp.lin[id].pa=0;
	return 1;
}

/**
 * \brief Comando dado pelo próprio processo se mata,  quando o mesmo completa sua execução.
 *Ações:
 * -liberar sua particao da memoria
 * -liberar linha da bcp
 * -Nao precisa retirar da fila de pronto ou bloqueado pois esta executando
 */
void exit() {
	int mypid;
	/*Usamod o getMyPid para pegar o pid do processo em execusão no momento.*/
	mypid = getMyPid();
	bcp.lin[mypid].pa = 0;
	//disposePart(5);
	setPid(0);
	while (1) {
	}
}

/**
 * \brief Permite a modificação do PID. Uso restrito do gerenciador de processos.
 * \param pid Número para qual se desja modificar o valor do processo atual.
 */
void setPid(int pid) {
	currentPid = pid;
}
/*
 *\brief Inicializa gerenciador
 */
void init_gProc() {
	ulong_t (*pont)();
	
	init_Bcp();
	init_lists();
	pont = (void *)resched;
	initTSS_SO((ulong_t)pont);
}
/**
 * \brief  Inicializa listas de prontos e bloqueados.
 */
void init_lists() {
	/*
	 [Poderiamos passar a LISTA como parâmetro pra as funções de ADD\REM
	 da lista de READY e BLOCK, porém decidimos implementar funções espe-
	 cificas para cada uma pois facilitaria futuras modificações uma vez
	 que cada lista sera tratada independentemente]
	 */
	list_Ready.tam = 0;
	list_Ready.head = NULL;
	list_Ready.tail = NULL;
	list_Block.tam = 0;
	list_Block.head = NULL;
	list_Block.tail = NULL;
}
/**
 * \brief Retorna o tamanho da lista de processos prontos.
 * \return um inteiro com o valor dos tamanho da lista de prontos.
 */
int size_listReady() {
	return list_Ready.tam;
}
/**
 * \brief Retorna o tamanho da lista de processos bloqueados.
 * \return um inteiro com o valor dos tamanho da lista de bloqueados.
 */
int size_listBlock() {
	return list_Block.tam;
}
/**
 * \brief Verifica se um Pid e válido.
 * \param pid: O número a ser verificado
 * \return Retorna 0 se o Pid for válido e -1 se ele não for. 
 */
int validPid(int pid) {
	if ((pid > 0) && (pid <= MAX_PROC))
		return 0;
	else
		return -1;
}
/**
 * \brief Adiciona a um processo a lista de bloqueado.
 * \param id: Pid do processo que será adicionado a lista.
*/
void add_listBlock(int id) {
	/*********************************
	 *Insere elemento no FIM  da fila*
	 *********************************/
	struct s_NoProc noAux;
	struct s_NoProc *prox;
	struct s_NoProc *pnovo;
	
	noAux.id = id;
	list_Block.tam++;
	prox = list_Block.head;
	if (prox == NULL) {
		prox = malloc(sizeof(struct s_NoProc));
		prox->id = noAux.id;
		prox->prio = noAux.prio;
		prox->next = NULL;
		prox->back = NULL; //primeiro elemento da FILA
		list_Block.head = prox;
		list_Block.tail = prox;
	} else {
		while (prox->next != NULL)
			prox = prox->next;
		//prox.next == NULL  verificar
		pnovo = malloc(sizeof(struct s_NoProc));
		prox->next = pnovo;
		pnovo->id = noAux.id;
		pnovo->prio = noAux.prio;
		pnovo->back = prox;
		pnovo->next = NULL;
		list_Block.tail = pnovo;
	}
}
/**
 * \brief Remove um processo a lista de bloqueado.
 * \param pid Número de indentificação do processo que será removido da lista.
 * \return Retorna 0 se for removido com sucesso e -1 se falhar.
*/
int rem_listBlock(int pid) {
	/************************************ 
	 * Remove elemento da cabeça da fila*
	*************************************/
	struct s_NoProc *prox;
	
	list_Block.tam++;
	prox = list_Block.head;
	if (prox == NULL) {
		return 0;
	} else {
		while (prox->next != NULL) {
			if (prox->id == pid) {
				return 0;
			}
			prox = prox->next;
		}
		//prox.next == NULL verificar
	}
	return -1;
}
/*fazer*/
int rem_procBlockList(int id) {
	//removendo da lista de block!
	printfInt(id);
	return 0;
}

/**
 * \brief Salva registradores mapeados.
 * \param pid Valor do pid do processo que se deseja salvar os registradores.
 */
void saveStack(int pid) {
	if (pid == 0) {
		//puts("\n Saving: Pilha do kernel !!! \n");
	} else {
		bcp.lin[pid].ebp =(void *) reg_ebp;
		bcp.lin[pid].esp =(void *) reg_esp;
		//showPid(pid);
	}

}

/**
 * \brief Desvia o fluxo de instrução da CPU para o processo com ID. Usa a função asm goAddress(unsigned int pont); 
 */
void loadStack(int id) {
	if (id == 0) {
		puts("\n Loading: Pilha do kernel !!! \n");
		__asm__ __volatile__
(	"hlt");
} else {
	reg_ebp =(T_32) bcp.lin[id].ebp;
	reg_esp =(T_32) bcp.lin[id].esp;
}
}

/*
 Função: suspend(int pid)
 Descrição: coloca o processo ID na lista de Ready.
 
 */
void suspend(int id) {
	if (validPid(id)!= -1) {
		bcp.lin[id].state = PROC_READY;
		add_listReady(id);
	}
}

/*
 Função: block(int pid) e unblock(int pid)
 Descrição: coloca o processo PID em estado BLOCK/Retira do BLOCK
 */

/*
 Função: rem_procReadyList(int id)
 Descrição: busca por processo com pid ID e remove das filas:
 - ready
 - block
 Retorna: 0 se n encontrou!
 1 se encontrou!
 */
int rem_procReadyList(int id) {
	struct s_NoProc *prox;
	struct s_NoProc *ant;
	prox = list_Ready.head;
	if (prox->id == id) {
		list_Ready.head=prox->next;
		OSdispose(prox);
		list_Ready.tam--;
		return 1;
	}
	ant = prox;
	prox = prox->next;
	while (prox != NULL) {
		if (prox->id == id) {//achou
			ant->next = prox->next;
			OSdispose(prox);
			list_Ready.tam--;
			return 1;
		}
		ant = prox;
		prox = prox->next;
	}
	return 0;
}

/*
 Função: rem_listReady()
 Descrição: remove primeiro elemento da FILA e retorna sua ID
 
 */
int rem_listReady() {
	struct s_NoProc *prox;
	struct s_noProc *ant;
	int retval;
	prox=list_Ready.head;
	if (prox == NULL) {
		return 0;
	} else {
		//PROX!=NULL então é elemento a ser retirado...
		retval=prox->id;
		list_Ready.tam--;
		ant = (struct s_noProc *)&prox;
		if (list_Ready.tam>0) {
			list_Ready.head=prox->next;
		} else {
			list_Ready.head=NULL;
			list_Ready.tail=NULL;
		}
		//puts("\n >>> Removendo elemento da fila de ready! \n");
		OSdispose(prox);
		return retval;
	}
}

/*
 Função: add_listReady(pid)
 Descrição: insere elemento no final da fila.
 */
void add_listReady(int id) {
	struct s_NoProc noAux;
	struct s_NoProc *prox;
	struct s_NoProc *pnovo;
	noAux.id=id;
	noAux.prio=bcp.lin[id].prio;
	list_Ready.tam++;
	prox=list_Ready.head;
	//puts("\n Iniciou ADD list Ready ! \n");
	//currentPid=0;
	if (prox == NULL) { //Sem elementos na fila
		//puts("\n Lista vazia.. Adicionando primeiro ! \n");
		prox=OSmalloc(sizeof(struct s_NoProc));
		if (prox == NULL) {
			puts("\n OSmalloc retornou NULL ! \n");
			__asm__ __volatile__("hlt");
	}
	else {
		//puts("\n OSmalloc retornou valido ! \n");
	}
	prox->id=noAux.id;
	prox->prio=noAux.prio;
	prox->next=NULL;
	prox->back=NULL; //primeiro elemento da FILA
	list_Ready.head=prox;
	list_Ready.tail=prox;
	}else {//Já existem elementos na fila PROX != NULL
	pnovo=OSmalloc(sizeof(struct s_NoProc));
	if (pnovo == NULL) {
		puts("\n OSmalloc retornou NULL ! \n");
		__asm__ __volatile__ ("hlt");
	}
	prox=list_Ready.tail;  //Endereço do ultimo elemento da fila!
	prox->next=pnovo;
	pnovo->id=noAux.id;
	pnovo->prio=noAux.prio;
	pnovo->back=prox;
	pnovo->next=NULL;
	list_Ready.tail=pnovo; //Novo processo no fim da fila
	}
}

/**
 * \brief Elege o próximo processo a executar. Algoritmo:
 1. verifica se algum processo bloqueado já terminou
 caso sim.. Inserir na lista de Ready
 2. verificar se existe processo para ser atendido em Ready
 caso sim.. 
 1. Colocar o processo atual na lista de Ready
 2. Load para o pr&oacute;ximo processo de Ready
 */
// Apenas teste... não usar a a block list...
int resched() {
	int lastProc;
	int nextProc;
	saveStack(currentPid);
	if (size_listReady()==0) {
		puts("\n Error: Lista Ready vazia! ! \n");
		__asm__ __volatile__
(		"hlt");
		return FALSE;
	}
	else { //Tem elemento na lista de ready!
		lastProc=currentPid;
		nextProc=rem_listReady(); //Remover elemento da lista de Ready!
		if (lastProc!=0) {
			suspend(lastProc);
		}
		currentPid=nextProc;
		if (validPid(currentPid)==-1) {
			puts("\n Error PID invalido !! \n");
			__asm__ __volatile__ ("hlt");
			return FALSE;
		}
		bcp.lin[currentPid].state=PROC_CURRENT;
		loadStack(currentPid);
	}
	return TRUE;
}
