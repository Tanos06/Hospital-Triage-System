#include<stdio.h>
#include<stdlib.h>

#include <curl/curl.h>

#define EMAIL "gaetanobona84@gmail.com"
#define PASSWORD "mxoyrllyqnprpkkt"
int send_discharge_email(const char * file_name,
  const char * patient_email) {
    CURL * curl = curl_easy_init();
    if (curl) {
      char credential[256];
      struct curl_slist * recipients=NULL;
      recipients = curl_slist_append(NULL, EMAIL);
      recipients = curl_slist_append(recipients, patient_email);
      recipients = curl_slist_append(recipients, "Content-Type: text/plain");
      sprintf(credential, "%s:%s", EMAIL, PASSWORD);
      curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
      curl_easy_setopt(curl, CURLOPT_USERPWD, credential);
      curl_easy_setopt(curl, CURLOPT_MAIL_FROM, EMAIL);
      curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
      struct curl_slist *header = curl_slist_append(NULL, "Referto dimissioni");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
      curl_mime * mime = curl_mime_init(curl);
      curl_mimepart * part = curl_mime_addpart(mime);
      curl_mime_data(part, "Salve, questo in allegato e' il suo referto di dimissioni", CURL_ZERO_TERMINATED);
      curl_mime_type(part, "text/plain");

      curl_mimepart * attachment = curl_mime_addpart(mime);
      curl_mime_filedata(attachment, file_name);
      curl_mime_type(attachment, "application/octet-stream");
      curl_mime_filename(attachment, file_name);
      curl_mime_encoder(attachment, "base64");
      curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
      CURLcode ret_curl = curl_easy_perform(curl);
      if (ret_curl == CURLE_OK) {
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, & response_code);

        if (response_code != 250) {
          fprintf(stderr, "Mailer response code %ld error\n", response_code);
          return 0;
        } else {
          curl_off_t total_time;
          curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, & total_time);
          printf("Mailer response code %ld OK Transfert in %.3f s\n", response_code, total_time / 1000000.0);
        }
      } else {
        puts("Send mail failed");
        return 0;
      }
      curl_slist_free_all(recipients);
      curl_slist_free_all(header);
      curl_mime_free(mime);
      curl_easy_cleanup(curl);
      return 1;
    } else {
      printf("Errore\n");
      return 0;
    }
  }
