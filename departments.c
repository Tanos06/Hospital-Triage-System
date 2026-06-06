#include <stdio.h>
#include <string.h>
#include "departments.h"

void init_departments(){
    FILE *checkFile = fopen("reparti.dat", "rb");
    if (checkFile == NULL) {
        // Il file non esiste! Il programma lo crea in automatico.
        Department default_depts[5] = {
            {1, "Cardiologia", 20, 0},
            {2, "Chirurgia Generale", 20, 0}, // Questo sarà rosso (pieno)
            {3, "Ortopedia", 20, 0},
            {4, "Pediatria", 20, 0},
            {5, "Terapia Intensiva", 20, 0}
        };
        FILE *newFile=fopen("reparti.dat","wb");
        if(newFile!=NULL){
            fwrite(&default_depts,sizeof(default_depts),1,newFile);
            fclose(newFile);
        }
    } else {
        fclose(checkFile);
    }
}
int load_departments(Department *dept_array, int max_departments) {

    FILE *file = fopen("reparti.dat", "rb");
    if (file == NULL) {

        return 0;
    }
    int count = fread(dept_array, sizeof(Department), max_departments, file);
    fclose(file);
    return count;
}
int get_total_patient(){
    FILE *patientFile=fopen("pazienti.dat","rb");
    if(patientFile!=NULL){
         long position=0;
        fseek(patientFile,0,SEEK_END);
        position=ftell(patientFile);
        int totalPatient=position/sizeof(Patient);
        return totalPatient;
    }
    return 0;
}
int save_patient(Patient *newPatient){
    FILE *patientFile=fopen("pazienti.dat","ab");
    if(patientFile!=NULL){
        //non c'è bisogno di usare & perchè la funzione già riceve un puntatore
        if(fwrite(newPatient,sizeof(Patient),1,patientFile)==0){
            fclose(patientFile);
            return 0;
        }
        fclose(patientFile);
    }else{
        return 0;
    }
    FILE *departmentFile=fopen("reparti.dat","r+b"); //apre il file in lettura e scrittura
    if(departmentFile!=NULL){
        Department departmentTemp;
        fseek(departmentFile,(newPatient->assignedDeptId-1)*sizeof(Department),SEEK_SET);
        fread(&departmentTemp,sizeof(Department),1,departmentFile);
        departmentTemp.bedsOccupied=departmentTemp.bedsOccupied+1;
        fseek(departmentFile,(newPatient->assignedDeptId-1)*sizeof(Department),SEEK_SET);
        if(fwrite(&departmentTemp,sizeof(Department),1,departmentFile)==0){
            fclose(departmentFile);
            return 0;
        }
        fclose(departmentFile);
        return 1;
    }else{
        return 0;
    }
    return 0;
}
