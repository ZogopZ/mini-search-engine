#############################################################################
# Makefile for: Operating_Systems: Project 2: splitNmerge	            #
#############################################################################


CC            = gcc 
CFLAGS        = -g -Wall -W
DEL_FILE      = rm -f
#include "../root/checker.h"
#include "../root/checker.o"

OBJECTS       = main.o \
		cli_utils.o \
		splitNmerge.o 

default: splitNmerge

splitNmerge: $(OBJECTS) 
	$(CC) $(CFLAGS) -o splitNmerge main.o cli_utils.o splitNmerge.o ../root/checker.o

main.o: main.c cli_utils.h \
		splitNmerge.h \
		../root/checker.h
	$(CC) -c $(CFLAGS) -o main.o main.c

cli_utils.o: cli_utils.c cli_utils.h \
		../root/checker.h	
	$(CC) -c $(CFLAGS) -o cli_utils.o cli_utils.c

splitNmerge.o: splitNmerge.c splitNmerge.h \
		splitNmerge.h \
		../root/checker.h
	$(CC) -c $(CFLAGS) -o splitNmerge.o splitNmerge.c

clean:
	-$(DEL_FILE)  splitNmerge
	-$(DEL_FILE)  $(OBJECTS)
