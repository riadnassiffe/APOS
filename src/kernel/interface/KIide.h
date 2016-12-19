#ifndef KIIDE_H_
#define KIIDE_H_

#include "../include/Ide.h"

/**
 * \brief Inicializa os dispositivos IDE que achar, sendo limitado até dois hds. 
 **/
extern void initIDE();
/**
 * \brief Retornar a quantidade de cilindros de um hd.
 * \param diskNumber Indica de qual hd será retornado a quantidade de cilindros.
 * \return Retorna a quantidade de cilindoros do HD.
 **/
extern int getDiskCylinders(short diskNumber);
/**
 * \brief Retornar a quantidade de cabeçalhos de um hd.
 * \param diskNumber Indica de qual hd será retornado a quantidade cabeçalhos.
 * \return Retorna a quantidade de cabeçalhos em um disco.
 **/
extern int getDiskHeads(short diskNumber);
/**
 * \brief Retornar a quantidade de setores  por trilha de um hd.
 * \param diskNumber indica de qual hd será retornado a quantidade setores.
 * \return Retorna a quantidade de setores po track de um disco.
 **/
extern int getDiskSectorsPerTrack(short diskNumber);
/**
 * \brief Essa função é para uso interno, para debugar o código e mostrar informações do hd.
 * \param diskNumber indica de qual hd será retornado a quantidade setores.
 * \return Imprimi na tela na tela todas as informações capturadas sobre o HD.
 **/
extern void getDiskInfo(int diskNumber);
/**
 * \brief Essa função verifica a quantidade de HDs detectados.
 * \return Retorna quantidade de discos inicializados.
 **/
extern int getDisks();
/**
 * \brief Calcula o tamanho do disco.
 * \return Retorna o tamanho do disco. 
 **/
extern int getDiskSize(short diskNumber);
/**
 * \brief Grava informações no disco.
 * \param driveNumber número do hd que os dados serão sendo gravados.
 * \param *address guarda endereço do HD que irá ser usado..
 * \param *buffer endereço da várivel que vai ser gravada no hd.
 * \return Retorna 0(zero), se tiver sucesso na escrita, e diferente de 0 se tiver algum problema. 
 **/
extern int writeDisk(short  driveNumber, struct diskAddress *address, char *buffer);
/**
 * \brief Ler informações no disco.
 * \param driveNumber Número do hd que os dados serão sendo lidos.
 * \param address Guarda o endereço do HD que irá ser usado.
 * \param buffer Endereço da várivel que vai ser gravadas as informações lidas no hd.
 * \return Retorna os dados gravador em uma certa região do HD.
 **/
extern int readDisk(short  driveNumber, struct diskAddress *address, char *buffer);
/**
 * \brief Espera o disco complentar uma operação  e informa se teve sucesso ou não.
 * \return Retorna o valor 0, caso a operação que estava sendo realizada terminou com sucesso.
 **/
#endif /*KIDE_H_*/
