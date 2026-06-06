#ifndef CHAT_H_INCLUDED
#define CHAT_H_INCLUDED
#include <stdbool.h>
//Inizializza la chat come server(il medico del reparto che attende messaggi)
int init_chat_server(int port);
//Connette la chat come Client(il medico dell'accettazione che contatta il reparto)
int connect_to_chat(const char *ip_address,int port);
//attende connessioni
int accept_client();
//Invia un messaggio di testo
int send_chat_message(const char *message);
//Riceve un messaggio di testo e lo salva nel buffer
int receive_chat_message(char *buffer,int buffer_size);
void server_receive_loop(void *arg);
void client_receive_loop(void *arg);
void add_message_to_history(char *message,int sender);
char get_message_sender(int i);
char *get_text_message(int i);
int get_total_messages();
bool get_is_client_connected();
//chiude la chat()
void close_listen_socket();
void close_connection_socket();
void close_winsock();


#endif // CHAT_H_INCLUDED
