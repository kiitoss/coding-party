#include "global.h"

#define FICHIER_CLE "cle.serv"
#define LETTRE_CODE_MECANO 'm'
#define LETTRE_CODE_CLIENT 'c'

#define REQUETE_TYPE_TRAVAIL 1

typedef struct {
    long type;
    int duree;
    int outils[NB_OUTILS];
} requete_mecano_t;

typedef struct {
    long type;
    pid_t pid_client;
} requete_client_t;

void init_fm(char code, key_t *cle, int *fm);

void connexion_fm(char *type_personne, char code, key_t *cle, int *fm);

void deconnexion_fm(int fm);

void fm_mecano_envoie_requete(int fm, int ordre_exp, int duree);

requete_mecano_t fm_mecano_attend_reponse(int fm, int ordre_exp);

void fm_client_envoie_requete(int fm, int type, pid_t pid_client);

requete_client_t fm_client_attend_reponse(int fm, int msgtyp);