// Echo.c

#include <stdint.h>

// Définition des adresses des périphériques.
#define INTC_ADDR 0x81000000
#define UART_ADDR 0x82000000

// Définition des registres des périphériques.
#define INTC_MASK_REG   INTC_ADDR
#define INTC_EVENTS_REG (INTC_ADDR + 4)
#define UART_DATA_REG   UART_ADDR

// Définition des masques pour détecter et acquitter les événements.
// masque = 2^(numéro de l'interruption)
#define INTC_EVENTS_UART_RX 1
#define INTC_EVENTS_UART_TX 2

/* -------------------------------------------------------------------------- *
 * Fonctions d'accès aux registres des périphériques.
 * -------------------------------------------------------------------------- */

// Lire un octet à l'adresse addr (équivaut à l'instruction LBU).
static inline uint8_t read8(uint32_t addr) {
    return *(volatile uint8_t*)addr;
}

// Écrire l'octet val à l'adresse addr (équivaut à l'instruction SB).
static inline void write8(uint32_t addr, uint8_t val) {
    *(uint8_t*)addr = val;
}

// Lire un mot de 32 bits à l'adresse addr (équivaut à l'instruction LW).
static inline uint32_t read32(uint32_t addr) {
    return *(volatile uint32_t*)addr;
}

// Écrire le mot de 32 bits val à l'adresse addr (équivaut à l'instruction SW).
static inline void write32(uint32_t addr, uint32_t val) {
    *(uint32_t*)addr = val;
}

/* -------------------------------------------------------------------------- *
 * Fonctions de lecture/écriture sur l'interface série.
 * -------------------------------------------------------------------------- */

// Envoyer un caractère à travers la liaison série.
void UART_send_char(char c) {
    // Ecrire le caractère dans le registre de données.
    write8(UART_DATA_REG, c);
    // Attendre que l'envoi soit terminé.
    while (!(read32(INTC_EVENTS_REG) & INTC_EVENTS_UART_TX));
    // Signaler que l'événement a été traité.
    write32(INTC_EVENTS_REG, INTC_EVENTS_UART_TX);
}

// Envoyer une chaîne de caractères à travers la liaison série.
void UART_send_string(const char *str) {
    // Tant que le caractère courant est non nul.
    while (*str) {
        // Envoyer le caractère courant.
        UART_send_char(*str);
        // Passer au caractère suivant.
        str ++;
    }
}

// Recevoir un caractère en provenance de la liaison série.
char UART_receive_char(void) {
    // Attendre la réception.
    while (!(read32(INTC_EVENTS_REG) & INTC_EVENTS_UART_RX));
    // Signaler que l'événement a été traité.
    write32(INTC_EVENTS_REG, INTC_EVENTS_UART_RX);
    // Retourner le caractère reçu.
    return read8(UART_DATA_REG);
}

/* -------------------------------------------------------------------------- *
 * Programme principal.
 * -------------------------------------------------------------------------- */

void main(void) {
    // Envoyer un message d'accueil.
    UART_send_string("Echo> ");
    // Afficher chaque caractère reçu jusqu'à ce que l'utilisateur presse <Entrée>
    char c;
    do {
    	c = UART_receive_char();
    	UART_send_char(c);
    } while (c != '\r');
    // Envoyer un message de fin.
    UART_send_string("\nBye!\n");
}
