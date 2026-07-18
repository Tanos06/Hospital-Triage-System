#include<stdlib.h>
#include<string.h>
#include "patient.h"

/**
 * @brief Struttura che rappresenta un paziente registrato nel sistema.
 */
struct Patient{
    char name[20];         /**< Nome del paziente. */
    char surname[20];      /**< Cognome del paziente. */
    char taxCode[17];      /**< Codice fiscale (chiave univoca). */
    int triage;            /**< Codice triage (1=rosso...5=bianco). */
    int assignedDeptId;    /**< ID del reparto assegnato. */
    char checkinTime[6];   /**< Orario di accettazione (HH:MM). */
};

Patient create_patient(const char* name, const char*surname, const char* taxCode, int triage, int assignedDeptId,const char* checkinTime){
    Patient newPatient=malloc(sizeof(struct Patient));
    if(newPatient==NULL) return NULL;
    setName(newPatient,name);
    setSurname(newPatient,surname);
    setTaxCode(newPatient,taxCode);
    setTriage(newPatient,triage);
    setassignedDeptId(newPatient,assignedDeptId);
    setCheckinTime(newPatient,checkinTime);
    return newPatient;
}
void destroy_patient(Patient p){
    free(p);
}
void setName(Patient p,const char* name){
    strcpy(p->name,name);
}
void setSurname(Patient p,const char* surname){
    strcpy(p->surname,surname);
}
void setTaxCode(Patient p,const char* taxCode){
    strcpy(p->taxCode,taxCode);
}
void setTriage(Patient p,int triage){
    p->triage=triage;
}
void setassignedDeptId(Patient p,int assignedDeptId){
    p->assignedDeptId=assignedDeptId;
}
void setCheckinTime(Patient p,const char* checkinTime){
    strcpy(p->checkinTime,checkinTime);
}
const char* getName(Patient p){
    return p->name;
}
const char* getSurname(Patient p){
    return p->surname;
}
const char* getTaxCode(Patient p){
    return p->taxCode;
}
int getTriage(Patient p){
    return p->triage;
}
int getAssignedDeptId(Patient p){
    return p->assignedDeptId;
}
const char* getCheckinTime(Patient p){
    return p->checkinTime;
}
int patient_write_to_file(Patient p, FILE* f) {
    if(p==NULL||f==NULL) return 0;
    return fwrite(p, sizeof(struct Patient), 1, f)==1;
}

Patient patient_read_from_file(FILE* f) {
    Patient p = malloc(sizeof(struct Patient));
    if (p == NULL) return NULL;
    if (fread(p, sizeof(struct Patient), 1, f) != 1) {
        free(p);
        return NULL;
    }
    return p;
}

int patient_record_size() {
    return sizeof(struct Patient);
}
