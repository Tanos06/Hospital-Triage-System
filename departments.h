#ifndef DEPARTMENTS_H_INCLUDED
#define DEPARTMENTS_H_INCLUDED

#include <stdbool.h>

/**
 * @file departments.h
 * @brief Modulo AD per la gestione dei reparti e dei pazienti su file.
 *
 * Fornisce le operazioni di I/O su file ad accesso diretto per i
 * reparti ospedalieri e per la registrazione dei pazienti.
 * La struttura dati interna (i file fisici) è lo stato nascosto del modulo.
 */

/**
 * @brief Rappresenta un paziente registrato in accettazione.
 *
 * Nota: il campo @p triage corrisponde al codice triage
 * (1=rosso, 5=bianco). Il campo @p assignedDeptId indica
 * il reparto di destinazione.
 */
typedef struct {
    char name[20];         /**< Nome del paziente. */
    char surname[20];      /**< Cognome del paziente. */
    char taxCode[17];      /**< Codice fiscale (chiave univoca). */
    int triage;            /**< Codice triage (1=rosso...5=bianco). */
    int assignedDeptId;    /**< ID del reparto assegnato. */
    char checkinTime[6];   /**< Orario di accettazione (HH:MM). */
} Patient;

/**
 * @brief Rappresenta un reparto ospedaliero con la disponibilità dei letti.
 */
typedef struct {
    int departmentId;        /**< Identificativo univoco del reparto. */
    char departmentName[21]; /**< Nome del reparto. */
    int totalBeds;           /**< Numero totale di letti nel reparto. */
    int bedsOccupied;        /**< Numero di letti attualmente occupati. */
} Department;

/**
 * @brief Inizializza il file dei reparti se non esiste.
 *
 * Crea il file "reparti.dat" con 5 reparti predefiniti se assente.
 *
 * @pre Nessuna.
 * @post Il file "reparti.dat" esiste sul filesystem.
 */
void init_departments();

/**
 * @brief Carica i reparti dal file ad accesso diretto in memoria.
 *
 * @param[out] dept_array      Array da popolare con i reparti letti.
 * @param[in]  max_departments Numero massimo di reparti da leggere.
 *
 * @return Numero di reparti effettivamente caricati, 0 in caso di errore.
 *
 * @pre Il file "reparti.dat" deve esistere.
 * @post dept_array contiene i dati dei reparti presenti nel file.
 */
int load_departments(Department *dept_array, int max_departments);

/**
 * @brief Restituisce il numero totale di pazienti registrati nel file.
 *
 * @return Numero di record Patient nel file "pazienti.dat",
 * 0 se il file non esiste o è vuoto.
 */
int get_total_patient();

/**
 * @brief Salva un nuovo paziente nel file e aggiorna il contatore letti del reparto.
 *
 * Usa fseek() per aggiornare direttamente il record del reparto
 * nel file ad accesso diretto, senza riscrivere l'intero file.
 *
 * @param[in] newPatient Puntatore al paziente da salvare.
 *
 * @return 1 in caso di successo, 0 in caso di errore.
 *
 * @pre newPatient deve contenere un assignedDeptId valido (1-5).
 * @post Il paziente è salvato in "pazienti.dat" e i letti del
 * reparto corrispondente in "reparti.dat" sono aggiornati.
 */
int save_patient(Patient *newPatient);

/**
 * @brief Valida sintatticamente gli input utente prima di creare il paziente.
 * * Implementa la programmazione difensiva bloccando stringhe vuote e
 * verificando che la priorità e il reparto appartengano alle classi
 * computazionali valide (1-5).
 * * @param[in] name     Nome in input.
 * @param[in] surname  Cognome in input.
 * @param[in] taxCode  Codice fiscale in input.
 * @param[in] triage   Stringa numerica priorità in input.
 * @param[in] deptId   Stringa numerica reparto in input.
 * * @return true se l'input è valido, false se contiene errori o classi invalide.
 */
bool validate_input_patient(const char* name, const char* surname, const char* taxCode, const char* triage, const char* deptId);

/**
 * @brief Carica l'intero archivio storico dei pazienti dal disco.
 * * Legge sequenzialmente il file binario e popola un array per ripristinare
 * in RAM (es. nell'Albero di Ricerca) lo stato all'avvio dell'applicazione.
 * * @param[out] patientArray Puntatore all'array di destinazione.
 * @param[in]  max_patients Numero massimo di pazienti consentiti nell'array.
 * * @return Numero di pazienti effettivamente caricati.
 */
int load_all_patients(Patient* patientArray, int max_patients);

#endif
