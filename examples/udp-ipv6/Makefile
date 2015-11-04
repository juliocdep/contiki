CONTIKI_PROJECT = udp-server

#all: udp-server udp-client
#all: udp-server
all: udp-client

CONTIKI = ../..
CONTIKI_WITH_IPV6 = 1
CFLAGS += -DUIP_CONF_ND6_SEND_NA=1
include $(CONTIKI)/Makefile.include
