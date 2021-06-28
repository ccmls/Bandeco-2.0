CC = gcc
CFLAGS = -g -pthread 

PROGRAMS = dining-hall-lock

all: $(PROGRAMS)

clean:
	rm -f *.o *~ $(PROGRAMS)