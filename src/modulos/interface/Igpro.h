#ifndef IGPRO_H_
#define IGPRO_H_

#include "../include/Gpro.h"

extern void printBcpStat();
extern void printSizeReady();
extern int rem_procReadyList(int id);
extern int createProcess(int pri, char name[MAX_NAMEPROC], unsigned int numArg, void (*pont)());
extern int getMyPid();
extern int setCurrentPid(int pid);
extern int getMyPart();
extern void init_gProc();
extern int size_listReady();
extern int size_listBlock();
extern void add_listReady(int id);
extern int rem_listReady();
extern void add_listBlock(int id);
extern int rem_listBlock(int pid);
extern int resched();
extern int killProcess(int id);
extern int getCurrentPid();
extern void setPid(int pid);
extern void exit();


#endif /*IGPRO_H_*/
