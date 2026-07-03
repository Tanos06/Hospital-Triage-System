#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <curl/curl.h>
#include<time.h>
#include "defensive.h"

/**
 * @file mail.c
 * @brief Implementazione del modulo AD per l'invio email del referto.
 *
 * Utilizza la libreria libcurl per la trasmissione SMTPS.
 * Le credenziali del mittente (indirizzo e app-password Gmail)
 * sono costanti private di questo modulo (information hiding).
 */

/** @brief Indirizzo email mittente (account ospedaliero). */
#define EMAIL "gaetanobona84@gmail.com"

/** @brief App-password Gmail per l'autenticazione SMTP. */
#define PASSWORD "udsbmpdctmmoskdg"

/** @brief Oggetto dell'email di dimissione. */
#define SUBJECT "Referto Dimissioni"

static char lastReportFilename[100]="";
static int emailSentCount=0;

/**
 * @brief Invia il referto di dimissione via email al paziente.
 * * Effettua prima la validazione, poi genera un file sequenziale ".txt" e
 * infine usa la libreria libcurl per il trasferimento sicuro con MIME attachment.
 */
int send_discharge_email(const char *patient_email, const char* patient_name, const char* patient_surname, const char* diagnosis) {
    if(!validate_parameters(patient_email, patient_name, patient_surname, diagnosis)){
        return 0;
    } else {
        char payload_text[512];
        char filename[100];
        sprintf(filename, "Referto_%s_%s.txt", patient_name, patient_surname);

        /* Creazione e scrittura Referto Medico */
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            fprintf(file, "=================================\n");
            fprintf(file, "    REFERTO MEDICO OSPEDALIERO   \n");
            fprintf(file, "=================================\n\n");
            fprintf(file, "Nome Paziente: %s %s\n", patient_name, patient_surname);
            fprintf(file, "Diagnosi e Cure: %s\n\n", diagnosis);

            time_t rawtime;
            struct tm * timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            fprintf(file, "Data: %02d/%02d/%d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
            fprintf(file, "Firma Medico: ___________________\n");
        } else {
            return 0; /**< ritorna 0 se c'è stato un errore riguardante la creazione del file */
        }
        fclose(file);

        /* Composizione Headers Email SMTPS */
        sprintf(payload_text, "From : %s\r\n" "To: %s\r\n" "Subject: %s\r\n", EMAIL, patient_email, SUBJECT);
        CURL *curl = curl_easy_init();
        if (curl) {
            char credential[256];
            struct curl_slist *recipients = NULL;
            struct curl_slist *headers = NULL;
            recipients = curl_slist_append(NULL, EMAIL);
            recipients = curl_slist_append(recipients, patient_email);
            headers = curl_slist_append(NULL, "Subject: Referto Dimissioni");

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            sprintf(credential, "%s:%s", EMAIL, PASSWORD);
            curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
            curl_easy_setopt(curl, CURLOPT_USERPWD, credential);
            curl_easy_setopt(curl, CURLOPT_MAIL_FROM, EMAIL);
            curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

            /* Costruzione del messaggio MIME multipart con testo e allegato */
            curl_mime *mime = curl_mime_init(curl);
            curl_mimepart *part = curl_mime_addpart(mime);
            curl_mime_data(part, "Salve, questo in allegato e' il suo referto di dimissioni", CURL_ZERO_TERMINATED);
            curl_mime_type(part, "text/plain");

            /* Allegato: file referto codificato in base64 */
            curl_mimepart *attachment = curl_mime_addpart(mime);
            curl_mime_filedata(attachment, filename);
            curl_mime_type(attachment, "application/octet-stream");
            curl_mime_filename(attachment, filename);
            curl_mime_encoder(attachment, "base64");

            /* Trasmissione in Rete */
            curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
            CURLcode ret_curl = curl_easy_perform(curl);
            if (ret_curl == CURLE_OK) {
                long response_code;
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
                if (response_code != 250) {
                    fprintf(stderr, "Mailer response code %ld error\n", response_code);
                    curl_slist_free_all(recipients);
                    curl_slist_free_all(headers);
                    curl_mime_free(mime);
                    curl_easy_cleanup(curl);
                    return 0;
                } else {
                    curl_off_t total_time;
                    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &total_time);
                    strcpy(lastReportFilename,filename);
                    emailSentCount++;
                    printf("Mailer response code %ld OK Transfert in %.3f s\n", response_code, total_time / 1000000.0);
                }
            } else {
                fprintf(stderr,"Send mail failed: %s\n",curl_easy_strerror(ret_curl));
                curl_slist_free_all(recipients);
                curl_slist_free_all(headers);
                curl_mime_free(mime);
                curl_easy_cleanup(curl);
                return 0;
            }

            /* Deallocazione Memoria cURL */
            curl_slist_free_all(recipients);
            curl_slist_free_all(headers);
            curl_mime_free(mime);
            curl_easy_cleanup(curl);
            return 1;

        } else {
            printf("Errore\n");
            return 0;
        }
    }
}
