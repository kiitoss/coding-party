#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

void init_fm(char code, key_t *cle, int *fm) {
    FILE *fich_cle;
    
    /* Creation de la cle */
    /* 1 - On teste si le fichier cle existe dans le repertoire courant */
    fich_cle = fopen(FICHIER_CLE, "r");
    if (fich_cle == NULL) {
        if (errno == ENOENT) {
            /* on le cree */
            fich_cle = fopen(FICHIER_CLE, "w");
            if (fich_cle == NULL) {
                fprintf(stderr, "Ouverture du garage impossible\n");
                *fm = -1;
                return;
            }
        } else {
            fprintf(stderr, "Ouverture du garage impossible\n");
            *fm = -1;
            return;
        }
    }

    *cle = ftok(FICHIER_CLE, code);
    if (*cle == -1) {
	    fprintf(stderr, "(chef) Probleme creation cle\n");
	    *fm = -1;
        return;
    }

    /* Creation file de message */
    *fm = msgget(*cle, IPC_CREAT | 0660);
    if (*fm == -1) {
	    fprintf(stderr, "Probleme creation file de message\n");
	    *fm = -1;
        return;
    }
}

void connexion_fm(char *type_personne, char code, key_t *cle, int *fm) {
    FILE *fich_cle;

    /* On teste si le fichier cle existe dans le repertoire courant */
    fich_cle = fopen(FICHIER_CLE,"r");
    if (fich_cle == NULL) {
        fprintf(stderr, "(%s) Lancement client impossible\n", type_personne);
        *fm = -1;
        return;
    }

    /* Creation de la cle */
    *cle = ftok(FICHIER_CLE, code);
    if (*cle == -1) {
	    fprintf(stderr, "(%s) Probleme creation cle\n", type_personne);
	    *fm = -1;
        return;
    }

    /* Recuperation file de message */
    *fm = msgget(*cle, 0);
    if (*fm == -1) {
	    fprintf(stderr, "(%s) Probleme recuperation file de message\n", type_personne);
	    *fm = -1;
        return;
    }
}

void deconnexion_fm(int fm) {
    msgctl(fm, IPC_RMID, NULL);
}




void fm_mecano_attend_requete(int fm, requete_mecano_t *req) {
    int res_rcv;

    /* attente de la requete */
    res_rcv = msgrcv(fm, req, sizeof(requete_mecano_t), 1, 0);
    if (res_rcv == -1) {
	    fprintf(stderr, "Erreur, numero %d\n", errno);
	    exit(EXIT_FAILURE);
    }
}

void fm_mecano_envoie_requete(int fm, long type_reponse, int duree, unsigned short *outils) {
    requete_mecano_t requete;

    /* creation de la requete */
    requete.type = REQUETE_TYPE_TAF;
    requete.type_reponse = type_reponse;
    requete.duree = duree;
    for (int i = 0; i < NB_OUTILS; i++) requete.outils[i] = outils[i];
    
    /* envoi de la requete */
    msgsnd(fm, &requete, sizeof(requete_mecano_t), 0);
}

void fm_mecano_attend_reponse(int fm, long type_reponse, reponse_mecano_t *rep) {
    /* attente de la reponse */
    if (msgrcv(fm, rep, sizeof(reponse_mecano_t), type_reponse, 0) == -1) {
	    fprintf(stderr, "Erreur reponse mecano, numero %d\n", errno);
	    exit(EXIT_FAILURE);
    }
}

void fm_mecano_envoie_reponse(int fm, long type) {
    reponse_mecano_t reponse;

    /* creation de la reponse */
    reponse.type = type;

    /* envoi de la reponse */
    msgsnd(fm, &reponse, sizeof(reponse), 0);
}







void fm_client_envoie_requete(int fm, pid_t pid_client) {
    requete_client_t requete;

    /* creation de la requete */
    requete.type = REQUETE_TYPE_TAF;
    requete.pid_client = pid_client;
    
    /* envoi de la requete */
    msgsnd(fm, &requete, sizeof(requete_client_t), 0);
}

void fm_client_attend_requete(int fm, requete_client_t *req) {
    int res_rcv;

    /* attente de la requete */
    res_rcv = msgrcv(fm, req, sizeof(requete_client_t), REQUETE_TYPE_TAF, 0);
    if (res_rcv == -1) {
	    fprintf(stderr, "Erreur, numero %d\n", errno);
	    exit(EXIT_FAILURE);
    }
}