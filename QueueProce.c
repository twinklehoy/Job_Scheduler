/*Maria Ruxandra Radulescu*/
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "QueueProce.h"

/* Inizializza una coda vuota e con capienza $capacity.
IOP q Coda da inizializzare.
IP capacity Capacita' iniziale della coda.
*/
void queueProceInitWithCapacity(QueueProce* q, int capacity) {
    q->queue = malloc(sizeof(Processo*)*capacity);
    assert(q->queue != NULL);
    q->capacity = capacity;
    q->size = 0;
    q->first = 0;
}/* queueProceInit */

/* Inizializza una coda vuota e con capienza 1.
IOP q Coda da inizializzare.
*/
void queueProceInit(QueueProce* q) {
    queueProceInitWithCapacity(q, 1);
}/* queueProceInit */

/* Ritorna la capienza di una coda.
IP q Coda di cui si vuole conoscere la capienza.
OR La capienza della coda $q.
*/
int queueProceCapacity(const QueueProce* q) {
    return q->capacity;
}/* queueProceCapacity */

/* Ritorna la grandezza di una coda.
IP q Coda di cui si vuole conoscere la grandezza.
OR La grandezza della coda $q.
*/
int queueProceSize(const QueueProce* q) {
    return q->size;
}/* queueProceSize */

/* Indica se la coda e` vuota.
IP q Coda di cui si vuole sapere se e` vuota.
OR Indicazione se la coda e` vuota.
*/
bool queueProceIsEmpty(const QueueProce* q) {
    return q->size == 0;
}/* queueProceIsEmpty */

/* Raddoppia la capienza dell'array $queue quando questo e` pieno.
IOP q Coda di cui raddoppiare la capienza.
*/
void doubleIfFull(QueueProce* q) {
    if (q->size == q->capacity) {
        int i;
        Processo** newQueue = malloc(2 * q->capacity * sizeof(Processo*));
        for (i = 0; i < q->size; i++) {
            int index = (q->first + i) % q->capacity;
            newQueue[i] = q->queue[index];
        }
        q->capacity *= 2;
        q->first = 0;
        free(q->queue);
        q->queue = newQueue;
    }/* if */
}/* doubleIfFull */

/* Aggiunge un elemento in fondo alla coda.
IOP q Coda a cui aggiungere un elemento.
IP elem Elemento da aggiungere alla coda.
*/
void queueProceAddLast(QueueProce* q, Processo* processo) {
    int index;
    doubleIfFull(q);
    index = (q->first + q->size) % q->capacity;
    q->queue[index] = processo;
    q->size++;
}/* queueProceAddLast */

/* Indica il primo elemento inserito.
IP q Coda da cui leggere il primo elemento.
OP f Primo elemento di $q, se c'e`.
OR Indicazione se c'e` un elemento.
*/
bool queueProceGetFirst(const QueueProce* q, Processo** processo) {
    if (queueProceIsEmpty(q))
        return false;
    else {
        *processo = q->queue[q->first];
        return true;
    }/* if-else */
} /* queueProceGetFirst */

/* Indica l'elemento da piu` tempo all'interno della coda e lo rimuove.
IOP q QueueProce Coda da cui leggere il primo elemento.
OP f Primo elemento di $q, se c'e`.
OR Indicazione se c'e` un elemento.
*/
bool queueProceRemoveFirst(QueueProce* q, Processo** processo) {
    if (queueProceGetFirst(q, processo)) {
        q->first = (q->first + 1) % q->capacity;
        q->size--;
        return true;
    }
    return false;
}/* queueProceRemoveFirst */

/* Svuota la coda e libera la sua memoria allocata dinamicamente.
IOP q Coda da liberare.
*/
void queueProceFree(QueueProce* q) {
    free(q->queue);
    q->queue = NULL;
    q->capacity = 0;
    q->size = 0;
    q->first = 0;
}/* queueProceFree */

