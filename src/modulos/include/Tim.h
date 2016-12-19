/*
 Handler do Timer
 - Tratador da interrupição do Clock
 
 */

#include "../../include/Defs.h"
#include "../interface/Itim.h"
#include "../../kernel/interface/KIirq.h"

#ifndef __TIM_H
#define __TIM_H

extern void resched();
extern void load_task();
extern char flag_ctx;

int getTimerLimit();
void setTimerLimit(int time);
void timer_handler();
void timer_wait(int ticks);
void timer_install();
#endif
