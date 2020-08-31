#############################################################################
# Makefile for: Operating_Systems: Project 2: myfind		            #
#############################################################################


CC            = gcc 
CFLAGS        = -g -Wall -W
LDFLAGS       = -lm
DEL_FILE      = rm -f

OBJECTS       = main.o \
		cli_utils.o \
		root.o \
		checker.o 
OTHER_FILES   = splitter \
		results

default: myfind

myfind: $(OBJECTS) 
	$(CC) $(CFLAGS) -o myfind main.o cli_utils.o root.o checker.o $(LDFLAGS)

main.o: main.c root.h \
		checker.h \
		cli_utils.h 		
	$(CC) -c $(CFLAGS) -o main.o main.c

cli_utils.o: cli_utils.c cli_utils.h \
		root.h \
		checker.h 
	$(CC) -c $(CFLAGS) -o cli_utils.o cli_utils.c

root.o: root.c root.h \
		checker.h \
		cli_utils.h 
	$(CC) -c $(CFLAGS) -o root.o root.c

checker.o: checker.c checker.h \
		cli_utils.h
	$(CC) -c $(CFLAGS) -o checker.o checker.c

clean:
	-$(DEL_FILE)  myfind
	-$(DEL_FILE)  $(OBJECTS)
	-$(DEL_FILE)  $(OTHER_FILES)
