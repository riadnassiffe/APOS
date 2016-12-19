/**
 * Cria��o das estruturas e defini��es para os sistema de arquivos.
 * Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
 * Aluno: R�ad Mattos Nassiffe		   riad.nassiffe@gmail.com
 * 
 */
#ifndef FATDEFS_H_
#define FATDEFS_H_
#define EOC  0xFFFF
#define checked 1
#define unchecked 0
#define PARTITIONMAXSIZE 65535
#include "../defs/Timer.h"



/**
 * \struct Name
 * \brief Representa o nome de um arquivo ou diret�rio.
 */
struct Name{
	/**Nome do arquivo com no Maximo 8 caracteres.*/
	char name[8];
	/**Tipo do arquivo com no Maximo 3 caracteres.*/
	char type[3];
}__attribute__((packed));

/**
 * \struct Attribute
 * \brief Representa os atributos que um arquivo ou diret�rio pode ter. 
 */
struct Attribute{
	/** vari�vel n�o utilizada*/
	 unsigned char reserved1:1;
	 /** vari�vel n�o utilizada*/
	 unsigned char reserved2:1;
	 /** se tiver valor 1 indica que o objeto � somente de leitura*/
	 unsigned char readOnly:1;
	 /**se tiver valor 1 indica que o objeto � um arquivo oculto*/
	 unsigned char hidden:1;
	 /** se tiver valor 1 indica que o objeto � arquivo de Sistema.*/
	 unsigned char system:1;
	 /**se tiver valor 1 indica que o objeto � existe, caso seja 0 ele foi apagado.*/
	 unsigned char status:1;
	 /**se tiver valor 1 indica que o objeto � um diret�rio.*/
	 unsigned char directory:1;
	 /**se tiver valor 1 indica que o objeto � um arquivo.*/
	 unsigned char file:1;
}__attribute__((packed));

/**
 * \struct fatDirectory
 *	\brief Estrutura que representa as entradas de um diret�rio.
 */
struct fatDirectory {
	/** Estrutura que da nome aos arquivos */
	struct Name file;
	/** Estrutura que representa os atributos do arquivo */
	struct Attribute attribute;
	/** vari�vel n�o utilizada*/
	char reserved[10];
	/** vari�vel que guarda o hora*/
	struct Time time;
	/** vari�vel que guarda a data*/
	struct Date date;
	/** vari�vel que guarda o cluster inicial da arquivo*/
	unsigned short int startCluster;
	/**variavel que guarda o tamanho da vari�vel do arquivo*/
	unsigned long int fileSize;
}__attribute__((packed));/***/

/**
 * \struct tBpb
 * \brief Esta estrutura que representa os dados guardados no setor de boot da parti��o.
 */
struct tBpb{
	/**instru��o de salto para o c�digo de boot*/	
  unsigned char  BS_jmpBoot[3];	
  /**Nome do sistema  que formatou o volume*/
  unsigned char  BS_OEMName[8];	
  /**Quantos bytes s�o gravados por setor*/	
  unsigned short  BPB_BytsPerSec;
  
  unsigned char  BPB_SecPerClus;/**Quantos setores por cluster*/	
  /**N�meros de setores reservados*/
  unsigned short  BPB_ResvdSecCnt;	
  /**N�mero de tabelas fat no disco.*/
  unsigned char  BPB_NumFATs;		
  /**Guarda o valor da lbl*/
  unsigned short  BPB_LBL;					
  /**N�mero total de setores no disco.*/
  unsigned short  BPB_TotSec16;	
  /**Tipo de midia, fixa, removivel ....*/
  unsigned char  BPB_Media;		
  /**Tamanho do setor na FAT*/
  unsigned short  BPB_FATSz16;				
  /**Quantidade de setor por trilha*/
  unsigned short  BPB_SecPerTrk;		
  /**N�mero de cabe�as  do HD*/
  unsigned short  BPB_NumHeads;			
  /**N�mero de setores antes do inicio da parti��o*/
  unsigned long BPB_HiddSec;
  /**Vers�o do sistema de arquivo.*/
  unsigned short  BPB_FSVer;				
  /**N�mero do cluster do Root*/	
  unsigned long BPB_RootClus;
  /**N�mero l�gico do drive da parti��o*/
  unsigned char  BS_DrvNum;		
  /**Espa�o reservado para alguma informa��o que extra.*/
  unsigned char  BS_Reserved2;	
  /** assinatura do boot, pode ser qualquer uma 0x29*/
  unsigned char  BS_BootSig;		
  /**Determinar um ID para o volume.*/
  unsigned long BS_VolID;						
  /**Determinar o nome do volume.*/
  unsigned char  BS_VolLab[11];					
  /**Determinar o tipo de sistema de arquivo.*/
  unsigned char  BS_FilSysType[8];		
  
}__attribute__ ((packed));															
	

/**
 * \struct FatInstance
 * \brief Guardar todas informa��es sobre o sistema de arquivo de um dispositivo.
 */
struct FatInstance{
	struct tBpb bs;
	/** ponteiro para a posi��o atual da FAT*/
	short int fatPos;
	/** entrada atual da fat, que esta sendo utilizada.*/
	struct fatDirectory actualDirectory;
	/** Tabela de aloca��o de arquivos*/
	unsigned short int  fatList[PARTITIONMAXSIZE];
	/** Primeira posi��o livre na FAT.*/
	short int LBL;
}__attribute__((packed));

#endif /*FATDEFS_H_*/

