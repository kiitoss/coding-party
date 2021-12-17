#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void arret();
void mon_sigaction(int signal, void (*f)(int));