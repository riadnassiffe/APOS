/*
 * Implementa��o baseada na vers�o anterior do APOS.
 * Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
 * Aluno: R�ad Mattos Nassiffe		   riad.nassiffe@gmail.com
 */
/**
 * \file Ide.h
 * \brief Fornece um arquivo com todas as defini��es e prototipa��o das fun��es
 *  utilizadas pelo  arquivo Ide.c.
 * \author R�ad Mattos Nassiffe
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
 |    |  1   |  1   | X | X | X | High Impedance 	 | n�o utilizado 		   | Control   |
 |3FX |  1   |  0   | 0 | X | X | High Impedance 	 | n�o utilizado 		   | Block     |
 |3FX |  1   |  0   | 1 | 0 | X | High Impedance 	 | n�o utilizado 		   | Registers |
 |3F6 |  1   |  0   | 1 | 1 | 0 | Altern Status  	 | Dispositivo de controle |    |      |
 |3F7 |  1   |  0   | 1 | 1 | 1 | Drive Address  	 | n�o utilizado           | <--+      |
 +----+------+------+---+---+---+--------------------+-------------------------+-----------+
 |1F0 |  0   |  1   | 0 | 0 | 0 | porta de dados	 | porta de dados	   	   | <--+      |
 |1F1 |  0   |  1   | 0 | 0 | 1 | erro de registrador| Precomp       		   |    |      |
 |1F2 |  0   |  1   | 0 | 1 | 0 | contador de setor  | contador de setor       | Command   |
 |1F3 |  0   |  1   | 0 | 1 | 1 | n�mero de setor 	 | n�mero de setor         | Block     |
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
/** \attention de acordo com a especifica��o do dispositivo IDE, disponivel no site do projeto BOCHS a melhor op��o para ativar o comando de escrita no HD
 * seria 0x31, entretanto ela n�o esta implementada no BOCHS, por isso foi usada a op��o 0x30, que tenta gravar a informa��o apenas uma vez, retornando 
 * erro se n�o conseguir, enquanto a 0x31 tentaria gravar a o dados mais de uma vez, antes de retornar erro.*/
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

/** Posi��es da lista de informa��o sobre o HD*/
#define IDE_INDENTIFY_HEADERS	    	0x03
#define IDE_INDENTIFY_CYLINDERS			0x01
#define IDE_INDENTIFY_BYTES_TRACK		0x04
#define IDE_INDENTIFY_BYTES_SECTOR		0x05
#define IDE_INDENTIFY_SECTORS_TRACK     0x06

/**macro utilizada para pegar os oito bits menos significativos */
#define LOW_BYTE(x)	(x & 0xff)
/**macro utilizada para pega os oito bits mais significativos */
#define HIGH_BYTE(x)	((x >> 8) & 0xff)

/**N�mero maximo de disco que o driver podem ser inicializados.*/
#define MAX_DISC   2

/**
 * \struct disk
 * 
 * \brief Estrutura que guardas as informa��es necess�rias de um HD.
 */
