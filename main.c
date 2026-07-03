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

    /* Inizializzazione moduli e strutture dati */
    init_departments();
    initArchive();
    triageQueue waitingQueue = initQueue();

    Patient history[100];
    int historyCount = load_all_patients(history, 100);
    for(int i = 0; i < historyCount; i++) {
        insertPatient(history[i]);
        enqueuetriage(waitingQueue, history[i]);
    }

    /* Inizializzazione Finestra Raylib */
    InitWindow(screenWidth, screenHeight, "Pronto Soccorso - Triage & Chat");
    SetTargetFPS(60);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    /* --- Variabili di Stato dell'Applicazione --- */
    int appState = 0; /**< 0: Menu, 1: Chat Server, 2: Chat Client, 3: Accettazione, 4: Medico, 5: Reparti */

    /* Variabili per input Chat */
    char chatInput[MAX_LEN] = "";
    bool chatEditMode = false;

    /* Variabili per input Accettazione Paziente */
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

    /* Variabili per input Area Medico (Referti) */
    char refName[20] = "";
    bool refNameEdit = false;
    char refSurname[20] = "";
    bool refSurnameEdit = false;
    char refEmail[30] = "";
    bool refEmailEdit = false;
    char diagnosis[100] = "";
    bool diagnosisEdit = false;

    /* Flag per la gestione degli errori e programmazione difensiva a schermo */
    bool showEmailSuccess = false;
    bool showEmailError = false;
    bool showValidationError = false;
    bool showMaxPatientsError = false;
    bool showPatientAlreadyPresent = false;

    /* Main Game Loop */
    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);

        /* ==========================================
         * STATO 0: MENU PRINCIPALE
         * ========================================== */
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
        /* ==========================================
         * STATO 1 & 2: CHAT CRITTOGRAFATA
         * ========================================== */
        else if (appState == 1 || appState == 2) {
            if (appState == 1) DrawText("CHAT - REPARTO (Server)", 10, 10, 20, BLUE);
            if (appState == 2) DrawText("CHAT - ACCETTAZIONE (Client)", 10, 10, 20, GREEN);

            bool isClientConnected = get_is_client_connected();
            if(isClientConnected) {
                DrawText("Client connesso. Chat attiva", 450, 10, 20, DARKGREEN);
            } else {
                DrawText("In attesa di connessione...", 450, 10, 20, ORANGE);
            }

            /* Stampa cronologia messaggi */
            int totalMessages = get_total_messages();
            for(int i = 0; i < totalMessages; i++){
                int y = 80 + (i * 30);
                if(get_message_sender(i) == 0){
                    DrawText(get_text_message(i), 650, y, 20, DARKBLUE);
                } else {
                    DrawText(get_text_message(i), 10, y, 20, DARKGREEN);
                }
            }

            if (GuiTextBox((Rectangle){ 10, 480, 650, 40 }, chatInput, MAX_LEN, chatEditMode)) {
                chatEditMode = !chatEditMode;
            }

            /* Invio del messaggio */
            if (GuiButton((Rectangle){ 670, 480, 100, 40 }, "INVIA") || (chatEditMode && IsKeyPressed(KEY_ENTER))) {
                if (strlen(chatInput) > 0) {
                    char message_to_send[MAX_LEN];
                    strcpy(message_to_send, chatInput);
                    if(totalMessages < 50){
                        add_message_to_history(message_to_send, 0);
                    }
                    encrypt_vigenere(message_to_send, KEY);
                    send_chat_message(message_to_send);

                    chatInput[0] = '\0';
                }
            }

            /* Uscita dalla chat e chiusura dei socket */
            if (GuiButton((Rectangle){ 10, 540, 150, 40 }, "INDIETRO")) {
                if(appState == 1){
                    close_listen_socket();
                    close_winsock();
                } else {
                    if(appState == 2){
                        close_connection_socket();
                        close_winsock();
                    }
                }
                appState = 0;
            }
        }
        /* ==========================================
         * STATO 3: ACCETTAZIONE PAZIENTI
         * ========================================== */
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
                int patientDepartments=atoi(deptInput)-1;
                /* Programmazione difensiva e controlli */
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
                } else if(get_department_bedsOccupied(patientDepartments)>=20){
                    showMaxPatientsError=true;
                    showValidationError=false;
                    showPatientAlreadyPresent=false;
                }else{
                    /* Dati validi: Costruzione e salvataggio */
                    showValidationError = false;
                    showMaxPatientsError = false;
                    showPatientAlreadyPresent = false;
                    int triageInt=atoi(triageInput);
                    int deptId=atoi(deptInput);
                    time_t rawtime;
                    time(&rawtime); // ottiene il tempo attuale in secondi
                    struct tm *timeinfo = localtime(&rawtime); // lo converte nel formato locale
                    char checkinTime[6];
                    sprintf(checkinTime, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
                    Patient *newPatient=create_patient(nameInput,surnameInput,taxCodeInput,triageInt,deptId,checkinTime);
                    save_patient(newPatient);
                    enqueuetriage(waitingQueue, *newPatient); // mette in coda in base alle priorità
                    insertPatient(*newPatient);
                    destroy_patient(newPatient);
                }
            }

            /* Stampe messaggi di errore (Difensiva) */
            if (showMaxPatientsError) {
                DrawText("ERRORE: Limite massimo di pazienti raggiunto!", 150, 400, 20, RED);
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
        /* ==========================================
         * STATO 4: AREA MEDICO (DIMISSIONE E EMAIL)
         * ========================================== */
        else if (appState == 4) {
            DrawText("AREA MEDICO - DIMISSIONE E REFERTO", 10, 10, 20, DARKBLUE);

            if(GuiButton((Rectangle){ 500, 80, 250, 50 }, "CHIAMA PROSSIMO")){
                if(!isEmptyQueue(waitingQueue)){
                    Patient nextPatient = dequeue(waitingQueue);
                    strcpy(refName, patient_get_name(&nextPatient));
                    strcpy(refSurname, patient_get_surname(&nextPatient));
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

            /* Generazione referto e invio Mail tramite modulo mail.c */
            if (GuiButton((Rectangle){ 250, 320, 300, 50 }, "STAMPA REFERTO TXT")) {
                if(send_discharge_email(refEmail, refName, refSurname, diagnosis) == 1){
                    showEmailSuccess = true;
                    showEmailError = false;
                    refName[0] = '\0';
                    refSurname[0] = '\0';
                    diagnosis[0] = '\0';
                    appState = 0;
                }else{
                    showEmailSuccess = false;
                    showEmailError = true;
                }
            }
            if(showEmailSuccess){
                DrawText("Referto generato e inviato con successo!", 200, 370, 20, DARKGREEN);
            }
            if(showEmailError){
                DrawText("Errore durante l'invio del referto!", 200, 370, 20, RED);
            }
            if (GuiButton((Rectangle){ 10, 540, 150, 40 }, "INDIETRO")) {
            showEmailError = false;
            showEmailSuccess = false;
            appState = 0;
            }
        }/* <--- FINE DELLO STATO 4 */

        /* ==========================================
         * STATO 5: VISUALIZZA REPARTI
         * ========================================== */
        else if (appState == 5) {
            DrawText("ELENCO REPARTI E DISPONIBILITA' LETTI", 10, 10, 20, DARKBLUE);
            loaded = load_departments(MAX_DEPARTMENTS);
            for (int i = 0; i < loaded; i++) {
                char deptInfo[200];
                sprintf(deptInfo, "%d. %s - Letti Occupati: %d su %d",
                        get_department_id(i),
                        get_department_name(i),
                        get_department_bedsOccupied(i),
                        get_department_totalBeds(i)
                        );

                Color textColor = DARKGRAY;
                if (get_department_bedsOccupied(i)>=get_department_totalBeds(i)) {
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

    /* Pulizia e rilascio memoria */
    close_listen_socket();
    CloseWindow();
    destroyQueue(waitingQueue); // Libera la memoria della coda prima di uscire
    destroyArchive();
    return 0;
}
