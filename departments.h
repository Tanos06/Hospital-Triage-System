#ifndef DEPARTMENTS_H_INCLUDED
#define DEPARTMENTS_H_INCLUDED


typedef struct {
    char name[21];
    char surname[21];
    char taxCode[17];
    int priority;
    int assignedDeptId;
    char checkinTime[10];
} Patient;

// Struttura definita nelle specifiche del progetto
typedef struct {
    int departmentId;
    char departmentName[21];
    int totalBeds;
    int bedsOccupied;
} Department;

/*
 * Carica i reparti dal file ad accesso diretto.
 * Restituisce il numero di reparti effettivamente caricati.
 */
void init_departments();
int load_departments(Department *dept_array, int max_departments);
int get_total_patient();
int save_patient(Patient *newPatient);
#endif
