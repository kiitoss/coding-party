#include "global.h"

#define MIN_CHEFS 2
#define MIN_MECANOS 3
#define MIN_OUTILS 1

#define MAX_CHEFS 1000
#define MAX_MECANOS 1000

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

/* Fonction principale du programme */
int main(int argc, char *argv[]) {
    int nb_chefs, nb_mecanos;
    char *outils[NB_OUTILS];
    int param_valides = 1;

    struct stat st;
    key_t cle_mecano;
    int mem_part_mecano;
    int *tab;

    pid_t pid_chef;

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
    

    fprintf(stderr, "Allumage des fours ... !\n");

    /* creation d'une string de taille optimale por sotcker l'ordre du chef */
    int taille_nb = ((int) log10(nb_chefs)) + 1;
    char nb_chef_str[taille_nb];

    for(int i = 1; i <= nb_chefs; i++) {
	    pid_chef = fork();   
        if (pid_chef == -1) break;
        if (pid_chef == 0) {
            sprintf(nb_chef_str, "%d", i);
            char *args_chef[4 + NB_OUTILS] = {"chef", nb_chef_str};
            for (int j = 0; j < NB_OUTILS; j++) args_chef[4 + i] = outils[i];
            args_chef[3 + NB_OUTILS] = NULL;
            execv("chef", args_chef);
            exit(EXIT_FAILURE);
        }
    }
    usleep(500000);
    fprintf(stderr, "... fours prêts !\n");


    /* On nettoie */
    /* Detachement SMP */
    shmdt(tab);

    /* Destruction SMP */
    shmctl(mem_part_mecano, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}