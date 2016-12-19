/*
 * Implementação baseada na versão anterior do APOS.
 * Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
 * Aluno: Ríad Mattos Nassiffe		   riad.nassiffe@gmail.com
 */
/**
 * \file Ide.h
 * \brief Fornece um arquivo com todas as definições e prototipação das funções
 *  utilizadas pelo  arquivo Ide.c.
 * \author Ríad Mattos Nassiffe
 */
#ifndef IDE_H_
#define IDE_H_
#include "../interface/KIio.h"
#include "../interface/KIscrn.h"
#include "../../include/Defs.h"

/*
 +----+------+------+---+---+---+---------------------+------------------------+
 |Addr|-CS1FX|-CS3FX|SA2|SA1|SA0| Read (-IOR)    	 | Write (-IOW)    		   |
 +----+------+------+---+---+---+---------------------+-------------------------+----------+
 |    |  0   |  0   | X | X | X | ilegal         	 | ilegal        		   | <--+      |
 |    |  1   |  1   | X | X | X | High Impedance 	 | não utilizado 		   | Control   |
 |3FX |  1   |  0   | 0 | X | X | High Impedance 	 | não utilizado 		   | Block     |
 |3FX |  1   |  0   | 1 | 0 | X | High Impedance 	 | não utilizado 		   | Registers |
 |3F6 |  1   |  0   | 1 | 1 | 0 | Altern Status  	 | Dispositivo de controle |    |      |
 |3F7 |  1   |  0   | 1 | 1 | 1 | Drive Address  	 | não utilizado           | <--+      |
 +----+------+------+---+---+---+--------------------+-------------------------+-----------+
 |1F0 |  0   |  1   | 0 | 0 | 0 | porta de dados	 | porta de dados	   	   | <--+      |
 |1F1 |  0   |  1   | 0 | 0 | 1 | erro de registrador| Precomp       		   |    |      |
 |1F2 |  0   |  1   | 0 | 1 | 0 | contador de setor  | contador de setor       | Command   |
 |1F3 |  0   |  1   | 0 | 1 | 1 | número de setor 	 | número de setor         | Block     |
 |1F4 |  0   |  1   | 1 | 0 | 0 | Cylinder Low   	 | Cylinder Low  		   | Registers |
 |1F5 |  0   |  1   | 1 | 0 | 1 | Cylinder High  	 | Cylinder High 		   |    |      |
 |1F6 |  0   |  1   | 1 | 1 | 0 | Drive / Head   	 | Drive / Head  		   |    |      |
 |1F7 |  0   |  1   | 1 | 1 | 1 | Status         	 | comando	               | <--+      |
 +----+------+------+---+---+---+---------------------+-------------------------+-----------+
 */

/** Registradores*/
#define IDE_REG_DEVICE_DATA_PORT	 0x1f0
#define IDE_REG_DEVICE_ERROR_PORT	 0x1f1
#define IDE_SECTOR_COUNT_REGISTER	 0x1f2
#define IDE_SECTOR_NUMBER_REGISTER	 0x1f3
#define IDE_CYLINDER_LOW_REGISTER	 0x1f4
#define IDE_REG_DEVICE_CYLINDER_HIGH 0x1f5
#define IDE_REG_DEVICE_HEAD 		 0x1f6
#define IDE_STATUS_REGISTER			 0x1f7
#define IDE_COMMAND_REGISTER		 0x1f7
#define IDE_REG_DEVICE_CONTROL  	 0x3F6

/** Drives */
#define IDE_Master	0xa0
#define IDE_Slave	0xb0

/** Comandos */
#define IDE_COMMAND_IDENTIFY_DRIVE		0xEC
#define IDE_COMMAND_SEEK	    		0x70
#define IDE_COMMAND_READ_SECTORS		0x21//0x20
#define IDE_COMMAND_READ_BUFFER			0xE4
/** \attention de acordo com a especificação do dispositivo IDE, disponivel no site do projeto BOCHS a melhor opção para ativar o comando de escrita no HD
 * seria 0x31, entretanto ela não esta implementada no BOCHS, por isso foi usada a opção 0x30, que tenta gravar a informação apenas uma vez, retornando 
 * erro se não conseguir, enquanto a 0x31 tentaria gravar a o dados mais de uma vez, antes de retornar erro.*/
#define IDE_COMMAND_WRITE_SECTORS		0x30//31
#define IDE_COMMAND_WRITE_BUFFER		0xE8
#define IDE_COMMAND_DIAGNOSTIC			0x90
#define IDE_COMMAND_ATAPI_IDENT_DRIVE	0xA1

/** Estado */
#define IDE_STATUS_DRIVE_BUSY			0x80
#define IDE_STATUS_DRIVE_READY			0x40
#define IDE_STATUS_DRIVE_WRITE_FAULT	0x20
#define IDE_STATUS_DRIVE_SEEK_COMPLETE	0x10
#define IDE_STATUS_DRIVE_DATA_REQUEST	0x08
#define IDE_STATUS_DRIVE_CORRECTED_DATA	0x04
#define IDE_STATUS_DRIVE_INDEX			0x02
#define IDE_STATUS_DRIVE_ERROR			0x01

/** Bist de controle do dispositivo */
#define IDE_DCR_NOINTERRUPT		0x02
#define IDE_DCR_RESET			0x04

/** Bits de controle */
#define IDE_CONTROL_REGISTER		0x3F6
#define IDE_CONTROL_SOFTWARE_RESET	0x04
#define IDE_CONTROL_INT_DISABLE		0x02

/** Posições da lista de informação sobre o HD*/
#define IDE_INDENTIFY_HEADERS	    	0x03
#define IDE_INDENTIFY_CYLINDERS			0x01
#define IDE_INDENTIFY_BYTES_TRACK		0x04
#define IDE_INDENTIFY_BYTES_SECTOR		0x05
#define IDE_INDENTIFY_SECTORS_TRACK     0x06

