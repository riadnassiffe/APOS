#include "include/Isrs.h"

/**
 * Lista com todas as messagens de exeções prevista.
 */
char *exception_messages[] =
{
		"Divisao por Zero",
		"Debug",
		"Interrupcao Nao Mascaravel",
		"Breakpoint",
		"Into Overflow Detectado",
		"Fora de limite",
		"Codigo de operacao invalido",
		"Sem CoProcessador",

		"Double Fault",
		"Coprocessor Segment Overrun",
		"Erro no TSS",
		"Segmento Não Presente",
		"Stack Fault (Falta de Pilha)",
		"General Protection Fault (GPF)",
		"Page Fault (Falta de Pagina)",
		"Unknown Interrupt (Interrupcao Desconhecida)",

		"Coprocessor Fault (Erro no CoProcessador)",
		"Alignment Check",
		"Machine Check",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",

		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved"
};

/**
 * Grava entradas na IDT.
 */
void isrs_install()
{
    //idt_set_TaskGate(0, 0x12, 0x85);
    idt_set_gate(0, (unsigned)isr0, 0x08, 0x8E);
    idt_set_gate(1, (unsigned)isr1, 0x08, 0x8E);
    idt_set_gate(2, (unsigned)isr2, 0x08, 0x8E);
    idt_set_gate(3, (unsigned)isr3, 0x08, 0x8E);
    idt_set_gate(4, (unsigned)isr4, 0x08, 0x8E);
    idt_set_gate(5, (unsigned)isr5, 0x08, 0x8E);
    idt_set_gate(6, (unsigned)isr6, 0x08, 0x8E);
    idt_set_gate(7, (unsigned)isr7, 0x08, 0x8E);

    idt_set_gate(8, (unsigned)isr8, 0x08, 0x8E);
    idt_set_gate(9, (unsigned)isr9, 0x08, 0x8E);
    idt_set_gate(10, (unsigned)isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned)isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned)isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned)isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned)isr15, 0x08, 0x8E);

    idt_set_gate(16, (unsigned)isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned)isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned)isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned)isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned)isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned)isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned)isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned)isr23, 0x08, 0x8E);

    idt_set_gate(24, (unsigned)isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned)isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned)isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned)isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned)isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned)isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned)isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned)isr31, 0x08, 0x8E);
}

/**
 * Todas as exeções que ocorrem no sistema, usam essa função para imprimir menssagens na tela, e paralisa o SO
 * ficando em loop infinito.
 */
void fault_handler(struct regs *r)
{
    if (r->int_no < 32)
    {    
        puts("\n***************************************\n");
        puts(">>            APOS Fault Handler \n");
        puts(">> Codigo do erro: ");
        printfInt(r->int_no);
        puts("\n>> Msg: ");
        puts(exception_messages[r->int_no]); 
        puts("\n>> Processo que gerou o erro: ");
        puts("\n***************************************\n");
        while(1){}
    }
}
