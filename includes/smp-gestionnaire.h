#ifndef SMP_GESTIONNAIRE_H
#define SMP_GESTIONNAIRE_H

#include <sys/shm.h>

#include "global.h"
#include "semap-gestionnaire.h"


void init_smp(key_t cle, int *tab, int *mem_part);

void deconnexion_smp(int *tab, int mem_part);

#endif