/**macro utilizada para pegar os oito bits menos significativos */
#define LOW_BYTE(x)	(x & 0xff)
/**macro utilizada para pega os oito bits mais significativos */
#define HIGH_BYTE(x)	((x >> 8) & 0xff)

/**Número maximo de disco que o driver podem ser inicializados.*/
#define MAX_DISC   2

/**
 * \struct disk
 * 
 * \brief Estrutura que guardas as informações necessárias de um HD.
 */
typedef struct {
	/** Guarda os valores do cilindro do HD*/ 
	short cylinders; 
	/** Guarda a quantidade de cabeças do HD*/
	short heads;
	/** Guarda a quantidade de setores por track no HD*/
	short sectorPerTrack;
	/** Guarda a quantidade de bytes por setor do HD*/
	short bytesPerSector;
	/** Guarda o estatus atual do HD, 1 se estiver ocupado e 0 se estiver livre.*/
	unsigned char status:1;
} disk;

/**
 * \struct diskAddress
 * \brief Estrutura responsável por armazenar o endereço de uma posição do HD.
 */
struct diskAddress {
	/** Endereço lógico do cluster no HD.*/
	short lba;
	/** Cabelhaçalho onde o cluster se encontra.*/
	short head;
	/** Setor do cilindro que o cluster se encontra.*/
	short sector;
	/** Cilindro do cabeção que o cluster se encontra.*/
	short cylinder;
	/** Guarda o valor 1 se houve algum erro no momento de calcular as o endereço do cluster.*/
	char error :1;
}__attribute__((packed));;

/**
 * \brief Inicializa os dispositivos IDE que achar, sendo limitado até dois hds. 
 **/
void initIDE();
/**
 * \brief Detecta HD e captura as informações dele.
 * \param diskNumber É um inteiro que corresponde ao número do HD que esta sendo detectado no momento, 0 para o master e 1 para o slave.
 * \attention Esse Código na versão passada havia sido baseado no GeekOs, agora modifica para detectar até dois HDs.
 * \return Retorna 0(zero), se o HD for encontrado e -1, caso ele não seja encontrado.
 **/
int detecDisk(short diskNumber);
/**
 * \brief Retornar a quantidade de cilindros de um hd.
 * \param diskNumber Indica de qual hd será retornado a quantidade de cilindros.
 * \return Retorna a quantidade de cilindros do HD.
 **/
int getDiskCylinders(short diskNumber);
/**
 * \brief Retornar a quantidade de cabeçalhos de um hd.
 * \param diskNumber Indica de qual hd será retornado a quantidade cabeçalhos.
 * \return Retorna a quantidade de cabeçalhos em um disco.
 **/
int getDiskHeads(short diskNumber);
/**
 * \brief Retornar a quantidade de setores  por trilha de um hd.
 * 
 * \param diskNumber indica de qual hd será retornado a quantidade setores.
 * \return Retorna a quantidade de setores por track de um disco.
 **/
int getDiskSectorsPerTrack(short diskNumber);
/**
 * \brief Essa função é para uso interno, para depurar o código e mostrar informações do hd.
 * \param diskNumber indica de qual hd será retornado a quantidade setores.
 * \return Imprimi na tela na tela todas as informações capturadas sobre o HD.
 **/
void getDiskInfo(int diskNumber);
/**
 * \brief Essa função verifica a quantidade de HDs detectados.
 * \return Retorna quantidade de discos inicializados.
 **/
int getDisks();
/**
 * \brief Calcula o tamanho do disco.
 * \return Retorna o tamanho do disco. 
 **/
int getDiskSize(short diskNumber);
/**
 * \brief Grava informações no disco.
 * \param driveNumber número do hd que os dados serão sendo gravados.
 * \param *address guarda endereço do HD que irá ser usado..
 * \param *buffer endereço da variável que vai ser gravada no hd.
 * \return Retorna 0(zero), se tiver sucesso na escrita, e diferente de 0 se tiver algum problema. 
 **/
int writeDisk(short  driveNumber, struct diskAddress *address, char *buffer);
/**
 * \brief Ler informações no disco.
 * \param driveNumber Número do hd que os dados serão sendo lidos.
 * \param address Guarda o endereço do HD que irá ser usado.
 * \param buffer Endereço da variável que vai ser gravadas as informações lidas no hd.
 * \return Retorna os dados gravador em uma certa região do HD.
 **/
int readDisk(short  driveNumber, struct diskAddress *address, char *buffer);
/**
 * \brief Espera o disco completar uma operação  e informa se teve sucesso ou não.
 * \return Retorna o valor 0, caso a operação que estava sendo realizada terminou com sucesso.
 **/
int diskWait();
/**
 * \brief Gera um delay.
 * \attention Durante o desenvolvimento foi encontrado um problema na execução de muitas repetições de tarefas, com acesso direto a HD. No código
 * fonte do GeekOs foi encontrado uma referencia a esse problema. De acordo com o comentário do GeekOS o problema pertenceria a implementação da 
 * máquina virtual BOCHS, entretanto essa atribui o erro a uma especificação do LBA28. Para resolver esse problema de forma correta, seria necessário
 * pegar alguns dados no HD, e calcular um delay. Como os APOS não implementa ainda um modulo de manipulação de tempo isso não foi possível. Por isso
 * configuramos no bochs uma certa freqüência para a máquina a ser emulada, e com base nessa freqüência achamos esse delay.
 * \return int retorna o valor 0, caso a operação que estava sendo realizada terminou com sucesso.
 **/
void delay();
#endif /*IDE_H_*/

