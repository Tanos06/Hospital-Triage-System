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
int save_patient(Patient *newPatient);

/**
 * @brief Carica l'intero archivio storico dei pazienti dal disco.
 */
int load_all_patients(Patient* patientArray, int max_patients);

/**
 * @brief Crea una nuova struttura Paziente allocandola dinamicamente in memoria.
 * * @param[in] name Nome del paziente.
 * @param[in] surname Cognome del paziente.
 * @param[in] taxCode Codice Fiscale.
 * @param[in] triage Codice di priorità.
 * @param[in] assignedDeptId ID del reparto.
 * @param[in] checkinTime Orario di check-in (formato stringa HH:MM).
 * @return Puntatore al nuovo paziente creato.
 */
Patient *create_patient(const char* name, const char* surname,const char* taxCode, int triage,int assignedDeptId, const char* checkinTime);

/**
 * @brief Dealloca un paziente creato dinamicamente.
 * * @param[in] p Puntatore al paziente da distruggere.
 */
void destroy_patient(Patient *p);

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
 * @brief Restituisce il nome di un paziente dato il puntatore alla struttura.
 * * @param[in] p Puntatore al paziente.
 * @return Puntatore alla stringa del nome.
 */
const char* patient_get_name(Patient *p);

/**
 * @brief Restituisce il cognome di un paziente dato il puntatore alla struttura.
 * * @param[in] p Puntatore al paziente.
 * @return Puntatore alla stringa del cognome.
 */
const char* patient_get_surname(Patient*p);

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
