#ifndef ARCHIVE_H_INCLUDED
#define ARCHIVE_H_INCLUDED

#include "triage.h"

/**
 * @file archive.h
 * @brief Modulo per la gestione dell'archivio dei pazienti.
 *
 * Il modulo fornisce le operazioni di inizializzazione,
 * inserimento, ricerca e visualizzazione dei pazienti
 * memorizzati nell'archivio.
 */

/**
 * @brief Inizializza l'archivio dei pazienti.
 */
void initArchive();

/**
 * @brief Inserisce un paziente nell'archivio.
 *
 * @param[in] p Paziente da inserire.
 */
void insertPatient(Patient p);

/**
 * @brief Visualizza i pazienti presenti nell'archivio
 * mediante visita inorder.
 */
void inorder();

/**
 * @brief Ricerca un paziente tramite codice fiscale.
 *
 * @param[in] cf Codice fiscale del paziente da ricercare.
 *
 * @return Puntatore al paziente trovato oppure NULL.
 */
Patient* searchPatient(char cf[]);

#endif
