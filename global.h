#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <math.h>

#define NB_OUTILS 4

#define FICHIER_CLE "cle.serv"
#define LETTRE_CODE_MECANO 'm'

/* Structure d'une entree dans la SMP chef-mecano */
typedef struct {
    int a;
} entree_smp_mecano;