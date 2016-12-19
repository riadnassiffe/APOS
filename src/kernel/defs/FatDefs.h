/**
 * Criação das estruturas e definições para os sistema de arquivos.
 * Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
 * Aluno: Ríad Mattos Nassiffe		   riad.nassiffe@gmail.com
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
 * \brief Representa o nome de um arquivo ou diretório.
 */
struct Name{
	/**Nome do arquivo com no Maximo 8 caracteres.*/
	char name[8];
	/**Tipo do arquivo com no Maximo 3 caracteres.*/
	char type[3];
}__attribute__((packed));

/**
 * \struct Attribute
 * \brief Representa os atributos que um arquivo ou diretório pode ter. 
 */
struct Attribute{
	/** variável não utilizada*/
	 unsigned char reserved1:1;
	 /** variável não utilizada*/
	 unsigned char reserved2:1;
	 /** se tiver valor 1 indica que o objeto é somente de leitura*/
	 unsigned char readOnly:1;
	 /**se tiver valor 1 indica que o objeto é um arquivo oculto*/
	 unsigned char hidden:1;
	 /** se tiver valor 1 indica que o objeto é arquivo de Sistema.*/
	 unsigned char system:1;
	 /**se tiver valor 1 indica que o objeto é existe, caso seja 0 ele foi apagado.*/
	 unsigned char status:1;
	 /**se tiver valor 1 indica que o objeto é um diretório.*/
	 unsigned char directory:1;
	 /**se tiver valor 1 indica que o objeto é um arquivo.*/
	 unsigned char file:1;
}__attribute__((packed));

/**
 * \struct fatDirectory
 *	\brief Estrutura que representa as entradas de um diretório.
 */
struct fatDirectory {
	/** Estrutura que da nome aos arquivos */
	struct Name file;
	/** Estrutura que representa os atributos do arquivo */
	struct Attribute attribute;
	/** variável não utilizada*/
	char reserved[10];
	/** variável que guarda o hora*/
	struct Time time;
	/** variável que guarda a data*/
	struct Date date;
	/** variável que guarda o cluster inicial da arquivo*/
	unsigned short int startCluster;
	/**variavel que guarda o tamanho da variável do arquivo*/
	unsigned long int fileSize;
}__attribute__((packed));/***/

/**
 * \struct tBpb
 * \brief Esta estrutura que representa os dados guardados no setor de boot da partição.
 */
struct tBpb{
	/**instrução de salto para o código de boot*/	
  unsigned char  BS_jmpBoot[3];	
  /**Nome do sistema  que formatou o volume*/
  unsigned char  BS_OEMName[8];	
  /**Quantos bytes são gravados por setor*/	
  unsigned short  BPB_BytsPerSec;
  
  unsigned char  BPB_SecPerClus;/**Quantos setores por cluster*/	
  /**Números de setores reservados*/
  unsigned short  BPB_ResvdSecCnt;	
  /**Número de tabelas fat no disco.*/
  unsigned char  BPB_NumFATs;		
  /**Guarda o valor da lbl*/
  unsigned short  BPB_LBL;					
  /**Número total de setores no disco.*/
  unsigned short  BPB_TotSec16;	
  /**Tipo de midia, fixa, removivel ....*/
  unsigned char  BPB_Media;		
  /**Tamanho do setor na FAT*/
  unsigned short  BPB_FATSz16;				
  /**Quantidade de setor por trilha*/
  unsigned short  BPB_SecPerTrk;		
  /**Número de cabeças  do HD*/
  unsigned short  BPB_NumHeads;			
  /**Número de setores antes do inicio da partição*/
  unsigned long BPB_HiddSec;
  /**Versão do sistema de arquivo.*/
  unsigned short  BPB_FSVer;				
  /**Número do cluster do Root*/	
  unsigned long BPB_RootClus;
  /**Número lógico do drive da partição*/
  unsigned char  BS_DrvNum;		
  /**Espaço reservado para alguma informação que extra.*/
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
 * \brief Guardar todas informações sobre o sistema de arquivo de um dispositivo.
 */
struct FatInstance{
	struct tBpb bs;
	/** ponteiro para a posição atual da FAT*/
	short int fatPos;
	/** entrada atual da fat, que esta sendo utilizada.*/
	struct fatDirectory actualDirectory;
	/** Tabela de alocação de arquivos*/
	unsigned short int  fatList[PARTITIONMAXSIZE];
	/** Primeira posição livre na FAT.*/
	short int LBL;
}__attribute__((packed));

#endif /*FATDEFS_H_*/

