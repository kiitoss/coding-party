#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/msg.h>
#include <sys/sem.h>


#include <time.h>

#define NB_OUTILS 4

#define FICHIER_CLE "cle.serv"
#define LETTRE_CODE_MECANO 'm'