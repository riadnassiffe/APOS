/*
 * Implementa��o baseada na vers�o anterior do APOS.
 * Orientador: Edeyson Andrade Gomes   edeyson@gmail.com
 * Aluno: R�ad Mattos Nassiffe		   riad.nassiffe@gmail.com
 */

/** 
 * \file 
 * \brief Este arquivo tem a finalidade de implementar uma camada de manipula��o
 * de hd. Ele fornece as fun��es de gravar, ler e detecta at� dois HD.
 * \author R�ad Mattos Nassiffe
 */
#include "include/Ide.h"
/**
 * \brief Vari�vel que ativa o modo debug, se seu valor for TRUE(1) e desativa se for FALSE(0)
 **/
int debug = FALSE;
/**
 * \struct disk
 * \brief Lista com as informa��es dos discos* carregados.
 */
static disk diskList[MAX_DISC];

/**
 * \brief Inicializa os dispositivos IDE que achar, sendo limitado at� dois hds. 
 **/
void initIDE() {
	int IdeStatus=0;
	
	int count=0;
	APOSDEBUG(debug, "Inicializando dispositivos IDE...\n");
	Out_Byte(IDE_REG_DEVICE_CONTROL, IDE_DCR_NOINTERRUPT | IDE_DCR_RESET);
	Out_Byte(IDE_REG_DEVICE_CONTROL, IDE_DCR_NOINTERRUPT);
	IdeStatus = diskWait();
	APOSDEBUG(debug, "Valor do status da IDE:");
	printfInt(IdeStatus);
	APOSDEBUG(debug, "\n");
	if (IdeStatus == 0) {
		APOSDEBUG(debug,
				"Registradires de controle da IDE limpados com succeso...\n");
		for (count=0; count < 2; count++) {
			APOSDEBUG(debug, "Tentando detecar o HD n�mero:");
			printfInt(count);
			detecDisk(count);
		}
		APOSDEBUG(debug, "Dispositivos IDE inicializados....\n");
	} else {
		APOSDEBUG(debug,
				"Falha ao limpar os registradores de controle da IDE...\n");
	}
}

/**
 * \brief Detecta HD e captura as informa��es dele.
 * \param diskNumber � um inteiro que corresponde ao n�mero do HD que esta sendo detectado no momento, 0 para o master e 1 para o slave.
 * \attention Esse C�digo na vers�o passada havia sido baseado no GeekOs, agora modifica para detectar at� dois HDs.
 * \return Retorna 0(zero), se o HD for encontrado e -1, caso ele n�o seja encontrado.
 **/
