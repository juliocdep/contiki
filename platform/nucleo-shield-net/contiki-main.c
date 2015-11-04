#include <stdint.h>
#include <stdio.h>
#include <sys/process.h>
#include <sys/procinit.h>
#include <etimer.h>
#include <sys/autostart.h>
#include <clock.h>

#include "contiki.h"
#include "net/netstack.h"
#include "net/rime/rime.h"
#include "dev/watchdog.h"

#include "board.h"

#if NETSTACK_CONF_WITH_IPV6
#include "net/ipv6/uip-ds6.h"
#endif /* NETSTACK_CONF_WITH_IPV6 */

#if UIP_CONF_ROUTER

#ifndef UIP_ROUTER_MODULE
#ifdef UIP_CONF_ROUTER_MODULE
#define UIP_ROUTER_MODULE UIP_CONF_ROUTER_MODULE
#else /* UIP_CONF_ROUTER_MODULE */
#define UIP_ROUTER_MODULE rimeroute
#endif /* UIP_CONF_ROUTER_MODULE */
#endif /* UIP_ROUTER_MODULE */

extern const struct uip_router UIP_ROUTER_MODULE;
#endif /* UIP_CONF_ROUTER */

#if DCOSYNCH_CONF_ENABLED
static struct timer mgt_timer;
#endif

#ifndef NETSTACK_CONF_WITH_IPV4
#define NETSTACK_CONF_WITH_IPV4 0
#endif

#define UIP_OVER_MESH_CHANNEL 8

#ifdef EXPERIMENT_SETUP
#include "experiment-setup.h"
#endif

#define NUCLEO_SERVER	1
#if NUCLEO_SERVER
	// Endereços para o server
	#define IEEE_802154_MAC_ADDRESS {0x8F, 0xB8, 0x2A, 0xEB, 0x56, 0xDD};
	uint16_t node_id = 0x0102;
#else
	// Endereços para o client
	#define IEEE_802154_MAC_ADDRESS {0x08, 0xB6, 0xB7, 0xBD, 0x4C, 0x8D};
	uint16_t node_id = 0x0101;
#endif

unsigned char ds2411_id[8];

#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else /* DEBUG */
#define PRINTF(...)
#endif /* DEBUG */

/*---------------------------------------------------------------------------*/
#if 0
int
force_float_inclusion()
{
	extern int __fixsfsi;
	extern int __floatsisf;
	extern int __mulsf3;
	extern int __subsf3;

	return __fixsfsi + __floatsisf + __mulsf3 + __subsf3;
}
#endif
/*---------------------------------------------------------------------------*/
void uip_log(char *msg)
{
	puts(msg);
}

/*---------------------------------------------------------------------------*/
#if 0
void
force_inclusion(int d1, int d2)
{
	snprintf(NULL, 0, "%d", d1 % d2);
}
#endif
/*---------------------------------------------------------------------------*/
static void set_rime_addr(void)
{
	linkaddr_t addr;
	int i;

	memset(&addr, 0, sizeof(linkaddr_t));
#if NETSTACK_CONF_WITH_IPV6
	memcpy(addr.u8, ds2411_id, sizeof(addr.u8));
#else
	if (node_id == 0)
	{
		for (i = 0; i < sizeof(linkaddr_t); ++i)
		{
			addr.u8[i] = ds2411_id[7 - i];
		}
	}
	else
	{
		addr.u8[0] = node_id & 0xff;
		addr.u8[1] = node_id >> 8;
	}
#endif
	linkaddr_set_node_addr(&addr);
	PRINTF("Rime started with address ");
	for (i = 0; i < sizeof(addr.u8) - 1; i++)
	{
		PRINTF("%d.", addr.u8[i]);
	}
	PRINTF("%d\n", addr.u8[i]);
}
/*---------------------------------------------------------------------------*/
#if !PROCESS_CONF_NO_PROCESS_NAMES
static void print_processes(struct process * const processes[])
{
	/*  const struct process * const * p = processes;*/
	printf("Starting");
	while (*processes != NULL)
	{
		printf(" '%s'", (*processes)->name);
		processes++;
	}
	putchar('\n');
}
#endif /* !PROCESS_CONF_NO_PROCESS_NAMES */
/*--------------------------------------------------------------------------*/
static void start_autostart_processes()
{
#if !PROCESS_CONF_NO_PROCESS_NAMES
	print_processes(autostart_processes);
#endif /* !PROCESS_CONF_NO_PROCESS_NAMES */
	autostart_start(autostart_processes);
}
/*---------------------------------------------------------------------------*/
#if NETSTACK_CONF_WITH_IPV6
static void
start_uip6()
{
	NETSTACK_NETWORK.init();

	process_start(&tcpip_process, NULL);

#if DEBUG
	PRINTF("Tentative link-local IPv6 address ");
	{
		uip_ds6_addr_t *lladdr;
		int i;
		lladdr = uip_ds6_get_link_local(-1);
		for(i = 0; i < 7; ++i)
		{
			PRINTF("%02x%02x:", lladdr->ipaddr.u8[i * 2],
					lladdr->ipaddr.u8[i * 2 + 1]);
		}
		PRINTF("%02x%02x\n", lladdr->ipaddr.u8[14], lladdr->ipaddr.u8[15]);
	}
#endif /* DEBUG */

	if(!UIP_CONF_IPV6_RPL)
	{
		uip_ipaddr_t ipaddr;
		int i;
		uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
		uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
		uip_ds6_addr_add(&ipaddr, 0, ADDR_TENTATIVE);
		PRINTF("Tentative global IPv6 address ");
		for(i = 0; i < 7; ++i)
		{
			PRINTF("%02x%02x:",
					ipaddr.u8[i * 2], ipaddr.u8[i * 2 + 1]);
		}
		PRINTF("%02x%02x\n",
				ipaddr.u8[7 * 2], ipaddr.u8[7 * 2 + 1]);
	}
}
#endif /* NETSTACK_CONF_WITH_IPV6 */
/*---------------------------------------------------------------------------*/
static void start_network_layer()
{
#if NETSTACK_CONF_WITH_IPV6
	start_uip6();
#endif /* NETSTACK_CONF_WITH_IPV6 */
	start_autostart_processes();
	/* To support link layer security in combination with NETSTACK_CONF_WITH_IPV4 and
	 * TIMESYNCH_CONF_ENABLED further things may need to be moved here */
}
/*---------------------------------------------------------------------------*/
#if WITH_TINYOS_AUTO_IDS
uint16_t TOS_NODE_ID = 0x1234; /* non-zero */
uint16_t TOS_LOCAL_ADDRESS = 0x1234; /* non-zero */
#endif /* WITH_TINYOS_AUTO_IDS */

