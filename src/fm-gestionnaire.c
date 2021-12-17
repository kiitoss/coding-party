#include "../includes/global.h"
#include "../includes/fm-gestionnaire.h"

void init_fm(char code, key_t *cle, int *fm) {
    FILE *fich_cle;
    
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

    *cle = ftok(FICHIER_CLE, code);
    if (*cle == -1) {
	    fprintf(stderr, "(chef) Probleme creation cle\n");
	    exit(EXIT_FAILURE);
    }

    /* Creation file de message */
    *fm = msgget(*cle, IPC_CREAT | 0660);
    if (*fm == -1) {
	    fprintf(stderr, "Probleme creation file de message\n");
	    exit(EXIT_FAILURE);
    }
}

void connexion_fm(char *type_personne, char code, key_t *cle, int *fm) {
    FILE *fich_cle;

    /* On teste si le fichier cle existe dans le repertoire courant */
    fich_cle = fopen(FICHIER_CLE,"r");
    if (fich_cle == NULL) {
        fprintf(stderr, "(%s) Lancement client impossible\n", type_personne);
        exit(EXIT_FAILURE);
    }

    /* Creation de la cle */
    *cle = ftok(FICHIER_CLE, code);
    if (*cle == -1) {
	    fprintf(stderr, "(%s) Probleme creation cle\n", type_personne);
	    exit(EXIT_FAILURE);
    }

    /* Recuperation file de message */
    *fm = msgget(*cle, 0);
    if (*fm == -1) {
	    fprintf(stderr, "(%s) Probleme recuperation file de message\n", type_personne);
	    exit(EXIT_FAILURE);
    }
}

void deconnexion_fm(int fm) {
    msgctl(fm, IPC_RMID, NULL);
}


void fm_mecano_envoie_requete(int fm, int type_reponse, int duree, int outils[NB_OUTILS]) {
    requete_mecano_t requete;

    /* creation de la requete */
    requete.type = REQUETE_TYPE_TAF;
    requete.type_reponse = type_reponse;
    requete.duree = duree;
    for (int i = 0; i < NB_OUTILS; i++) requete.outils[i] = outils[i];
    
    /* envoi de la requete */
    msgsnd(fm, &requete, sizeof(requete_mecano_t), 0);
}

void fm_mecano_envoie_reponse(int fm, long type) {
    reponse_mecano_t reponse;

    /* creation de la reponse */
    reponse.type = type;
    
    /* envoi de la reponse */
    msgsnd(fm, &reponse, sizeof(reponse_mecano_t), 0);
}


requete_mecano_t fm_mecano_attend_requete(int fm) {
    requete_mecano_t requete;
    int res_rcv;

    /* attente de la requete */
    res_rcv = msgrcv(fm, &requete, sizeof(requete_mecano_t), REQUETE_TYPE_TAF, 0);
    if (res_rcv == -1) {
	    fprintf(stderr, "Erreur, numero %d\n", errno);
	    exit(EXIT_FAILURE);
    }

    return requete;
}

reponse_mecano_t fm_mecano_attend_reponse(int fm, long type) {
    reponse_mecano_t reponse;
    int res_rcv;

    /* attente de la reponse */
    res_rcv = msgrcv(fm, &reponse, sizeof(reponse_mecano_t), type, 0);
    if (res_rcv == -1) {
	    fprintf(stderr, "Erreur, numero %d\n", errno);
	    exit(EXIT_FAILURE);
    }

    return reponse;
}

void fm_client_envoie_requete(int fm, pid_t pid_client) {
    requete_client_t requete;

    /* creation de la requete */
    requete.type = REQUETE_TYPE_TAF;
    requete.pid_client = pid_client;
    
    /* envoi de la requete */
    msgsnd(fm, &requete, sizeof(requete_client_t), 0);
}

void fm_client_envoie_reponse(int fm, long type) {
    reponse_client_t reponse;

    /* creation de la reponse */
    reponse.type = type;
    
    /* envoi de la reponse */
    msgsnd(fm, &reponse, sizeof(reponse_client_t), 0);
}

requete_client_t fm_client_attend_requete(int fm) {
    requete_client_t requete;
    int res_rcv;

    /* attente de la requete */
    res_rcv = msgrcv(fm, &requete, sizeof(requete_client_t), REQUETE_TYPE_TAF, 0);
    if (res_rcv == -1) {
	    fprintf(stderr, "Erreur, numero %d\n", errno);
	    exit(EXIT_FAILURE);
    }

    return requete;
}

reponse_client_t fm_client_attend_reponse(int fm, int type_reponse) {
    reponse_client_t reponse;
    int res_rcv;

    /* attente de la reponse */
    res_rcv = msgrcv(fm, &reponse, sizeof(reponse_client_t), type_reponse, 0);
    if (res_rcv == -1) {
	    fprintf(stderr, "Erreur, numero %d\n", errno);
	    exit(EXIT_FAILURE);
    }

    return reponse;
}