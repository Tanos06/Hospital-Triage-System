#include <stdio.h>
#include <stdlib.h>
#include "triage.h"

/**
 * @brief Inizializza la coda di triage.
 *
 * @pre q deve essere un puntatore valido.
 * @post La coda risulta vuota, quindi q->head vale NULL.
 */
void initQueue(PriorityQueue* q) {

    q->head = NULL;

}

/**
 * @brief Verifica se la coda è vuota.
 *
 * @pre q deve essere un puntatore valido.
 * @post La coda non viene modificata.
 */
int isEmptyQueue(PriorityQueue* q) {

    if(q->head == NULL) {
        return 1;
    }

    return 0;
}

/**
 * @brief Stampa tutti i pazienti presenti nella coda.
 *
 * @pre q deve essere un puntatore valido.
 * @post La coda non viene modificata.
 */
void printQueue(PriorityQueue* q) {

    QueueNode* temp = q->head;

    while(temp != NULL) {

        printf("Nome: %s\n", temp->data.name);

        printf("Codice triage: %d\n",
               temp->data.tcode);

        printf("\n");

        temp = temp->next;
    }
}

/**
 * @brief Inserisce un paziente nella coda rispettando il codice di triage.
 *
 * @pre q deve essere un puntatore valido.
 * @pre p deve contenere dati validi.
 * @post Il paziente viene inserito nella posizione corretta in base alla priorità.
 */
void enqueuePriority(PriorityQueue* q,
                     Patient p) {

    QueueNode* new =
        malloc(sizeof(QueueNode));

    if(new == NULL) {
        printf("Memoria non disponibile\n");
        return;
    }

    new->data = p;

    if(q->head == NULL ||
       p.tcode <
       q->head->data.tcode) {

        new->next = q->head;
        q->head = new;

        return;
    }

    QueueNode* temp = q->head;

    while(temp->next != NULL &&
          temp->next->data.tcode
          <= p.tcode) {

        temp = temp->next;
    }

    new->next = temp->next;

    temp->next = new;
}

/**
 * @brief Estrae il paziente con priorità maggiore dalla coda.
 *
 * @pre q deve essere un puntatore valido.
 * @post Se la coda non è vuota, il primo nodo viene rimosso e la memoria liberata.
 * @post Se la coda è vuota, viene stampato un messaggio di errore.
 */
Patient dequeue(PriorityQueue* q) {

    Patient p;

    if(q->head == NULL) {

        printf("Coda vuota\n");

        return p;
    }

    QueueNode* temp = q->head;

    p = temp->data;
    q->head = temp->next;
    free(temp);

    return p;
}
