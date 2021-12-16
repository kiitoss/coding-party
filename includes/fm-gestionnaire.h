#include "global.h"

#define FICHIER_CLE "cle.serv"
#define LETTRE_CODE_MECANO 'm'
#define LETTRE_CODE_CLIENT 'c'

typedef struct {
    long type;
    int duree;
    int outils[NB_OUTILS];
    int numero_client;
} requete_t;


void connexion_fm(char *type_personne, char code, key_t *cle, int *fm);

void deconnexion_fm(int fm);

void envoie_requete(int fm, int ordre_exp, int duree);

requete_t attend_reponse(int fm, int ordre_exp);