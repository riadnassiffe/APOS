/*
 * Implementa��o baseada na vers�o anterior do APOS.
 * Orientador: Edeyson Andrade Gomes   edeyson\gmail.com
 * Aluno: R�ad Mattos Nassiffe		   riad.nassiffe\gmail.com
 */
/**
 * \file
 * Este arquivo tem a finalidade de implementar a parte de mais baixo n�vel do sistema
 * de arquivo do APOS. Esse sistema de arquivo segue os princ�pios de um sistema de arquivo FAT.
 * \author R�ad Mattos Nassiffe.
 */
#include "include/Fat.h"

/**
 * \brief Estrutura que guarda informa��es sobre a formata��o das parti��es encontradas.
 **/
struct FatInstance *VolumesListed;
/**
 * \brief Variavel que ativa o modo debug, se seu valor for TRUE(1) e desativa se for FALSE(0)
 **/
int debugVariable = FALSE;

/**
 * \brief Esta fun��o deve carrega todos os volumes detectados, e listados pelo drive IDE.c
 * \return Retorna 0, para indicar que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int loadFatIDEDrivers() {
	int status;
	int count;
	int ide = 0;
	struct FatInstance *list;
	
	//pegar a quantidade de volumes encontrados.
	ide = getDisks();
	APOSDEBUG(debugVariable, "ide = : ");
	printfInt(ide);
	APOSDEBUG(debugVariable, "hd..... \n");

	/*
	 * Cria uma lista, com entrada para cada dispositivo encontrado. A lista
	 * � criada de acordo com o n�mero de dispositivos encontrados.Se for encontrado um,
	 * cria-se uma lista com apenas uma entrada, caso contrario cria-se uma lista com duas 
	 * entradas
	 */
	if (ide == 1) {
		APOSDEBUG(debugVariable, "inicializando com apenas 1 hd");
		static struct FatInstance instance;
		list = &instance;
		//Passa o endere�o do da lista de drives ide para IDEdriverList.
		VolumesListed = list;
		APOSDEBUG(debugVariable, " \n inicializado com apenas 1 hd");
	} else if (ide == 2) {
		static struct FatInstance instance[2];
		list = &instance[0];
		//Passa o endere�o do da lista de drives ide para IDEdriverList.
		VolumesListed = list;
	} else {
		APOSDEBUG(debugVariable,
				"nenhum dispositivo de armazenamento foi encontrado..... \n");
	}
	/*
	 *Para cada dispositivo encontrado ele verifica se eles j� foram formatados,
	 *caso n�o tenham sido, ele formata.
	 */
	for (count = 0; count < ide; count++) {
		APOSDEBUG(debugVariable, " \n executando loadList...");
		/*
		 * A fun��o abaixo tenta caregar as informa��es do HD.
		 */
		status = loadFatList(count);
		APOSDEBUG(debugVariable, " voltando para loadFatIDEDrivers...\n");
		/*caso o loadFatlist retoner sucesso ele mostra a mensagem de sistema de 
		 * arquivo compativel encontrado, caso contrario formata a parti��o.
		 */
		if (status == 0) {
			APOSDEBUG(debugVariable,
					"Dispositivo com sistema e arquivos compativel encontrado....\n");
		} else {
			APOSDEBUG(debugVariable,
					"Dispositivo com sistema e arquivos incompativel encontrado....\n");
			APOSDEBUG(debugVariable, "Sistema dispositivo sendo formatado \n");
			status = formatDisk(count);
			if (status == 0) {
				APOSDEBUG(debugVariable, "Disco formatado com sucesso .... \n");
			} else {
				APOSDEBUG(debugVariable, "Problema ao formatar o disco .... \n");
			}
		}
	}
	APOSDEBUG(debugVariable, "todo os ja dispositivos foram carregados ...\n");
	return EXIT_SUCCESS;
}
/**
 * \brief  Tenta carregar na LBA 0 do dispositivo o BS, do sistema de parti��o, se n�o conseguir, formata o volume.  
 * \param driveNumber  Informa o n�mero do dispositivo na lista de HD.
 * \return 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int loadFatList(short int driveNumber) {
	char buffer[512];
	int result;
	int cluster;
	int count;
	int hdCount;
	struct tBpb bsinfo;
	struct diskAddress address;

	hdCount = 0;
	address.error = 0;

	APOSDEBUG(debugVariable, "\n comecando a executar o calcAddress.... \n");
	/*
	 * Tenta recuperar  a tabela FAT, que esta gravada no hd,
	 * primeiro tenta carregar o setor de boot(bs), depois
	 * a FAT, caso tenha sucesso em carregar o BS.
	 */
	APOSDEBUG(debugVariable, "\n comecando a executar o loadList.... \n");
	//Ler a regi�o zero da parti��o e pega o bs que deve est� gravado l�, no caso da
	//parti��o j� ter sido formatada posteriormente.
	/*
	 * Essa fun��o calcula atrav�s do LBA o endere�o. O valor 1 � passado de 
	 * forma est�tica pois a BS, deve esta contida em apenas 1 setor.
	 */

	address.lba = 0;
	result = calcAddress(driveNumber, &address);
	readDisk(driveNumber, &address, buffer);
	memcpy(&bsinfo, buffer, 512);
	if (result == 0) {
		if (bsinfo.BS_VolID == 2) {
			APOSDEBUG(debugVariable, "informacoes do HD consistentes.... \n");
			//VolumesListed[driveNumber].fatList = &fat[0];
			//Incrementa o hdCount para ele come�ar a ler os blocos que guardam a FAT
			hdCount++;
			count = 0;//zera o count. 
			cluster = bsinfo.BPB_FATSz16 * sizeof(unsigned short int)
					/ (bsinfo.BPB_BytsPerSec * bsinfo.BPB_SecPerClus);
			if (bsinfo.BPB_FATSz16 % (bsinfo.BPB_SecPerClus * 512)) {
				cluster = cluster + 1;
			}
			//Passamos 1 de forma fixa para lba, pois sempre consideremos que
			//toda FAT come�a a no LBA 1 e termina do LBA 257(ou seja ela sempre
			// tem o tamanho de 256)
			unsigned int position = 0;
			for (count = 0; count < cluster; count++) {
				address.lba = count + 1;
				calcAddress(driveNumber, &address);
				result = readDisk(driveNumber, &address, buffer);
				memcpy((void *) &VolumesListed[driveNumber].fatList[position],
						(void *)buffer, 512);
				position = position + 256;
			}
			//Colocando o ponteiro de fatPos para apontar para o cluster do 
			//raiz do sistema. 			
			VolumesListed[driveNumber].fatPos = bsinfo.BPB_RootClus;
			//Carregando o valor da LBL
			VolumesListed[driveNumber].LBL = bsinfo.BPB_LBL;
			address.lba = VolumesListed[driveNumber].fatPos;
			calcAddress(driveNumber, &address);
			//Lemos do HD, o conteudo apontado pela posi��o atual da FAT e
			//guardamos ele em actualDirectory.
			result = readDisk(driveNumber, &address, buffer);
			VolumesListed[driveNumber].actualDirectory
					= *((struct fatDirectory *) buffer);
			VolumesListed[driveNumber].bs = bsinfo;
			VolumesListed[driveNumber].actualDirectory.startCluster
					= VolumesListed[driveNumber].bs.BPB_RootClus;
		} else {
			//APOSDEBUG(debugVariable,"informacoes lidas do HD nao sao consistentes,precisamos formanta-la...\n");
			return EXIT_FAILURE;
		}
	} else {
		APOSDEBUG(debugVariable, "erro na leitura do HD");

	}
	APOSDEBUG(debugVariable, "fim de loadList...\n");
	return EXIT_SUCCESS;
}
/**
 * \brief Fomata o disco, no sistema de parti��o AFS2.0, criando uma bs e um Fat para ele.
 * \param  driveNumber - corresponde ao n�mero do dispositivo que ser� formatado.
 * \return Retorno 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int formatDisk(short int driveNumber) {
	unsigned short int count;
	unsigned short int cluster;
	struct tBpb bs;
	char buffer[512];
	char fatBuffer[512];
	struct diskAddress address;

	bs.BPB_FATSz16 = PARTITIONMAXSIZE;

	//Inicializando os valores da BS.
	bs.BPB_BytsPerSec = 512;
	bs.BPB_FSVer = 1;
	bs.BPB_Media = 'f';
	bs.BPB_NumFATs = 1;
	bs.BPB_NumHeads = getDiskHeads(driveNumber);
	bs.BPB_ResvdSecCnt = 0;
	bs.BPB_SecPerClus = 1;
	bs.BPB_SecPerTrk = getDiskSectorsPerTrack(driveNumber);
	bs.BPB_TotSec16 = getDiskSectorsPerTrack(driveNumber)
			* getDiskHeads(driveNumber) * getDiskCylinders(driveNumber);
	bs.BS_BootSig = 0x29;
	bs.BS_DrvNum = driveNumber;
	bs.BS_FilSysType[0] = 'a';
	bs.BS_FilSysType[1] = 'f';
	bs.BS_FilSysType[2] = 's';
	bs.BS_OEMName[0] = 'a';
	bs.BS_OEMName[1] = 'p';
	bs.BS_OEMName[2] = 'o';
	bs.BS_OEMName[3] = 's';
	bs.BS_Reserved2 = '0';
	bs.BS_VolID = 2;
	bs.BS_VolLab[0] = 'c';
	bs.BS_jmpBoot[0] = 'a';
	APOSDEBUG(debugVariable, "Inicializando Fat \n ");

	//inicialza o cluster, atravez da  variavel BPB_FATSz16, que contem a quantidade maxima de cluster que a parti��o pode ter.
	cluster = bs.BPB_FATSz16;
	count = 1;

	while (count < cluster) {
		VolumesListed[driveNumber].fatList[count] = count + 1;
		count++;
	}

	VolumesListed[driveNumber].fatList[count] = EOC;

	//gravando FAT no HD
	/*
	 * Pegamos o numero total de setores listados pelo sistema de arquivos,
	 * multiplicamos pelo tamanho de cada um, e depois dividimos pela capacidade
	 * de cada cluster, para saber quantos cluster teremos de usar para guardar
	 * os arquivos.
	 */

	APOSDEBUG(debugVariable, "Preparando para gravar FAT \n");
	cluster = bs.BPB_FATSz16 * sizeof(unsigned short int)/ (bs.BPB_SecPerClus
			* bs.BPB_BytsPerSec);
	APOSDEBUG(debugVariable, "\n");
	if (bs.BPB_FATSz16 % (((int)bs.BPB_SecPerClus) * 512)) {
		cluster = cluster + 1;
	}

	//Passamos primeiro da forma fixa, para lba, pois sempre consideremos que
	//toda FAT come�a a no LBA 1 e termina do LBA 257(ou seja ela sempre
	// tem o tamanho de 256)
	APOSDEBUG(debugVariable, "Gravando FAT \n");

	unsigned int position = 0;
	VolumesListed[driveNumber].fatList[cluster + 2] = EOC;
	for (count = 0; count < cluster; count++) {

		address.lba = count + 1;

		calcAddress(driveNumber, &address);

		memcpy((void *) fatBuffer,
				(void *) &VolumesListed[driveNumber].fatList[position], 512);
		writeDisk(driveNumber, &address, fatBuffer);
		position= position + 256;
	}

	APOSDEBUG(debugVariable, "FAT gravada \n");
	//O n�meros de setores escondidos recebe 1 do BS, mais o n�mero de cluster ocupado pela Fat.
	bs.BPB_HiddSec = 1 + cluster;
	APOSDEBUG(debugVariable, " valor do hiddsec = ");
	printfInt(bs.BPB_HiddSec);
	APOSDEBUG(debugVariable, "\n");
	//O cluster do root � primeiro logo depois dos setores reservados para o BS + FAT
	bs.BPB_RootClus = bs.BPB_HiddSec + 1;
	bs.BPB_LBL = bs.BPB_RootClus + 1;
	//gravando BS no HD
	VolumesListed[driveNumber].bs = bs;
	APOSDEBUG(debugVariable,
			"Passando as inforacoes da bs para a instancia na memoria \n");

	//buffer = (char *) &bs;
	memcpy(buffer, &bs, 512);
	address.lba = 0;
	calcAddress(driveNumber, &address);
	APOSDEBUG(debugVariable, "Gravando bbs no HD \n");
	writeDisk(driveNumber, &address, buffer);
	APOSDEBUG(debugVariable, "Bs gravada no HD \n");
	//Apos a formata��o do volume, o primeiro espa�o dispon�vel � o primeiro cluster dois do cluster do root.
	APOSDEBUG(debugVariable, "Achando lbl e o cluster root \n");
	VolumesListed[driveNumber].LBL = VolumesListed[driveNumber].bs.BPB_LBL;
	VolumesListed[driveNumber].fatPos
			= VolumesListed[driveNumber].bs.BPB_RootClus;

	//Inicializando valores do root enteryDirectory
	//ser� inicializado de inicio apenas os valores de estatus, pois � le que diz
	//se existe ou n�o algum arquivo (1 = existe e 0 = n�o existe).

	struct fatDirectory rootDir[16];

	for (count = 0; count < 16; count++) {
		rootDir[count].attribute.status = unchecked;
	}

	memcpy((void *) buffer, (void *) &rootDir, 512);

	address.lba = VolumesListed[driveNumber].fatPos;
	APOSDEBUG(debugVariable,
			"Carregando root para actualDirectory e gravando no hd \n");
	//gravando os valores do root entryDirectory
	calcAddress(driveNumber, &address);
	//gava as informa��es de rootDir no hd
	writeDisk(driveNumber, &address, buffer);

	APOSDEBUG(debugVariable,
			"dados de actualDiretory gravado, agora sendo inicializado. \n");
	VolumesListed[driveNumber].actualDirectory.startCluster
			= VolumesListed[driveNumber].fatPos;
	VolumesListed[driveNumber].actualDirectory.fileSize = 32;
	VolumesListed[driveNumber].actualDirectory.attribute.directory = checked;
	VolumesListed[driveNumber].actualDirectory.attribute.file = unchecked;
	VolumesListed[driveNumber].actualDirectory.attribute.status = checked;
	VolumesListed[driveNumber].actualDirectory.attribute.readOnly = checked;
	VolumesListed[driveNumber].actualDirectory.attribute.system = checked;
	VolumesListed[driveNumber].actualDirectory.startCluster
			= VolumesListed[driveNumber].bs.BPB_RootClus;
	APOSDEBUG(debugVariable,
			" dados de actualDiretory gravado, inicializados. \n");

	return EXIT_SUCCESS;
}
/**
 * \brief Grava as ultimas modifica��es da FAT no HD.
 * \param driveNumber - corresponde ao n�mero do dispositivo que ser� formatado.
 * \return 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int updateFat(int driveNumber) {
	unsigned short int cluster;
	unsigned int position;
	unsigned short int count;
	char fatBuffer[512];
	struct diskAddress address;

	//calcula quantos clusters ser�o necess�rios para gravar a FAT
	cluster = VolumesListed[driveNumber].bs.BPB_FATSz16
			* sizeof(unsigned short int)
			/ (VolumesListed[driveNumber].bs.BPB_SecPerClus
					* VolumesListed[driveNumber].bs.BPB_BytsPerSec);

	if (VolumesListed[driveNumber].bs.BPB_FATSz16
			% (((int)VolumesListed[driveNumber].bs.BPB_SecPerClus) * 512)) {
		cluster = cluster + 1;
	}

	APOSDEBUG(debugVariable, "\n Atualizando FAT \n");

	position = 0;
	for (count = 0; count < cluster; count++) {

		address.lba = count + 1;

		calcAddress(driveNumber, &address);

		memcpy((void *) &fatBuffer[0],
				(void *) &VolumesListed[driveNumber].fatList[position], 512);
		writeDisk(driveNumber, &address, fatBuffer);
		position = position + 256;
	}

	address.lba = 0;
	calcAddress(driveNumber, &address);
	VolumesListed[driveNumber].bs.BPB_LBL = VolumesListed[driveNumber].LBL;
	memcpy((void *) &fatBuffer[0], (void *) &VolumesListed[driveNumber].bs, 512);
	writeDisk(driveNumber, &address, fatBuffer);

	APOSDEBUG(debugVariable, "FAT atualizada \n");
	return EXIT_SUCCESS;
}

/**
 * \brief Pegar o valor da LBA*, e calcula o endere�o do HD.
 * \param volumeNumber Corresponde ao n�mero do dispositivo que ser� formatado.
 * \param address   Passa o valor da LBA para que a fun��o possa calcular endere�o de um cluster no HD
 * \return A fun��o retorna 0, para indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int calcAddress(short int volumeNumber, struct diskAddress *address) {
	address->sector = address->lba % getDiskSectorsPerTrack(volumeNumber) + 1;
	address->cylinder = address->lba / (getDiskHeads(volumeNumber)
			* getDiskSectorsPerTrack(volumeNumber));
	address->head= (address->lba / getDiskSectorsPerTrack(volumeNumber))
			% getDiskHeads(volumeNumber);
	address->error = FALSE;
	return EXIT_SUCCESS;
}

/**
 *\brief Acrescenta uma entrada de um diret�rio para outro.
 * \param driveNumber - corresponde ao n�mero do dispositivo que ser� formatado.
 * \param lba indica o endere�o l�gico da parti��o.
 * \param name nome do item que dever� ser guardado na estrutura de entrada de um diret�rio.
 * \param attribute passa os atributos do item, sele � um arquivo ou diret�rio, se � um arquivo de sistema ou somente leitura.
 * \param fileSize N�mero de clusters ocupado por um item do diret�rio.
 * \return 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int addDirectoryEntry(short int driveNumber, unsigned short int lba,
		char *name, struct Attribute attribute, unsigned long int fileSize) {

	short int count;
	struct fatDirectory *newEntry;
	char buffer[512];
	short int position;
	struct diskAddress address;

	address.lba = lba;
	calcAddress(driveNumber, &address);
	readDisk(driveNumber, &address, buffer);
	newEntry = (struct fatDirectory *) buffer;

	count = 0;
	position = 16;
	while (position == 16) {
		while (count < 16) {
			//se status n�o estiver marcado, a posi��o � selecionada
			if (newEntry[count].attribute.status == unchecked) {
				position = count;
				count = 16;
			}
			count++;
		}
		//Se passou pelo while acima en�o encotrou entrada disponivel, verifica se existe outro 
		//entrada para esse diretorio.
		if (position == 16&& VolumesListed[driveNumber].fatList[address.lba]
				!= EOC) {
			lba = VolumesListed[driveNumber].fatList[address.lba];
			address.lba = lba;
			calcAddress(driveNumber, &address);
			readDisk(driveNumber, &address, buffer);
			newEntry = (struct fatDirectory *) buffer;
			count = 0;
		} else if (position == 16) {
			position = 17;
		}
	}
	//Caso tenha encontrado espa�o sobrando.
	if (position != 17) {

		setDirectoryEntry(&newEntry[position], name, attribute,
				VolumesListed[driveNumber].LBL, fileSize);

		memcpy(buffer, newEntry, 512);
		writeDisk(driveNumber, &address, buffer);
		//caso n�o encontre espa�o disponivel.
	} else {
		struct fatDirectory new[16];

		address.lba = VolumesListed[driveNumber].LBL;
		VolumesListed[driveNumber].LBL
				= VolumesListed[driveNumber].fatList[VolumesListed[driveNumber].LBL];
		VolumesListed[driveNumber].fatList[lba] = address.lba;
		VolumesListed[driveNumber].fatList[address.lba] = EOC;

		for (count = 1; count < 16; count++) {
			new[count].startCluster = 0;
			new[count].attribute.status = 0;
		}

		setDirectoryEntry(&new[0], name , attribute ,
				VolumesListed[driveNumber].LBL, (unsigned long int )fileSize );
		memcpy(buffer, new, 512);
		calcAddress(driveNumber, &address);
		writeDisk(driveNumber, &address, buffer);

		updateFat(driveNumber);
	}
	return EXIT_SUCCESS;
}

/**
 * \brief Seta valores para um DirectoryEntry.
 * \param newEntry Corresponde a nova entrada que receber� os valores.
 * \param name Nome do item que dever� ser guardado na estrutura de entrada.
 * \param attribute Passa os atributos do item, sele � um arquivo ou diret�rio, se � um arquivo de sistema ou somente leitura.
 * \param startCluster Cluster da parti��o na qual o volume esta inicialmente armazenado.
 * \param fileSize N�mero de clusters ocupado por um item do diret�rio.
 * \return O retorno 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int setDirectoryEntry(struct fatDirectory *newEntry, char *name,
		struct Attribute attribute, unsigned short int startCluster,
		unsigned long int fileSize) {
	short int count;

	newEntry->attribute.directory = attribute.directory;
	newEntry->attribute.file = attribute.file;
	newEntry->attribute.hidden = attribute.hidden;
	newEntry->attribute.readOnly = attribute.readOnly;
	newEntry->attribute.status = checked;

	count = 0;

	while (name[count] != '.'&& count < 8) {
		newEntry->file.name[count] = name[count];
		count++;
	}

	if (name[count] == '.') {
		newEntry->file.name[count]='\0';
	}

	if (newEntry->attribute.file == checked) {
		memcpy(newEntry->file.type, &name[count + 1], 3);
	}

	newEntry->startCluster = startCluster;
	newEntry->fileSize = fileSize;
	return EXIT_SUCCESS;
}

/**
 * \brief Ler um arquivo do diret�rio atual.
 * \param driveNumber Corresponde ao n�mero do dispositivo que ser� lido.
 * \param name Indica o nome do arquivo que ser� lido.
 * \param type Referencia o tipo do arquivo que esta sendo gravado.
 * \return 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int readFile(short int driveNumber, char *name, char *type) {
	short int count;
	struct fatDirectory *entry;
	char buffer[512];
	short int position;
	struct diskAddress address;
	char typeBuffer[4];
	char nameBuffer[9];
	int letter;

	address.lba = VolumesListed[driveNumber].actualDirectory.startCluster;
	calcAddress(driveNumber, &address);
	readDisk(driveNumber, &address, buffer);
	entry = (struct fatDirectory *) buffer;
	count = 0;

	position = 16;
	//O while mais interno serve para verificar se a entrada de diret�rio esta no cluster atual,
	//e o while mais externo serve para verificar que aquele diret�rio n�o ocupa mais de um cluster.
	//Cada cluster consegue ter at� 16 itens gravados nele.
	while (position == 16) {
		while (count < 16) {
			typeBuffer[0] = entry[count].file.type[0];
			typeBuffer[1] = entry[count].file.type[1];
			typeBuffer[2] = entry[count].file.type[2];
			typeBuffer[3] = '\0';
			letter = 0;

			while (letter < 8&& entry[count].file.name[letter] != '\0') {
				nameBuffer[letter] = entry[count].file.name[letter];
				letter++;
			}

			nameBuffer[letter] = '\0';

			if ((strcmp(name, nameBuffer))&& (strcmp(type, typeBuffer))) {
				position = count;
				count = 16;
			}
			count++;
		}
		/*
		 * 
		 * Caso n�o encontre o arquivo a deletar, ele procura por outro cluster do diret�rio,
		 * at� ter checado todos as entradas preenchidas no diret�rio.
		 */
		if (position == 16&& VolumesListed[driveNumber].fatList[address.lba]
				!= EOC) {
			address.lba = VolumesListed[driveNumber].fatList[address.lba];
			calcAddress(driveNumber, &address);
			readDisk(driveNumber, &address, buffer);
			entry = (struct fatDirectory *) buffer;
			count = 0;
			/* caso n�o encontre o arquivo e j� tenha lido todos os cluster do diret�rio,
			 * ele sai, sem modificar nada.
			 */
		} else if (position == 16) {
			position = 17;
		}
	}

	if (position != 16) {

		address.lba = entry[position].startCluster;
		calcAddress(driveNumber, &address);
		readDisk(driveNumber, &address, buffer);
		puts(buffer);
	} else {
		puts("arquivo nao encontrado....");
	}

	return EXIT_SUCCESS;
}
/**
 * \brief Cria uma arquivo no diret�rio atual.
 * 
 * \param driveNumber indica o HD que o arquivo ser� gravado.
 * \param name � o nome do arquivo.
 * \param attr indicar as permiss�es
 * \param file � o conte�do do arquivo
 */
