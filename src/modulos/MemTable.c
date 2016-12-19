#include "include/MemTable.h"

/*estrutura que guarda as informa��es sobre as parti��es de mem�ria.*/
struct stab_mem tabMem;
/*estrutura que guarda o mapa de bits.
 * O tamanho do array foi determinado pela quantidade m�xima de 
 * mem�ria, dividida pelo tamanho da p�gina. 
 * 4294967296 / 4096 = 1048576
 * */
struct bitMap memoryBlockList [1048576];
/********************************
 * Quantidade de blocos alocados*
 ********************************/
unsigned int totalMemoryblocks;

/**
 * \brief  Fun��o respons�vel por inicializar o mapa de bit.
 * \param boot_info - Essa vari�vel traz o valor do total de mem�ria da m�quina,.
 * \return Retorna 0, para indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int initMemBitMap(multiboot_info_t *boot_info) {
	unsigned int count;
	
	/*************************************************************
	 * Nesse momento a quantidade de checamos quantidade mem�ria,*
	 * para ver qual deve ser o tamanho do mapa de bits.         *
	 * ***********************************************************/
	totalMemoryblocks = (boot_info->mem_upper + boot_info->mem_lower) * 1024 / PAGE;
	if (((boot_info->mem_upper + boot_info->mem_lower ) * 1024) % PAGE != 0) {
		totalMemoryblocks = totalMemoryblocks + 1;
	}
	for (count = 0; count < 1048576; count++) {
		if (count < totalMemoryblocks && count > BASE) {
			memoryBlockList[count].block = FREE;
		} else {
			memoryBlockList[count].block = USED;
		}
	}
	initMemTable();
	return 0;
}
/**
 * \brief Inicializa a tabela que guarda as parti��es da mem�ria.
 * \return Retorno de 0, indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int initMemTable() {
	int numLine;
	
	
	/*Aloca a primeira parti��o de mem�ria, que � a do SO.*/
	partAlloc(0,TAM10K,TAM10K,TAM1MB);
	for (numLine = 1; numLine < MAX_PART; numLine++) {
		addLinPart(numLine, 0, 0, 0, 0, 0, 0,0,0);
	}
	return 0;
}
/**
 * \brief  Essa fun��o escreve as linhas da tabela de parti��es da m�moria e marca as p�ginas utilizadas pela parti��o.
 * \param numLine: N�mero da linha da tabela em que a parti��o ser� alocada.
 * \param pa O valor 0 indica que a linha esta livre e 1 indica que ela esta ocupada.
 * \param pid N�mero do PID do processo.
 * \param c_point  Ponteiro para c�digo do processo .
 * \param c_size Tamanho restante da regi�o de c�digo do processo.
 * \param h_point Ponteiro para heap do processo .
 * \param h_size Tamanho restante da heap do processo.
 * \param s_point  Ponteiro para stack do processo .
 * \param s_size Tamanho restante da pilha do processo.
 */
