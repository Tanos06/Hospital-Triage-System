#ifndef DEPARTMENTS_H_INCLUDED
#define DEPARTMENTS_H_INCLUDED

#include <stdbool.h>
#include "patient.h"

/**
 * @file departments.h
 * @brief Modulo AD per la gestione dei reparti e dei pazienti su file.
 *
 * Fornisce le operazioni di I/O su file ad accesso diretto per i
 * reparti ospedalieri e per la registrazione dei pazienti.
 * La struttura dati interna (i file fisici) è lo stato nascosto del modulo.
 */

/**
 * @brief Inizializza il file dei reparti se non esiste.
 */
void init_departments();

/**
 * @brief Carica i reparti dal file ad accesso diretto in memoria.
 */
int load_departments(int max_departments);

/**
 * @brief Restituisce il numero totale di pazienti registrati nel file.
 */
int get_total_patient();

/**
 * @brief Salva un nuovo paziente nel file e aggiorna il contatore letti del reparto.
 */
int save_patient(Patient newPatient);

/**
 * @brief Carica l'intero archivio storico dei pazienti dal disco.
 */
int load_all_patients(Patient* patientArray, int max_patients);

/**
 * @brief Restituisce il numero di reparti attualmente caricati in memoria.
 * * @return Numero di reparti.
 */
int get_loaded_count();

/**
 * @brief Restituisce l'ID di un reparto dato il suo indice nell'array.
 * * @param[in] i Indice del reparto.
 * @return ID del reparto.
 */
int get_department_id(int i);

/**
 * @brief Restituisce il nome di un reparto dato il suo indice nell'array.
 * * @param[in] i Indice del reparto.
 * @return Puntatore alla stringa del nome.
 */
const char* get_department_name(int i);


/**
 * @brief Restituisce il numero totale di posti letto di un reparto.
 * * @param[in] i Indice del reparto.
 * @return Numero di letti totali.
 */
int get_department_totalBeds(int i);

/**
 * @brief Restituisce il numero di posti letto attualmente occupati in un reparto.
 * * @param[in] i Indice del reparto.
 * @return Numero di letti occupati.
 */
int get_department_bedsOccupied(int i);

#endif
