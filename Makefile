UNAME = $(shell uname)
CC=gcc
ifeq ($(UNAME),Darwin)
CC=gcc-4.9
endif

SOURCES=sort2.c sort.c mean.c hello.c

EXECUTABLES=$(SOURCES:.c=.out)

%.out: %.c
	$(CC) -fopenmp $< -o $@

all: $(EXECUTABLES)

clean:
	rm -f $(EXECUTABLES)
