/**
 * @file main.c
 * @brief Entry point e gestore dell'interfaccia grafica (GUI) del sistema ospedaliero.
 *
 * Questo file funge da "Direttore d'Orchestra" (Controller): gestisce esclusivamente
 * il ciclo grafico tramite la libreria Raylib e delega tutta la logica di business
 * (salvataggi, code, code di rete, alberi binari) ai rispettivi moduli, rispettando
 * rigorosamente l'approccio AD/ADT.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <stdbool.h>
#include <time.h>
#include "chat.h"
#include "security.h"
#include "departments.h"
#include "mail.h"
#include "archive.h"
#include "triage.h"
#include "defensive.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Cattura gli errori critici (assert) delle librerie esterne.
 * * Sostituisce la gestione predefinita degli assert su Windows per evitare
 * crash silenziosi, stampando il file e la riga esatta che ha generato l'errore.
 * * @param[in] _Message Messaggio di errore generato dall'assert.
 * @param[in] _File    Percorso del file in cui si è verificato l'errore.
 * @param[in] _Line    Numero di riga in cui si è verificato l'errore.
 */
void __imp__wassert(const wchar_t* _Message, const wchar_t* _File, unsigned _Line) {
    printf("\n============================================\n");
    printf("[ERRORE CRITICO - ASSERT FALLITO]\n");
    printf("Messaggio: %ls\n", _Message);
    printf("File: %ls\n", _File);
    printf("Riga: %u\n", _Line);
    printf("============================================\n\n");
    exit(1);
}

#ifdef __cplusplus
}
#endif

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

/** @brief Lunghezza massima dei messaggi e dei buffer di input. */
#define MAX_LEN 512
/** @brief Chiave utilizzata per la cifratura di Vigenère nella chat. */
#define KEY "MEDICINA"
/** @brief Numero massimo di reparti ospedalieri gestibili. */
#define MAX_DEPARTMENTS 10

/**
 * @brief Funzione principale del programma.
 *
 * Inizializza l'ambiente grafico, carica i dati persistenti dal disco
 * all'interno delle strutture dati in RAM (Coda di Priorità e Albero Binario)
 * e gestisce il Game Loop dell'interfaccia utente basata su stati (appState).
 *
 * @return 0 al termine dell'esecuzione corretta.
 */
