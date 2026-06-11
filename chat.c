#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "chat.h"
#include "security.h"

/**
 * @file chat.c
 * @brief Implementazione del modulo AD per la chat in rete locale.
 *
 * Usa WinSock2 per la comunicazione TCP tra server (reparto) e
 * client (accettazione). I messaggi vengono cifrati/decifrati con
 * il cifrario di Vigenère tramite il modulo security.
 *
 * Lo stato interno del modulo (socket, cronologia, flag) è privato
 * (variabili static) e non accessibile dall'esterno.
 */

#define iFamily    AF_INET      /**< Famiglia di indirizzi IPv4. */
#define iType      SOCK_STREAM  /**< Tipo socket: stream TCP. */
#define iProtocol  IPPROTO_TCP  /**< Protocollo TCP. */
#define MAX_LEN    512          /**< Lunghezza massima di un messaggio. */
#define KEY        "MEDICINA"   /**< Chiave del cifrario di Vigenère. */

/** @brief Socket di ascolto del server. */
static SOCKET listen_socket = INVALID_SOCKET;

/** @brief Socket della connessione attiva (sia lato server che client). */
static SOCKET connection_socket = INVALID_SOCKET;

/** @brief Cronologia dei messaggi: max 50, ognuno lungo max 512 caratteri. */
static char chatHistory[50][MAX_LEN];

/** @brief Mittente di ogni messaggio: 0=inviato, 1=ricevuto. */
static char chatSender[50];

/** @brief Numero totale di messaggi nella cronologia. */
static int totalMessages = 0;

/**
 * @brief Flag di connessione attiva.
 *
 * Dichiarata volatile perché può essere modificata in qualsiasi
 * momento dal thread del receive loop.
 */
volatile static bool isClientConnected = false;

/**
 * @brief Avvia il sottosistema WinSock (driver di rete).
 *
 * @return 1 in caso di successo, codice di errore WSA in caso di fallimento.
 */
static int start_winsock() {
    WSADATA wsaData = {0};
    int iResult = 0;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("L'WSAStartup failed: %d\n ", iResult);
        return iResult;
    }
    return 1;
}

/**
 * @brief Inizializza la chat come server.
 */
