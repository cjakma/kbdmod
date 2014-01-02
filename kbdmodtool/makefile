# Name: Makefile
# Project: kbdmodtools
# Author: Kwangyoung Lee
# Creation Date: 2014-01-04
# Tabsize: 4

EXE_SUFFIX= .exe

CC=				gcc
CXX=			   g++
CFLAGS=			-O2 -Wall $(USBFLAGS)
LIBS=			$(USBLIBS)
ARCH_COMPILE=	
ARCH_LINK=		

OBJ=		keymap.o
PROGRAM=	keymap$(EXE_SUFFIX)

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) $(ARCH_LINK) $(CFLAGS) -o $(PROGRAM) $(OBJ) $(LIBS)


strip: $(PROGRAM)
	strip $(PROGRAM)

clean:
	rm -f $(OBJ) $(PROGRAM)

.c.o:
	$(CC) $(ARCH_COMPILE) $(CFLAGS) -c $*.c -o $*.o
