/*Maria Ruxandra Radulescu*/
#include <stdio.h>

/* Un processo in una coda circolare.
~ ide Identificatore del processo a partire da 0.
~ timeLeft Tempo rimasto al termine dell'esecuzione del processo.
~ end Istante di tempo in cui il processo conclude la sua evoluzione.
~ total Tempo totale di attesa (escluso il tempo di loading di se stesso).
~ max Tempo massimo di attesa (escluso il tempo di loading di se stesso).
*/
typedef struct {
    int ide;
    int timeLeft;
    int end;
    int total;
    int max;
} Processo;

void initProcesso(Processo*, int, int);
void printProcesso(Processo* , FILE* );