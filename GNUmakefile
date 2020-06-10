# Make file for Scintilla on Linux, macOS, or Windows
# @file makefile
# Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
# The License.txt file describes the conditions under which this software may be distributed.
# This makefile assumes GCC 9.0+ is used and changes will be needed to use other compilers.
# Clang 9.0+ can be used with CLANG=1 on command line.
# Builds for GTK+ 2 and 3. GTK 3 requires GTK3=1 on command line.
# Also works with ming32-make on Windows.

.SUFFIXES: .c .o .h .a

WARNINGS += -Wall -pedantic -Wextra

ifdef CLANG
# Can choose aspect to sanitize: address and undefined can simply change SANITIZE but for
# thread also need to create Position Independent Executable -> search online documentation
SANITIZE = address
#SANITIZE = undefined
CXX = clang++
CC = clang
BASE_FLAGS += -fsanitize=$(SANITIZE)
endif

GTK_VERSION = $(if $(GTK3),gtk+-3.0,gtk+-2.0)
CONFIG_FLAGS:=$(shell pkg-config --cflags $(GTK_VERSION))
CONFIGLIB:=$(shell pkg-config --libs $(GTK_VERSION) gthread-2.0 gmodule-no-export-2.0)

PROG = rosetta

INCLUDES=-I../scintilla/include
DEFINES += -DGTK

DEFINES += -D$(if $(DEBUG),DEBUG,NDEBUG)
BASE_FLAGS += $(if $(DEBUG),-g,-Os)

LIBS += -lm -lstdc++

all: $(PROG)

clean:
	rm -rf *.o $(PROG)*

%.o: %.c
	$(CC) $(DEFINES) $(INCLUDES) $(CONFIG_FLAGS) $(BASE_FLAGS) $(CFLAGS) -w -c $<

$(PROG): main.o ../scintilla/bin/scintilla.a
	$(CXX) $(BASE_FLAGS) $^ -o $@ $(CONFIGLIB) $(LIBS)
