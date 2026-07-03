#ifndef DEFENSIVE_H_INCLUDED
#define DEFENSIVE_H_INCLUDED
#include<stdbool.h>
#include<string.h>

/**
 * @file defensive.h
 * @brief Modulo per la validazione degli input e programmazione difensiva.
 */

/**
 * @brief Valida i parametri necessari per l'invio dell'email di dimissione.
 * * Controlla che le stringhe fornite in input non siano vuote.
 * * @param[in] patient_email   Email del paziente.
 * @param[in] patient_name    Nome del paziente.
 * @param[in] patient_surname Cognome del paziente.
 * @param[in] diagnosis       Diagnosi refertata.
 * * @return true se tutti i parametri sono validi, false se almeno uno è vuoto.
 */
bool validate_parameters(const char* patient_email, const char* patient_name, const char* patient_surname, const char* diagnosis);

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

#endif // DEFENSIVE_H_INCLUDED
