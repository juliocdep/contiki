//#include <gpio.h>
//#include <nvic.h>
#include <stdint.h>
#include <stdio.h>
//#include <debug-uart.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>

#include "contiki.h"
#include "net/netstack.h"
#include "net/rime/rime.h"

#include "board.h"

#define SEMIHOSTING		0

#if SEMIHOSTING
extern void initialise_monitor_handles(void);
#endif

unsigned int idle_count = 0;

int main()
{
#if SEMIHOSTING
	initialise_monitor_handles();
#endif

//  dbg_setup_uart();
	printf("Initialising\n");

	BoardInitMcu();
	BoardInitPeriph();

	LedOn(LED_RX);
	LedOn(LED_TX);

	clock_init();
	process_init();

	ctimer_init();
	//rtimer_init();
	queuebuf_init();
	netstack_init();

	process_start(&etimer_process, NULL);
	autostart_start(autostart_processes);
	printf("Processes running\n");
	while (1)
	{
		do
		{
		} while (process_run() > 0);
		idle_count++;
		/* Idle! */
		/* Stop processor clock */
		/* asm("wfi"::); */
	}
	return 0;
}
