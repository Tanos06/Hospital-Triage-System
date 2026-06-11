#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @file security.c
 * @brief Implementazione del cifrario di Vigenère.
 *
 * COME FUNZIONA IL CIFRARIO DI VIGENÈRE:
 * Sposta tutte le lettere dello stesso numero di posizione,
 * ma la chiave di spostamento cambia per ogni singola lettera
 * del testo, basandosi su una parola chiave (es. MEDICINA).
 *
 * Esempio: Parola da cifrare: "paziente", chiave: "XYZ"
 * - La 'P' viene spostata usando il valore della 'X'
 * - La 'A' viene spostata usando il valore della 'Y'
 * - Quando si arriva alla fine della chiave, si ricomincia.
 *
 * Nota: l'algoritmo non considera segni di punteggiatura
 * e caratteri speciali.
 */

/**
 * @brief Cifra una stringa di testo con il cifrario di Vigenère.
 *
 * @param[in,out] text Stringa da cifrare (modificata in-place).
 * @param[in]     key  Chiave di cifratura.
 *
 * @pre  text e key devono essere stringhe terminate con '\0'.
 * @post text contiene il testo cifrato.
 */
void encrypt_vigenere(char *text, char *key) {
    int i = 0, j = 0;
    int key_len = strlen(key);
    while (text[i] != '\0') {
        if (isalpha(text[i])) {
            /* Valore numerico della chiave corrente (0-25) */
            int temp_key = toupper(key[j]) - 'A';
            if (isupper(text[i])) {
                text[i] = ((text[i] - 'A' + temp_key) % 26) + 'A';
            } else if (islower(text[i])) {
                text[i] = ((text[i] - 'a' + temp_key) % 26) + 'a';
            }
            j = (j + 1) % key_len;
        }
        i++;
    }
}

/**
 * @brief Decifra una stringa cifrata con il cifrario di Vigenère.
 *
 * @param[in,out] text Stringa cifrata (modificata in-place).
 * @param[in]     key  Chiave di decifratura.
 *
 * @pre  text e key devono essere stringhe terminate con '\0'.
 * @post text contiene il testo in chiaro.
 */
void decrypt_vigenere(char *text, char *key) {
    int i = 0, j = 0;
    int key_len = strlen(key);
    while (text[i] != '\0') {
        if (isalpha(text[i])) {
            int temp_key = toupper(key[j]) - 'A';
            if (isupper(text[i])) {
                /* Sottrazione con correzione +26 per evitare valori negativi */
                text[i] = (((text[i] - 'A' - temp_key) + 26) % 26) + 'A';
            } else if (islower(text[i])) {
                text[i] = (((text[i] - 'a' - temp_key) + 26) % 26) + 'a';
            }
            j = (j + 1) % key_len;
        }
        i++;
    }
}
