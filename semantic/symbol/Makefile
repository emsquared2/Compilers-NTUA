###############################################################################
#  CVS version:
#     $Id: Makefile,v 1.2 2004/05/05 22:00:08 nickie Exp $
###############################################################################
#
#  Makefile    : Makefile
#  Project     : PCL Compiler
#  Version     : 1.0 alpha
#  Written by  : Nikolaos S. Papaspyrou (nickie@softlab.ntua.gr)
#  Date        : May 14, 2003
#  Description : Generic symbol table in C
#
#  ---------
#  National Technical University of Athens.
#  School of Electrical and Computer Engineering.
#  Division of Computer Science and Technology.
#  Software Technology Laboratory


CFILES   = symbol.c error.c general.c symbtest.c
HFILES   = symbol.h error.h general.h
OBJFILES = $(patsubst %.c,%.o,$(CFILES))
EXEFILES = symbtest

SRCFILES = $(HFILES) $(CFILES)

CC=gcc
CFLAGS=-Wall -ansi -pedantic -g

%.o : %.c
	$(CC) $(CFLAGS) -c $<

symbtest: symbtest.o symbol.o error.o general.o
	$(CC) $(CFLAGS) -o $@ $^

general.o  : general.c general.h error.h
error.o    : error.c general.h error.h
symbol.o   : symbol.c symbol.h general.h error.h
symbtest.o : symbtest.c symbol.h error.h

clean:
	$(RM) $(EXEFILES) $(OBJFILES) *~

count:
	wc -l -c Makefile $(SRCFILES)

checkin:
	cvs commit Makefile $(SRCFILES)

tar:
	tar cvfzh symbol.tar.gz Makefile $(SRCFILES)

zip:
	zip symbol.zip Makefile $(SRCFILES)
