/*
 * Cria uma definição das funções implementadas pelo if.c
 * e que podem ser reutilizados por outros arquivos.
 */

#ifndef IIF_H_
#define IIF_H_

extern void command_analyzer(unsigned char *command);
extern void command_interface();
extern void initInterfaceShare();
extern void setInterfaceFree(int pid);
extern void setInterfaceNotFree(int pid);
extern int getInterfaceState(int pid);
extern void changeInterfaceMode(int pid);
extern void printfAPOS(unsigned char *text);

#endif /*IIF_H_*/
