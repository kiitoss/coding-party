#include "global.h"


int main(int argc, char *argv[]) {
    struct stat st;
    key_t cle_mecano;

    int mem_part_mecano;
    int *tab;

    /* On teste si le fichier cle existe dans le repertoire courant */
    if ((stat(FICHIER_CLE, &st) == -1) && (open(FICHIER_CLE, O_RDONLY | O_CREAT, 0660) == -1)) {
	    fprintf(stderr,"Pas de fichier cle pour les mecanos et probleme creation fichier cle\n");
        exit(EXIT_FAILURE);
    }

    cle_mecano = ftok(FICHIER_CLE, LETTRE_CODE_MECANO);
    if (cle_mecano == -1) {
	    fprintf(stderr, "Probleme creation cle\n");
	    exit(EXIT_FAILURE);
    }

    /* On cree le SMP et on teste s'il existe deja */
    mem_part_mecano = shmget(cle_mecano, sizeof(entree_smp_mecano), IPC_CREAT | 0660);
    if (mem_part_mecano == -1) {
	    fprintf(stderr, "Probleme creation SMP ou il existe deja\n");
	    exit(EXIT_FAILURE);
    }

    /* Attachement de la memoire partagee */
    tab = shmat(mem_part_mecano, NULL, 0);
    if (tab == (int *) -1){
	    fprintf(stderr, "Probleme attachement\n");
	    /* Il faut detruire le SMP puisqu'on l'a cree : */
	    shmctl(mem_part_mecano,IPC_RMID,NULL);
	    exit(EXIT_FAILURE);
    }

    printf("un chef vient d'arriver\n");

    /* On nettoie */
    /* Detachement SMP */
    shmdt(tab);

    /* Destruction SMP */
    shmctl(mem_part_mecano, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}