extern void initialise_monitor_handles(void);

int main()
{
	initialise_monitor_handles();

	printf("Initialising\n");

	// Initialize hardware
	BoardInitMcu();
	BoardInitPeriph();
	clock_init();

	/* XXX hack: Fix it so that the 802.15.4 MAC address is compatible
	 with an Ethernet MAC address - byte 0 (byte 2 in the DS ID)
	 cannot be odd. */
	ds2411_id[2] &= 0xfe;

	rtimer_init();

	// Initialize energest first (but after rtimer)
	energest_init();
	ENERGEST_ON(ENERGEST_TYPE_CPU);

	// for setting "hardcoded" IEEE 802.15.4 MAC addresses
#ifdef IEEE_802154_MAC_ADDRESS
	{
		uint8_t ieee[] = IEEE_802154_MAC_ADDRESS;
		memcpy(ds2411_id, ieee, sizeof(uip_lladdr.addr));
		ds2411_id[7] = node_id & 0xff;
	}
#endif

	random_init(ds2411_id[0] + node_id);

	// Initialize Contiki and our processes
	process_init();
	process_start(&etimer_process, NULL);

	ctimer_init();

	set_rime_addr();

	{
		uint8_t longaddr[8];
		uint16_t shortaddr;

		shortaddr = (linkaddr_node_addr.u8[0] << 8) + linkaddr_node_addr.u8[1];
		memset(longaddr, 0, sizeof(longaddr));
		linkaddr_copy((linkaddr_t *) &longaddr, &linkaddr_node_addr);
		PRINTF("MAC %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x ", longaddr[0], longaddr[1], longaddr[2], longaddr[3], longaddr[4], longaddr[5], longaddr[6],
				longaddr[7]);
	}

	PRINTF(CONTIKI_VERSION_STRING " started. ");
	if (node_id > 0)
	{
		PRINTF("Node id is set to %u.\n", node_id);
	}
	else
	{
		PRINTF("Node id is not set.\n");
	}

#if NETSTACK_CONF_WITH_IPV6
	memcpy(&uip_lladdr.addr, ds2411_id, sizeof(uip_lladdr.addr));
	/* Setup nullmac-like MAC for 802.15.4 */
	/*   sicslowpan_init(sicslowmac_init(&cc2420_driver)); */
	/*   PRINTF(" %s channel %u\n", sicslowmac_driver.name, CC2420_CONF_CCA_THRESH); */

	/* Setup X-MAC for 802.15.4 */
	queuebuf_init();
	NETSTACK_RDC.init();
	NETSTACK_MAC.init();

	PRINTF("%s %s %s, channel check rate %lu Hz, radio channel %u\n",
			NETSTACK_LLSEC.name, NETSTACK_MAC.name, NETSTACK_RDC.name,
			CLOCK_SECOND / (NETSTACK_RDC.channel_check_interval() == 0 ? 1:
					NETSTACK_RDC.channel_check_interval()),
					RF_CHANNEL);

#else /* NETSTACK_CONF_WITH_IPV6 */

	NETSTACK_RDC.init();
	NETSTACK_MAC.init();
	NETSTACK_NETWORK.init();

	PRINTF("%s %s %s, channel check rate %lu Hz, radio channel %u\n", NETSTACK_LLSEC.name, NETSTACK_MAC.name, NETSTACK_RDC.name,
			CLOCK_SECOND / (NETSTACK_RDC.channel_check_interval() == 0 ? 1 : NETSTACK_RDC.channel_check_interval()), CC2420_CONF_CHANNEL);
#endif /* NETSTACK_CONF_WITH_IPV6 */

#if TIMESYNCH_CONF_ENABLED
	timesynch_init();
	timesynch_set_authority_level((linkaddr_node_addr.u8[0] << 4) + 16);
#endif /* TIMESYNCH_CONF_ENABLED */

	watchdog_start();

	NETSTACK_LLSEC.bootstrap(start_network_layer);

	/*
	 * This is the scheduler loop.
	 */
#if DCOSYNCH_CONF_ENABLED
	timer_set(&mgt_timer, DCOSYNCH_PERIOD * CLOCK_SECOND);
#endif

	// Indica que o sistema foi inicializado
	LedOn(LED_DBG1);

	/*  watchdog_stop();*/
	while (1)
	{
		int r;
		do
		{
			/* Reset watchdog and handle polls and events */
			watchdog_periodic();
			r = process_run();
		} while (r > 0);
	}

	return 0;
}
/*---------------------------------------------------------------------------*/
#if LOG_CONF_ENABLED
void
log_message(char *m1, char *m2)
{
  printf("%s%s\n", m1, m2);
}
#endif /* LOG_CONF_ENABLED */
