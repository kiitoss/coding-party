#include "global.h"

typedef struct {
    long type;
    int duree;
    int outils[NB_OUTILS];
} requete_t;


int connexion_fm_mecano(char *type_travailleur);

void deconnexion_fm_mecano(int fm);

void envoie_requete(int fm, int ordre_exp, int duree);

requete_t attend_reponse(int fm, int ordre_exp);