/* Funzione che carica un processo, aggiorna $instant e scrive sul file di output.
IOP queue Puntatore a QueueProce coda di processi.
IOP pp Puntatore a puntatore ad un processo. A differenza di run e di extract il processo
viene ritornato.
IP instant Istante di tempo in cui viene caricato un processo.
IP timeLoading Unita' di tempo che il processo impega a caricarsi.
OP fp File su cui scrivere.
OF Scrive dati sul singolo processo sul file $fp.
OR instant Istante di tempo aggiornato con il tempo di caricamento.
*/
int loadProcess(QueueProce* queue, Processo**pp, int instant, int timeLoading, FILE* fp){
    int timeWaited;
    Processo* p;
    printf("IDE con punto: %d", (**pp).ide);
    queueProceRemoveFirst(queue, pp);
    p = *pp;
    fprintf(fp, "Instant %d: ide %d loading\n", instant, p->ide);
    timeWaited = instant - p->end;
    p->total += timeWaited;
    p->max = MAX(p->max, timeWaited);
    instant+= timeLoading;
    return instant;
}

/*Funzione giusta secondo il prof*/
/*int loadProcess(QueueProce* queue, Processo**pp, int instant, int timeLoading, FILE* fp){
    int timeWaited;
    queueProceRemoveFirst(queue, pp);
    fprintf(fp, "Instant %d: ide %d loading\n", instant, (*pp)->ide);
    timeWaited = instant - (*pp)->end;
    (*pp)->total += timeWaited;
    (*pp)->max = MAX((*pp)->max, timeWaited);
    instant+= timeLoading;
    return instant;
}*/

/* Funzione che gestisce l'evoluzione di un processo, aggiorna $instant e scrive sul file di output.
IOP p Puntatore ad un processo.
IP queue Puntatore a QueueProce coda circolare di processi.
IP instant Istante di tempo in cui il processo inizia la sua evoluzione.
IP timeSlice Unita' di tempo massima per l'esecuzione di un processo. Se in coda esiste solo un processo,
esso ignora $timeSlice e continua la sua evoluzione fino alla fine.
OP fp File su cui scrivere.
OF Scrive dati sul singolo processo sul file $fp.
OR instant Istante di tempo aggiornato con il tempo di evoluzione.
*/
int runProcess(QueueProce* queue, Processo*p, int instant, int timeSlice, FILE* fp){
    int timeRunning;
    fprintf(fp, "Instant %d: ide %d running, required %d\n", instant, p->ide, p->timeLeft);
    if (queueProceSize(queue) == 0){
        timeRunning = p->timeLeft;
    }else{
        timeRunning = MIN(p->timeLeft, timeSlice);
    }
    p->timeLeft -= timeRunning;
    instant += timeRunning;
    return instant;
}/* runProcess */

/* Funzione che estrae un processo dalla coda se ha finito il suo tempo totale di esecuzione,
oppure lo rimette in coda in caso contrario.
IOP queue Puntatore a QueueProce coda circolare di processi.
IOP p Puntatore a Processo.
IP instant Istante di tempo in cui il processo viene estratto dalla coda.
OP fp File su cui scrivere.
OF Scrive dati sul singolo processo sul file $fp.
*/
void extractProcess(QueueProce* queue, Processo *p, int instant, FILE* fp){
    fprintf(fp, "Instant %d: ide %d extracted, required %d\n", instant, p->ide, p->timeLeft);
    if (p->timeLeft > 0) 
        queueProceAddLast(queue, p);
    p->end = instant;
}/* extractProcess */

/* Fino a quando la coda di processi non è vuota, carico, eseguo ed estraggo i processi.
IOP queue Puntatore a QueueProce coda circolare di processi.
IP timeLoading Unita' di tempo che il processo impega a caricarsi.
IP timeSlice Unita' di tempo massima per l'esecuzione di un processo.
OP fp File su cui scrivere.
OF Scrive dati sul singolo processo sul file $fp.
*/
void executeProcesses(QueueProce* queue, int timeLoading, int timeSlice, FILE* fp){
    int instant = 0;
    Processo* p;
    while(!queueProceIsEmpty(queue)){
        instant = loadProcess(queue, &p, instant, timeLoading, fp);
        instant = runProcess(queue, p, instant, timeSlice, fp);
        extractProcess(queue, p, instant, fp);
    }
}/* executeProcesses */