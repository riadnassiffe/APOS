#ifndef KISRS_H_
#define KISRS_H_

extern void isrs_install();
extern void fault_handler(struct regs *r);

#endif /*KISRS_H_*/