int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    int loaded;

    init_departments();
    initArchive();
    triageQueue waitingQueue = initQueue();

    /* CORREZIONE: Patient history[100] è già un array di puntatori,
       load_all_patients ora prende correttamente Patient* */
    Patient history[100];
    int historyCount = load_all_patients(history, 100);
    for(int i = 0; i < historyCount; i++) {
        insertPatient(history[i]);
        enqueuetriage(waitingQueue, history[i]);
        /* NOTA: nessun destroy_patient qui — la proprietà passa
           all'archivio, che libererà la memoria a fine programma */
    }

    InitWindow(screenWidth, screenHeight, "Pronto Soccorso - Triage & Chat");
    SetTargetFPS(60);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    int appState = 0;

    char chatInput[MAX_LEN] = "";
    bool chatEditMode = false;

    char nameInput[20] = "";
    bool nameEditMode = false;
    char surnameInput[20] = "";
    bool surnameEditMode = false;
    char taxCodeInput[17] = "";
    bool taxCodeEditMode = false;
    char triageInput[10] = "";
    bool triageEditMode = false;
    char deptInput[10] = "";
    bool deptEditMode = false;

    char refName[20] = "";
    bool refNameEdit = false;
    char refSurname[20] = "";
    bool refSurnameEdit = false;
    char refEmail[30] = "";
    bool refEmailEdit = false;
    char diagnosis[100] = "";
    bool diagnosisEdit = false;

    bool showEmailSuccess = false;
    bool showEmailError = false;
    bool showValidationError = false;
    bool showMaxPatientsError = false;
    bool showPatientAlreadyPresent = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (appState == 0) {
            DrawText("Scegli il tuo ruolo", 300, 70, 20, DARKGRAY);

            if (GuiButton((Rectangle){ 250, 130, 300, 50 }, "REPARTO (Server)")) {
                if (init_chat_server(5000) == 1) {
                    appState = 1;
                    _beginthread(server_receive_loop, 0, NULL);
                }
            }
            if (GuiButton((Rectangle){ 250, 210, 300, 50 }, "ACCETTAZIONE (Client)")) {
                if (connect_to_chat("127.0.0.1", 5000) == 1) {
                    appState = 2;
                    _beginthread(client_receive_loop, 0, NULL);
                }
            }
            if (GuiButton((Rectangle){ 250, 290, 300, 50 }, "ACCETTAZIONE PAZIENTI (Triage)")) {
                appState = 3;
            }
            if (GuiButton((Rectangle){ 250, 370, 300, 50 }, "VISUALIZZA REPARTI")) {
                appState = 5;
            }
            if (GuiButton((Rectangle){ 250, 450, 300, 50 }, "AREA MEDICO (Genera Referto)")) {
                appState = 4;
            }
        }
        else if (appState == 1 || appState == 2) {
            /* ... invariato ... */
        }
        else if (appState == 3) {
            DrawText("SCHEDA ACCETTAZIONE PAZIENTE", 10, 10, 20, DARKPURPLE);

            DrawText("Nome:", 50, 85, 20, DARKGRAY);
            if (GuiTextBox((Rectangle){ 250, 80, 300, 40 }, nameInput, 20, nameEditMode)) nameEditMode = !nameEditMode;
            DrawText("Cognome:", 50, 145, 20, DARKGRAY);
            if (GuiTextBox((Rectangle){ 250, 140, 300, 40 }, surnameInput, 20, surnameEditMode)) surnameEditMode = !surnameEditMode;
            DrawText("Codice Fiscale:", 50, 205, 20, DARKGRAY);
            if (GuiTextBox((Rectangle){ 250, 200, 300, 40 }, taxCodeInput, 17, taxCodeEditMode)) taxCodeEditMode = !taxCodeEditMode;
            DrawText("Priorità (1 a 5):", 50, 265, 20, DARKGRAY);
            if (GuiTextBox((Rectangle){ 250, 260, 300, 40 }, triageInput, 5, triageEditMode)) triageEditMode = !triageEditMode;
            DrawText("Reparto (1 a 5):", 50, 325, 20, DARKGRAY);
            if (GuiTextBox((Rectangle){ 250, 320, 300, 40 }, deptInput, 10, deptEditMode)) deptEditMode = !deptEditMode;

            if (GuiButton((Rectangle){ 250, 380, 300, 50 }, "SALVA PAZIENTE")) {
                int totalPatient = get_total_patient();

                if(validate_input_patient(nameInput, surnameInput, taxCodeInput, triageInput, deptInput) == false){
                    showValidationError = true;
                    showMaxPatientsError = false;
                    showPatientAlreadyPresent = false;
                } else if(totalPatient >= 100) {
                    showMaxPatientsError = true;
                    showValidationError = false;
                    showPatientAlreadyPresent = false;
                } else if(searchPatient(taxCodeInput)){
                    showPatientAlreadyPresent = true;
                    showMaxPatientsError = false;
                    showValidationError = false;
                } else {
                    showValidationError = false;
                    showMaxPatientsError = false;
                    showPatientAlreadyPresent = false;

                    int triageInt = atoi(triageInput);
                    int deptId = atoi(deptInput);
                    time_t rawtime;
                    time(&rawtime);
                    struct tm *timeinfo = localtime(&rawtime);
                    char checkinTime[6];
                    sprintf(checkinTime, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);

                    Patient newPatient = create_patient(nameInput, surnameInput, taxCodeInput,
                                                        triageInt, deptId, checkinTime);
                    if(newPatient != NULL) {
                        save_patient(newPatient);              /* CORREZIONE: Patient, non Patient* */
                        enqueuetriage(waitingQueue, newPatient); /* CORREZIONE: newPatient, non *newPatient */
                        insertPatient(newPatient);               /* CORREZIONE: newPatient, non *newPatient */
                        /* NIENTE destroy_patient qui: la memoria è ora
                           condivisa da coda e archivio — la libererà
                           l'archivio a fine programma (vedi archive.c) */
                    }
                }
            }

            if (showMaxPatientsError) {
                DrawText("ERRORE: Limite massimo di 100 pazienti raggiunto!", 150, 400, 20, RED);
            }
            if(showValidationError){
                DrawText("ERRORE: Campi inserita in maniera errata!", 150, 460, 20, RED);
            }
            if(showPatientAlreadyPresent){
                DrawText("ERRORE: Paziente gia' presente!", 150, 520, 20, RED);
            }

            if (GuiButton((Rectangle){ 10, 540, 150, 40 }, "INDIETRO")) {
                appState = 0;
                showMaxPatientsError = false;
                showValidationError = false;
                showPatientAlreadyPresent = false;
                nameInput[0] = '\0';
                surnameInput[0] = '\0';
                taxCodeInput[0] = '\0';
                triageInput[0] = '\0';
                deptInput[0] = '\0';
            }
        }
        else if (appState == 4) {
            DrawText("AREA MEDICO - DIMISSIONE E REFERTO", 10, 10, 20, DARKBLUE);

            /* Funzionalità di dominio che usa esplicitamente triageQueue */
            if(GuiButton((Rectangle){ 500, 80, 250, 50 }, "CHIAMA PROSSIMO")){
                if(!isEmptyQueue(waitingQueue)){
                    Patient nextPatient = dequeue(waitingQueue);
                    if(nextPatient != NULL) {
                        /* CORREZIONE: getName/getSurname (non patient_get_name),
                           niente più & perché Patient è già un puntatore */
                        strcpy(refName, getName(nextPatient));
                        strcpy(refSurname, getSurname(nextPatient));
                    }
                }
            }

            DrawText("Nome:", 50, 85, 20, DARKGRAY);
            if (GuiTextBox((Rectangle){ 200, 80, 200, 40 }, refName, 20, refNameEdit)) refNameEdit = !refNameEdit;
            DrawText("Cognome:", 50, 145, 20, DARKGRAY);
            if (GuiTextBox((Rectangle){ 200, 140, 200, 40 }, refSurname, 20, refSurnameEdit)) refSurnameEdit = !refSurnameEdit;
            DrawText("Email: ", 50, 205, 20, DARKGRAY);
            if(GuiTextBox((Rectangle){ 200, 200, 200, 40 }, refEmail, 30, refEmailEdit)) refEmailEdit = !refEmailEdit;
            DrawText("Diagnosi:", 50, 265, 20, DARKGRAY);
            if (GuiTextBox((Rectangle){ 200, 260, 500, 40 }, diagnosis, 100, diagnosisEdit)) diagnosisEdit = !diagnosisEdit;

            if (GuiButton((Rectangle){ 250, 320, 300, 50 }, "STAMPA REFERTO TXT")) {
                if(send_discharge_email(refEmail, refName, refSurname, diagnosis) == 1){
                    showEmailSuccess = true;
                    showEmailError = false;
                    refName[0] = '\0';
                    refSurname[0] = '\0';
                    diagnosis[0] = '\0';
                    appState = 0;
                } else {
                    showEmailSuccess = false;
                    showEmailError = true;
                }
            }
            if(showEmailSuccess){
                DrawText("Referto generato e inviato con successo!", 200, 370, 20, DARKGREEN);
                /* Mostra lo stato locale del modulo mail — dimostra che
                   mail ora ha più di una funzione pubblica */
                char statsInfo[150];
                sprintf(statsInfo, "Totale referti inviati: %d", get_emails_sent_count());
                DrawText(statsInfo, 200, 400, 16, DARKGRAY);
            }
            if(showEmailError){
                DrawText("Errore durante l'invio del referto!", 200, 370, 20, RED);
            }
            if (GuiButton((Rectangle){ 10, 540, 150, 40 }, "INDIETRO")) {
                showEmailError = false;
                showEmailSuccess = false;
                appState = 0;
            }
        }
        else if (appState == 5) {
            DrawText("ELENCO REPARTI E DISPONIBILITA' LETTI", 10, 10, 20, DARKBLUE);
            loaded = load_departments(MAX_DEPARTMENTS);
            for (int i = 0; i < loaded; i++) {
                char deptInfo[200];
                sprintf(deptInfo, "%d. %s - Letti Occupati: %d su %d",
                        get_department_id(i),
                        get_department_name(i),
                        get_department_bedsOccupied(i),
                        get_department_totalBeds(i));

                Color textColor = DARKGRAY;
                if (get_department_bedsOccupied(i) >= get_department_totalBeds(i)) {
                    strcat(deptInfo, "  [ REPARTO PIENO ]");
                    textColor = RED;
                } else {
                    strcat(deptInfo, "  [ POSTI DISPONIBILI ]");
                    textColor = DARKGREEN;
                }
                DrawText(deptInfo, 50, 80 + (i * 40), 20, textColor);
            }
            if (GuiButton((Rectangle){ 10, 540, 150, 40 }, "INDIETRO")) {
                appState = 0;
            }
        }

        EndDrawing();
    }

    close_listen_socket();
    CloseWindow();
    destroyQueue(waitingQueue);   /* libera solo i QueueNode, non i Patient */
    destroyArchive();             /* libera i TreeNode E i Patient (proprietario) */
    return 0;
}
