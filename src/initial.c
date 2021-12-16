#include <math.h>
#include <signal.h>

#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

#define MIN_CHEFS 2
#define MIN_MECANOS 3
#define MIN_OUTILS 1

#define MAX_CHEFS 500
#define MAX_MECANOS 500

int fm;

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

void arret() {
    fprintf(stdout,"Le garage ferme ses portes.\n");
    killpg(0, SIGUSR1);
    deconnexion_fm_mecano(fm);
    exit(EXIT_FAILURE);
}

void mon_sigaction(int signal, void (*f)(int)) {
    struct sigaction action;
    action.sa_handler = f;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(signal,&action,NULL);
}


void exec_travailleurs(int nb, char *path, int argc, char *argv[]) {
    pid_t pid;

    /* creation d'une string de taille optimale por sotcker l'ordre du travailleur */
    int taille_ordre = ((int) log10((double) nb)) + 1;
    char ordre[taille_ordre];

    char *argv_exec[4 + argc];
    argv_exec[0] = path;
    for (int j = 0; j < argc; j++) {
        argv_exec[1 + j] = argv[j];
    }
    argv_exec[2 + argc] = NULL;

    for(int i = 1; i <= nb; i++) {
	    pid = fork(); 
         
        if (pid == -1) break;
        if (pid == 0) {
            sprintf(ordre, "%d", i);
            argv_exec[1] = ordre;
            execv(path, argv_exec);
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

    FILE *fich_cle;
    key_t cle_mecano;

    int semap;

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


    /* Creation de la cle */
    /* 1 - On teste si le fichier cle existe dans le repertoire courant */
    fich_cle = fopen(FICHIER_CLE, "r");
    if (fich_cle == NULL){
        if (errno == ENOENT){
            /* on le cree */
            fich_cle = fopen(FICHIER_CLE, "w");
            if (fich_cle == NULL){
                fprintf(stderr, "Ouverture du garage impossible\n");
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "Ouverture du garage impossible\n");
            exit(EXIT_FAILURE);
        }
    }

    cle_mecano = ftok(FICHIER_CLE, LETTRE_CODE_MECANO);
    if (cle_mecano == -1) {
	    fprintf(stderr, "(chef) Probleme creation cle\n");
	    exit(EXIT_FAILURE);
    }

    /* Creation file de message */
    fm = msgget(cle_mecano, IPC_CREAT | 0660);
    if (fm == -1) {
	    fprintf(stderr, "Probleme creation file de message\n");
	    exit(EXIT_FAILURE);
    }

    /* On cree le semaphore (meme cle) */
    semap = semget(cle_mecano, NB_OUTILS, IPC_CREAT | 0660);
    if (semap == -1) {
	    fprintf(stderr, "Probleme creation ensemble de semaphore ou il existe deja\n");
	    deconnexion_fm_mecano(fm);
	    exit(EXIT_FAILURE);
    }

    /* On l'initialise */
    if (semctl(semap, 1, SETALL, outils) == -1) {
	    printf("Probleme initialisation semaphore\n");
	    /* On detruit les IPC deje crees : */
        semctl(semap, 1, IPC_RMID, NULL);
        deconnexion_fm_mecano(fm);
        exit(EXIT_FAILURE);
    }

    mon_sigaction(SIGUSR1, arret);

    fprintf(stderr, "Allumage des fours\t\t");
    exec_travailleurs(nb_chefs, "chef", NB_OUTILS, outils);
    fprintf(stderr, "\tfours prêts !\n");
    
    fprintf(stderr, "\nEchauffement des mecaniciens\t");
    exec_travailleurs(nb_mecanos, "mecanicien", 0, NULL);
    fprintf(stderr, "\tmecaniciens prêts !\n");

    
    for(;;) {}
    

    return EXIT_SUCCESS;
}