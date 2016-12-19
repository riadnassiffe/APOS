#ifndef TIMER_H_
#define TIMER_H_

/*
 * Delay loop; spins for given number of iterations.
 */
static void Spin(int count);

/*
 * Spin for at least given number of microseconds.
 * FIXME: I'm sure this implementation leaves a lot to
 * be desired.
 */
void Micro_Delay(int us);

#endif /*TIMER_H_*/
