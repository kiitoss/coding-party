#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include <signal.h>
#include <sys/types.h>

#include <time.h>

#define NB_OUTILS 4

struct pid_fils {
    pid_t pid;
    struct pid_fils *suivant;
} pid_fils;

/* Couleurs dans xterm */
#define couleur(param) fprintf(stdout,"\033[%sm",param)

#define NOIR  "30"
#define ROUGE "31"
#define VERT  "32"
#define JAUNE "33"
#define BLEU  "34"
#define CYAN  "36"
#define BLANC "37"
#define REINIT "0"

#endif