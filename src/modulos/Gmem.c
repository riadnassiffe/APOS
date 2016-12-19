/** 
 * \file 
 * \brief Este arquivo implementadao gerenciador da heap, da stack.
 */
#include "include/Gmem.h"
int debugGMem = 0;

/**
 * \brief realoca memória, dos processos.
 **/
BOOL memRelocDisp(int proc,unsigned int size) {
	struct s_free_node *aux;
	
	if ( getTableMemoryCab(proc) == NULL){
		return 0;
	}else {
		aux=getTableMemoryCab(proc);
		while (aux->next != NULL) {
			if (aux->size >= size)
			return 1;
			aux=aux->next;
		}
	}
	return 0;
}

/**
 * \brief Retorna: Ponteiro para a HEAP do processo atual.
 */
void *malloc(int size) {
	void *end;
	int tab_lin;
	struct s_free_node *aux;
	void *pont;
	if ((tab_lin = findMyPart())!=-1) {
		//verificar se existe espaço...
		if (getTableMemoryHeapSize(tab_lin)>=(unsigned long int)size) {
			setTableMemoryHeapSize(tab_lin,size);
			pont=(void *)getTableMemoryHeapPoint(tab_lin);
			setTableMenoryHeapPoint(tab_lin,size);
			return pont;
		} else {
			//verificar se existe espaço liberado
			if (memRelocDisp(tab_lin, size) == FALSE) 
				//Se passar sabemos que CAB.NEXT é valido!
				return NULL;
			else {
				aux=getTableMemoryCab(tab_lin);
				if (aux->size >=(unsigned long int)size) { 
					//entao serve!
					setTableMemoryCab(tab_lin, aux->next);
					end=aux->end;
					dispose(aux);
					return end;
				} else {
					while (aux->next != NULL) {
						if (aux->size >= (unsigned long int)size) {
							setTableMemoryCab(tab_lin,aux->next);
							end=aux->end;
							dispose(aux);
							return end;
						}
						aux=aux->next;
					}
				}
			}
		}
		return NULL;
	}
	return NULL;
}

/**
 * \brief Libera Ponteiro da HEAP do processo atual.
 */
void dispose(void *pont) {
	int procPid = getMyPid();
	int tab_lin;
	unsigned int rsize;
	struct s_free_node *node;
	//struct s_free_node *aux;
	setCurrentPid(0);
	rsize = sizeof(*pont);
	if ((tab_lin = findMyPart())!=-1) {
		if (getTableMemoryCab(tab_lin) == NULL) {
			//primeiro no a ser liberado!
			//primeiro no a ser liberado!
			node = malloc(sizeof(struct s_free_node)); 
			// Alocacao do Dispose ocorre na área do S.O !  mas a liberacao na do processo!
			node->end=&pont;
			node->size=sizeof(*pont);
			node->next=NULL;
			setTableMemoryCab(tab_lin, node);
		}
		else
		{ //ja existem liberacoes.. colocar no no inicio! (mais rápido..)
			node = malloc(sizeof(struct s_free_node));
			node->next = getTableMemoryCab(tab_lin);
			node->size = sizeof(*pont);
			node->end = &pont;
			setTableMemoryCab(tab_lin, node);
		}
	}
	setCurrentPid(procPid); 
	//restaurando o PID!
}

/**
 * \brief Retorna: retorna o numero da linha da tabela de mem&oacute;ria do respectivo processo em execução
 */
int findMyPart() {
	unsigned long int cont=0;
	
	while (cont<=getMemoryTableSize()) {
		if ((getTableMemoryPa(cont) ==(unsigned long int) 1) && (getTableMemoryPid(cont) ==(unsigned long int) getMyPid()))
			return cont;
		cont++;
	}
	return -1;
}

int findFreePart() {
	int cont=0;
	
	while (getMemoryTableSize() >=(unsigned int) cont) {
		if (getTableMemoryPa(cont) == 0)
			return cont;
		cont++;
	}
	return -1;
}

/**
 * \brief Retorna: aloca particao para uso do processo atual
 **/
int alocPart(int pid) {
	return partAlloc(pid,TAM5K,TAM5K,TAM5K);
}

/**
 * \brief libera uma particao de memória
 **/
int disposePart(int pid) {
	partDispose(pid);
	return 0;
}


/**
 * \brief Verificar se existe espaco para alocacao na área usada para indexar relocacao:
 * Retorno:   0: ERROR
 * 			  1: OK
 */
int OSmemRelocDisp(int size) {
	struct s_free_node *aux;
	
	if (getTableMemoryCab(0) == NULL)
		return 0;
	else {
		aux = getTableMemoryCab(0);
		while (aux->next != NULL) {
			if (aux->size >=(unsigned int)size)
				return 1;
			else
				aux=aux->next;
		}
	}
	return 0;
}

/**
 * \brief Alloca memória, apenas para SO.
 * \return Ponteiro para a HEAP do SO.
 **/
void *OSmallocF(int size) {
	void *pont;
	
	if (getTableMemoryHeapSize(0) >=(unsigned long int)size) {
		setTableMemoryHeapSize(0, size);
		pont = (void *)getTableMemoryHeapPoint(0);
		setTableMenoryHeapPoint(0,size);
		//Não usa mais tracker!
		return pont;
	} else {
		puts("\n Erro Final na HEAP. Não existe espaço para permitir relocação ! !! \n");
		__asm__ __volatile__(	"hlt");
}
return NULL;
}

