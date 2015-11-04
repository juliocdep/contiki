/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Testing the abc layer in Rime
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"

#include "board.h"

/*#include "dev/button-sensor.h"

 #include "dev/leds.h"

 #include <stdio.h>*/

char* msg = "Hello";

/*---------------------------------------------------------------------------*/
PROCESS(example_abc_process, "ABC example");
AUTOSTART_PROCESSES(&example_abc_process);
/*---------------------------------------------------------------------------*/

static void abc_recv(struct abc_conn *c)
{
	char* recivedMsg = (char *) packetbuf_dataptr();

	if (strncmp((const char*) recivedMsg, "on", 2) == 0)
	{
		LedOn(LED_DBG1);
	}
	else if (strncmp((const char*) recivedMsg, "off", 3) == 0)
	{
		LedOff(LED_DBG1);
	}
	else if (strncmp((const char*) recivedMsg, "toggle", 6) == 0)
	{
		LedToggle(LED_DBG1);
	}

	printf("abc message received '%s'\n", (char *) recivedMsg);
}

static const struct abc_callbacks abc_call = {abc_recv};
static struct abc_conn abc;

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_abc_process, ev, data)
{
	static struct etimer et;
	int i;

	PROCESS_EXITHANDLER(abc_close(&abc);)

	PROCESS_BEGIN();

	/*linkaddr_t newAddres;
	 newAddres.u8[0] = 3;
	 newAddres.u8[1] = 3;
	 linkaddr_set_node_addr(newAddres);*/

	abc_open(&abc, 128, &abc_call);

	while (1)
	{
//		PROCESS_WAIT_EVENT();

		/* Delay 2-4 seconds */
		etimer_set(&et, CLOCK_SECOND * 2 + random_rand() % (CLOCK_SECOND * 2));

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		printf("\n\nEnter a number:\n");
		printf("1 - Led on\n2 - Led off\n3 - Toggle Led\n");
		scanf("%i", &i);

		switch (i)
		{
		case 1:
			packetbuf_copyfrom("on", 3);
			break;

		case 2:
			packetbuf_copyfrom("off", 4);
			break;

		case 3:
		default:
			packetbuf_copyfrom("toggle", 7);
			break;
		}

		abc_send(&abc);
		printf("abc message sent (%i)\n", i);
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
