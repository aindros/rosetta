# Make file for rosetta on FreeBSD
#
# Copyright 2020 by Alessandro Iezzi <info@alessandroiezzi.it>
#
# The LICENSE file describes the conditions under which this software may be distributed.
#
# Make arguments:
#  GNU=1
#  GTK2=1
#  DEBUG=1
#  SCI_DIR=/path/to/scintilla

.SUFFIXES: .c .o .h .a

PROG = rosetta
SCI_DIR=./scintilla
SOURCES!=ls *.c
OBJECTS=${SOURCES:.c=.o}

WARNINGS = -Wall -pedantic -Wextra

# Can choose aspect to sanitize: address and undefined can simply change SANITIZE but for
# thread also need to create Position Independent Executable -> search online documentation
SANITIZE = address
#SANITIZE = undefined

.ifdef GNU
  LINKER = gcc
  CC     = gcc
.else
  LINKER = cc
  CC     = cc
.endif

.ifdef GTK2
  GTK_VERSION = gtk+-2.0
.else
  GTK_VERSION = gtk+-3.0
.endif

# -isystem instead of -I, disables warnings from headers files
CONFIG_FLAGS != pkg-config --cflags $(GTK_VERSION) | sed 's/-I/-isystem/g'
CONFIGLIB    != pkg-config --libs $(GTK_VERSION) gthread-2.0 gmodule-no-export-2.0

# INCLUDES   = -I$(SCI_DIR)/include
INCLUDES   = -isystem$(SCI_DIR)/include
DEFINES    = -DGTK

.ifdef DEBUG
  DEFINES += -DDEBUG
  BASE_FLAGS += -g
.else
  DEFINES += -DNDEBUG
  BASE_FLAGS += -Os
.endif

CFLAGS += $(BASE_FLAGS) $(WARNINGS) -std=c89
LINKERFLAGS = $(BASE_FLAGS) -fsanitize=$(SANITIZE)

LIBS = -lm -lstdc++

all: $(PROG)

clean:
	rm -rf *.o $(PROG)*

.c.o:
	$(CC) $(DEFINES) $(INCLUDES) $(CONFIG_FLAGS) $(CFLAGS) -c ${.IMPSRC}

$(PROG): $(OBJECTS) $(SCI_DIR)/bin/scintilla.a
	$(LINKER) $(LINKERFLAGS) ${.ALLSRC} -o ${.TARGET} $(CONFIGLIB) $(LIBS)

$(SCI_DIR)/bin/scintilla.a:
	cd $(SCI_DIR)/gtk && make