int init_chat_server(int port) {
    if (start_winsock()) {
        listen_socket = socket(iFamily, iType, iProtocol);
        if (listen_socket == INVALID_SOCKET) {
            printf("socket function failed with error= %d\n", WSAGetLastError());
            return WSAGetLastError();
        } else {
            struct sockaddr_in saServer;
            saServer.sin_family = AF_INET;
            saServer.sin_addr.s_addr = INADDR_ANY;
            saServer.sin_port = htons(port); /**< htons converte l'intero in u_short (network byte order) */
            int codeBind = bind(listen_socket, (struct sockaddr*)&saServer, sizeof(saServer));
            if (codeBind == SOCKET_ERROR) {
                printf("Bind Error: %d\n", WSAGetLastError());
                return 0;
            }
            /* Massimo 2 connessioni in coda (2 medici che si parlano) */
            int codeListen = listen(listen_socket, 2);
            if (codeListen == SOCKET_ERROR) {
                printf("Listen Error: %d\n", WSAGetLastError());
                return 0;
            } else {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief Connette la chat come client.
 */
int connect_to_chat(const char *ip_address, int port) {
    if (start_winsock()) {
        connection_socket = socket(iFamily, iType, iProtocol);
        if (connection_socket == INVALID_SOCKET) {
            printf("socket function failed with error= %d\n", WSAGetLastError());
            return WSAGetLastError();
        } else {
            struct sockaddr_in saClient;
            saClient.sin_family = AF_INET;
            saClient.sin_addr.s_addr = inet_addr(ip_address);
            saClient.sin_port = htons(port);
            int codeConnect = connect(connection_socket, (struct sockaddr*)&saClient, sizeof(saClient));
            if (codeConnect == SOCKET_ERROR) {
                printf("Connect Error: %d\n", WSAGetLastError());
                return 0;
            } else {
                return 1;
            }
        }
    } else {
        return 0;
    }
}

/**
 * @brief Accetta una connessione client in ingresso.
 */
int accept_client() {
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    connection_socket = accept(listen_socket, (struct sockaddr*)&client_addr, &client_addr_len);
    if (connection_socket == INVALID_SOCKET) {
        printf("Accept Error: %d\n", WSAGetLastError());
        return 0;
    } else {
        return 1;
    }
    return 0;
}

/**
 * @brief Invia un messaggio al peer connesso.
 */
int send_chat_message(const char *message) {
    int sizeMessage = strlen(message);
    int codeSend = send(connection_socket, message, sizeMessage, 0);
    if (codeSend == SOCKET_ERROR) {
        printf("Send Error: %d\n", WSAGetLastError());
        return 0;
    } else {
        return 1;
    }
}

/**
 * @brief Riceve un messaggio e lo scrive nel buffer.
 */
int receive_chat_message(char *buffer, int buffer_size) {
    int iResult = recv(connection_socket, buffer, buffer_size, 0);
    if (iResult == SOCKET_ERROR) {
        printf("Receive Error: %d\n", WSAGetLastError());
        return 0;
    } else {
        if (iResult > 0) {
            /* Aggiunta del terminatore per una corretta stampa del buffer */
            buffer[iResult] = '\0';
            return 1;
        } else { /* iResult == 0: connessione chiusa ordinatamente dal peer */
            printf("Chat conclusa\n");
            return 0;
        }
    }
    return 0;
}

/**
 * @brief Loop di ricezione lato server (eseguito in thread separato).
 */
void server_receive_loop(void *arg) {
    char message_received[MAX_LEN];
    isClientConnected = false;
    if (accept_client() == 1) {
        isClientConnected = true;
        while (true) {
            if (receive_chat_message(message_received, MAX_LEN) == 1) {
                decrypt_vigenere(message_received, KEY);
                if (totalMessages < 50) {
                    add_message_to_history(message_received, 1);
                }
            } else {
                isClientConnected = false;
                break;
            }
        }
    }
}

/**
 * @brief Loop di ricezione lato client (eseguito in thread separato).
 */
void client_receive_loop(void *arg) {
    char message_received[MAX_LEN];
    isClientConnected = true;
    while (true) {
        if (receive_chat_message(message_received, MAX_LEN) == 1) {
            decrypt_vigenere(message_received, KEY);
            if (totalMessages < 50) {
                add_message_to_history(message_received, 1);
            }
        } else {
            isClientConnected = false;
            break;
        }
    }
}

/**
 * @brief Aggiunge un messaggio alla cronologia interna.
 */
void add_message_to_history(char *message, int sender) {
    if (totalMessages < 50) {
        strcpy(chatHistory[totalMessages], message);
        chatSender[totalMessages] = sender;
        totalMessages++;
    }
}

/** @brief Restituisce il mittente del messaggio all'indice i. */
char get_message_sender(int i) {
    return chatSender[i];
}

/** @brief Restituisce il testo del messaggio all'indice i. */
char *get_text_message(int i) {
    return chatHistory[i];
}

/** @brief Restituisce lo stato di connessione del client. */
bool get_is_client_connected() {
    return isClientConnected;
}

/** @brief Restituisce il numero totale di messaggi in cronologia. */
int get_total_messages() {
    return totalMessages;
}

/** @brief Chiude il socket di ascolto. */
void close_listen_socket() {
    int close_listen_socket = closesocket(listen_socket);
    if (close_listen_socket != 0) {
        printf("Errore durante il tentativo di chiusura della connessione %d\n", WSAGetLastError());
        return;
    }
    listen_socket = INVALID_SOCKET;
}

/** @brief Chiude il socket di connessione attiva. */
void close_connection_socket() {
    int close_code = closesocket(connection_socket);
    if (close_code != 0) {
        printf("Errore durante il tentativo di chiusura della connessione %d\n", WSAGetLastError());
        return;
    }
    connection_socket = INVALID_SOCKET;
}

/** @brief Termina WinSock e libera le risorse di rete. */
void close_winsock() {
    int shutdown_winsock = WSACleanup();
    if (shutdown_winsock == SOCKET_ERROR) {
        printf("Errore durante il tentativo di Windows Sockets %d\n", WSAGetLastError());
        return;
    }
}
