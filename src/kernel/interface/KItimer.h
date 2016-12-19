#ifndef KITIRMER_H_
#define KITIRMER_H_

/*
 * Delay loop; spins for given number of iterations.
 */
extern static void Spin(int count);

/*
 * Spin for at least given number of microseconds.
 * FIXME: I'm sure this implementation leaves a lot to
 * be desired.
 */
extern void Micro_Delay(int us);

#endif /*KITIRMER_H_*/
