#ifndef FM_GESTIONNAIRE_H
#define GM_GESTIONNAIRE_H

#include "global.h"

#define FICHIER_CLE "cle.serv"
#define LETTRE_CODE_MECANO 'm'
#define LETTRE_CODE_CLIENT 'c'

#define REQUETE_TYPE_TAF 1

typedef struct {
    long type;
    long type_reponse;
    int duree;
    unsigned short outils[NB_OUTILS];
} requete_mecano_t;

typedef struct {
    long type;
} reponse_mecano_t;

typedef struct {
    long type;
    int ordre_chef;
    pid_t pid_client;
} requete_client_t;

void init_fm(char code, key_t *cle, int *fm);

void connexion_fm(char *type_personne, char code, key_t *cle, int *fm);

void deconnexion_fm(int fm);

void fm_mecano_envoie_requete(int fm, long type_reponse, int duree, unsigned short *outils);

void fm_mecano_envoie_reponse(int fm, long type);

void fm_mecano_attend_requete(int fm, requete_mecano_t *req);

void fm_mecano_attend_reponse(int fm, long type_reponse, reponse_mecano_t *rep);

void fm_client_envoie_requete(int fm, int ordre_chef, pid_t pid_client);

void fm_client_attend_requete(int fm, int ordre_chef, requete_client_t *req);

#endif