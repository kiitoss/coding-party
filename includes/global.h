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

#include <time.h>

#define NB_OUTILS 4

struct pid_fils {
    pid_t pid;
    struct pid_fils *suivant;
} pid_fils;

#endif