int createFile(short int driveNumber, char *name, char attr, char *file) {
	unsigned int fileSize;
	unsigned short int dirPos;
	struct Attribute attribute;
	struct diskAddress address;
	unsigned short int cluster, count;

	attribute.file = checked;
	attribute.directory = unchecked;
	attribute.status = checked;

	if (attr == 1) {
		attribute.readOnly = checked;
	}

	//pegar o n�mero do cluster inicial, onde o arquivo ser� gravado,
	// para adicionar a ele mais um item.
	dirPos = VolumesListed[driveNumber].actualDirectory.startCluster;
	fileSize = strlenght(file);
	addDirectoryEntry(driveNumber, dirPos, name, attribute, fileSize);

	cluster = fileSize / VolumesListed[driveNumber].bs.BPB_BytsPerSec;
	if (fileSize % 512) {
		cluster= cluster + 1;
	}
	count = 0;
	for (; cluster != 0; cluster--) {
		address.lba = VolumesListed[driveNumber].LBL;
		VolumesListed[driveNumber].LBL
				= VolumesListed[driveNumber].fatList[VolumesListed[driveNumber].LBL];
		calcAddress(driveNumber, &address);
		writeDisk(driveNumber, &address, &file[count]);
		count = count + VolumesListed[driveNumber].bs.BPB_BytsPerSec;
	}
	VolumesListed[driveNumber].fatList[address.lba] = EOC;

	updateFat(driveNumber);
	return EXIT_SUCCESS;
}
/**
 * \brief Deleta um arquivo do diret�rio atual. 
 * \param driveNumber Corresponde ao n�mero do dispositivo que ser� formatado.
 * \param name Nome do arquivo que dever� ser apagado.
 * \param type Tipo do arquivo que dever� ser apagado.
 * \param dirStartCluster Caso esse par�metro seja igual a 0 procurar� o arquivo no diretorio atual, caso contrario,
 * ele pegar� o n�mero do cluster do diret�rio passado e procurar� o arquivo a ser deletado nele.
 * \return 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int delFile(short int driveNumber, char *name, char *type,
		unsigned short int dirStartCluster) {
	short int count;
	struct fatDirectory *entry;
	char buffer[512];
	short int position;
	struct diskAddress address;
	char typeBuffer1[4];
	char typeBuffer2[4];

	//caso n�o seja informado nenhum diret�rio para remo��o do arquivo o sistema levaram em considera��o que ele esta na no diret�rio atual.
	if (dirStartCluster == 0) {
		address.lba = VolumesListed[driveNumber].actualDirectory.startCluster;
	} else {
		address.lba = dirStartCluster;
	}
	calcAddress(driveNumber, &address);
	readDisk(driveNumber, &address, buffer);
	entry = (struct fatDirectory *) buffer;
	count = 0;

	position = 16;
	while (position == 16) {
		while (count < 16) {
			memcpy(typeBuffer1, entry[count].file.type, 3);
			typeBuffer1[3] = '\0';
			memcpy(typeBuffer2, type, 3);
			typeBuffer2[3] = '\0';
			if ((strcmp(name, entry[count].file.name)) && (strcmp(typeBuffer2,
					typeBuffer1))) {
				position = count;
				count = 15;
			}
			count++;
		}
		/*
		 * Caso n�o encontre o arquivo a deletar, ele procura por outro cluster do diret�rio,
		 * at� ter checado todas as entradas do diret�rio.
		 */
		if (position == 16 && VolumesListed[driveNumber].fatList[address.lba] != EOC) {
			address.lba = VolumesListed[driveNumber].fatList[address.lba];
			calcAddress(driveNumber, &address);
			readDisk(driveNumber, &address, buffer);
			entry = (struct fatDirectory *) buffer;
			count = 0;
			/* 
			 * caso n�o encontre o arquivo e j� tenha lido todos os cluster do diret�rio,
			 * ele sai, sem modificar nada.
			 */
		} else if (position == 16) {
			position = 17;
		}
	}
	/*
	 * quando encontra o arquivo a ser apagado ele marca como unchecked.
	 */
	if (position != 17) {

		entry[position].attribute.status = unchecked;

		count = entry[position].startCluster;

		memcpy(buffer, entry, 512);
		writeDisk(driveNumber, &address, buffer);
		position = count;
		while (VolumesListed[driveNumber].fatList[count] != EOC) {
			count = VolumesListed[driveNumber].fatList[count];
		}
		VolumesListed[driveNumber].fatList[count]
				= VolumesListed[driveNumber].LBL;
		VolumesListed[driveNumber].LBL = position;
		updateFat(driveNumber);

		/*
		 * Se n�o encontrar imprimi umas mensagem na tela avisando.
		 */
	} else {
		puts("arquivo para remocao nao encontrado");
	}
	return EXIT_SUCCESS;
}

