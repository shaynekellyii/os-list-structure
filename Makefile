CC = gcc
CFLAGS = -g -Wall -Wextra -I.
PROG = list_test
OBJS = list.o list_testdriver.o

run: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

list.o: list.h