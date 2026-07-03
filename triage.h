#ifndef TRIAGE_H_INCLUDED
#define TRIAGE_H_INCLUDED
#include "patient.h"


/**
 * @file triage.h
 * @brief Modulo per la gestione della coda di triage.
 *
 * Il modulo definisce il tipo opaco triageQueue e le
 * operazioni per l'inizializzazione, l'inserimento,
 * l'estrazione, la stampa e la distruzione della coda.
 */

/**
 * @brief Tipo opaco che rappresenta una coda di priorità.
 */
typedef struct triageQueue* triageQueue;

/**
 * @brief Crea e inizializza una nuova coda.
 *
 * @return Coda inizializzata.
 */
triageQueue initQueue();

/**
 * @brief Verifica se la coda è vuota.
 *
 * @param[in] q Coda da controllare.
 *
 * @return 1 se la coda è vuota, 0 altrimenti.
 */
int isEmptyQueue(triageQueue q);

/**
 * @brief Inserisce un paziente nella coda rispettando
 * la priorità.
 *
 * @param[in,out] q Coda da modificare.
 * @param[in] p Paziente da inserire.
 */
void enqueuetriage(triageQueue q,Patient p);

/**
 * @brief Estrae il paziente in testa alla coda.
 *
 * @param[in,out] q Coda da modificare.
 *
 * @return Paziente estratto.
 */
Patient dequeue(triageQueue q);

/**
 * @brief Visualizza il contenuto della coda.
 *
 * @param[in] q Coda da visualizzare.
 */
void printQueue(triageQueue q);

/**
 * @brief Dealloca completamente la coda.
 *
 * Libera tutti i nodi ancora presenti e la struttura
 * dati associata alla coda.
 *
 * @param[in,out] q Coda da distruggere.
 */
void destroyQueue(triageQueue q);

#endif // TRIAGE_H_INCLUDED
