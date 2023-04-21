CONTIKI_PROJECT=source
all: $(CONTIKI_PROJECT)

#PROJECT_SOURCEFILES += mysensor.c
#PROJECT_SOURCEFILES += callbacks.c

#PLATFORMS_ONLY = srf06-cc26xx cc2538dk openmote-cc2538 zoul native simplelink

CONTIKI=/home/mhmd/contiki-ng
include $(CONTIKI)/Makefile.include
