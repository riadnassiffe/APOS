#ifndef TIMER_H_
#define TIMER_H_
/**
  *Estrutura responsável por representar a hora.
  */
struct Time{
	/** Representa os segundos*/
	unsigned short sec:5;
	/** Representar os minutos*/
	unsigned short min:6;
	/** Representar os horas*/
	unsigned short hour:5;
}__attribute__((packed));

/**
 * \struct 
 * \brief Estrutura responsavel por representar a data.
 */
struct Date{
	/** Guarda o dia */
	unsigned short day:5;
	/** Guarda mês */
	unsigned short month:4;
	/** Guarda ano */
	unsigned short year:7;
	
}__attribute__((packed));
#endif
