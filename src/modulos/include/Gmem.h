/** 
 * \file 
 * \brief Este arquivo contém as definições, necessárias para gerenciador de memória.
 */

#ifndef __GMEM_H
#define __GMEM_H
#include "../../include/Defs.h"
#include "../../kernel/interface/KIgdt.h"
#include "../../kernel/interface/KIscrn.h"
#include "../../modulos/interface/IMemTable.h"
#include "../../modulos/interface/Igpro.h"
#include "../../modulos/defs/MemStructs.h"


#define MAX_PART 11
#define TAM2K 2048
#define TAM4K 4096
#define TAM5K 5120
#define TAM10K 10240 
#define TAM1MB 1048576
#define TAM5MB 5242880
#define NULL 0x00

/**
 * \brief realoca memória, dos processos.
 **/
BOOL memRelocDisp(int proc,unsigned int size);

/**
 * \brief Retorna: Ponteiro para a HEAP do processo atual.
 */
void *malloc(int size);

/**
 * \brief Libera Ponteiro da HEAP do processo atual.
 */
void dispose(void *pont);
/**
 * \brief Retorna: retorna o numero da linha da tabela de mem&oacute;ria do respectivo processo em execução
 */
int findMyPart();

int findFreePart();

/**
 * \brief Retorna: aloca particao para uso do processo atual
 **/
int alocPart(int pid);

/**
 * \brief libera uma particao de memória
 **/
int disposePart(int pid);


/**
 * \brief Verificar se existe espaco para alocacao na área usada para indexar relocacao:
 * Retorno:   0: ERROR
 * 			  1: OK
 */
int OSmemRelocDisp(int size);

/**
 * \brief Alloca memória, apenas para SO.
 * \return Ponteiro para a HEAP do SO.
 **/
void *OSmallocF(int size);
/**
 * \brief adiciona tracker para manter referências do ponteiro..
 * \return -Retorna 
 *			0 : Error
 *			1 : Sucesso
 **/
int addOSTrackPoint(struct s_free_node noAlocado);
/**
 * \brief Retorna um endereço de alocação valida da lista de malloc a função já marca o 
 * ponteiro na lista como usado!
 */
void *getOStrackPoint(int size);

/**
 * \brief  libera o ponteiro de malloc para o processo!
 * \return Apenas setar PA=0! se retornar 0 erro e 1 sucesso
 */
int releaseOStrackPoint(void *end);

/**
 * \brief  Ponteiro para a HEAP do SO.
 * \attention Este MALLOC é utilizado apenas pelo Sistema Operacional!
 **/
void *OSmalloc(int size);

/**
 * \brief Libera Ponteiro da HEAP do SO.
 */
void OSdispose(void *pont);


#endif
