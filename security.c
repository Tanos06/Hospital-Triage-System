#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*COME FUNZIONA IL CIFRARIO DI VIGENËRE:Sposta tutte le lettere dello stesso numero di posizione
ma la chiave di spostamento cambia per ogni singola lettera del testo, basandosi su una parola chiave
in questo caso (MEDICINA).
Esempio:
Parola da cifrare: paziente. chiave:XYZ
La P verr‡ spostata usando il valore della X
LA A verr‡ spostata usando il valore della Y
Stessa cosa per la Z. Quando si arriva alla fine della chiave, si ricomincia di nuovo.
PS:Questo algoritmo non considera i segni di punteggiatura e caratteri speciali.*/

void encrypt_vigenere(char *text,char *key){
    int i=0,j=0;
    int key_len=strlen(key);
    while(text[i]!='\0'){
        if(isalpha(text[i])){
            int temp_key=toupper(key[j])-'A'; //per creare la chiave temporanea sottraiamo il valore di 'A' dal valore della chiave corrente per ottenere il suo valore numerico (0-25)
            if(isupper(text[i])){//controlliamo se le lettere sono maiuscole o minuscole prorprio come nel cifrario di cesare per non rompere la tabella ASCII
                text[i]=((text[i]-'A'+temp_key)%26)+'A';
            }else if(islower(text[i])){
                text[i]=((text[i]-'a'+temp_key)%26)+'a';
            }
            j=(j+1)%key_len;
        }
        i++;
    }
}

void decrypt_vigenere(char *text,char *key){
    int i=0,j=0;
    int key_len=strlen(key);
    while(text[i]!='\0'){
        if(isalpha(text[i])){
            int temp_key=toupper(key[j])-'A';
            if(isupper(text[i])){
                text[i]=(((text[i]-'A'-temp_key)+26)%26)+'A'; //Qui l'unica differenza dalla crittografia Ë che al posto di sommare A lo sottraiamo
            }else if(islower(text[i])){
                text[i]=(((text[i]-'a'-temp_key)+26)%26)+'a';
            }
            j=(j+1)%key_len;
        }
        i++;
    }
}
