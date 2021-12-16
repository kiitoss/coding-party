CC = gcc
OPTIONS = -Wall


all : coding-party

coding-party : initial chef mecanicien  client

initial : src/initial.c includes/global.h includes/fm-gestionnaire.h
	$(CC) $(OPTIONS) src/initial.c src/fm-gestionnaire.c -o initial -lm

chef : src/chef.c src/fm-gestionnaire.c includes/global.h includes/fm-gestionnaire.h
	$(CC) $(OPTIONS) src/chef.c src/fm-gestionnaire.c -o chef

mecanicien : src/mecanicien.c src/fm-gestionnaire.c includes/global.h includes/fm-gestionnaire.h
	$(CC) $(OPTIONS) src/mecanicien.c src/fm-gestionnaire.c -o mecanicien

client : src/client.c includes/global.h includes/fm-gestionnaire.h
	$(CC) $(OPTIONS) src/client.c src/fm-gestionnaire.c -o client


clean :
	rm -f initial chef mecanicien client cle.serv
