#ifndef KFAT_H_
#define KFAT_H_

 /*Procura por um cluster, atravez do nome do arquivo.*/
extern int fatSeekCluster();
/*Reconhece a quantidade de parti��es a serem carregadas.*/
extern int loadFatIDEDrivers();
/*Formata o disco, para um sistema de arquivos supportado pelo APOS. */
extern int formatDisk(short int driveNumber);
/*Calcula atravez da LBA um endere�� no hd.*/
extern int calcAddress(short int volumeNumber, struct diskAddress *address);
/*Ler um determinado arquivo no diret�rio atual.*/
extern int readFile(short int driveNumber, char *name, char *type) ;
/*Cria um diret�rio.*/
extern int createDIR(short int driveNumber, char *name,char attr);
/*Cria um arquivos.*/
extern int createFile(short int driveNumber, char *name, char attr, char *file);
/*Deleta um item da lista de diretorios, de forma l�gica.*/
extern int delDirectoryEntry(short int driveNumber, short int startCluster);
/*Deleta um arquivo de forma logica*/
extern int delFile(short int driveNumber, char *name, char *type,unsigned short int dirStartCluster);
/*Deleta um diret�rio*/
extern int delDir(short int driveNumber, char *fileName);
/*lista todos os item de um diret�rio.*/
extern int printDir(short int driveNumber);
/**atualiza  o diret�rio atual para algum que seja filho do atual.*/
extern int refreshDirectoryEnter(short int driveNumber, char *name);
#endif /*KFAT_H_*/
