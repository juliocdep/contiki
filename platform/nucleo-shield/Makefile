TARGET=nucleo-shield

all: nucleo-shield 


CONTIKI=../..

CONTIKI_TARGET_MAIN=contiki-main.c

PROJECT_SOURCEFILES = parity.c 


randgen: randgen.c
	gcc -DNDEBUG -I $(CONTIKI)/cpu/stm32/ -I . -I $(CONTIKI)/core  randgen.c -o randgen

randcheck: randcheck.c
	gcc -DNDEBUG -I $(CONTIKI)/cpu/stm32/ -I . -I $(CONTIKI)/core  randcheck.c -o randcheck

clean: nucleo-shield_clean

.PHONY: nucleo-shield_clean

nucleo-shield_clean:

include $(CONTIKI)/Makefile.include

