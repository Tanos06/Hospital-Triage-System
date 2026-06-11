#ifndef SECURITY_H_INCLUDED
#define SECURITY_H_INCLUDED

/**
 * @file security.h
 * @brief Modulo per la cifratura e decifratura dei dati sensibili.
 *
 * Implementa il cifrario di Vigenère per proteggere i dati
 * trasmessi via chat tra i reparti.
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
void encrypt_vigenere(char *text, char *key);

/**
 * @brief Decifra una stringa cifrata con il cifrario di Vigenère.
 *
 * @param[in,out] text Stringa cifrata (modificata in-place).
 * @param[in]     key  Chiave di decifratura (stessa usata in cifratura).
 *
 * @pre  text e key devono essere stringhe terminate con '\0'.
 * @post text contiene il testo in chiaro.
 */
void decrypt_vigenere(char *text, char *key);

#endif // SECURITY_H_INCLUDED