void addLinPart(int numLine, int pa, int pid, T_32 c_point, T_32 c_size,T_32 h_point, T_32 h_size,T_32 s_point, T_32 s_size) {
	tabMem.lin[numLine].pa = pa;
	tabMem.lin[numLine].own_pid = pid;
	tabMem.lin[numLine].code_point = c_point;
	tabMem.lin[numLine].codePointStart = c_point;
	tabMem.lin[numLine].code_size = c_size;
	tabMem.lin[numLine].heap_point = h_point;
	tabMem.lin[numLine].heapPointStart = h_point;
	tabMem.lin[numLine].heap_size = h_size;
	tabMem.lin[numLine].stack_point = s_point;
	tabMem.lin[numLine].stackPointStart = s_point;
	tabMem.lin[numLine].stack_size = s_size;
	tabMem.lin[numLine].cab = NULL;
	tabMem.lin[numLine].relocSize=0;
}
/**
 * \brief Essa fun��o deve allocar uma parti��o da mem�ria para o processo, de acordo
 *  com o tamanho da pilha de c�digo, stack e parti��es livres.
 * \param pid N�mero do PID do processo.
 * \param sizeCode Tamanho restante da regi�o de c�digo do processo
 * \param sizeHeap Tamanho restante da heap do processo
 * \param  sizeStack Tamanho restante da pilha do processo
 * \return  O valor 0, Indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int partAlloc(int pid,T_32 sizeCode,T_32 sizeStack,T_32 sizeHeap) {
	T_32 cont;
	T_32 allocHeapPoint,allocCodePoint, allocStackPoint;
	T_32 block;

	cont = 0;
	while (tabMem.lin[cont].pa != 0 && cont != MAX_PART) {
		cont++;
	}
	if (cont == MAX_PART) {
		puts("nao existe espaco particao livre na tabela, para criar uma nova particao de memoria...");
		return -1;
	}
	sizeCode = sizeCode / PAGE;
	if(sizeCode % PAGE != 0){
		sizeCode = sizeCode + 1; 
	}
	allocCodePoint = searchFreeSpace(sizeCode);
	if (allocCodePoint != 0) {
		for(block = allocCodePoint; block < (allocCodePoint + sizeCode); block++) {
			memoryBlockList[block].block = USED;
		}
	} else {
		puts("Nao existe espaco de memoria disponivel");
		return -1;
	}
	sizeHeap = sizeHeap / PAGE;
	if(sizeHeap % PAGE != 0){
		sizeHeap = sizeHeap + 1; 
	}
	allocHeapPoint = searchFreeSpace(sizeHeap);
	if (allocHeapPoint != 0) {
		for (block = allocHeapPoint; block < (allocHeapPoint + sizeHeap); block++) {
			memoryBlockList[block].block = USED;
		}
	} else {
		for(block = allocCodePoint;block < (allocCodePoint + sizeCode);block++) {
					memoryBlockList[block].block = FREE;
				}
		puts("Nao existe espaco de memoria disponivel");
		return -1;
	}
	sizeStack = sizeStack / PAGE;
	if(sizeStack % PAGE != 0){
		sizeStack = sizeStack + 1; 
	}
	allocStackPoint = searchFreeSpace(sizeStack);
	if(allocStackPoint > 0) {
		for(block = allocStackPoint;block < (allocStackPoint + sizeStack);block++) {
			memoryBlockList[block].block = USED;
		}
	}
	else {
		for(block = allocCodePoint;block < (allocCodePoint + sizeCode);block++) {
			memoryBlockList[block].block = FREE;
		}
		for(block = allocHeapPoint;block < (allocHeapPoint + sizeHeap);block++) {
					memoryBlockList[block].block = FREE;
				}
		puts("Nao exist espaco de memoria disponivel");
		return -1;
	}
	addLinPart(cont,1,pid,(allocCodePoint * PAGE)-1,(sizeCode * PAGE),(allocHeapPoint * PAGE)-1,(sizeHeap * PAGE),(allocStackPoint * PAGE)-1, (sizeStack * PAGE));
	return cont;
}
/**
 * \brief Essa fun��o verifica se existe espa�o para aloca��o do tamanho desejado. 
 * \param  sizeCode: � o tamanho de espa�o desejado para aloca��o.
 * \return  0 que n�o foi poss�vel alocar o espa�o desejado.
 */
unsigned int searchFreeSpace(T_32 sizeCode){
	T_32 blockNumber;
	T_32 selectedBlocks;
	T_32 count;
	
	blockNumber = 0;
	count = BASE;
	selectedBlocks = 0;
	while(selectedBlocks < sizeCode && count < totalMemoryblocks){
		if(memoryBlockList[count].block == FREE){
			if(selectedBlocks == 0){
				blockNumber = count;
			}
			selectedBlocks++;
		}else{
			selectedBlocks = 0;
			blockNumber = 0;
		}
		count++;
	}	
	if(selectedBlocks != sizeCode){
		blockNumber = 0;
	}
return blockNumber;
}	
/**
 * \brief Mostrar tabela de mem�ria na tela.
 * \return Imprime na tela a situa��o da tabela de Parti��es.
 */
void showMemoryTable() {
	int cont;

	puts("\n Tabela de particoes: \n");

	for (cont=0; cont<=MAX_PART; cont++) {
		if (tabMem.lin[cont].pa != 0) {
			puts("\n Linha: ");
			printfInt(cont);
			puts("\n Processo dono: ");
			printfInt(tabMem.lin[cont].own_pid);
		}
	}
	puts("\n");
}

