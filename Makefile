CC = gcc
OPTIONS = -W -Wall


all : initial

initial : initial.c
	$(CC) $(OPTIONS) initial.c -o initial

clean :
	rm -f initial
