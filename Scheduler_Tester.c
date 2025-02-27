/*Maria Ruxandra Radulescu*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "QueueProce.h"

/* Funzione che stampa su un file le statistiche finali riguardanti:
~ La media, arrotondata all’intero per difetto, del tempo massimo di attesa di tutti i processi.
~ Il massimo del tempo massimo di attesa di tutti i processi.
~ La media, arrotondata all’intero per difetto, del tempo totale di attesa di tutti i processi. 
IP processi Puntatore alla lista di processi.
IP numProcessi Numero di processi.
OP fp FILE su cui stampare tabella e statistiche.
OF Stampa della tabella e statistiche sul file $fp.
*/
void printStats(Processo* processi, int numProcessi, FILE* fp){
    int maxAvg=0, maxMax=0, totalAvg = 0, i=0;
    fprintf(fp, "Ide    End   Max   Total\n");
    for (i = 0; i < numProcessi; i++) {
        printProcesso(&processi[i], fp);
        maxAvg+=processi[i].max;
        maxMax = MAX(maxMax, processi[i].max);
        totalAvg+=processi[i].total;
    }
    fprintf(fp, "Average of max: %d\n", maxAvg/numProcessi);
    fprintf(fp, "Max of max: %d\n", maxMax);
    fprintf(fp, "Average of total: %d\n", totalAvg/numProcessi);
}/* printStats */

/*Funzione che legge dati da un file e li elabora.
IP nomeFileIn Nome del file da elaborare.
OP numProcessi Numero di processi.
IOP processi Puntatore a puntatore ad un processo.
IOP queue Puntatore a QueueProce coda circolare di processi.
OR Esito
   0: elaborazione riuscita;
  -1: apertura fallita di $nomeFileIn;
*/
int loadInputFile(const char* nomeFileIn, int* numProcessi, Processo** processi, QueueProce* queue){
    FILE* inputFile;
    int i, tempo;
    if ((inputFile = fopen(nomeFileIn, "r")) == NULL) {
        return -1;
    }
    fscanf(inputFile, "%d", numProcessi);
    *processi = calloc(*numProcessi, sizeof(Processo));
    assert(processi != NULL);
    queueProceInitWithCapacity(queue, *numProcessi+1);
    for (i = 0; i < *numProcessi; i++) {
        fscanf(inputFile, "%d", &tempo);
        initProcesso(&(*processi)[i], i, tempo);
        queueProceAddLast(queue, &(*processi)[i]);
    }
    fclose(inputFile);
    return 0;
}/* loadInputFile */

/* Funzione che apre in modalita' di scrittura il file di output e chiama
le funzioni principali per la risoluzione del problema di scheduling.
Serve anche a mantenere il main() piu' leggibile.
IOP queue Puntatore a QueueProce coda circolare di processi.
IOP processi Puntatore a puntatore a struct Processo.
IP numProcessi Numero di processi che vengono messi nella coda.
IP timeLoading Unita' di tempo che il processo impega a caricarsi.
IP timeSlice Unita' di tempo massima per l'esecuzione di un processo.
IP nomeFileOut Nome del file di output su cui scrivere.
OF File $fp su cui vengono stampati i dati.
OR Esito
   0: elaborazione riuscita;
  -2: apertura fallita di $nomeFileOut;
*/
int runRoundRobin(QueueProce* queue, Processo** processi, int numProcessi, int timeLoading,
                     int timeSlice, const char* nomeFileOut){
    FILE* fp;
    if ((fp= fopen(nomeFileOut, "w")) == NULL) {
        printf("Errore nell'apertura del file di output.\n");
        return -2;
    }
    fprintf(fp, "timeLoading: %d timeSlice: %d\n", timeLoading, timeSlice);
    executeProcesses(queue, timeLoading, timeSlice, fp);
    printStats(*processi, numProcessi, fp);
    fclose(fp);
    return 0;
}/* runTest */

/*
IP argv[1] Nome del file da elaborare.
IP argv[2] Nome del file di output su cui andremo a scrivere.
IP argv[3] Valore di timeLoading, ovvero il tempo che il processo impega a caricarsi.
IP argv[4] Valore di timeSlice, ovvero il tempo massimo per l'esecuzione di un processo.
OR Esito  
    0: elaborazione riuscita;
   -1: apertura fallita di $nomeFileIn;
   -2: apertura fallita di $nomeFileOut;
   -3: argomenti mancanti;
*/
int main(int argc, char* argv[]) {
    int numProcessi, timeLoading, timeSlice;
    int esito=0;
    char *fileInput, *fileOutput;
    QueueProce* queue = malloc(sizeof(QueueProce));
    Processo *processi;
    if (argc < 5) {
        printf("Apertura fallita.\n");
        printf("Bisogna inserire: %s fileInput fileOutput timeLoading timeSlice\n", argv[0]);
        return -3;
    }
    fileInput = argv[1];
    fileOutput = argv[2];
    timeLoading = atoi(argv[3]);
    assert(timeLoading != 0);
    timeSlice = atoi(argv[4]);
    assert(timeSlice != 0);
    esito = loadInputFile(fileInput,&numProcessi, &processi,queue);
    if(esito==0){
        esito = runRoundRobin(queue, &processi, numProcessi, timeLoading, timeSlice, fileOutput);
    }
    free(processi);
    queueProceFree(queue);
    return esito;
}/* main */
