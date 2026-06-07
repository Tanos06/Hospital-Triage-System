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

} TreeNode;

/**
 * @brief Radice dell'archivio BST.
 *
 * Variabile privata del modulo. Rappresenta il punto
 * di accesso all'intero albero binario di ricerca.
 */
static TreeNode* root;

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
static TreeNode* insertRecursive(TreeNode* node,
                                 Patient p) {

    if(node == NULL) {

        TreeNode* newNode =
            malloc(sizeof(TreeNode));

        if(newNode != NULL) {

            newNode->data = p;
            newNode->left = NULL;
            newNode->right = NULL;
        }

        return newNode;
    }

    if(strcmp(p.fcode, node->data.fcode) < 0) {

        node->left = insertRecursive(node->left, p);

    }
    else if(strcmp(p.fcode, node->data.fcode) > 0) {

        node->right = insertRecursive(node->right, p);

    }
    else {

        printf("Paziente gia' presente in archivio.\n");

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
 * @param[in] fcode Codice fiscale da ricercare.
 *
 * @return Puntatore al nodo trovato oppure NULL.
 *
 * @pre fcode deve essere una stringa valida.
 * @post L'albero non viene modificato.
 */
static TreeNode* searchRecursive(TreeNode* node,
                                 char fcode[]) {

    if(node == NULL) {
        return NULL;
    }

    if(strcmp(fcode, node->data.fcode) == 0) {
        return node;
    }

    if(strcmp(fcode, node->data.fcode) < 0) {
        return searchRecursive(node->left, fcode);
    }
    else {
        return searchRecursive(node->right, fcode);
    }
}

/**
 * @brief Ricerca un paziente tramite codice fiscale.
 *
 * @param[in] fcode Codice fiscale del paziente da cercare.
 *
 * @return Puntatore al paziente trovato oppure NULL.
 *
 * @pre fcode deve essere una stringa valida.
 * @post L'archivio non viene modificato.
 */
Patient* searchPatient(char fcode[]) {

    TreeNode* found =
        searchRecursive(root, fcode);

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
static void inorderRecursive(TreeNode* node) {

    if(node != NULL) {

        inorderRecursive(node->left);

        printf("Codice fiscale: %s\n",
               node->data.fcode);

        printf("Nome: %s\n",
               node->data.name);

        printf("Codice triage: %d\n\n",
               node->data.tcode);

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
