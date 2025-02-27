/*Maria Ruxandra Radulescu*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Processo.h"

/*Macro che restituisce il minimo tra due elementi*/
#define MIN(a,b) (((a)<(b))?(a):(b))
/*Macro che restituisce il massimo tra due elementi*/
#define MAX(a,b) (((a)>(b))?(a):(b))

/* Coda di processi
~ queue Puntatore a puntatore a Processo.
~ size Grandezza della coda.
~ capacity Capacita' totale della coda.
~ first Primo elemento della coda.
*/
typedef struct {
    Processo** queue;
    int size;
    int capacity;
    int first;
} QueueProce;

void queueProceInitWithCapacity(QueueProce*, int);

void queueProceInit(QueueProce*);

int queueProceCapacity(const QueueProce*);

int queueProceSize(const QueueProce*);

bool queueProceIsEmpty(const QueueProce*);

void queueProceAddLast(QueueProce*, Processo*);

bool queueProceGetFirst(const QueueProce*, Processo**);

bool queueProceRemoveFirst(QueueProce* , Processo**);

void queueProceFree(QueueProce*);

void executeProcesses(QueueProce*, int, int, FILE*);

