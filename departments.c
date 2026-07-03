#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "departments.h"
#include "patient.h"

/**
 * @file departments.c
 * @brief Implementazione del modulo AD per la gestione dei reparti.
 *
 * Lo stato del modulo è persistito su file ad accesso diretto:
 * - "reparti.dat": database dei reparti e disponibilità letti.
 * - "pazienti.dat": registro dei pazienti accettati.
 */



/**
 * @brief Rappresenta un reparto ospedaliero con la disponibilità dei letti.
 */
typedef struct {
    int departmentId;        /**< Identificativo univoco del reparto. */
    char departmentName[21]; /**< Nome del reparto. */
    int totalBeds;           /**< Numero totale di letti nel reparto. */
    int bedsOccupied;        /**< Numero di letti attualmente occupati. */
} Department;

static Department deptState[10];
static int deptCount=0;
/**
 * @brief Inizializza il file dei reparti se non esiste.
 *
 * Se "reparti.dat" non è trovato, viene creato con 5 reparti
 * predefiniti: Cardiologia, Chirurgia Generale, Ortopedia,
 * Pediatria, Terapia Intensiva (20 letti ciascuno, 0 occupati).
 */
void init_departments() {
    FILE *checkFile = fopen("reparti.dat", "rb");
    if (checkFile == NULL) {
        /* Il file non esiste: creazione con reparti predefiniti */
        Department default_depts[5] = {
            {1, "Cardiologia", 20, 0},
            {2, "Chirurgia Generale", 20, 0},
            {3, "Ortopedia", 20, 0},
            {4, "Pediatria", 20, 0},
            {5, "Terapia Intensiva", 20, 0},
        };
        FILE *newFile = fopen("reparti.dat", "wb");
        if (newFile != NULL) {
            fwrite(&default_depts, sizeof(default_depts), 1, newFile);
            fclose(newFile);
        }
    } else {
        fclose(checkFile);
    }
}

/**
 * @brief Carica i reparti dal file ad accesso diretto in memoria.
 */
int load_departments(int max_departments) {
    FILE *file = fopen("reparti.dat", "rb");
    if (file == NULL) {
        return 0;
    }
    int count = fread(deptState, sizeof(Department), max_departments, file);
    fclose(file);
    return count;
}

/**
 * @brief Restituisce il numero totale di pazienti nel file.
 *
 * Calcola il numero di record dividendo la dimensione totale
 * del file per la dimensione di un singolo record Patient.
 */
int get_total_patient() {
    FILE *patientFile = fopen("pazienti.dat", "rb");
    if (patientFile != NULL) {
        long position = 0;
        fseek(patientFile, 0, SEEK_END);
        position = ftell(patientFile);
        int totalPatient = position / sizeof(Patient);
        return totalPatient;
    }
    return 0;
}

/**
 * @brief Salva un nuovo paziente e aggiorna il reparto assegnato.
 *
 * Usa fseek() con offset calcolato come:
 * (assignedDeptId - 1) * sizeof(Department)
 * per accedere direttamente al record del reparto corretto,
 * incrementando il contatore dei letti occupati.
 */
int save_patient(Patient *newPatient) {
    FILE *patientFile = fopen("pazienti.dat", "ab");
    if (patientFile != NULL) {
        /* newPatient è già un puntatore, non serve & */
        if (fwrite(newPatient, sizeof(Patient), 1, patientFile) == 0) {
            fclose(patientFile);
            return 0;
        }
        fclose(patientFile);
    } else {
        return 0;
    }
    /* Aggiornamento del reparto con accesso diretto tramite fseek */
    FILE *departmentFile = fopen("reparti.dat", "r+b");
    if (departmentFile != NULL) {
        Department departmentTemp;
        fseek(departmentFile, (newPatient->assignedDeptId - 1) * sizeof(Department), SEEK_SET);
        fread(&departmentTemp, sizeof(Department), 1, departmentFile);
        departmentTemp.bedsOccupied = departmentTemp.bedsOccupied + 1;
        fseek(departmentFile, (newPatient->assignedDeptId - 1) * sizeof(Department), SEEK_SET);
        if (fwrite(&departmentTemp, sizeof(Department), 1, departmentFile) == 0) {
            fclose(departmentFile);
            return 0;
        }
        fclose(departmentFile);
        return 1;
    } else {
        return 0;
    }
    return 0;
}



/**
 * @brief Recupera tutti i file binari AD in memoria sequenziale.
 */
int load_all_patients(Patient* patientArray, int max_patients) {
    FILE* file = fopen("pazienti.dat", "rb");
    if(file == NULL) {
        return 0;
    }
    int count = fread(patientArray, sizeof(Patient), max_patients, file);
    fclose(file);
    return count;
}

Patient *create_patient(const char* name, const char* surname,const char* taxCode, int triage,int assignedDeptId, const char* checkinTime){
    Patient *patient=(malloc(sizeof(Patient)));
    if(patient==NULL){return NULL;}
    strcpy(patient->name,name);
    strcpy(patient->surname,surname);
    strcpy(patient->taxCode,taxCode);
    patient->triage=triage;
    patient->assignedDeptId=assignedDeptId;
    strcpy(patient->checkinTime,checkinTime);
    return patient;
}

void destroy_patient(Patient *p){
    free(p);
}

int get_loaded_count(){
    return deptCount;
}
int get_department_id(int i){
    return deptState[i].departmentId;
}
const char* get_department_name(int i){
    return deptState[i].departmentName;
}
const char* patient_get_name(Patient *p){
    return p->name;
}
const char* patient_get_surname(Patient*p){
    return p->surname;
}
int get_department_totalBeds(int i){
    return deptState[i].totalBeds;
}
int get_department_bedsOccupied(int i){
    return deptState[i].bedsOccupied;
}
