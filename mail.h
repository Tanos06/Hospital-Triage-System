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
 * Apre il file di testo indicato e lo allega ad un'email
 * spedita tramite SMTPS (smtp.gmail.com:465) all'indirizzo
 * del paziente. Il corpo del messaggio è un testo fisso di
 * notifica.
 *
 * @param[in] file_name     Percorso del file .txt del referto da allegare.
 * @param[in] patient_email Indirizzo email del destinatario.
 *
 * @return 1 in caso di invio riuscito, 0 in caso di errore.
 *
 * @pre file_name deve essere un percorso valido di un file esistente.
 * @pre patient_email deve essere un indirizzo email valido.
 * @post L'email con il referto allegato è recapitata al paziente.
 */
int send_discharge_email(const char *file_name, const char *patient_email);

#endif // MAIL_H_INCLUDED
