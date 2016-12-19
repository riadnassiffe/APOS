#ifndef KIKB_H_
#define KIKB_H_

extern void keyboard_install();
extern char eGetChar();
extern unsigned char *eGetString(); //com echo
extern unsigned char *GetString(); //sem echo
extern void msgError();

#endif /*KIKB_H_*/
