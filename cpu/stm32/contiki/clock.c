#include <sys/clock.h>
#include <sys/cc.h>
#include <sys/etimer.h>

#include "delay.h"

static volatile clock_time_t current_clock = 0;
static volatile unsigned long current_seconds = 0;
static unsigned int second_countdown = CLOCK_SECOND;

void SysTick_Handler(void)
{
	current_clock++;

	if (etimer_pending() && etimer_next_expiration_time() <= current_clock)
	{
		etimer_request_poll();
		/* printf("%d,%d\n", clock_time(),etimer_next_expiration_time  	()); */
	}

	if (--second_countdown == 0)
	{
		current_seconds++;
		second_countdown = CLOCK_SECOND;
	}
}

void clock_init()
{
}

clock_time_t clock_time(void)
{
	return current_clock;
}

unsigned long clock_seconds(void)
{
	return current_seconds;
}

void clock_set_seconds(unsigned long sec)
{
	current_seconds = sec;
}

void clock_wait(clock_time_t t)
{
	while(--t);
}

void clock_delay_usec(uint16_t dt)
{

}

void clock_delay(unsigned int delay)
{
	DelayMs(delay);
}
