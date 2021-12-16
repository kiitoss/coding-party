typedef struct {
    long type;
} requete_t;

typedef struct {
    long type;
    int ordre_exp;
} reponse_t;


int connexion_fm_mecano(char *type_travailleur);

void deconnexion_fm_mecano(int fm);

void envoie_requete(int fm, int ordre_exp);

reponse_t attend_reponse(int fm, int ordre_exp);