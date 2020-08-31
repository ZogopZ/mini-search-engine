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
		leaf.o 

default: leaves

leaves: $(OBJECTS) 
	$(CC) $(CFLAGS) -o leaves main.o cli_utils.o leaf.o ../root/checker.o

main.o: main.c leaf.h \
		cli_utils.h \
		../root/checker.h
	$(CC) -c $(CFLAGS) -o main.o main.c

cli_utils.o: cli_utils.c cli_utils.h \
		../root/checker.h	
	$(CC) -c $(CFLAGS) -o cli_utils.o cli_utils.c

leaf.o: leaf.c leaf.h \
		cli_utils.h \
		../root/checker.h
	$(CC) -c $(CFLAGS) -o leaf.o leaf.c

clean:
	-$(DEL_FILE)  leaves
	-$(DEL_FILE)  $(OBJECTS)