/**
 * \brief Libera o espa�o  de mem�ria ocupado por um processo.
 * \param  pid: indica o n�mero do processo que deve ter sua parti��o desalocada.
 * \return  0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int partDispose(int pid) {
	unsigned int cont;

	cont = 0;
	while (tabMem.lin[cont].own_pid != pid && cont != MAX_PART) {
		cont++;
	}

	if (cont == MAX_PART) {
		puts("nao existe espaco particao livre na tabela de particao de memoria...");
		return -1;
	}else{
		for(cont = tabMem.lin[cont].codePointStart;cont < tabMem.lin[cont].code_size / PAGE;cont++){
			memoryBlockList[cont].block = FREE;
		}
		for(cont = tabMem.lin[cont].heapPointStart;cont < tabMem.lin[cont].heap_point + tabMem.lin[cont].heap_size;cont++){
					memoryBlockList[cont].block = FREE;
				}
		for(cont = tabMem.lin[cont].stackPointStart;cont < tabMem.lin[cont].stack_point + tabMem.lin[cont].stack_size;cont++){
			memoryBlockList[cont].block = FREE;
		}
		addLinPart(pid, 0, 0, 0, 0, 0,0,0,0);
	}
	return 0;
}

/**
 * \brief Retorna o valor do ponteiro da pilha do stack, de uma determinada linha. 
 * \param numTab: � a linha, que se deseja tirar as informa��es. 
 * \return  O valor do ponteiro da pilha de stack.
 */
T_32 getProcStackPointer(int numTab) {
	return tabMem.lin[numTab].stack_point;
}
/**
 * \brief Retorna o valor restante do tamanho da pilha de stack. 
 * \param numTab: � a linha, que se deseja tirar as informa��es. 
 * \return  O valor de espa�o restante, na pilha de stack.
 */
T_32 getProcStackSize(int numTab) {
	return tabMem.lin[numTab].stack_size;
}

unsigned int getMemoryTableSize() {
	return MAX_PART;
}

unsigned long int getTableMemoryPa(int line) {
	return tabMem.lin[line].pa;
}

unsigned long int getTableMemoryPid(int line) {
	return tabMem.lin[line].own_pid;
}

struct s_free_node *getTableMemoryCab(int line) {
	return tabMem.lin[line].cab;
}

unsigned long int getTableMemoryHeapPoint(int line) {
	return tabMem.lin[line].heap_point;
}

unsigned long int getTableMemoryHeapSize(int line) {
	return tabMem.lin[line].heap_size;
}

void setTableMemoryRelocSize(int line, int size) {
	tabMem.lin[line].relocSize = tabMem.lin[line].relocSize + size;
}

void setTableMemoryHeapSize(int line, unsigned short int heapSize) {
	tabMem.lin[line].heap_size-= heapSize;
}

void setTableMemoryPa(int line, int myPart) {
	tabMem.lin[line].pa = myPart;
}

void setTableMemoryPid(int line, int pid) {
	tabMem.lin[line].own_pid = pid;
}

void setTableMemoryCab(int line, struct s_free_node *cab) {
	tabMem.lin[line].cab = cab;
}

void setTableMenoryHeapPoint(int line, unsigned short int heapPointSize) {
	tabMem.lin[line].heap_point+= heapPointSize;
}

void checkRelocSize() {
	switch (tabMem.lin[0].relocSize) {
	case 0: {
		puts("\n Tam de Lista Reloc: 0 \n");
		break;
	}
	case 1: {
		puts("\n Tam de Lista Reloc: 1 \n");
		break;
	}
	case 2: {
		puts("\n Tam de Lista Reloc: 2 \n");
		break;
	}
	case 3: {
		puts("\n Tam de Lista Reloc: 3 \n");
		break;
	}
	case 4: {
		puts("\n Tam de Lista Reloc: 4 \n");
		break;
	}
	case 5: {
		puts("\n Tam de Lista Reloc: 5 \n");
		break;
	}
	}
}
