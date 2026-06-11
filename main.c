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

#ifdef __cplusplus
extern "C" {
#endif

void __imp__wassert(const wchar_t* _Message, const wchar_t* _File, unsigned _Line) {
    // 1. Stampiamo l'errore reale nella console.
    // Usiamo %ls invece di %s perché i parametri sono wchar_t (Wide Characters)
    printf("\n============================================\n");
    printf("[ERRORE CRITICO - ASSERT FALLITO]\n");
    printf("Messaggio: %ls\n", _Message);
    printf("File: %ls\n", _File);
    printf("Riga: %u\n", _Line);
    printf("============================================\n\n");

    // 2. Terminiamo il programma con codice di errore 1 (anomalia)
    exit(1);
}

#ifdef __cplusplus
}
#endif

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define MAX_LEN 512
#define KEY "MEDICINA"
#define MAX_DEPARTMENTS 10


int main() {
	const int screenWidth = 800;
	const int screenHeight = 600;
    Department departmentsList[MAX_DEPARTMENTS];
    int loaded;
	init_departments();

	initArchive(); //inizializza l'albero binario
	triageQueue waitingQueue=initQueue();//Crea la coda di priorità

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
	char deptInput[10]="";
	bool deptEditMode=false;

	char refName[20] = "";
	bool refNameEdit = false;
	char refSurname[20] = "";
	bool refSurnameEdit = false;
	char diagnosis[100] = "";
	bool diagnosisEdit = false;

	bool showMaxPatientsError = false;

	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(RAYWHITE);

		if (appState == 0) {
			DrawText("Scegli il tuo ruolo", 300, 70, 20, DARKGRAY);

			if (GuiButton((Rectangle) {
			250, 130, 300, 50
		}, "REPARTO (Server)")) {
				if (init_chat_server(5000) == 1) {
					appState = 1;
                    _beginthread(server_receive_loop, 0, NULL);
				}
			}

			if (GuiButton((Rectangle) {
			250, 210, 300, 50
		}, "ACCETTAZIONE (Client)")) {
				if (connect_to_chat("127.0.0.1", 5000) == 1) {
					appState = 2;
					_beginthread(client_receive_loop, 0, NULL);
				}
			}

			if (GuiButton((Rectangle) {
			250, 290, 300, 50
		}, "ACCETTAZIONE PAZIENTI (Triage)")) {
				appState = 3;
			}

			if (GuiButton((Rectangle) {
			250, 370, 300, 50
		}, "VISUALIZZA REPARTI")) {
				appState = 5;
			}

			if (GuiButton((Rectangle) {
			250, 450, 300, 50
		}, "AREA MEDICO (Genera Referto)")) {
				appState = 4;
			}
		}
		else if (appState == 1 || appState == 2) {
			if (appState == 1) DrawText("CHAT - REPARTO (Server)", 10, 10, 20, BLUE);
			if (appState == 2) DrawText("CHAT - ACCETTAZIONE (Client)", 10, 10, 20, GREEN);
            bool isClientConnected=get_is_client_connected();
            if(isClientConnected) {
                DrawText("Client connesso. Chat attiva", 450, 10, 20, DARKGREEN);
            } else {
                DrawText("In attesa di connessione...", 450, 10, 20, ORANGE);
            }
            int totalMessages=get_total_messages();
            for(int i=0;i<totalMessages;i++){
                int y=80+(i*30);
                if(get_message_sender(i)==0){
                    DrawText(get_text_message(i),650,y,20,DARKBLUE);
                }else{
                    DrawText(get_text_message(i),10,y,20,DARKGREEN);
                }
            }

			if (GuiTextBox((Rectangle) {
			10, 480, 650, 40
		}, chatInput, MAX_LEN, chatEditMode)) {
				chatEditMode = !chatEditMode;
			}

			if (GuiButton((Rectangle) {
			670, 480, 100, 40
		}, "INVIA") || (chatEditMode && IsKeyPressed(KEY_ENTER))) {
				if (strlen(chatInput) > 0) {
					char message_to_send[MAX_LEN];
					strcpy(message_to_send, chatInput);
                    if(totalMessages<50){
                        add_message_to_history(message_to_send,0);
					}
					encrypt_vigenere(message_to_send, KEY);
					send_chat_message(message_to_send);

					chatInput[0] = '\0';
				}
			}

			if (GuiButton((Rectangle) {
			10, 540, 150, 40
		}, "INDIETRO")) {
		    if(appState==1){
                close_listen_socket();
                close_winsock();
		    }else{
                if(appState==2){
                    close_connection_socket();
                    close_winsock();
                }
		    }
				appState = 0;
			}
		}
		else if (appState == 3) {
			DrawText("SCHEDA ACCETTAZIONE PAZIENTE", 10, 10, 20, DARKPURPLE);

			DrawText("Nome:", 50, 85, 20, DARKGRAY);
			if (GuiTextBox((Rectangle) {
			250, 80, 300, 40
		}, nameInput, 20, nameEditMode)) nameEditMode = !nameEditMode;

			DrawText("Cognome:", 50, 145, 20, DARKGRAY);
			if (GuiTextBox((Rectangle) {
			250, 140, 300, 40
		}, surnameInput, 20, surnameEditMode)) surnameEditMode = !surnameEditMode;

			DrawText("Codice Fiscale:", 50, 205, 20, DARKGRAY);
			if (GuiTextBox((Rectangle) {
			250, 200, 300, 40
		}, taxCodeInput, 17, taxCodeEditMode)) taxCodeEditMode = !taxCodeEditMode;

			DrawText("Priorità (1 a 5):", 50, 265, 20, DARKGRAY);
			if (GuiTextBox((Rectangle) {
			250, 260, 300, 40
		}, triageInput, 5, triageEditMode)) triageEditMode = !triageEditMode;
            DrawText("Reparto (1 a 5):", 50, 325, 20, DARKGRAY);
            if (GuiTextBox((Rectangle) {
			250, 320, 300, 40
		}, deptInput, 10, deptEditMode)) deptEditMode = !deptEditMode;

			if (GuiButton((Rectangle) {
			250, 380, 300, 50
		}, "SALVA PAZIENTE")) {
				int totalPatient=get_total_patient();
				if(totalPatient>=100) {
					showMaxPatientsError=true;
				} else {
					Patient newPatient;
					strcpy(newPatient.name,nameInput);
					strcpy(newPatient.surname,surnameInput);
					strcpy(newPatient.taxCode,taxCodeInput);
					int triage=atoi(triageInput);
					newPatient.triage=triage;
					time_t rawtime;
					time(&rawtime);//ottiene il tempo attuale in secondi
					struct tm *timeinfo=localtime(&rawtime);//lo converte nel formato locale
					sprintf(newPatient.checkinTime,"%02d:%02d",timeinfo->tm_hour,timeinfo->tm_min);
					int deptId=atoi(deptInput);
					newPatient.assignedDeptId=deptId;
					save_patient(&newPatient);
					enqueuetriage(waitingQueue,newPatient); //mette in coda in base alle priorità
					insertPatient(newPatient);//Salva nell'albero binario storico
				}
			}

			if (showMaxPatientsError) {
				DrawText("ERRORE: Limite massimo di 100 pazienti raggiunto!", 150, 400, 20, RED);
			}

			if (GuiButton((Rectangle) {
			10, 540, 150, 40
		}, "INDIETRO")) {
				appState = 0;
				showMaxPatientsError = false;

				nameInput[0] = '\0';
				surnameInput[0] = '\0';
				taxCodeInput[0] = '\0';
				triageInput[0] = '\0';
			}
		}
		else if (appState == 4) {
			DrawText("AREA MEDICO - DIMISSIONE E REFERTO", 10, 10, 20, DARKBLUE);
			if(GuiButton((Rectangle){500,80,250,50},"CHIAMA PROSSIMO")){
                if(!isEmptyQueue(waitingQueue)){
                    Patient nextPatient=dequeue(waitingQueue);
                    strcpy(refName,nextPatient.name);
                    strcpy(refSurname,nextPatient.surname);
                }
			}

			DrawText("Nome:", 50, 85, 20, DARKGRAY);
			if (GuiTextBox((Rectangle) {
			200, 80, 200, 40
		}, refName, 20, refNameEdit)) refNameEdit = !refNameEdit;

			DrawText("Cognome:", 50, 145, 20, DARKGRAY);
			if (GuiTextBox((Rectangle) {
			200, 140, 200, 40
		}, refSurname, 20, refSurnameEdit)) refSurnameEdit = !refSurnameEdit;

			DrawText("Diagnosi:", 50, 205, 20, DARKGRAY);
			if (GuiTextBox((Rectangle) {
			200, 200, 500, 40
		}, diagnosis, 100, diagnosisEdit)) diagnosisEdit = !diagnosisEdit;

			if (GuiButton((Rectangle) {
			250, 300, 300, 50
		}, "STAMPA REFERTO TXT")) {

				char filename[100];
				sprintf(filename, "Referto_%s_%s.txt", refName, refSurname);

				FILE *file = fopen(filename, "w");
				if (file != NULL) {
					fprintf(file, "=================================\n");
					fprintf(file, "    REFERTO MEDICO OSPEDALIERO   \n");
					fprintf(file, "=================================\n\n");
					fprintf(file, "Nome Paziente: %s %s\n", refName, refSurname);
					fprintf(file, "Diagnosi e Cure: %s\n\n", diagnosis);

					time_t rawtime;
					struct tm * timeinfo;
					time(&rawtime);
					timeinfo = localtime(&rawtime);
					fprintf(file, "Data: %02d/%02d/%d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

					fprintf(file, "Firma Medico: ___________________\n");

					fclose(file);
				}
				int code=send_discharge_email(filename,"gaetanobona84@gmail.com");
				printf("code error: %d",code);
				refName[0] = '\0';
				refSurname[0] = '\0';
				diagnosis[0] = '\0';
				appState = 0;
			}

			if (GuiButton((Rectangle) {
			10, 540, 150, 40
		}, "INDIETRO")) {
				appState = 0;
			}
		}
		else if (appState == 5) {
			DrawText("ELENCO REPARTI E DISPONIBILITA' LETTI", 10, 10, 20, DARKBLUE);
			loaded=load_departments(departmentsList,MAX_DEPARTMENTS);
			for (int i = 0; i < loaded; i++) {
				char deptInfo[200];
				sprintf(deptInfo, "%d. %s - Letti Occupati: %d su %d",
				        departmentsList[i].departmentId,
				        departmentsList[i].departmentName,
				        departmentsList[i].bedsOccupied,
				        departmentsList[i].totalBeds);

				Color textColor = DARKGRAY;
				if (departmentsList[i].bedsOccupied >= departmentsList[i].totalBeds) {
					strcat(deptInfo, "  [ REPARTO PIENO ]");
					textColor = RED;
				} else {
					strcat(deptInfo, "  [ POSTI DISPONIBILI ]");
					textColor = DARKGREEN;
				}

				DrawText(deptInfo, 50, 80 + (i * 40), 20, textColor);
			}

			if (GuiButton((Rectangle) {
			10, 540, 150, 40
		}, "INDIETRO")) {
				appState = 0;
			}
		}

		EndDrawing();
	}

	close_listen_socket();
	CloseWindow();
	destroyQueue(waitingQueue); //Libera la memoria della coda prima di uscire
	return 0;
}
