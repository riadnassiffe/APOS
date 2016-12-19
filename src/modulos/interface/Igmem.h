#ifndef IGMEM_H_
#define IGMEM_H_

extern void *malloc(int size);
extern void dispose(void *pont);
extern void *OSmalloc(int size);
extern void OSdispose(void *pont);
extern int alocPart(int pid);
extern int disposePart(int pid);

#endif /*IGMEM_H_*/
