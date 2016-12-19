#ifndef MEMSTRUCTS_H_
#define MEMSTRUCTS_H_

#define FREE 0x00
#define USED 0x01
#define PAGE 4096
#define BASE 100
/*
 Estrutura: Linha da tabela de particoes fixas do gerenciador de mem�ria
 
 [pa: presente[1]/ausente[0]]
 [own_pid: pid do processo dono ]
 [code_point: ponteiro para codigo do processo ]
 [code_size: tamanho restante da região de código]
 [heap_point: ponteiro para a heap do processo]
 [heap_size: tamanho restante da heap do processo]
 [stack_point: ponteiro para pilha do processo ]
 [stack_size: tamanho restante da pilha do processo]
 */

struct sdesc_tabMem {
	int pa;
	int own_pid;
	unsigned long int code_point;
	unsigned long int codePointStart;
	unsigned long int code_size;
	unsigned long int heap_point;
	unsigned long int heapPointStart;
	unsigned long int heap_size;
	unsigned long int stack_point;
	unsigned long int stackPointStart;
	unsigned long int stack_size;
	struct s_free_node *cab;
	unsigned long int relocSize;
}__attribute__((packed));

struct s_free_node{
	void *end;
	int pa;
	int own_pid;
	unsigned int size;
	struct s_free_node *next;
}__attribute__((packed));


struct bitMap{
	char block:1;
}__attribute__((packed));

#endif /*MEMSTRUCTS_H_*/
