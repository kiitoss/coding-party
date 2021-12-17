#include <math.h>

#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"

#define MIN_CHEFS 2
#define MIN_MECANOS 3
#define MIN_OUTILS 1

#define MAX_CHEFS 500
#define MAX_MECANOS 500

int fm_mecano, semap, fm_client;

struct pid_fils *fils = NULL;

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

void arret_general() {
    struct pid_fils *tmp;
    couleur(BLANC);
    fprintf(stdout,"\nLE GARAGE FERME SES PORTES\n");
    fprintf(stdout,"  =======================\n");

    while (fils != NULL) {
        tmp = fils;
        fils = fils->suivant;
        kill(tmp->pid, SIGUSR1);
        free(tmp);
    }
    
    semctl(semap, 1, IPC_RMID, NULL);

    deconnexion_fm(fm_mecano);
    deconnexion_fm(fm_client);

    couleur(REINIT);

    exit(EXIT_SUCCESS);
}

int taille_int(int i) {
    return ((int) log10((double) i)) + 1;
}

void nouveau_fils(pid_t pid) {
    struct pid_fils *nouveau = malloc(sizeof(struct pid_fils));
    nouveau->pid = pid;
    nouveau->suivant = fils;
    fils = nouveau;
}

void exec_travailleurs(int nb, char *path, int argc, char *argv[]) {
    pid_t pid;

    /* creation d'une string de taille optimale por sotcker l'ordre du travailleur */
    int taille_ordre = taille_int(nb);
    char ordre[taille_ordre];

    char *argv_exec[4 + argc];
    argv_exec[0] = path;

    
    for (int i = 0; i < argc; i++) {
        argv_exec[1 + i] = argv[i];
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
        } else {
            nouveau_fils(pid);
        }
    }
    usleep(500000);
}

void exec_client(int nb_chefs) {
    pid_t pid;
    
    /* creation d'une string de taille optimale por sotcker l'ordre du travailleur */
    int taille_chefs = taille_int(nb_chefs);
    char nb_chefs_str[taille_chefs];
    sprintf(nb_chefs_str, "%d", nb_chefs);

    for(;;) {
        pid = fork(); 
         
        if (pid == -1) break;
        if (pid == 0) {
            execl("client", "client", nb_chefs_str, NULL);
            exit(EXIT_FAILURE);
        } else {
            nouveau_fils(pid);
        }

        sleep(2);
    }
}

/* Fonction principale du programme */
int main(int argc, char *argv[]) {
    int nb_chefs, nb_mecanos;
    char *outils_str[NB_OUTILS];
    unsigned short outils[NB_OUTILS];
    int param_valides = 1;

    
    key_t cle_mecano;
    key_t cle_client;

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
        outils_str[i] = argv[3 + i];
        outils[i] = atoi(outils_str[i]);
        if (outils[i] < MIN_OUTILS) {
            param_valides = 0;
            break;
        }
    }
    if (!param_valides) usage(argv[0]);


    init_fm(LETTRE_CODE_MECANO, &cle_mecano, &fm_mecano);

    /* On cree le semaphore (meme cle) */
    semap = semget(cle_mecano, NB_OUTILS, IPC_CREAT | 0660);
    if (semap == -1) {
	    fprintf(stderr, "Probleme creation ensemble de semaphore ou il existe deja\n");
	    deconnexion_fm(fm_mecano);
	    exit(EXIT_FAILURE);
    }

    /* On l'initialise */
    if (semctl(semap, 0, SETALL, outils) == -1) {
	    fprintf(stderr, "Probleme initialisation semaphore\n");
	    /* On detruit les IPC deje crees : */
        semctl(semap, 0, IPC_RMID, NULL);
        deconnexion_fm(fm_mecano);
        exit(EXIT_FAILURE);
    }


    init_fm(LETTRE_CODE_CLIENT, &cle_client, &fm_client);

    mon_sigaction(SIGUSR1, arret_general);

    couleur(BLANC);
    fprintf(stdout, "\nOUVERTURE DU GARAGE\n");
    fprintf(stdout, "   ================\n");

    exec_travailleurs(nb_chefs, "chef", NB_OUTILS, outils_str);
    exec_travailleurs(nb_mecanos, "mecanicien", 0, NULL);

    couleur(BLANC);
    fprintf(stdout, "\nLE GARAGE EST OUVERT\n");
    fprintf(stdout, "   ================\n");

    
    exec_client(nb_chefs);

    return EXIT_SUCCESS;
}