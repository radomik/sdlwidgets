.PHONY: obj

SRC := $(shell realpath .)

fast_events:
	gcc -c $(CFLAGS) $(SRC)/FeSDLUtils.c      -o $(ODIR)/FeSDLUtils.o
	gcc -c $(CFLAGS) $(SRC)/FeFastEvents.c    -o $(ODIR)/FeFastEvents.o
	gcc -c $(CFLAGS) $(SRC)/FeTrace.c         -o $(ODIR)/FeTrace.o
	
clean:
	if [ -w $(ODIR)/FeSDLUtils.o ] ; then         rm $(ODIR)/FeSDLUtils.o; fi
	if [ -w $(ODIR)/FeFastEvents.o ] ; then       rm $(ODIR)/FeFastEvents.o; fi
	if [ -w $(ODIR)/FeTrace.o ] ; then            rm $(ODIR)/FeTrace.o; fi

all:
	make clean
	make fast_events