/**
 * \brief Cria um diretorio.
 * \param driveNumber - corresponde ao n�mero do dispositivo que ser� formatado.
 * \param attr indica os atributos do diret�rio.
 * \param name nome do arquivo que dever� ser apagado.
 * \return 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int createDIR(short int driveNumber, char *name, char attr) {
	char buffer[512];
	short int count;
	short int startCluster;
	short int fileSize;
	struct diskAddress address;
	struct fatDirectory new[16];
	struct Attribute attribute;

	attribute.directory = checked;
	attribute.status = checked;
	attribute.file = unchecked;

	if (attr == '1') {
		attribute.readOnly = checked;
	}

	fileSize = 1;

	addDirectoryEntry(driveNumber,
			VolumesListed[driveNumber].actualDirectory.startCluster, name,
			attribute, fileSize);
	//pega o primeiro cluster dispon�vel na LBL para garvar o arquivo.
	startCluster = VolumesListed[driveNumber].LBL;
	VolumesListed[driveNumber].LBL
			= VolumesListed[driveNumber].fatList[VolumesListed[driveNumber].LBL];

	for (count = 0; count < 16; count++) {
		new[count].attribute.status = unchecked;
	}

	memcpy(buffer, new, 512);
	address.lba = startCluster;
	calcAddress(driveNumber, &address);
	writeDisk(driveNumber, &address, buffer);
	VolumesListed[driveNumber].fatList[address.lba] = EOC;

	updateFat(driveNumber);
	return EXIT_SUCCESS;
}
/**
 * \brief Remove um DirectoryEntry do diret�rio atual.
 * \param driveNumber - corresponde ao n�mero do dispositivo que ser� formatado.
 * \param startCluster Cluster da parti��o na qual o volume esta inicialmente armazenado.
 * \return 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int delDirectoryEntry(short int driveNumber, short int startCluster) {
	short int count;
	unsigned short int position;
	struct fatDirectory *directoryEntry;
	struct diskAddress address;
	char buffer[512];
	short int end = 0;

	address.lba = startCluster;
	calcAddress(driveNumber, &address);

	readDisk(driveNumber, &address, buffer);
	directoryEntry = (struct fatDirectory *) buffer;

	position = startCluster;
	/*
	 *Neste loop deve-se passar por todos os registros que est�o guardados,
	 * caso algum dos registros seja um arquivo ele deve chamar a fun��o de deletar arquivos e 
	 * delet�-lo, caso seja um diret�rio, ele dever� chamar essa fun��o de forma
	 * recursiva, at� deletar todos arquivos e diret�rios dentro dele.
	 */
	count = 0;
	end = 0;
	while (end!= 1) {
		while (count < 16) {
			if (directoryEntry[count].attribute.status == checked
					&& directoryEntry[count].attribute.file == checked) {
				//poderia ser passado o startCluster no lugar do position, entretando o position � mais eficiente,
				//pois passa diretamente o lugar da fat que o arquivo a ser deletado esta.
				delFile(driveNumber, directoryEntry[count].file.name,
						directoryEntry[count].file.type, position);
			} else if (directoryEntry[count].attribute.status == checked
					&& directoryEntry[count].attribute.directory == checked) {
				delDirectoryEntry(driveNumber,
						directoryEntry[count].startCluster);
				directoryEntry[count].attribute.status = unchecked;
			}

			count++;
		}

		if (VolumesListed[driveNumber].fatList[position] == EOC) {
			end = 1;
		} else {
			position = VolumesListed[driveNumber].fatList[position];
			count = 0;
			address.lba = position;
			calcAddress(driveNumber, &address);
			readDisk(driveNumber, &address, buffer);
			directoryEntry = (struct fatDirectory *) buffer;
		}
	}

	VolumesListed[driveNumber].fatList[position]
			= VolumesListed[driveNumber].LBL;
	VolumesListed[driveNumber].LBL = startCluster;

	updateFat(driveNumber);
	return EXIT_SUCCESS;
}
/**
 * \brief Deleta todos os arquivos e diret�rios contidos no arquivo tb.
 * \param driveNumber Corresponde ao n�mero do dispositivo que ser� formatado.
 * \param fileName Nome do diret�rio que dever� ser apagado.
 * \return 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
//ATEN��O, ele usa recurs�o.
int delDir(short int driveNumber, char *fileName) {
	int count;
	short int position;
	struct diskAddress address;
	struct fatDirectory *entry;
	char buffer[512];

	address.lba= VolumesListed[driveNumber].actualDirectory.startCluster;
	calcAddress(driveNumber, &address);
	readDisk(driveNumber, &address, buffer);
	entry = (struct fatDirectory *) buffer;
	count = 0;
	position = 16;
	while (position == 16) {
		while (count < 16) {
			if (strcmp(fileName, entry[count].file.name)) {
				position = count;
				count = 16;
			}
			count++;
		}
		if (position == 16 && VolumesListed[driveNumber].fatList[address.lba]
				!= EOC) {
			address.lba = VolumesListed[driveNumber].fatList[address.lba];
			calcAddress(driveNumber, &address);
			readDisk(driveNumber, &address, buffer);
			entry = (struct fatDirectory *) buffer;
			count = 0;
		} 
	}
	if(position < 16){
		delDirectoryEntry(driveNumber, entry[position].startCluster);
	
		entry[position].attribute.status = unchecked;
	
		memcpy(buffer, entry, 512);
		writeDisk(driveNumber, &address, buffer);
	
		updateFat(driveNumber);
		return EXIT_SUCCESS;
	}else{
		puts("esse diretorio nao existe.");
		return EXIT_FAILURE;
	}
	
}
/**
 * \brief atualiza  o diret�rio atual para algum que seja filho do atual.
 * \param driveNumber  corresponde ao n�mero do dispositivo que ser� formatado.
 * \param name corresponde ao nome do novo diret�rio que o sistema carregar� como atual.
 *  \return 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int refreshDirectoryEnter(short int driveNumber, char *name) {
	short int count;
	struct fatDirectory *directoryEntry;
	struct diskAddress address;
	char buffer[512];
	unsigned short int position;
	int letter;
	char nameBuffer[9];

	address.lba = VolumesListed[driveNumber].actualDirectory.startCluster;
	calcAddress(driveNumber, &address);
	readDisk(driveNumber, &address, buffer);
	directoryEntry = (struct fatDirectory *) buffer;

	count = 0;
	position = 16;
	while (count < 17) {
		while (count < 16) {
			letter = 0;

			while (letter < 8&& directoryEntry[count].file.name[letter] != '\0') {
				nameBuffer[letter] = directoryEntry[count].file.name[letter];
				letter++;
			}
			nameBuffer[letter] = '\0';

			if (strcmp(nameBuffer, name)
					&& directoryEntry[count].attribute.directory == checked) {
				position = count;
				count = 17;
			}
			count++;
		}
		if (VolumesListed[driveNumber].fatList[address.lba] != EOC&& position
				== 16) {
			address.lba = VolumesListed[driveNumber].fatList[address.lba];
			calcAddress(driveNumber, &address);
			readDisk(driveNumber, &address, buffer);
			directoryEntry = (struct fatDirectory *) buffer;
			count = 0;
		}/* else if (count == 17) {
			count = 17;
		}*/
	}

	if (position < 16) {
		VolumesListed[driveNumber].actualDirectory = directoryEntry[position];
		puts("Diretorio atualizado....");
	}else{
		puts("Diretorio nao existente....");
	}

	return EXIT_SUCCESS;
}
/**
 * \brief Esta fun��o serve para imprimir na tela todas as informa��es do diretorio atual.
 * \param driveNumber - corresponde ao n�mero do dispositivo que ser� formatado.
 * \return 0 indica que houve sucesso na opera��o, qualquer outro n�mero significa que houve erro.
 */