typedef struct {
	/** Guarda os valores do cilindro do HD*/ 
	short cylinders; 
	/** Guarda a quantidade de cabe�as do HD*/
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
 * \brief Estrutura respons�vel por armazenar o endere�o de uma posi��o do HD.
 */
struct diskAddress {
	/** Endere�o l�gico do cluster no HD.*/
	short lba;
	/** Cabelha�alho onde o cluster se encontra.*/
	short head;
	/** Setor do cilindro que o cluster se encontra.*/
	short sector;
	/** Cilindro do cabe��o que o cluster se encontra.*/
	short cylinder;
	/** Guarda o valor 1 se houve algum erro no momento de calcular as o endere�o do cluster.*/
	char error :1;
}__attribute__((packed));;

/**
 * \brief Inicializa os dispositivos IDE que achar, sendo limitado at� dois hds. 
 **/
void initIDE();
/**
 * \brief Detecta HD e captura as informa��es dele.
 * \param diskNumber � um inteiro que corresponde ao n�mero do HD que esta sendo detectado no momento, 0 para o master e 1 para o slave.
 * \attention Esse C�digo na vers�o passada havia sido baseado no GeekOs, agora modifica para detectar at� dois HDs.
 * \return Retorna 0(zero), se o HD for encontrado e -1, caso ele n�o seja encontrado.
 **/
int detecDisk(short diskNumber);
/**
 * \brief Retornar a quantidade de cilindros de um hd.
 * \param diskNumber Indica de qual hd ser� retornado a quantidade de cilindros.
 * \return Retorna a quantidade de cilindros do HD.
 **/
int getDiskCylinders(short diskNumber);
/**
 * \brief Retornar a quantidade de cabe�alhos de um hd.
 * \param diskNumber Indica de qual hd ser� retornado a quantidade cabe�alhos.
 * \return Retorna a quantidade de cabe�alhos em um disco.
 **/
int getDiskHeads(short diskNumber);
/**
 * \brief Retornar a quantidade de setores  por trilha de um hd.
 * 
 * \param diskNumber indica de qual hd ser� retornado a quantidade setores.
 * \return Retorna a quantidade de setores por track de um disco.
 **/
int getDiskSectorsPerTrack(short diskNumber);
/**
 * \brief Essa fun��o � para uso interno, para depurar o c�digo e mostrar informa��es do hd.
 * \param diskNumber indica de qual hd ser� retornado a quantidade setores.
 * \return Imprimi na tela na tela todas as informa��es capturadas sobre o HD.
 **/
void getDiskInfo(int diskNumber);
/**
 * \brief Essa fun��o verifica a quantidade de HDs detectados.
 * \return Retorna quantidade de discos inicializados.
 **/
int getDisks();
/**
 * \brief Calcula o tamanho do disco.
 * \return Retorna o tamanho do disco. 
 **/
int getDiskSize(short diskNumber);
/**
 * \brief Grava informa��es no disco.
 * \param driveNumber n�mero do hd que os dados ser�o sendo gravados.
 * \param *address guarda endere�o do HD que ir� ser usado..
 * \param *buffer endere�o da vari�vel que vai ser gravada no hd.
 * \return Retorna 0(zero), se tiver sucesso na escrita, e diferente de 0 se tiver algum problema. 
 **/
int writeDisk(short  driveNumber, struct diskAddress *address, char *buffer);
/**
 * \brief Ler informa��es no disco.
 * \param driveNumber N�mero do hd que os dados ser�o sendo lidos.
 * \param address Guarda o endere�o do HD que ir� ser usado.
 * \param buffer Endere�o da vari�vel que vai ser gravadas as informa��es lidas no hd.
 * \return Retorna os dados gravador em uma certa regi�o do HD.
 **/
int readDisk(short  driveNumber, struct diskAddress *address, char *buffer);
/**
 * \brief Espera o disco completar uma opera��o  e informa se teve sucesso ou n�o.
 * \return Retorna o valor 0, caso a opera��o que estava sendo realizada terminou com sucesso.
 **/
int diskWait();
/**
 * \brief Gera um delay.
 * \attention Durante o desenvolvimento foi encontrado um problema na execu��o de muitas repeti��es de tarefas, com acesso direto a HD. No c�digo
 * fonte do GeekOs foi encontrado uma referencia a esse problema. De acordo com o coment�rio do GeekOS o problema pertenceria a implementa��o da 
 * m�quina virtual BOCHS, entretanto essa atribui o erro a uma especifica��o do LBA28. Para resolver esse problema de forma correta, seria necess�rio
 * pegar alguns dados no HD, e calcular um delay. Como os APOS n�o implementa ainda um modulo de manipula��o de tempo isso n�o foi poss�vel. Por isso
 * configuramos no bochs uma certa freq��ncia para a m�quina a ser emulada, e com base nessa freq��ncia achamos esse delay.
 * \return int retorna o valor 0, caso a opera��o que estava sendo realizada terminou com sucesso.
 **/
void delay();
#endif /*IDE_H_*/

