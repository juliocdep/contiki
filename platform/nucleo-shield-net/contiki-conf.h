#ifndef __CONTIKI_CONF_H__CDBB4VIH3I__
#define __CONTIKI_CONF_H__CDBB4VIH3I__

#include <stdint.h>

#define WITH_UIP 1
#define WITH_ASCII 1

#define CLOCK_CONF_SECOND 	1000

/* These names are deprecated, use C99 names. */
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef int8_t s8_t;
typedef int16_t s16_t;
typedef int32_t s32_t;

typedef unsigned int clock_time_t;
typedef unsigned int uip_stats_t;

#ifndef BV
#define BV(x) (1<<(x))
#endif

#define USB_EP1_SIZE 64
#define USB_EP2_SIZE 64

#define CCIF
#define CLIF

/* start of conitki config. */
#define PLATFORM_HAS_LEDS               0
#define PLATFORM_HAS_BUTTON             0

#ifndef RF_CHANNEL
#define RF_CHANNEL                      915000000
#endif /* RF_CHANNEL */

#define NETSTACK_CONF_WITH_IPV6 		1

/* Network setup for IPv6 */
#define NETSTACK_CONF_NETWORK           sicslowpan_driver
#define NETSTACK_CONF_MAC               csma_driver
#define NETSTACK_CONF_RDC               contikimac_driver
#define NETSTACK_CONF_RADIO             semtech_radio_driver
#define NETSTACK_CONF_FRAMER            framer_802154

#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE 8

/* Specify whether the RDC layer should enable
   per-packet power profiling. */
#define CONTIKIMAC_CONF_COMPOWER         1
#define XMAC_CONF_COMPOWER               1
#define CXMAC_CONF_COMPOWER              1

/* Specify a minimum packet size for 6lowpan compression to be
   enabled. This is needed for ContikiMAC, which needs packets to be
   larger than a specified size, if no ContikiMAC header should be
   used. */
#define SICSLOWPAN_CONF_COMPRESSION_THRESHOLD 63

#define CXMAC_CONF_ANNOUNCEMENTS         0
#define XMAC_CONF_ANNOUNCEMENTS          0

#define QUEUEBUF_CONF_NUM                8

#define PACKETBUF_CONF_ATTRS_INLINE 1

#define CONTIKIMAC_CONF_BROADCAST_RATE_LIMIT 0

#define IEEE802154_CONF_PANID       0xABCD

#define SHELL_VARS_CONF_RAM_BEGIN 0x1100
#define SHELL_VARS_CONF_RAM_END 0x2000

#define PROFILE_CONF_ON 0
#ifndef ENERGEST_CONF_ON
#define ENERGEST_CONF_ON 1
#endif /* ENERGEST_CONF_ON */

#define ELFLOADER_CONF_TEXT_IN_ROM 0
#ifndef ELFLOADER_CONF_DATAMEMORY_SIZE
#define ELFLOADER_CONF_DATAMEMORY_SIZE 0x400
#endif /* ELFLOADER_CONF_DATAMEMORY_SIZE */
#ifndef ELFLOADER_CONF_TEXTMEMORY_SIZE
#define ELFLOADER_CONF_TEXTMEMORY_SIZE 0x800
#endif /* ELFLOADER_CONF_TEXTMEMORY_SIZE */


#define AODV_COMPLIANCE
#define AODV_NUM_RT_ENTRIES 32

#define WITH_ASCII 1

#define PROCESS_CONF_NUMEVENTS 8
#define PROCESS_CONF_STATS 1
/*#define PROCESS_CONF_FASTPOLL    4*/

#ifdef NETSTACK_CONF_WITH_IPV6

#define LINKADDR_CONF_SIZE              8

#define UIP_CONF_LL_802154              1
#define UIP_CONF_LLH_LEN                0

#define UIP_CONF_ROUTER                 1

/* configure number of neighbors and routes */
#ifndef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS     20
#endif /* NBR_TABLE_CONF_MAX_NEIGHBORS */
#ifndef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES   20
#endif /* UIP_CONF_MAX_ROUTES */

#define UIP_CONF_ND6_SEND_RA		0
#define UIP_CONF_ND6_SEND_NA		0
#define UIP_CONF_ND6_REACHABLE_TIME     600000
#define UIP_CONF_ND6_RETRANS_TIMER      10000

#define NETSTACK_CONF_WITH_IPV6                   1
#ifndef UIP_CONF_IPV6_QUEUE_PKT
#define UIP_CONF_IPV6_QUEUE_PKT         0
#endif /* UIP_CONF_IPV6_QUEUE_PKT */
#define UIP_CONF_IPV6_CHECKS            1
#define UIP_CONF_IPV6_REASSEMBLY        0
#define UIP_CONF_NETIF_MAX_ADDRESSES    3
#define UIP_CONF_IP_FORWARD             0
#ifndef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE		240
#endif

