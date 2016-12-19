#ifndef KMEM_H_
#define KMEM_H_

extern void showMemoryTable();
extern void addLinPart(int numTab, int pa, int pid, T_32 c_point, T_32 c_size, T_32 h_point, T_32 h_size, T_32 s_point, T_32 s_size);
extern void checkRelocSize();
extern T_32 getProcStackPointer(int numTab);
extern T_32 getProcStackSize(int numTab);
extern unsigned int getMemoryTableSize();
extern unsigned long int getTableMemoryPa(int line);
extern unsigned long int getTableMemoryPid(int line);
extern struct s_free_node *getTableMemoryCab(int line);
extern unsigned long int getTableMemoryHeapPoint(int line);
extern unsigned long int getTableMemoryHeapSize(int line);
extern void setTableMemoryRelocSize(int line,int size);
extern void setTableMemoryHeapSize(int line, unsigned short int heapSize);
extern void setTableMemoryPa(int line, int myPart);
extern void setTableMemoryPid(int line, int pid);
extern void setTableMemoryCab(int line, struct s_free_node *cab);
extern void setTableMenoryHeapPoint(int line, unsigned short int heapPointSize);
extern int partDispose(int pid);
extern int partAlloc(int pid,T_32 sizeCode,T_32 sizeStack,T_32 sizeHeap);

#endif /*KMEM_H_*/
