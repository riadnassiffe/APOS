#ifndef KFAT_H_
#define KFAT_H_

 /*Procura por um cluster, atravez do nome do arquivo.*/
extern int fatSeekCluster();
/*Reconhece a quantidade de partições a serem carregadas.*/
extern int loadFatIDEDrivers();
/*Formata o disco, para um sistema de arquivos supportado pelo APOS. */
extern int formatDisk(short int driveNumber);
/*Calcula atravez da LBA um endereçõ no hd.*/
extern int calcAddress(short int volumeNumber, struct diskAddress *address);
/*Ler um determinado arquivo no diretório atual.*/
extern int readFile(short int driveNumber, char *name, char *type) ;
/*Cria um diretório.*/
extern int createDIR(short int driveNumber, char *name,char attr);
/*Cria um arquivos.*/
extern int createFile(short int driveNumber, char *name, char attr, char *file);
/*Deleta um item da lista de diretorios, de forma lógica.*/
extern int delDirectoryEntry(short int driveNumber, short int startCluster);
/*Deleta um arquivo de forma logica*/
extern int delFile(short int driveNumber, char *name, char *type,unsigned short int dirStartCluster);
/*Deleta um diretório*/
extern int delDir(short int driveNumber, char *fileName);
/*lista todos os item de um diretório.*/
extern int printDir(short int driveNumber);
/**atualiza  o diretório atual para algum que seja filho do atual.*/
extern int refreshDirectoryEnter(short int driveNumber, char *name);
#endif /*KFAT_H_*/
