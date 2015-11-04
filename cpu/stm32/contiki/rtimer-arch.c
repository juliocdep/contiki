/*
 * rtimer-arch.c
 *
 *  Created on: 21/10/2015
 *      Author: Julio
 */

#include "rtimer-arch.h"

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

void rtimer_arch_init(void)
{
	// TODO: Implementar
}

void rtimer_arch_schedule(rtimer_clock_t t)
{
	// TODO: Implementar
}

void rtimer_arch_set(rtimer_clock_t t)
{
	// TODO: Implementar
}

rtimer_clock_t rtimer_arch_now(void)
{
	// TODO: Implementar
	return 0;
}
