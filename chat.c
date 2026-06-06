#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include "chat.h"
#include "security.h"

#define iFamily AF_INET
#define iType SOCK_STREAM
#define iProtocol IPPROTO_TCP
#define MAX_LEN 512
#define KEY "MEDICINA"

static SOCKET listen_socket=INVALID_SOCKET;
static SOCKET connection_socket=INVALID_SOCKET;
static char chatHistory[50][MAX_LEN]; //terrà in memoria 50 messaggi ognugno lungo massimo 512 caratteri
static char chatSender[50];//Ricorda chi ha scritto il messaggio nella riga corrispondente (0 per quelli inviati e 1 per quelli ricevuti)
static int totalMessages=0;
volatile static bool isClientConnected=false; //siccome questa variabile può cambiare in qualsiasi momento da un altro thread che gestisce receive loop, è meglio rileggerla sempre dalla memoria e non ottimizzarla

static int  start_winsock(){
    WSADATA wsaData={0};
    int iResult=0;
    iResult=WSAStartup(MAKEWORD(2,2),&wsaData);//Avvia winsock per "accendere" tutti i driver di rete
    if(iResult != 0){
        printf("L'WSAStartup failed: %d\n ",iResult);
        return iResult;
    }
    return 1;
}
int init_chat_server(int port){
    if(start_winsock()){
        listen_socket=socket(iFamily,iType,iProtocol);//se winsock si è avviato correttamente crea un listen_socket
        if(listen_socket==INVALID_SOCKET){
            printf("socket function failed with error= %d\n",WSAGetLastError());
            return WSAGetLastError();
        }else{
            struct sockaddr_in saServer; //definiamo la struttura sockaddr_in per "definire" il server
            saServer.sin_family=AF_INET;
            saServer.sin_addr.s_addr=INADDR_ANY;
            saServer.sin_port=htons(port);//htons converte l'intero in u_short
            int codeBind=bind(listen_socket,(struct sockaddr*)&saServer,sizeof(saServer));//associa il socket all'indirizzo e alla porta appena configurati
            if(codeBind==SOCKET_ERROR){
                printf("Bind Error: %d\n",WSAGetLastError());
                return 0;
            }
            int codeListen=listen(listen_socket,2);//mette il socket in stato di ascolto. Con "2" intendiamo quanti medici si possono connettere tra loro
            if(codeListen==SOCKET_ERROR){
                printf("Listen Error: %d\n",WSAGetLastError());
                return 0;
            }else{
                return 1;
            }
        }
    }
    return 0;
}
int connect_to_chat(const char *ip_address,int port){
    if(start_winsock()){
        connection_socket=socket(iFamily,iType,iProtocol);
        if(connection_socket==INVALID_SOCKET){
            printf("socket function failed with error= %d\n",WSAGetLastError());
            return WSAGetLastError();
        }else{
            struct sockaddr_in saClient;
            saClient.sin_family=AF_INET;
            saClient.sin_addr.s_addr=inet_addr(ip_address);
            saClient.sin_port=htons(port);
            int codeConnect=connect(connection_socket,(struct sockaddr*)&saClient,sizeof(saClient));
            if(codeConnect==SOCKET_ERROR){
                printf("Connect Error: %d\n",WSAGetLastError());
                return 0;
            }else{
                return 1;
            }
        }
    }else{
        return 0;
    }
}
int accept_client(){
    struct sockaddr_in client_addr;
    int client_addr_len=sizeof(client_addr);
    connection_socket=accept(listen_socket,(struct sockaddr*)&client_addr,&client_addr_len);
    if(connection_socket==INVALID_SOCKET){
        printf("Accept Error: %d\n",WSAGetLastError());
        return 0;
    }else{
        return 1;
    }
    return 0;
}

int send_chat_message(const char *message){
    int sizeMessage=strlen(message);
    int codeSend=send(connection_socket,message,sizeMessage,0);
    if(codeSend==SOCKET_ERROR){
        printf("Send Error: %d\n",WSAGetLastError());
        return 0;
    }else{
        return 1;
    }
}

int receive_chat_message(char *buffer,int buffer_size){
    int iResult=recv(connection_socket,buffer,buffer_size,0);
    if(iResult==SOCKET_ERROR){ //Errore durante l'operazione di ricezione
        printf("Receive Error: %d\n",WSAGetLastError());
        return 0;
    }else{
        if(iResult>0){
                /*questo caso rappresenta il numero di byte (caratteri) scritti nel buffer. Aggiungiamo il carattere '\0' per far si che
                quando andiamo a stampare il messaggio il pc sappia dove finire di stampare il buffer*/
            buffer[iResult]='\0';
            return 1;
        }else{ //iResult==0
            //La chat (quindi la connessione) è stata chiusa dal client
            printf("Chat conclusa\n");
            return 0;

        }
    }
    return 0;
}
void server_receive_loop(void *arg) {
	char message_received[MAX_LEN];
    isClientConnected=false;
	if(accept_client()==1){
        isClientConnected=true;
        while(true) {
            if(receive_chat_message(message_received, MAX_LEN) == 1) {
                decrypt_vigenere(message_received, KEY);
                if(totalMessages<50){
                    add_message_to_history(message_received,1);
                }
            } else {
                isClientConnected=false;
                break;
            }
        }
	}
}

void client_receive_loop(void *arg){
    char message_received[MAX_LEN];
    isClientConnected=true;
    while(true){
        if(receive_chat_message(message_received, MAX_LEN) == 1) {
            decrypt_vigenere(message_received, KEY);
            if(totalMessages<50){
                add_message_to_history(message_received,1);
            }
        }else {
            isClientConnected=false;
            break;
        }
    }
}
void add_message_to_history(char *message,int sender){
    if(totalMessages<50){
        strcpy(chatHistory[totalMessages],message);
        chatSender[totalMessages]=sender;
        totalMessages++;
    }
}
char get_message_sender(int i){
    return chatSender[i];
}
char *get_text_message(int i){
    return chatHistory[i];
}
bool get_is_client_connected(){
    return isClientConnected;
}
int get_total_messages(){
    return totalMessages;
}
void close_listen_socket(){
    int close_listen_socket=closesocket(listen_socket);
    if(close_listen_socket!=0){
        printf("Errore durante il tentativo di chiusura della connessione %d\n",WSAGetLastError());
        return;
    }
    listen_socket=INVALID_SOCKET;
}
void close_connection_socket(){
    int close_code=closesocket(connection_socket);
    if(close_code!=0){
        printf("Errore durante il tentativo di chiusura della connessione %d\n",WSAGetLastError());
        return;
    }
    connection_socket=INVALID_SOCKET;
}
void close_winsock(){
    int shutdown_winsock=WSACleanup();
    if(shutdown_winsock==SOCKET_ERROR){
        printf("Errore durante il tentativo di Windows Sockets %d\n",WSAGetLastError());
        return;
    }
}
