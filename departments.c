#include <stdio.h>
#include <string.h>
#include "departments.h"

/**
 * @file departments.c
 * @brief Implementazione del modulo AD per la gestione dei reparti.
 *
 * Lo stato del modulo è persistito su file ad accesso diretto:
 * - "reparti.dat": database dei reparti e disponibilità letti.
 * - "pazienti.dat": registro dei pazienti accettati.
 */

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
            {5, "Terapia Intensiva", 20, 0}
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
int load_departments(Department *dept_array, int max_departments) {
    FILE *file = fopen("reparti.dat", "rb");
    if (file == NULL) {
        return 0;
    }
    int count = fread(dept_array, sizeof(Department), max_departments, file);
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
