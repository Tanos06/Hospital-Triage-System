#ifndef DEFENSIVE_H_INCLUDED
#define DEFENSIVE_H_INCLUDED
#include <stdbool.h>
#include <stdio.h>

/**
 * @file defensive.h
 * @brief Modulo dedicato alla programmazione difensiva del sistema.
 *
 * Copre due classi computazionali distinte:
 * - CLASSE 1: validazione degli input forniti da tastiera (stringhe
 *   vuote, valori numerici fuori range).
 * - CLASSE 2: validazione delle operazioni di I/O su file (apertura
 *   fallita, stream non valido).
 */

/**
 * @brief CLASSE 1 — Valida i dati di un paziente inseriti da tastiera.
 *
 * @param[in] name     Nome in input.
 * @param[in] surname  Cognome in input.
 * @param[in] taxCode  Codice fiscale in input.
 * @param[in] triage   Stringa numerica priorità (1-5).
 * @param[in] deptId   Stringa numerica reparto (1-5).
 *
 * @return true se l'input è valido, false altrimenti.
 */
bool validate_input_patient(const char* name, const char* surname,
                            const char* taxCode, const char* triage,
                            const char* deptId);

/**
 * @brief CLASSE 1 — Valida i dati per l'invio del referto via email.
 *
 * @param[in] patient_email   Email del paziente.
 * @param[in] patient_name    Nome del paziente.
 * @param[in] patient_surname Cognome del paziente.
 * @param[in] diagnosis       Diagnosi da refertare.
 *
 * @return true se tutti i campi sono validi, false altrimenti.
 */
bool validate_parameters(const char* patient_email, const char* patient_name,
                         const char* patient_surname, const char* diagnosis);

/**
 * @brief CLASSE 2 — Verifica che un file sia stato aperto correttamente.
 *
 * Copre la classe computazionale degli errori di I/O: file inesistente,
 * permessi insufficienti, disco pieno o altro errore di apertura.
 *
 * @param[in] f Puntatore FILE restituito da fopen().
 *
 * @return true se il file è stato aperto correttamente, false altrimenti.
 */
bool validate_file_open(FILE* f);

#endif // DEFENSIVE_H_INCLUDED
