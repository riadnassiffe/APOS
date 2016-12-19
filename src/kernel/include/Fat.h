/*
 * Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
 * Aluno: R�ad Mattos Nassiffe		   riad.nassiffe@gmail.com
 */
#ifndef FAT_H_
#define FAT_H_

#include "../interface/KIscrn.h"
#include "Ide.h"
#include "../../libs/include/Slib.h"
#include "../../modulos/interface/Igmem.h"
#include "../defs/FatDefs.h"
#include "../../include/Defs.h"

/**Procura por um cluster, atrav�s do nome do arquivo.*/
int fatSeekCluster();/*n�o implementado*/
/**Reconhece a quantidade de parti��es a serem carregadas.*/
int loadFatIDEDrivers();
/**Carrega as informa��es do volume formatado, para inicializar a fat*/
int loadFatList(short int driveNumber);
/**Formata o disco, para um sistema de arquivos suportado pelo APOS. */
int formatDisk(short int driveNumber);
/**Calcula atravez da LBA um endere�� no hd.*/
int calcAddress(short int volumeNumber, struct diskAddress *address);
/**Ler um determinado arquivo no diret�rio atual.*/
int readFile(short int driveNumber, char *name, char *type) ;
/**adiciona um item a um diret�rio.*/
int addDirectoryEntry(short int driveNumber, unsigned short int lba,char *name, struct Attribute attribute, unsigned long int fileSize);
/**Define valores para uma entrada de um diret�rio.*/
int setDirectoryEntry(struct fatDirectory *newEntry, char *name,struct Attribute attribute, unsigned short int startCluster,unsigned long int fileSize);
/**Cria um diret�rio.*/
int createDIR(short int driveNumber, char *name,char attr);
/**Cria um arquivos.*/
int createFile(short int driveNumber, char *name, char attr, char *file);
/**Deleta um item da lista de diret�rios, de forma l�gica.*/
int delDirectoryEntry(short int driveNumber, short int startCluster);
/**Deleta um arquivo de forma l�gica*/
int delFile(short int driveNumber, char *name, char *type,unsigned short int dirStartCluster);
/**Deleta um diret�rio*/
int delDir(short int driveNumber, char *fileName);
/**lista todos os item de um diret�rio.*/
int printDir(short int driveNumber);
/**Atualiza a tabela fat no hd.*/
int updateFat(int driveNumber);
/**atualiza  o diret�rio atual para algum que seja filho do atual.*/
int refreshDirectoryEnter(short int driveNumber, char *name);

#endif /*FAT_H_*/

