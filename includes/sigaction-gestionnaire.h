#ifndef SIGACTION_GESTIONNAIRE_H
#define SIGACTION_GESTIONNAIRE_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void arret();

void mon_sigaction(int signal, void (*f)(int));

#endif