#define SICSLOWPAN_CONF_COMPRESSION             SICSLOWPAN_COMPRESSION_HC06
#ifndef SICSLOWPAN_CONF_FRAG
#define SICSLOWPAN_CONF_FRAG                    1
#define SICSLOWPAN_CONF_MAXAGE                  8
#endif /* SICSLOWPAN_CONF_FRAG */
#define SICSLOWPAN_CONF_MAX_ADDR_CONTEXTS       2
#ifndef SICSLOWPAN_CONF_MAX_MAC_TRANSMISSIONS
#define SICSLOWPAN_CONF_MAX_MAC_TRANSMISSIONS   5
#endif /* SICSLOWPAN_CONF_MAX_MAC_TRANSMISSIONS */
#else /* NETSTACK_CONF_WITH_IPV6 */
#define UIP_CONF_IP_FORWARD      1
#define UIP_CONF_BUFFER_SIZE     108
#endif /* NETSTACK_CONF_WITH_IPV6 */

#define UIP_CONF_ICMP_DEST_UNREACH 1

#define UIP_CONF_DHCP_LIGHT
#define UIP_CONF_LLH_LEN         0
#ifndef  UIP_CONF_RECEIVE_WINDOW
#define UIP_CONF_RECEIVE_WINDOW  48
#endif
#ifndef  UIP_CONF_TCP_MSS
#define UIP_CONF_TCP_MSS         48
#endif
#define UIP_CONF_MAX_CONNECTIONS 4
#define UIP_CONF_MAX_LISTENPORTS 8
#define UIP_CONF_UDP_CONNS       12
#define UIP_CONF_FWCACHE_SIZE    30
#define UIP_CONF_BROADCAST       1
#define UIP_ARCH_IPCHKSUM        1
#define UIP_CONF_UDP             1
#define UIP_CONF_UDP_CHECKSUMS   1
#define UIP_CONF_PINGADDRCONF    0
#define UIP_CONF_LOGGING         0

#define UIP_CONF_TCP_SPLIT       0

#ifndef AES_128_CONF
#define AES_128_CONF cc2420_aes_128_driver
#endif /* AES_128_CONF */





//#define UIP_CONF_IPV6_MULTICAST		1
#define UIP_CONF_IPV6_RPL			1
#define RPL_BORDER_ROUTER			1

#if UIP_CONF_IPV6_RPL
/* RPL motes use the uip.c link layer address or optionally the harded coded address (but without the prefix!)
 * Different instances can be made by changing the link layer portion of HARD_CODED_ADDRESS in contiki-main.c
 * Rename them to e.g. webserver6.10, webserver6.11, ...
 * They should all attach to a minimal-net rpl border that uses the same primary interface.
 * For multihop testing, configure intermediate notes as routers.
 */
#define HARD_CODED_ADDRESS      "bbbb::10"  //the prefix is ignored for a rpl node
#define UIP_CONF_ND6_SEND_RA            0
#define UIP_CONF_ND6_REACHABLE_TIME     600000
#define UIP_CONF_ND6_RETRANS_TIMER      10000

#if RPL_BORDER_ROUTER
/* RPL border router accepts packets from the host through the fallback and directs them to
 * the primary interface. Thus the fallback and rpl dag prefix must be the same. The prefix of
 * the primary interface does not matter!
 * Rename this build to e.g. webrouter. Then on Windows create two loopback interfaces, bbbb:: and fdfd::
 * Attach the RPL end nodes to fdfd:: and the webrouter to fdfd:: with bbbb:: as the fallback.
 * Direct browser to bbbb::ff:fe00:1/status.html, bbbb::ff:fe00:10/status.html, bbbb::ff:fe00:20/status.html.
 * The traffic will go through the bbbb:: interface to the router, then out the fdfd:: interface to the end
 * nodes. The end nodes must be explicitly added as neighbors to avoid a blocking NS
 * netsh interface ipv6 add neighbor bbbb::ff:fe00:10 33-33-ff-33-44-10 interface=16 (# of the bbbb interface)
 * netsh interface ipv6 add neighbor bbbb::ff:fe00:20 33-33-ff-33-44-20 interface=16 (# of the bbbb interface)
 *
 * Instead of using the fdfd:: loopback it is also possible to attach the border router to another interface,
 * e.g. the jackdaw RNDIS <->  repeater. Then RPL will configure on the radio network and the RF motes will
 * be reached through bbbb::<mote link layer address>.
 * Possibly minimal-net RPL motes could also be added to this interface?
 *
 */
#undef UIP_CONF_ROUTER
#define UIP_CONF_ROUTER             1
//#define RPL_CONF_STATS              0
//#define UIP_CONF_BUFFER_SIZE	 1300
#undef UIP_FALLBACK_INTERFACE
#define UIP_FALLBACK_INTERFACE rpl_interface
//#define WPCAP_FALLBACK_ADDRESS     "bbbb::1"  //bbbb::1 is the default fallback prefix
#undef HARD_CODED_ADDRESS
#define HARD_CODED_ADDRESS            "bbbb::1" //bbbb::ff:fe00:1 is the RPL border router default
//#define UIP_CONF_ND6_SEND_RA		0
//#define UIP_CONF_ND6_REACHABLE_TIME 600000
//#define UIP_CONF_ND6_RETRANS_TIMER  10000
#endif

#endif

#endif /* __CONTIKI_CONF_H__CDBB4VIH3I__ */
