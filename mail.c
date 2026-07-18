#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include "defensive.h"
#include "mail.h"

#define EMAIL "gaetanobona84@gmail.com"
#define PASSWORD "mxoyrllyqnprpkkt"
#define SUBJECT "Referto Dimissioni"

/* Stato locale del modulo AD */
static char lastReportFilename[100] = "";
static int emailSentCount = 0;

int send_discharge_email(const char *patient_email, const char* patient_name,
                         const char* patient_surname, const char* diagnosis) {
    if(!validate_parameters(patient_email, patient_name, patient_surname, diagnosis)){
        return 0;
    }

    char payload_text[512];
    char filename[100];
    sprintf(filename, "Referto_%s_%s.txt", patient_name, patient_surname);

    FILE *file = fopen(filename, "w");
    if (!validate_file_open(file)) {   /* CLASSE 2: errore I/O */
        return 0;
    }

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
    fclose(file);

    sprintf(payload_text, "From : %s\r\n" "To: %s\r\n" "Subject: %s\r\n", EMAIL, patient_email, SUBJECT);
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

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

    curl_mime *mime = curl_mime_init(curl);
    curl_mimepart *part = curl_mime_addpart(mime);
    curl_mime_data(part, "Salve, questo in allegato e' il suo referto di dimissioni", CURL_ZERO_TERMINATED);
    curl_mime_type(part, "text/plain");

    curl_mimepart *attachment = curl_mime_addpart(mime);
    curl_mime_filedata(attachment, filename);
    curl_mime_type(attachment, "application/octet-stream");
    curl_mime_filename(attachment, filename);
    curl_mime_encoder(attachment, "base64");

    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    CURLcode ret_curl = curl_easy_perform(curl);
    int success = 0;
    if (ret_curl == CURLE_OK) {
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (response_code == 250) {
            /* Aggiornamento dello stato locale del modulo dopo invio riuscito */
            strcpy(lastReportFilename, filename);
            emailSentCount++;
            success = 1;
        }
    }

    curl_slist_free_all(recipients);
    curl_slist_free_all(headers);
    curl_mime_free(mime);
    curl_easy_cleanup(curl);

    return success;
}

int get_emails_sent_count() {
    return emailSentCount;
}

const char* get_last_report_filename() {
    return lastReportFilename;
}
