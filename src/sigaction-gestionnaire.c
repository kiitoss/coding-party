#include "../includes/sigaction-gestionnaire.h"

void arret() {    
    fprintf(stdout,"Arret.\n");
    exit(EXIT_FAILURE);
}

void mon_sigaction(int signal, void (*f)(int)) {
    struct sigaction action;
    action.sa_handler = f;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(signal,&action,NULL);
}