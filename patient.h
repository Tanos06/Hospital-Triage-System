#ifndef PATIENT_H_INCLUDED
#define PATIENT_H_INCLUDED

typedef struct{
    char name[20];         /**< Nome del paziente. */
    char surname[20];      /**< Cognome del paziente. */
    char taxCode[17];      /**< Codice fiscale (chiave univoca). */
    int triage;            /**< Codice triage (1=rosso...5=bianco). */
    int assignedDeptId;    /**< ID del reparto assegnato. */
    char checkinTime[6];   /**< Orario di accettazione (HH:MM). */
}Patient;

#endif // PATIENT_H_INCLUDED
