CC=gcc
CFLAGS=-Wall -Wextra -pedantic
LIBS=
INSTALL_DIR=~/bin

PROGRAM=signal-fork-demo

all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).c
	$(CC) $(CFLAGS) -o $(PROGRAM) $(PROGRAM).c $(LIBS)

install: $(PROGRAM)
	mkdir -p $(INSTALL_DIR)
	cp $(PROGRAM) $(INSTALL_DIR)

.PHONY: clean
clean:
	rm -f $(PROGRAM)
