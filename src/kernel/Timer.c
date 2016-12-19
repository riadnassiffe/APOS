#include "include/Timer.h"

static void Spin(int count)
{
/*
	 * The assembly code is the logical equivalent of
	 *      while (count-- > 0) { // waste some time }
	 * We rely on EAX being used as the counter
	 * variable.
	 */

	int result;
	__asm__ __volatile__
("1: decl %%eax\n\t"
"cmpl $0, %%eax\n\t"
"nop; nop; nop; nop; nop; nop\n\t"
"nop; nop; nop; nop; nop; nop\n\t"
"jg 1b"
: "=a" (result)
: "a" (count)
);
}

/*
 * Spin for at least given number of microseconds.
 * FIXME: I'm sure this implementation leaves a lot to
 * be desired.
 */
void Micro_Delay(int us) {
	int num = us * s_spinCountPerTick;
	int denom = US_PER_TICK;

	int numSpins = num / denom;
	int rem = num % denom;

	if (rem > 0)
		++numSpins;

	Spin(numSpins);
}
