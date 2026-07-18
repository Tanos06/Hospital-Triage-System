#include <stdio.h>
#include <stdlib.h>
#include "triage.h"

/**
 * @brief Nodo della lista concatenata utilizzata per
 * implementare la coda di priorità.
 *
 * Struttura privata del modulo.
 */
typedef struct queueNode {

    Patient data;              /**< Paziente memorizzato nel nodo. */
    struct queueNode* next;    /**< Puntatore al nodo successivo. */

} QueueNode;

/**
 * @brief Struttura che rappresenta una coda di priorità.
 *
 * Struttura privata del modulo.
 */
struct triageQueue {

    QueueNode* head;           /**< Puntatore al primo nodo della coda. */

};

/**
 * @brief Crea e inizializza una nuova coda.
 *
 * @return Puntatore alla coda creata oppure NULL
 * se l'allocazione fallisce.
 *
 * @pre Nessuna.
 * @post Se l'allocazione ha successo, la coda è vuota.
 */
triageQueue initQueue() {

    triageQueue q = malloc(sizeof(struct triageQueue));

    if(q != NULL) {

        q->head = NULL;
    }

    return q;
}

/**
 * @brief Verifica se la coda è vuota.
 *
 * @param[in] q Coda da controllare.
 *
 * @return 1 se la coda è vuota, 0 altrimenti.
 *
 * @pre q deve essere una coda valida.
 * @post La coda non viene modificata.
 */
int isEmptyQueue(triageQueue q) {

    if(q == NULL || q->head == NULL) {
        return 1;
    }

    return 0;
}

/**
 * @brief Inserisce un paziente nella coda rispettando
 * l'ordinamento per priorità.
 *
 * @param[in,out] q Coda da modificare.
 * @param[in] p Paziente da inserire.
 *
 * @pre q deve essere una coda valida.
 * @post Il paziente viene inserito nella posizione
 * corretta secondo la priorità.
 */
void enqueuetriage(triageQueue q,
                     Patient p) {

    if(q == NULL) {
        return;
    }

    QueueNode* newNode =
        malloc(sizeof(QueueNode));

    if(newNode == NULL) {
        printf("Memoria non disponibile.\n");
        return;
    }

    newNode->data = p;

    if(q->head == NULL ||getTriage(p) < getTriage(q->head->data)) {
        newNode->next = q->head;
        q->head = newNode;
        return;
    }

    QueueNode* temp = q->head;

    while(temp->next != NULL && getTriage(temp->next->data)<=getTriage(p)) {
        temp = temp->next;
    }

    newNode->next = temp->next;
    temp->next = newNode;
}

/**
 * @brief Estrae il paziente in testa alla coda.
 *
 * @param[in,out] q Coda da modificare.
 *
 * @return Paziente estratto.
 *
 * @pre q deve essere una coda valida e non vuota.
 * @post Il primo nodo della coda viene rimosso e
 * la relativa memoria viene deallocata.
 */
Patient dequeue(triageQueue q) {

    Patient p = NULL;

    if(q == NULL || q->head == NULL) {
        printf("Coda vuota.\n");
        return p;
    }

    QueueNode* temp = q->head;
    p = temp->data;
    q->head = temp->next;

    free(temp);

    return p;
}

/**
 * @brief Visualizza il contenuto della coda.
 *
 * @param[in] q Coda da visualizzare.
 *
 * @pre q deve essere una coda valida.
 * @post La coda non viene modificata.
 */
void printQueue(triageQueue q) {

    if(q == NULL || q->head == NULL) {

        printf("Coda vuota.\n");
        return;
    }

    QueueNode* temp = q->head;

    while(temp != NULL) {

        printf("Nome: %s\n", getName(temp->data));
        printf("Cognome: %s\n", getSurname(temp->data));
        printf("Codice fiscale: %s\n", getTaxCode(temp->data));
        printf("Priorita': %d\n", getTriage(temp->data));
        printf("\n");
        temp = temp->next;
    }
}

/**
 * @brief Dealloca completamente la coda.
 *
 * Libera tutti i nodi ancora presenti e,
 * successivamente, la struttura triageQueue.
 *
 * @param[in,out] q Coda da distruggere.
 *
 * @pre Nessuna.
 * @post Tutta la memoria associata alla coda viene
 * deallocata.
 */
void destroyQueue(triageQueue q) {

    if(q == NULL) {
        return;
    }

    QueueNode* temp;

    while(q->head != NULL) {

        temp = q->head;
        q->head = q->head->next;

        free(temp);
    }

    free(q);
}
