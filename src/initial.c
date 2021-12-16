#include <math.h>

#include "../includes/global.h"

#define MIN_CHEFS 2
#define MIN_MECANOS 3
#define MIN_OUTILS 1

#define MAX_CHEFS 500
#define MAX_MECANOS 500

/* Fonction d'usage du programme */
void usage(char *s) {
    fprintf(stderr, "Usage: %s <nb_chefs> <nb_mecanos>", s);
    for (int i = 1; i <= NB_OUTILS; i++) {
        fprintf(stderr, " nb_%d", i);
    }
    fprintf(stderr, "\n\tnb_chefs: nombre de chefs ( > %d )", MIN_CHEFS - 1);
    fprintf(stderr, "\n\tnb_mecanos: nombre de mecanos ( > %d )", MIN_MECANOS - 1);
    fprintf(stderr, "\n\tnb_i: nombre d'outils ( > %d )\n", MIN_OUTILS - 1);
    exit(EXIT_FAILURE);
}


void lance_travailleurs(int nb, char *path, int argc, char *args[]) {
    pid_t pid;

    /* creation d'une string de taille optimale por sotcker l'ordre du travailleur */
    int taille_ordre = ((int) log10((double) nb)) + 1;
    char ordre[taille_ordre];

    char *args_exec[4 + argc];
    args_exec[0] = path;
    for (int j = 0; j < argc; j++) {
        args_exec[4 + j] = args[j];
    }
    args_exec[3 + argc] = NULL;

    for(int i = 1; i <= nb; i++) {
	    pid = fork();   
        if (pid == -1) break;
        if (pid == 0) {
            sprintf(ordre, "%d", i);
            args_exec[1] = ordre;
            execv(path, args_exec);
            exit(EXIT_FAILURE);
        }
    }
    usleep(500000);
}

/* Fonction principale du programme */
int main(int argc, char *argv[]) {
    int nb_chefs, nb_mecanos;
    char *outils[NB_OUTILS];
    int param_valides = 1;

    struct stat st;
    key_t cle_mecano;
    int mem_part_mecano;
    int *tab;

    /* Verficiation des parametres */
    if (argc < 3 + NB_OUTILS) {
        usage(argv[0]);
    }

    nb_chefs = atoi(argv[1]);
    if (nb_chefs < MIN_CHEFS) usage(argv[0]);
    if (nb_chefs > MAX_CHEFS) nb_chefs = MAX_CHEFS;

    nb_mecanos = atoi(argv[2]);
    if (nb_mecanos < MIN_MECANOS) usage(argv[0]);
    if (nb_mecanos > MAX_MECANOS) nb_mecanos = MAX_MECANOS;

    for (int i = 0; i < NB_OUTILS; i++) {
        outils[i] = argv[3+i];
        if (atoi(outils[i]) < MIN_OUTILS) {
            param_valides = 0;
            break;
        }
    }
    if (!param_valides) usage(argv[0]);


    /* On teste si le fichier cle existe dans le repertoire courant */
    if ((stat(FICHIER_CLE, &st) == -1) && (open(FICHIER_CLE, O_RDONLY | O_CREAT | O_EXCL, 0660) == -1)) {
	    fprintf(stderr,"Pas de fichier cle pour les mecanos et probleme creation fichier cle\n");
        exit(EXIT_FAILURE);
    }

    cle_mecano = ftok(FICHIER_CLE, LETTRE_CODE_MECANO);
    if (cle_mecano == -1) {
	    fprintf(stderr, "(chef) Probleme creation cle\n");
	    exit(EXIT_FAILURE);
    }

    /* On cree le SMP et on teste s'il existe deja */
    mem_part_mecano = shmget(cle_mecano, sizeof(entree_smp_mecano), IPC_CREAT | IPC_EXCL | 0660);
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
    

    fprintf(stderr, "Allumage des fours\t\t");
    lance_travailleurs(nb_chefs, "chef", NB_OUTILS, outils);
    fprintf(stderr, "\tfours prêts !\n");

    fprintf(stderr, "\nEchauffement des mecaniciens\t");
    lance_travailleurs(nb_mecanos, "mecanicien", 0, NULL);
    fprintf(stderr, "\tmecaniciens prêts !\n");


    /* On nettoie */
    /* Detachement SMP */
    shmdt(tab);

    /* Destruction SMP */
    shmctl(mem_part_mecano, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}