#ifndef ITIM_H_
#define ITIM_H_

extern void timer_wait(int ticks);
extern void timer_install();
extern int getTimerLimit();
extern void setTimerLimit(int time);
extern void timer_handler();


#endif /*ITIM_H_*/
