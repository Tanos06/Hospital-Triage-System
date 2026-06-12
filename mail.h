#ifndef MAIL_H_INCLUDED
#define MAIL_H_INCLUDED

/**
 * @file mail.h
 * @brief Modulo AD per l'invio automatizzato del referto di dimissione via email.
 *
 * Fornisce la funzione per l'apertura del referto sequenziale (.txt)
 * e il suo inoltro via protocollo SMTPS (Gmail) all'indirizzo
 * email del paziente al termine del trattamento in Pronto Soccorso.
 *
 * Dipendenza esterna: libcurl.
 */

/**
 * @brief Invia il referto di dimissione via email al paziente.
 *
 * Crea o sovrascrive un file di testo contenente il referto
 * e lo allega ad un'email spedita tramite SMTPS (smtp.gmail.com:465).
 *
 * @param[in] patient_email   Indirizzo email del destinatario.
 * @param[in] patient_name    Nome del paziente dimesso.
 * @param[in] patient_surname Cognome del paziente dimesso.
 * @param[in] diagnosis       Stringa testuale con diagnosi e cure.
 *
 * @return 1 in caso di invio riuscito, 0 in caso di errore logico o di rete.
 *
 * @pre I parametri di testo non devono essere stringhe vuote.
 * @post L'email con il referto allegato è recapitata al paziente.
 */
int send_discharge_email(const char *patient_email, const char* patient_name, const char* patient_surname, const char* diagnosis);

#endif // MAIL_H_INCLUDED
