CC = gcc
OPTIONS = -W -Wall


all : coding-party

coding-party : initial chef mecanicien

initial : src/initial.c includes/global.h
	$(CC) $(OPTIONS) src/initial.c -o initial -lm

chef : src/chef.c src/smp-gestionnaire.c includes/global.h includes/smp-gestionnaire.h
	$(CC) $(OPTIONS) src/chef.c src/smp-gestionnaire.c -o chef

mecanicien : src/mecanicien.c src/smp-gestionnaire.c includes/global.h includes/smp-gestionnaire.h
	$(CC) $(OPTIONS) src/mecanicien.c src/smp-gestionnaire.c -o mecanicien

clean :
	rm -f initial chef mecanicien cle.serv
