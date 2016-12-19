#ifndef KIIDE_H_
#define KIIDE_H_

#include "../include/Ide.h"

/**
 * \brief Inicializa os dispositivos IDE que achar, sendo limitado at� dois hds. 
 **/
extern void initIDE();
/**
 * \brief Retornar a quantidade de cilindros de um hd.
 * \param diskNumber Indica de qual hd ser� retornado a quantidade de cilindros.
 * \return Retorna a quantidade de cilindoros do HD.
 **/
extern int getDiskCylinders(short diskNumber);
/**
 * \brief Retornar a quantidade de cabe�alhos de um hd.
 * \param diskNumber Indica de qual hd ser� retornado a quantidade cabe�alhos.
 * \return Retorna a quantidade de cabe�alhos em um disco.
 **/
extern int getDiskHeads(short diskNumber);
/**
 * \brief Retornar a quantidade de setores  por trilha de um hd.
 * \param diskNumber indica de qual hd ser� retornado a quantidade setores.
 * \return Retorna a quantidade de setores po track de um disco.
 **/
extern int getDiskSectorsPerTrack(short diskNumber);
/**
 * \brief Essa fun��o � para uso interno, para debugar o c�digo e mostrar informa��es do hd.
 * \param diskNumber indica de qual hd ser� retornado a quantidade setores.
 * \return Imprimi na tela na tela todas as informa��es capturadas sobre o HD.
 **/
extern void getDiskInfo(int diskNumber);
/**
 * \brief Essa fun��o verifica a quantidade de HDs detectados.
 * \return Retorna quantidade de discos inicializados.
 **/
extern int getDisks();
/**
 * \brief Calcula o tamanho do disco.
 * \return Retorna o tamanho do disco. 
 **/
extern int getDiskSize(short diskNumber);
/**
 * \brief Grava informa��es no disco.
 * \param driveNumber n�mero do hd que os dados ser�o sendo gravados.
 * \param *address guarda endere�o do HD que ir� ser usado..
 * \param *buffer endere�o da v�rivel que vai ser gravada no hd.
 * \return Retorna 0(zero), se tiver sucesso na escrita, e diferente de 0 se tiver algum problema. 
 **/
extern int writeDisk(short  driveNumber, struct diskAddress *address, char *buffer);
/**
 * \brief Ler informa��es no disco.
 * \param driveNumber N�mero do hd que os dados ser�o sendo lidos.
 * \param address Guarda o endere�o do HD que ir� ser usado.
 * \param buffer Endere�o da v�rivel que vai ser gravadas as informa��es lidas no hd.
 * \return Retorna os dados gravador em uma certa regi�o do HD.
 **/
extern int readDisk(short  driveNumber, struct diskAddress *address, char *buffer);
/**
 * \brief Espera o disco complentar uma opera��o  e informa se teve sucesso ou n�o.
 * \return Retorna o valor 0, caso a opera��o que estava sendo realizada terminou com sucesso.
 **/
#endif /*KIDE_H_*/
