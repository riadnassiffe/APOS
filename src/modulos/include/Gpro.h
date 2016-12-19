/*
 ESCALONADOR
 - Gerenciamento de Processos 
 */

 
#include "../../include/Defs.h"
#include "../../libs/include/Slib.h"
#include "../../kernel/interface/KIscrn.h"
#include "../../kernel/interface/KIgdt.h"
#include "../interface/Igmem.h"
#include "../include/MemTable.h"
/*
 ==============================================================================
 MAPEAR
 - Mapeando registradores gerais!
 */

extern T_32 reg_eax;
extern T_32 reg_ebx;
extern T_32 reg_ecx;
extern T_32 reg_edx;
extern T_32 reg_edi;
extern T_32 reg_esi;
extern T_32 reg_ebp;
extern T_32 reg_esp;
extern T_16 reg_cs;
extern T_16 reg_ds;
extern T_16 reg_es;
extern T_16 reg_ss;
extern T_16 reg_gs;
extern T_16 reg_fs;
extern T_32 reg_eip;
extern T_32 reg_eflags;

#define MAX_NAMEPROC 10 
#define PROC_STATE int
#define PROC_NAME char[MAX_NAMEPROC]
#define MAX_NAMEPROC 10
#define MAX_PROC 11
#define PROC_CURRENT 0
#define PROC_READY 1
#define PROC_BLOCK 2

extern void create_common_stack();

void suspend(int id);
int resched();
int validPid(int pid);
void init_gProc();
void init_Bcp();
void init_lists();
int getMyPart();
void setPid(int pid);
void exit();
int rem_procReadyList(int id);
void saveStack(int pid);
void loadStack(int id);

int setCurrentPid(int pid);
int getMyPid();
int getFreePid();
int getCurrentPid();

int createProcess(int pri, char name[MAX_NAMEPROC], unsigned int numArg, void (*pont)());

int killProcess(int id);

void add_listReady(int id);
int rem_listReady();

int size_listReady();
int size_listBlock();

void add_listBlock(int id);
int rem_listBlock(int pid);

void printBcpStat();
void printSizeReady();

/*
 struct s_NoProc
 Estrutura que define o formato dos objetos da lista de READY\BLOCK
 */
struct s_NoProc {
	int id;
	unsigned int prio;
	struct s_NoProc *next;
	struct s_NoProc *back;
};

/*
 struct s_list
 Estrutura que define o formato da lista de Ready e Block
 
 */
struct s_list {
	int tam;
	struct s_NoProc *head;
	struct s_NoProc *tail;
};

struct s_list list_Ready;
struct s_list list_Block;

/*

 struct s_linBcp
 Estrutura que define o formato da linha do bloco de controle do processo
 PRESENTE || AUSENTE \ ESTADO \ PRIORIDADE \ NOME_PROCESSO \ 
 NUM_ARGUMENTOS \ STRUCT REGS 
 
 */
struct s_linBcp {
	int pa;
	int myPart;
	int state;
	unsigned int prio;
	char pname[MAX_NAMEPROC];
	unsigned int numArgs;
	void (*pont)();
	void *esp;
	void *ebp;
};
/*
Funções de manipulação das estruturas:
struct s_NoProc{int id,struct s_NoProc *next,struct s_NoProc *back}
struct s_list{int tam,struct s_NoProc *head,struct s_NoProc *tail}
struct s_list list_Ready;
struct s_list list_Block;   
*/
/*
 struct s_bcp
 Estrutura que define a BCP.
 */
struct s_bcp {
	struct s_linBcp lin[MAX_PROC];
};

