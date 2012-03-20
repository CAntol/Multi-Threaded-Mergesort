CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Werror
LDFLAGS = -lpthread
PROGS = mergetest public00 public01 public02 public03 public04 public05

all: $(PROGS)

.PHONY: all clean

clean:
	rm -f $(PROGS) *.o *~

mergesort.o: mergesort.h

mergetest.o: mergesort.h

public%.o:  mergesort.h

mergetest: mergetest.o mergesort.o
	$(CC) -o mergetest mergetest.o mergesort.o -lpthread

public%: public%.o mergesort.o
	$(CC) -o $@ $@.o mergesort.o -lpthread