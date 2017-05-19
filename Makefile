CFLAGS=-g -Wall -Wextra -I.
list: list_testdriver.c list.c
	gcc -o list_testdriver list_testdriver.c list.c ${CFLAGS}.