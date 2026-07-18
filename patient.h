#ifndef PATIENT_H_INCLUDED
#define PATIENT_H_INCLUDED
#include<stdio.h>

/**
 * @file patient.h
 * @brief Modulo per la definizione della struttura dati del Paziente.
 *
 * Contiene la definizione del tipo strutturato (typedef struct) utilizzato
 * per rappresentare un paziente all'interno dell'intero sistema di Triage.
 */

typedef struct Patient* Patient;

Patient create_patient(const char* name, const char*surname, const char* taxCode, int triage, int assignedDeptId,const char* checkinTime);
void destroy_patient(Patient p);
const char* getName(Patient p);
const char* getSurname(Patient p);
const char* getTaxCode(Patient p);
int getTriage(Patient p);
int getAssignedDeptId(Patient p);
const char* getCheckinTime(Patient p);
void setName(Patient p,const char* name);
void setSurname(Patient p,const char* surname);
void setTaxCode(Patient p,const char* taxCode);
void setTriage(Patient p,int triage);
void setassignedDeptId(Patient p,int assignedDeptId);
void setCheckinTime(Patient p,const char* checkinTime);
int patient_write_to_file(Patient p, FILE* f);
Patient patient_read_from_file(FILE* f);
int patient_record_size();

#endif // PATIENT_H_INCLUDED