int detecDisk(short int diskNumber) {
	int count, status;
	short info[256];
	int IdeStatus = 0;

	/*
	 * Caso n�o tenha sido detectado nenhum HD ele tenta detectar o principal(Master), caso algum j� tenha sido detectado,
	 *ele tenta detecar o escravo(slave).
	 */
	if (diskNumber == 0) {
		//enviamos para aos registradores do dispositivo, o c�digo correspondente a HD master.
		Out_Byte(IDE_REG_DEVICE_HEAD, IDE_Master);
	} else {
		//enviamos para aos registradores do dispositivo, o c�digo correspondente a HD slave.
		Out_Byte(IDE_REG_DEVICE_HEAD, IDE_Slave);
	}
	//enviamos para o dispositivo, respons�vel de executar a a��es, um c�digo para ele tentar reconhecer algum dispositivo,
	// que neste momento j� deve ter seu c�digo no registrador esperado pelo commando.
	Out_Byte(IDE_STATUS_REGISTER,IDE_COMMAND_IDENTIFY_DRIVE);
	
	IdeStatus = diskWait();
	
	/*
	 *Antes de dar o HD como reconhecido, precisamos verificar se a opera��o ocorreu com sucesso ou n�o, para isso temos que verificar o valor armazenado no 
	 * registrador IDE_STATUS_REGISTER(0x1f7)
	 */ 
	status = In_Byte(IDE_STATUS_REGISTER);

	//Se o valor armazenado for 0, significa que o dispositivo foi reconhecido, com sucesso, caso o contrario n�o existe ou pode esta com algum problema.
	if (IdeStatus == 0) {
		//caso hd tenha sido reconhecido com sucesso, precisamos agora ir no registrador onde ele guarda as informa��es e ler elas.
		for (count=0; count < 256; count++) {
			info[count]= In_Word(IDE_REG_DEVICE_DATA_PORT);
		}

		//a opera��o traz um vetor com 256 posi��es, sendo que no momento somente 4 delas nos importam, que s�o as seguintes:
		diskList[diskNumber].cylinders=info[IDE_INDENTIFY_CYLINDERS];
		diskList[diskNumber].heads=info[IDE_INDENTIFY_HEADERS];
		diskList[diskNumber].sectorPerTrack=info[IDE_INDENTIFY_SECTORS_TRACK];
		diskList[diskNumber].bytesPerSector=info[IDE_INDENTIFY_BYTES_SECTOR];
		diskList[diskNumber].status = 0;
		getDiskInfo(diskNumber);

	}else{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
/**
 * \brief Retornar a quantidade de cilindros de um hd.
 * \param diskNumber Indica de qual hd ser� retornado a quantidade de cilindros.
 * \return Retorna a quantidade de cilindros do HD.
 **/
int getDiskCylinders(short diskNumber) {
	return diskList[diskNumber].cylinders;
}
/**
 * \brief Retornar a quantidade de cabe�alhos de um hd.
 * \param diskNumber Indica de qual hd ser� retornado a quantidade cabe�alhos.
 * \return Retorna a quantidade de cabe�alhos em um disco.
 **/
int getDiskHeads(short diskNumber) {
	return diskList[diskNumber].heads;
}
/**
 * \brief Retornar a quantidade de setores por trilha de um hd.
 * 
 * \param diskNumber indica de qual hd ser� retornado a quantidade setores.
 * \return Retorna a quantidade de setores po track de um disco.
 **/
int getDiskSectorsPerTrack(short diskNumber) {
	return diskList[diskNumber].sectorPerTrack;
}
/**
 * \brief Essa fun��o � para uso interno, para depurar o c�digo e mostrar informa��es do hd.
 * \param diskNumber indica de qual hd ser� retornado a quantidade setores.
 * \return Imprimi na tela na tela todas as informa��es capturadas sobre o HD.
 **/
void getDiskInfo(int diskNumber) {
	puts("\n*** Configuracao do disco IDE");
	printfInt(diskNumber);
	puts("\n Cilindros: ");
	printfInt(diskList[diskNumber].cylinders);
	puts("\n Cabecas: ");
	printfInt(diskList[diskNumber].heads);
	puts("\n Setor por trilha: ");
	printfInt(diskList[diskNumber].sectorPerTrack);
	puts("\n Bytes por setor: ");
	printfInt(diskList[diskNumber].bytesPerSector);
	puts("\n*** IDE DISK   *** \n");
}
/**
 * \brief Essa fun��o verifica a quantidade de HDs detectados.
 * \return Retorna quantidade de discos inicializados.
 **/
int getDisks() {
	if (diskList[1].bytesPerSector != 0)
		return 2;
	else if (diskList[0].bytesPerSector != 0) {
		return 1;
	} else {
		return 0;
	}
}
/**
 * \brief Calcula o tamanho do disco.
 * \return Retorna o tamanho do disco. 
 **/
int getDiskSize(short diskNumber) {
	return diskList[diskNumber].bytesPerSector
			*diskList[diskNumber].sectorPerTrack* diskList[diskNumber].heads
			*diskList[diskNumber].cylinders;
}
/**
 * \brief Grava informa��es no disco.
 * \param driveNumber n�mero do hd que os dados ser�o sendo gravados.
 * \param *address guarda endere�o do HD que ir� ser usado..
 * \param *buffer endere�o da vari�vel que vai ser gravada no hd.
 * \return Retorna 0(zero), se tiver sucesso na escrita, e diferente de 0 se tiver algum problema. 
 **/
int writeDisk(short int driveNumber, struct diskAddress *address, char *buffer) {
	unsigned int count;/** contador para quantidade de vezes que o comando de grava��o ser� efetuada.*/
	short *w_buffer;/** vari�vel por passar os valores a serem gravados no HD.*/
	int IdeStatus = 0;
	

	if (diskList[driveNumber].status != 1) {
	 diskList[driveNumber].status = 1;
	//Desativando as interrup��es em ASM
	__asm__ __volatile__
("cli");
				
	delay();
	//Zera o registrador de erro, para evitar que alguma sugeira seja interpretada como erro mais tarde.
	Out_Byte(IDE_REG_DEVICE_ERROR_PORT,0x00);
	//Informa quantos setores ser�o lidos
	Out_Byte(IDE_SECTOR_COUNT_REGISTER, 1);
	//informa o n�mero do cd que ser� lido
	Out_Byte(IDE_SECTOR_NUMBER_REGISTER, address->sector);
	//Pega os primeiros 8 bits  do endere�o do cilindro.
	Out_Byte(IDE_CYLINDER_LOW_REGISTER, LOW_BYTE(address->cylinder));
	//Pega os 8 ultimos bits do endere�o do cilindro e passa.
	Out_Byte(IDE_REG_DEVICE_CYLINDER_HIGH, HIGH_BYTE(address->cylinder));
	//Verifica se que esta gravando � o HD 
	if (driveNumber == 0) {
		Out_Byte(IDE_REG_DEVICE_HEAD, IDE_Master | address->head);
	} else if (driveNumber == 1) {
		Out_Byte(IDE_REG_DEVICE_HEAD, IDE_Slave | address->head);
	}
	//escreve no registrador de controle da IDE, para ela gravar um setor por vez
	Out_Byte(IDE_STATUS_REGISTER, IDE_COMMAND_WRITE_SECTORS);
	delay();
	IdeStatus = diskWait();
	if (IdeStatus == EXIT_SUCCESS) {
		w_buffer=(short *)buffer;
		for (count=0; count < 256; count++) {
			Out_Word(IDE_REG_DEVICE_DATA_PORT, w_buffer[count]);
		}
		delay();
	}
	IdeStatus = diskWait();
	//Reativando structinterrup��es em ASM
	__asm__ __volatile__
(	"sti");
				diskList[driveNumber].status = 0;
	 }

	return EXIT_SUCCESS;

}

/**
 * \brief Ler informa��es no disco.
 * \param driveNumber N�mero do hd que os dados ser�o sendo lidos.
 * \param address Guarda o endere�o do HD que ir� ser usado.
 * \param buffer Endere�o da vari�vel que vai ser gravadas as informa��es lidas no hd.
 * \return Retorna os dados gravador em uma certa regi�o do HD.
 **/
int readDisk(short int driveNumber, struct diskAddress *address, char *buffer) {
	unsigned int count;///contador para quantidade de vezes que o comando de grava��o ser� usado.
	short *w_buffer;//
	int IdeStatus=0;
	
	//Verifica se o dispositivo esta liberado para gravar.
	if (diskList[driveNumber].status != 1) {
		diskList[driveNumber].status = 1;
		__asm__ __volatile__
("cli");
		delay();
		//Zera o registrador de erro, para evitar que alguma sugeira seja interpretada como erro mais tarde.
		Out_Byte(IDE_REG_DEVICE_ERROR_PORT,0x00);
		//Informa quantos setores ser�o lidos
		Out_Byte(IDE_SECTOR_COUNT_REGISTER, 1);
		//informa o n�mero do cd que ser� lido
		Out_Byte(IDE_SECTOR_NUMBER_REGISTER, address->sector);
		//Pega os primeiros 8 bits  do endere�o do cilindro.
		Out_Byte(IDE_CYLINDER_LOW_REGISTER, LOW_BYTE(address->cylinder));
		//Pega os 8 �ltimos bits do endere�o do cilindro e passa.
		Out_Byte(IDE_REG_DEVICE_CYLINDER_HIGH, HIGH_BYTE(address->cylinder));
		//Verifica se que esta gravando � o HD 
		if (driveNumber == 0) {
			Out_Byte(IDE_REG_DEVICE_HEAD, IDE_Master | address->head);
		} else if (driveNumber == 1) {
			Out_Byte(IDE_REG_DEVICE_HEAD, IDE_Slave | address->head);
		}
		//escreve no registrador de controlo da IDE, para ela ler um setor por vez
		Out_Byte(IDE_COMMAND_REGISTER, IDE_COMMAND_READ_SECTORS);
		//espera at� que as informa��es que queremos gravar sejam passadas.
		delay();
		IdeStatus = diskWait();
		if (IdeStatus == EXIT_SUCCESS) {
			w_buffer=(short *)buffer;
			//			timesToRead = 256 * numberOfSectors;
			for (count = 0; count < 256; count++) {
				w_buffer[count] = In_Word(IDE_REG_DEVICE_DATA_PORT);

			}
			delay();
		}

		__asm__ __volatile__
(	"sti");
				diskList[driveNumber].status = 0;
	}
	return EXIT_SUCCESS;
}

/**
 * \brief Espera o disco completar uma opera��o  e informa se teve sucesso ou n�o.
 * \return Retorna o valor 0, caso a opera��o que estava sendo realizada terminou com sucesso.
 **/
int diskWait() {
	while (In_Byte(IDE_STATUS_REGISTER) & IDE_STATUS_DRIVE_BUSY)
		; //espera ocupada [enquanto est. ocupado do disco] 
	if (In_Byte(IDE_STATUS_REGISTER) & IDE_STATUS_DRIVE_ERROR) {
		//caso ocorra um erro na opera��o do HD, ele ser� percebido aqui.
		APOSDEBUG(debug, "ocorreu erro no acesso ao dispositivo fisico \n");
		int error = In_Byte(IDE_REG_DEVICE_ERROR_PORT);
		APOSDEBUG(debug,
				"ERROR: no acesso ao dispositivo fisico- o numero do erro foi: \n"
						+error);
		return error;
	} else {
		APOSDEBUG(debug, "Sucesso no acesso ao dispositivo fisico \n");
		return EXIT_SUCCESS;
	}
}

/**
 * \brief Gera um delay.
 * \attention Durante o desenvolvimento foi encontrado um problema na execu��o de muitas repeti��es de tarefas, com acesso direto a HD. No c�digo
 * fonte do GeekOs foi encontrado uma referencia a esse problema. De acordo com o coment�rio do GeekOS o problema pertenceria a implementa��o da 
 * m�quina virtual BOCHS, entretanto essa atribui o erro a uma especifica��o do LBA28. Para resolver esse problema de forma correta, seria necess�rio
 * pegar alguns dados no HD, e calcular um delay. Como os APOS n�o implementa ainda um modulo de manipula��o de tempo isso n�o foi poss�vel. Por isso
 * configuramos no bochs uma certa freq��ncia para a m�quina a ser emulada, e com base nessa freq��ncia achamos esse delay.
 * \return int retorna o valor 0, caso a opera��o que estava sendo realizada terminou com sucesso.
 **/
void delay() {
	int lk;
	for (lk = 0; lk < 2000; lk++)
		;
}

/*  _________________________________
 * | *LBA= Endere�o l�gico do bloco  |
 * |_________________________________|
 * http://www.osdever.net/tutorials/lba.php
 * http://bochs.sourceforge.net/techspec/IDE-reference.txt
 * http://ata-atapi.com/
 */

