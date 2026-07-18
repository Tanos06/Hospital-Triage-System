#ifndef MAIL_H_INCLUDED
#define MAIL_H_INCLUDED

/**
 * @file mail.h
 * @brief Modulo AD per l'invio automatizzato del referto di dimissione via email.
 *
 * Mantiene uno stato locale (ultimo referto generato, conteggio invii)
 * aggiornato ad ogni invio riuscito.
 */

int send_discharge_email(const char *patient_email, const char* patient_name,
                         const char* patient_surname, const char* diagnosis);

/**
 * @brief Restituisce il numero totale di email inviate con successo.
 *
 * @return Contatore delle email inviate dall'avvio del programma.
 */
int get_emails_sent_count();

/**
 * @brief Restituisce il nome dell'ultimo file di referto generato.
 *
 * @return Stringa con il nome dell'ultimo referto, vuota se nessuno inviato.
 */
const char* get_last_report_filename();

#endif // MAIL_H_INCLUDED
