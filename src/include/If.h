/*
 Interface do APOS
 */
#include "Defs.h"
#include "../kernel/interface/KIscrn.h"//sair daqui
#include "../libs/include/Slib.h" 
#include "../modulos/interface/Igpro.h"
#include "../modulos/interface/Itim.h"
#include "../kernel/interface/KIkb.h"//sair daqui
#include "../kernel/include/Fat.h"//sair daqui

/**
 * Finaliza o SO e desliga a m�quina
 */
extern void turnoff();//sair daqui

/**
 * Estrutura quarda o valor do status da tela.
 */
struct s_shareScreen {
	int listAllow[11]; ///Se 0 n�o imprime se for 1 imprime
};

/**
 * 
 */
void initInterfaceShare();
/**
 * Analiza os comandos digitados na interface, e executa caso eles estejam implementados. 
 */
void command_analyzer(unsigned char *command);
/**
 * Troca o estado da interface
 */
void changeInterfaceMode(int pid);
/**
 * Seta a interface como visivel
 */
void setInterfaceFree(int pid);
/**
 * Seta a interface como n�o visivel
 */
void setInterfaceNotFree(int pid);
/**
 * Retorna o estado da interface
 */
int getInterfaceState(int pid);
/**
 *  Responsavel por listar os comandos existentes para usu�rio
 */
void command_list();
/**
 *  Responsavel por finalizar o SO.
 */
void command_exit();
/**
 *Fun&ccedil;�o principal da interface do S.O, ela � responsavel por capturar os dados digitados pelo usu�rio
 */
void command_interface();
/**
 * Identifica se o parametro � um commando ou n�o
 */
int get_parm(unsigned char *command);

/*Imprimi de acordo com estado da tela*/
void printfAPOS(unsigned char *text);
