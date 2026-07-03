#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archive.h"

/**
 * @brief Nodo dell'albero binario di ricerca.
 *
 * Struttura privata del modulo archive.
 * Ogni nodo contiene un paziente e i puntatori
 * ai sottoalberi sinistro e destro.
 */
typedef struct treeNode {

    Patient data;              /**< Paziente memorizzato nel nodo. */
    struct treeNode* left;     /**< Puntatore al figlio sinistro. */
    struct treeNode* right;    /**< Puntatore al figlio destro. */

} treeNode;

/**
 * @brief Radice dell'archivio BST.
 *
 * Variabile privata del modulo. Rappresenta il punto
 * di accesso all'intero albero binario di ricerca.
 */
static treeNode* root;

/**
 * @brief Inizializza l'archivio dei pazienti.
 *
 * @pre Nessuna.
 * @post La radice dell'albero viene impostata a NULL.
 */
void initArchive() {

    root = NULL;

}

/**
 * @brief Inserisce ricorsivamente un paziente nel BST.
 *
 * @param[in] node Radice del sottoalbero corrente.
 * @param[in] p Paziente da inserire.
 *
 * @return Radice aggiornata del sottoalbero.
 *
 * @pre Il paziente deve contenere un codice fiscale valido.
 * @post Il paziente viene inserito nel corretto sottoalbero,
 *       se non è già presente.
 */
static treeNode* insertRecursive(treeNode* node,
                                 Patient p) {

    if(node == NULL) {

        treeNode* newNode =
            malloc(sizeof(treeNode));

        if(newNode != NULL) {

            newNode->data = p;
            newNode->left = NULL;
            newNode->right = NULL;
        }

        return newNode;
    }

    if(strcmp(p.taxCode, node->data.taxCode) < 0) {

        node->left = insertRecursive(node->left, p);

    }
    else if(strcmp(p.taxCode, node->data.taxCode) > 0) {

        node->right = insertRecursive(node->right, p);

    }
    else {
        //printf("Paziente gia' presente in archivio.\n");
    }

    return node;
}

/**
 * @brief Inserisce un paziente nell'archivio.
 *
 * @param[in] p Paziente da inserire.
 *
 * @pre Il paziente deve contenere dati validi.
 * @post L'archivio contiene il paziente inserito,
 *       se non era già presente.
 */
void insertPatient(Patient p) {
    root = insertRecursive(root, p);
}

/**
 * @brief Ricerca ricorsivamente un paziente nel BST.
 *
 * @param[in] node Radice del sottoalbero corrente.
 * @param[in] taxCode Codice fiscale da ricercare.
 *
 * @return Puntatore al nodo trovato oppure NULL.
 *
 * @pre taxCode deve essere una stringa valida.
 * @post L'albero non viene modificato.
 */
static treeNode* searchRecursive(treeNode* node,
                                 char taxCode[]) {

    if(node == NULL) {
        return NULL;
    }

    if(strcmp(taxCode, node->data.taxCode) == 0) {
        return node;
    }

    if(strcmp(taxCode, node->data.taxCode) < 0) {
        return searchRecursive(node->left, taxCode);
    }
    else {
        return searchRecursive(node->right, taxCode);
    }
}

/**
 * @brief Ricerca un paziente tramite codice fiscale.
 *
 * @param[in] taxCode Codice fiscale del paziente da cercare.
 *
 * @return Puntatore al paziente trovato oppure NULL.
 *
 * @pre taxCode deve essere una stringa valida.
 * @post L'archivio non viene modificato.
 */
Patient* searchPatient(char taxCode[]) {

    treeNode* found =
        searchRecursive(root, taxCode);

    if(found != NULL) {
        return &(found->data);
    }

    return NULL;
}

/**
 * @brief Esegue la visita inorder del BST.
 *
 * @param[in] node Radice del sottoalbero corrente.
 *
 * @pre Nessuna.
 * @post I pazienti del sottoalbero vengono stampati
 *       in ordine crescente di codice fiscale.
 */
static void inorderRecursive(treeNode* node) {

    if(node != NULL) {

        inorderRecursive(node->left);

        printf("Codice fiscale: %s\n",
               node->data.taxCode);

        printf("Nome: %s\n",
               node->data.name);

        printf("Cognome: %s\n",
               node->data.surname);

        printf("Priorita': %d\n\n",
               node->data.triage);

        inorderRecursive(node->right);
    }
}

/**
 * @brief Visualizza tutti i pazienti presenti nell'archivio.
 *
 * @pre L'archivio deve essere inizializzato.
 * @post I pazienti vengono stampati tramite visita inorder.
 */
void inorder() {

    inorderRecursive(root);

}

/**
 * @brief Funzione ricorsiva di supporto per la deallocazione dei nodi dell'albero.
 *
 * @param[in,out] node Radice del sottoalbero corrente da deallocare.
 */
static void destroyRecursive(treeNode* node){
    if(node!=NULL){
        destroyRecursive(node->left);
        destroyRecursive(node->right);
        free(node);
    }
}

/**
 * @brief Libera la memoria allocata dinamicamente per l'albero e resetta la radice.
 */
void destroyArchive(){
    destroyRecursive(root);
    root=NULL;
}
