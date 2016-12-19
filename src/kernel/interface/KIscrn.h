#ifndef KISCRN_H_
#define KISCRN_H_

//extern void printfAPOS(unsigned char *text);
extern void init_video(void);
extern void puts(const char *text);
extern void putch(unsigned char c);
extern void cls();
extern void printf(unsigned char *text);
extern void printfInt(int num);
extern void APOSDEBUG(int debug, char *text);

#endif /*KISCRN_H_*/
