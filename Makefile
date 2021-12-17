CC = gcc
OPTIONS = -W -Wall
IPCS = fm-gestionnaire.o smp-gestionnaire.o semap-gestionnaire.o sigaction-gestionnaire.o

all: coding-party

coding-party: initial chef client mecanicien simple-clean

initial: src/initial.c includes/global.h ${IPCS}
	$(CC) $(OPTIONS) src/initial.c ${IPCS} -o initial -lm

chef: src/chef.c includes/global.h ${IPCS}
	$(CC) $(OPTIONS) src/chef.c ${IPCS} -o chef

mecanicien: src/mecanicien.c includes/global.h ${IPCS}
	$(CC) $(OPTIONS) src/mecanicien.c ${IPCS} -o mecanicien

client: src/client.c includes/global.h ${IPCS}
	$(CC) $(OPTIONS) src/client.c ${IPCS} -o client



fm-gestionnaire.o: src/fm-gestionnaire.c includes/fm-gestionnaire.h
	$(CC) $(OPTIONS) src/fm-gestionnaire.c -c

smp-gestionnaire.o: src/smp-gestionnaire.c includes/smp-gestionnaire.h
	$(CC) $(OPTIONS) src/smp-gestionnaire.c -c

semap-gestionnaire.o: src/semap-gestionnaire.c includes/semap-gestionnaire.h
	$(CC) $(OPTIONS) src/semap-gestionnaire.c -c

sigaction-gestionnaire.o: src/sigaction-gestionnaire.c includes/sigaction-gestionnaire.h
	$(CC) $(OPTIONS) src/sigaction-gestionnaire.c -c





simple-clean:
	rm *.o

clean :
	rm -f initial chef mecanicien client cle.serv
