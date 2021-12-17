#include <math.h>

#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"
#include "../includes/sigaction-gestionnaire.h"
#include "../includes/smp-gestionnaire.h"
#include "../includes/semap-gestionnaire.h"

#define MIN_CHEFS 2
#define MIN_MECANOS 3
#define MIN_OUTILS 1

#define MAX_CHEFS 500
#define MAX_MECANOS 500

int fm_mecano, semap_fm_mecano, fm_client, smp, semap_smp;

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
    
    deconnexion_fm(fm_mecano);
    deconnexion_semap(semap_fm_mecano);
    deconnexion_fm(fm_client);
    deconnexion_fm(smp);
    deconnexion_semap(semap_smp);

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
        argv_exec[2 + i] = argv[i];
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


void init_ipcs(int nb_chefs, unsigned short *outils) {
    key_t cle_mecano;
    key_t cle_client;

    int *tab = malloc(sizeof(int) * nb_chefs);

    unsigned short val_init_semap_client[1]={1};

    init_fm(LETTRE_CODE_MECANO, &cle_mecano, &fm_mecano);
    if (fm_mecano == -1) {
        exit(EXIT_FAILURE);
    }

    init_semap(cle_mecano, &semap_fm_mecano, outils);
    if (semap_fm_mecano == -1) {
        /* On detruit les IPC deje crees */
        deconnexion_fm(fm_mecano);
        exit(EXIT_FAILURE);
    }

    init_fm(LETTRE_CODE_CLIENT, &cle_client, &fm_client);
    if (fm_client == -1) {
        /* On detruit les IPC deje crees */
        deconnexion_fm(fm_mecano);
        deconnexion_semap(semap_fm_mecano);
        exit(EXIT_FAILURE);
    }

    init_smp(cle_client, tab, &smp);
    if (smp == -1) {
	    /* On detruit les IPC deje crees : */
        deconnexion_fm(fm_mecano);
        deconnexion_semap(semap_fm_mecano);
        deconnexion_fm(fm_client);
        exit(EXIT_FAILURE);
    }

    init_semap(cle_client, &semap_smp, val_init_semap_client);
    if (semap_smp == -1) {
	    /* On detruit les IPC deje crees : */
        deconnexion_fm(fm_mecano);
        deconnexion_semap(semap_fm_mecano);
        deconnexion_fm(fm_client);
        deconnexion_fm(smp);
        exit(EXIT_FAILURE);
    }

    /* Tout est OK, on initialise */
    for (int i = 0; i < nb_chefs; i++) {
        tab[i] = 0;
    }
}

void ouvre_garage(int nb_chefs, int nb_mecanos, char *outils_str[]) {
    couleur(BLANC);
    fprintf(stdout, "\nOUVERTURE DU GARAGE\n");
    fprintf(stdout, "   ================\n");

    exec_travailleurs(nb_chefs, "chef", NB_OUTILS, outils_str);
    exec_travailleurs(nb_mecanos, "mecanicien", 0, NULL);

    couleur(BLANC);
    fprintf(stdout, "\nLE GARAGE EST OUVERT\n");
    fprintf(stdout, "   ================\n");
}

/* Fonction principale du programme */
int main(int argc, char *argv[]) {
    int nb_chefs, nb_mecanos;
    char *outils_str[NB_OUTILS];
    unsigned short outils[NB_OUTILS];
    int param_valides = 1;

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

    mon_sigaction(SIGUSR1, arret_general);

    init_ipcs(nb_chefs, outils);

    ouvre_garage(nb_chefs, nb_mecanos, outils_str);
    
    exec_client(nb_chefs);

    return EXIT_SUCCESS;
}