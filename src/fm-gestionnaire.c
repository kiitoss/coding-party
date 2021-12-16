#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

void connexion_fm_mecano(char *type_travailleur, key_t *cle, int *fm) {
    FILE *fich_cle;

    /* On teste si le fichier cle existe dans le repertoire courant */
    fich_cle = fopen(FICHIER_CLE,"r");
    if (fich_cle == NULL) {
        fprintf(stderr, "(%s) Lancement client impossible\n", type_travailleur);
        exit(EXIT_FAILURE);
    }

    /* Creation de la cle */
    *cle = ftok(FICHIER_CLE, LETTRE_CODE_MECANO);
    if (*cle == -1) {
	    fprintf(stderr, "(%s) Probleme creation cle\n", type_travailleur);
	    exit(EXIT_FAILURE);
    }

    /* Recuperation file de message */
    *fm = msgget(*cle, 0);
    if (*fm == -1) {
	    fprintf(stderr, "(%s) Probleme recuperation file de message\n", type_travailleur);
	    exit(EXIT_FAILURE);
    }
}

void deconnexion_fm_mecano(int fm) {
    msgctl(fm, IPC_RMID, NULL);
    execlp("ipcrm", "ipcrm", "-a", NULL);
}


void envoie_requete(int fm, int ordre_exp, int duree) {
    requete_t requete;

    /* creation de la requete */
    requete.type = ordre_exp;
    requete.duree = duree;
    
    /* envoi de la requete */
    msgsnd(fm, &requete, sizeof(requete_t), 0);
}


requete_t attend_reponse(int fm, int ordre_exp) {
    requete_t reponse;
    int res_rcv;

    /* attente de la reponse */
    res_rcv = msgrcv(fm, &reponse, sizeof(requete_t), ordre_exp, 0);
    if (res_rcv == -1) {
	    fprintf(stderr, "Erreur, numero %d\n", errno);
	    exit(EXIT_FAILURE);
    }

    return reponse;
}