int printDir(short int driveNumber) {
	short int count;
	struct fatDirectory *directoryEntry;
	char buffer[512];
	short int position;
	struct diskAddress address;

	address.lba = VolumesListed[driveNumber].actualDirectory.startCluster;
	calcAddress(driveNumber, &address);
	readDisk(driveNumber, &address, buffer);
	directoryEntry = (struct fatDirectory *) buffer;
	count = 0;
	position = 0;

	while (position == 0) {
		APOSDEBUG(debugVariable, " \n comecando a listar os arquivos \n");
		while (count < 16) {
			if (directoryEntry[count].attribute.status == checked) {
				puts(" \n");
				puts(directoryEntry[count].file.name);
				if (directoryEntry[count].attribute.file == checked) {
					puts(".");
					char typeBuffer[4];
					typeBuffer[0] = directoryEntry[count].file.type[0];
					typeBuffer[1] = directoryEntry[count].file.type[1];
					typeBuffer[2] = directoryEntry[count].file.type[2];
					typeBuffer[3] = '\0';
					puts(typeBuffer);
				} else {
					puts("\\");
				}
				puts("                             ");
				printfInt(directoryEntry[count].startCluster);
				puts("--");
				printfInt((int)directoryEntry[count].fileSize);
			}
			count++;

		}
		APOSDEBUG(debugVariable, " \n finalizando leitura de um registro \n");
		//caso o o diretorio tenha mais de 16 items, ele pega o proximo cluster
		if (VolumesListed[driveNumber].fatList[address.lba] != EOC) {
			APOSDEBUG(debugVariable, " \n comecando a ler outro registro \n");
			address.lba = VolumesListed[driveNumber].fatList[address.lba];
			calcAddress(driveNumber, &address);
			readDisk(driveNumber, &address, buffer);
			directoryEntry = (struct fatDirectory *) buffer;
			count = 0;
		} else {
			puts("\n fim lista.... \n");
			position = 20;
		}
	}
	return EXIT_SUCCESS;
}
/**
 * \brief Modifica qualquer informa��o de um determinada item de um diret�rio.
 * \param driveNumber n�mero do hd que o arquivo se encontra.
 * \param fileName nome do aquivo que deve ser modifcado.
 * \param directoryEntry Dados que devem ser alterados.
 * \return retona 0 em caso de sucesso.
 */
