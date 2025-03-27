#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>

union semun
{
    int val;
};

void sem_init();
void sem_p(int index);
void sem_v(int index);
void sem_destroy();

