#ifndef MEM_H_
#define MEM_H_
#include "../../include/Defs.h"
#include "MultiBoot.h"
#include "../../kernel/interface/KIgdt.h"
#include "../../kernel/interface/KIscrn.h"
#include "../defs/MemStructs.h"

#define MAX_PART 11
#define TAM2K 2048
#define TAM4K 4096
#define TAM5K 5120
#define TAM10K 10240 
#define TAM1MB 1048576
#define TAM5MB 5242880
#define NULL 0x00
#define PARTITIONSIEZE 2102272 //2MB 5K
#define TAM5K 5120
#define TAM10K 10240 
#define TAM1MB 1048576
#define TAM2MB 2097152
#define BLOCKSIZE 	0X1000

struct stab_mem {
	struct sdesc_tabMem lin[MAX_PART];
};

int initMemTable();
int initMemBitMap(multiboot_info_t *boot_info);
void showMemoryTable();
void initMemoryTable();
void initMemoryTableLine(int numLin);
void addLinPart(int numLine, int pa, int pid, T_32 c_point, T_32 c_size,T_32 h_point, T_32 h_size,T_32 s_point, T_32 s_size);
int partAlloc(int pid,T_32 sizeCode,T_32 sizeStack,T_32 sizeHeap);
void checkRelocSize();
T_32 getProcStackPointer(int numTab);
T_32 getProcStackSize(int numTab);
unsigned int getMemoryTableSize();
unsigned long int getTableMemoryPa(int line);
unsigned long int getTableMemoryPid(int line);
struct s_free_node *getTableMemoryCab(int line);
unsigned long int getTableMemoryHeapPoint(int line);
unsigned long int getTableMemoryHeapSize(int line);
void setTableMemoryRelocSize(int line,int size);
void setTableMemoryHeapSize(int line, unsigned short int heapSize);
void setTableMemoryPa(int line, int myPart);
void setTableMemoryPid(int line, int pid);
void setTableMemoryCab(int line, struct s_free_node *cab);
void setTableMenoryHeapPoint(int line, unsigned short int heapPointSize);
unsigned int searchFreeSpace(T_32 sizeCode);

#endif /*MEM_H_*/