int modifyEntry(short int driveNumber, struct Name fileName,
		struct fatDirectory *directoryEntry) {
	int position;
	int count;
	struct fatDirectory *entry;
	char buffer[512];
	struct diskAddress address;

	address.lba = VolumesListed[driveNumber].actualDirectory.startCluster;
	calcAddress(driveNumber, &address);
	readDisk(driveNumber, &address, buffer);
	directoryEntry = (struct fatDirectory *) buffer;

	count = 0;
	position = 16;

	if (directoryEntry->attribute.status == checked&& strlenght(fileName.type)
			== 0) {
		while (position == 16) {
			while (count < 16) {
				if (strcmp(fileName.name, entry[count].file.name)) {
					position = count;
					count = 16;
				}
				count++;
			}
			if (position == 16
					&& VolumesListed[driveNumber].fatList[address.lba]!= EOC) {
				address.lba = VolumesListed[driveNumber].fatList[address.lba];
				calcAddress(driveNumber, &address);
				readDisk(driveNumber, &address, buffer);
				entry = (struct fatDirectory *) buffer;
				count = 0;
			} else if (position == 16) {
				position = 17;
			}
		}
	} else if (directoryEntry->attribute.status == checked
			&& strlenght(fileName.type) != 0) {
		while (position == 16) {
			while (count < 16) {
				if ((strcmp(fileName.name, entry[count].file.name))&& (strcmp(
						fileName.type, entry[count].file.type))) {
					position = count;
					count = 16;
				}
				count++;
			}
			if (position == 16
					&& VolumesListed[driveNumber].fatList[address.lba]!= EOC) {
				address.lba = VolumesListed[driveNumber].fatList[address.lba];
				calcAddress(driveNumber, &address);
				readDisk(driveNumber, &address, buffer);
				entry = (struct fatDirectory *) buffer;
				count = 0;
			} else if (position == 16) {
				position = 17;
			}
		}
	}
	if (position != 17) {
		entry[position] = *directoryEntry;
		memcpy(buffer, entry, 512);

		writeDisk(driveNumber, &address, buffer);
	} else {
		puts("arquivo nao encontrado");
	}

	return EXIT_SUCCESS;
}

