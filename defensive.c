#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "defensive.h"
bool validate_parameters(const char* patient_email, const char* patient_name, const char* patient_surname, const char* diagnosis){
    if(strlen(patient_email) == 0 || strlen(patient_name) == 0 || strlen(patient_surname) == 0 || strlen(diagnosis) == 0) return false;
    return true;
}

/**
 * @brief Implementazione del sistema di programmazione difensiva.
 */
bool validate_input_patient(const char* name, const char* surname, const char* taxCode, const char* triage, const char* deptId) {
    if(strlen(name) == 0 || strlen(surname) == 0 || strlen(taxCode) == 0 || strlen(triage) == 0 || strlen(deptId) == 0) return false;

    int t = atoi(triage);
    if(t < 1 || t > 5) return false;

    int d = atoi(deptId);
    if(d < 1 || d > 5) return false;

    return true;
}