/**
 * \brief adiciona tracker para manter referências do ponteiro..
 * \return -Retorna 
 *			0 : Error
 *			1 : Sucesso
 **/
int addOSTrackPoint(struct s_free_node noAlocado) {
	struct s_free_node *aux;
	struct s_free_node *prox;
	if (getTableMemoryCab(0) == NULL) {
		//Primeiro item a ser adicionado!
		aux=OSmallocF(sizeof(struct s_free_node));
		if (aux==NULL) {
			puts("\n Error na criação do Tracker! \n");
		} else {
			puts("\n Tracker criado com sucesso\n");
		}
		aux->end = noAlocado.end;
		aux->pa = noAlocado.pa;
		aux->size = noAlocado.size;
		aux->next = NULL;
		setTableMemoryCab(0,aux);
		setTableMemoryRelocSize(0,1);
		return 1;
	} else {
		//podemos adicionar logo no primeiro! mais rapido.
		//preparando tracker..
		prox = OSmallocF(sizeof(struct s_free_node));
		if (prox == NULL) {
			puts("\n Error na criação do Tracker! \n");
		} else {
		}
		prox->end = noAlocado.end;
		prox->pa = noAlocado.pa;
		prox->size = noAlocado.size;
		//Tracker sera novo cabeça... e antigo sera apontado por tracker! 
		prox->next = getTableMemoryCab(0);
		setTableMemoryCab(0,prox);
		setTableMemoryRelocSize(0,1);
		return 1;
	}
	return 0;
}

/**
 * \brief Retorna um endereço de alocação valida da lista de malloc a função já marca o 
 * ponteiro na lista como usado!
 */
void *getOStrackPoint(int size) {
	void *end;
	struct s_free_node *aux;
	if (getTableMemoryCab(0) == NULL)
		return NULL;
	else {
		aux=getTableMemoryCab(0);
		while (aux->next != NULL) {
			if (aux->pa == 0&& aux->size >=(unsigned int) size) {
				//significa que ponteiro é livre e tem tamanho suficiente
				end = (void *)aux->end;
				aux->pa = 1;
				//marcar como ocupado
				return end;
			}
			aux = aux->next;
		}
	}
	return NULL;
}

/**
 * \brief  libera o ponteiro de malloc para o processo!
 * \return Apenas setar PA=0! se retornar 0 erro e 1 sucesso
 */
int releaseOStrackPoint(void *end) {
	struct s_free_node *aux;
	if (getTableMemoryCab(0) == NULL) {
		puts("\n >>> Erro no releaseOStrackPoint : tabMem.lin[0].cab == NULL, memoria do so nao inicializa \n");
		return 0;
	} else {
		aux=getTableMemoryCab(0);
		while (aux != NULL) {
			if (aux->end == end ) {
				//achou ponteiro a liberar    
				aux->pa = 0;
				return 1;
			}
			aux = aux->next;
		}
	}
	puts("\n >>> Error. Ponteiro buscado para liberacao nao existe! \n");
	__asm__ __volatile__
(	"hlt");
	return 0;
}




/**
 * \brief  Ponteiro para a HEAP do SO.
 * \attention Este MALLOC é utilizado apenas pelo Sistema Operacional!
 **/
void *OSmalloc(int size) {
	//struct s_free_node *aux;
	struct s_free_node track;
	void *auxEnd;
	void *pont;
	if (getTableMemoryHeapSize(0) >=(unsigned long int)size) {
		setTableMemoryHeapSize(0, size);
		pont=(void *)getTableMemoryHeapPoint(0);
		setTableMenoryHeapPoint(0,size);
		//Alocado espaço para ponteiro, agora criar tracker para o ponteiro.
		track.end=pont;
		if (track.end != pont) {
			puts("\n >>> Error. OSmalloc. track.end != pont \n");
			__asm__ __volatile__
(		"hlt");
	}
	track.pa = 1;
	track.size = size;
	addOSTrackPoint(track);
	return pont;
}
else
{//verificar se existe espaço liberado
	if (OSmemRelocDisp(size) == 0) {
		//Verificar se existe mem diponivel..
		puts("\n Nao tem espaco.. HEAP do SO cheia !! \n");
		__asm__ __volatile__ ("hlt");
		return NULL;
	}
	else
	{ //Recuperar um valor da lista de CAB!
		auxEnd=getOStrackPoint(size);
		if (auxEnd == NULL) {
			puts("\n HEAP do SO cheia !! \n");
			__asm__ __volatile__ ("hlt");
		}
		else
		return auxEnd;
	}
}
return NULL;
}


/**
 * \brief Libera Ponteiro da HEAP do SO.
 */
void OSdispose(void *pont) {

	if (pont == NULL) {
		puts("\n >>> Dispose on NULL error!! \n");
		__asm__ __volatile__
		(	"hlt");
	}
	if (releaseOStrackPoint(pont) == 0) {
		puts("\n >>> Release do track retorna erro !! \n");
		__asm__ __volatile__ ("hlt");
	}
	else
	{
		APOSDEBUG(debugGMem,"\n *** Release do track funcionando corretamente !! \n");
	}
}
