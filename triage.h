#ifndef TRIAGE_H_INCLUDED
#define TRIAGE_H_INCLUDED

/**
 * @file triage.h
 * @brief Modulo per la gestione della coda di triage.
 *
 * Il modulo definisce il tipo strutturato Patient e l'ADT
 * PriorityQueue, utilizzato per gestire i pazienti in attesa
 * secondo il codice di priorità del triage.
 */

/**
 * @brief Rappresenta un paziente del sistema di triage.
 */
typedef struct {

    char fcode[17]; /**< Codice fiscale del paziente. */
    char name[30];  /**< Nome del paziente. */
    int age;        /**< Età del paziente. */
    int tcode;      /**< Codice di triage associato al paziente. */

} Patient;

/**
 * @brief Nodo della coda di triage.
 *
 * Ogni nodo contiene un paziente e il puntatore
 * al nodo successivo.
 */
typedef struct queueNode {

    Patient data;              /**< Paziente memorizzato nel nodo. */
    struct queueNode* next;    /**< Puntatore al nodo successivo. */

} QueueNode;

/**
 * @brief Coda prioritaria dei pazienti.
 *
 * La coda contiene il puntatore al primo nodo.
 */
typedef struct {

    QueueNode* head; /**< Puntatore alla testa della coda. */

} PriorityQueue;

/**
 * @brief Inizializza una coda vuota.
 *
 * @param[in,out] q Puntatore alla coda da inizializzare.
 */
void initQueue(PriorityQueue* q);

/**
 * @brief Verifica se la coda è vuota.
 *
 * @param[in] q Puntatore alla coda da controllare.
 * @return 1 se la coda è vuota, 0 altrimenti.
 */
int isEmptyQueue(PriorityQueue* q);

/**
 * @brief Inserisce un paziente nella coda rispettando la priorità.
 *
 * @param[in,out] q Puntatore alla coda da modificare.
 * @param[in] p Paziente da inserire.
 */
void enqueuePriority(PriorityQueue* q, Patient p);

/**
 * @brief Estrae il paziente con priorità maggiore dalla coda.
 *
 * @param[in,out] q Puntatore alla coda da modificare.
 * @return Paziente rimosso dalla coda.
 */
Patient dequeue(PriorityQueue* q);

/**
 * @brief Stampa il contenuto della coda.
 *
 * @param[in] q Puntatore alla coda da visualizzare.
 */
void printQueue(PriorityQueue* q);

#endif // TRIAGE_H_INCLUDED
