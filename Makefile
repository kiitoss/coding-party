CC = gcc
OPTIONS = -W -Wall


all : coding-party

coding-party : initial chef

initial : initial.c
	$(CC) $(OPTIONS) initial.c -o initial

chef : chef.c
	$(CC) $(OPTIONS) chef.c -o chef

clean :
	rm -f initial chef cle.serv
