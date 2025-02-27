/*Maria Ruxandra Radulescu*/
#include "Processo.h"
#include <stdio.h>

/* Funzione che inizializza un processo.
IOP processo, ide, time.
OP end, total, max.
*/
void initProcesso(Processo* processo, int ide, int time) {
    processo->ide = ide;
    processo->timeLeft = time;
    processo->end = 0;
    processo->total = 0;
    processo->max = 0;
}

/* Funzione che stampa i su un file $ide, $end, $max e $total nel formato richiesto
dalla consegna.
IP p Puntatore ad un processo.
OP fp File su cui scrivere.
OF Scrive dati della tabella su $fp.
*/
void printProcesso(Processo* p, FILE* fp) {
    fprintf(fp, "%2d %7d %5d %7d\n", p->ide, p->end, p->max, p->total);
}/* printProcesso */