# Variables
CC = gcc
CFLAGS = -g -Wall -Wstrict-prototypes
LDFLAGS = -g
BINS = finddups
SRC = finddups.c
OBJ = $(SRC:.c=.o)

# Targets
.PHONY: default clean immaculate

default: $(BINS)

clean:
	rm -f core* *.o *~ $(BINS)

immaculate: clean

$(BINS): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(BINS)
	./finddups ../ . /dev/shm finddups ./finddups read-only
	./finddups
