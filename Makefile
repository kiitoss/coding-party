CC = gcc
OPTIONS = -W -Wall


all : coding-party

coding-party : initial chef

initial : initial.c global.h
	$(CC) $(OPTIONS) initial.c -o initial -lm

chef : chef.c global.h
	$(CC) $(OPTIONS) chef.c -o chef

clean :
	rm -f initial chef cle.serv
