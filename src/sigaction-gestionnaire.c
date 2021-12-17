#include "../includes/sigaction-gestionnaire.h"

void arret() {    
    exit(EXIT_FAILURE);
}

void mon_sigaction(int signal, void (*f)(int)) {
    struct sigaction action;
    action.sa_handler = f;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(signal,&action,NULL);
}

void desactive_signaux() {
    sigset_t masque;

    sigfillset(&masque);
    sigprocmask(SIG_SETMASK, &masque, NULL);

    sigdelset(&masque, SIGUSR1);
    sigprocmask(SIG_SETMASK, &masque, NULL);
}