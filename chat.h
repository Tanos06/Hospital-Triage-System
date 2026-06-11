#ifndef CHAT_H_INCLUDED
#define CHAT_H_INCLUDED
#include <stdbool.h>

/**
 * @file chat.h
 * @brief Modulo AD per la chat testuale in rete locale tra reparti.
 *
 * Implementa una comunicazione bidirezionale via socket TCP (WinSock2).
 * Il medico del reparto funge da server; il medico dell'accettazione
 * funge da client. I messaggi sono cifrati con il cifrario di Vigenère.
 *
 * Lo stato interno (socket, cronologia messaggi) è nascosto nel file .c.
 */

/**
 * @brief Inizializza la chat come server (reparto in attesa di connessioni).
 *
 * @param[in] port Porta TCP su cui mettersi in ascolto.
 * @return 1 in caso di successo, codice di errore WSA o 0 in caso di fallimento.
 */
int init_chat_server(int port);

/**
 * @brief Connette la chat come client (accettazione che contatta il reparto).
 *
 * @param[in] ip_address Indirizzo IP del server a cui connettersi.
 * @param[in] port       Porta TCP del server.
 * @return 1 in caso di successo, 0 in caso di fallimento.
 */
int connect_to_chat(const char *ip_address, int port);

/**
 * @brief Attende e accetta una connessione in ingresso (lato server).
 *
 * @return 1 in caso di successo, 0 in caso di errore.
 */
int accept_client();

/**
 * @brief Invia un messaggio di testo al peer connesso.
 *
 * @param[in] message Stringa da inviare (deve essere già cifrata se necessario).
 * @return 1 in caso di successo, 0 in caso di errore.
 */
int send_chat_message(const char *message);

/**
 * @brief Riceve un messaggio di testo e lo salva nel buffer.
 *
 * @param[out] buffer      Buffer in cui salvare il messaggio ricevuto.
 * @param[in]  buffer_size Dimensione massima del buffer.
 * @return 1 in caso di successo, 0 se la connessione è chiusa o in errore.
 */
int receive_chat_message(char *buffer, int buffer_size);

/**
 * @brief Loop di ricezione per il server (eseguito in thread separato).
 *
 * Accetta la connessione, decifra i messaggi ricevuti e li aggiunge
 * alla cronologia. Aggiorna il flag isClientConnected.
 *
 * @param[in] arg Parametro non utilizzato (richiesto dalla firma _beginthread).
 */
void server_receive_loop(void *arg);

/**
 * @brief Loop di ricezione per il client (eseguito in thread separato).
 *
 * Decifra i messaggi ricevuti e li aggiunge alla cronologia.
 * Aggiorna il flag isClientConnected.
 *
 * @param[in] arg Parametro non utilizzato (richiesto dalla firma _beginthread).
 */
void client_receive_loop(void *arg);

/**
 * @brief Aggiunge un messaggio alla cronologia interna del modulo.
 *
 * @param[in] message Testo del messaggio da aggiungere.
 * @param[in] sender  0 = messaggio inviato, 1 = messaggio ricevuto.
 */
void add_message_to_history(char *message, int sender);

/**
 * @brief Restituisce il mittente del messaggio alla posizione i.
 *
 * @param[in] i Indice nella cronologia (0-based).
 * @return 0 se il messaggio è stato inviato, 1 se ricevuto.
 */
char get_message_sender(int i);

/**
 * @brief Restituisce il testo del messaggio alla posizione i.
 *
 * @param[in] i Indice nella cronologia (0-based).
 * @return Puntatore alla stringa del messaggio.
 */
char *get_text_message(int i);

/**
 * @brief Restituisce il numero totale di messaggi nella cronologia.
 *
 * @return Numero di messaggi presenti (max 50).
 */
int get_total_messages();

/**
 * @brief Restituisce lo stato della connessione con il peer.
 *
 * @return true se il client è connesso, false altrimenti.
 */
bool get_is_client_connected();

/**
 * @brief Chiude il socket di ascolto (lato server).
 */
void close_listen_socket();

/**
 * @brief Chiude il socket di connessione attiva.
 */
void close_connection_socket();

/**
 * @brief Termina WinSock e libera le risorse di rete.
 */
void close_winsock();

#endif // CHAT_H_INCLUDED
