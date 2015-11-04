TARGET=nucleo-shield-net

all: nucleo-shield-net 


CONTIKI=../..

CONTIKI_TARGET_MAIN=contiki-main.c

PROJECT_SOURCEFILES = parity.c 


randgen: randgen.c
	gcc -DNDEBUG -I $(CONTIKI)/cpu/stm32/ -I . -I $(CONTIKI)/core  randgen.c -o randgen

randcheck: randcheck.c
	gcc -DNDEBUG -I $(CONTIKI)/cpu/stm32/ -I . -I $(CONTIKI)/core  randcheck.c -o randcheck

clean: nucleo-shield-net_clean

.PHONY: nucleo-shield-net_clean

nucleo-shield-net_clean:

include $(CONTIKI)/Makefile.include

