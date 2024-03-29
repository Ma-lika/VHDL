#include "Platform.h"

// Le contrôleur d'interruptions.
InterruptController *const intc = (InterruptController*)0x81000000;

// L'interface série asynchrone (UART).
static UART uart_priv = {
    // L'adresse de base des registres de l'UART.
    .address     = 0x82000000,
    // Le masque des événements en réception.
    .rx_evt_mask = EVT_MASK(0),
    // Le masque des événements en émission.
    .tx_evt_mask = EVT_MASK(1),
    // Le contrôleur d'interruption qui gère les événements de l'UART
    .intc        = intc
};

UART *const uart = &uart_priv;

// Le timer à usage général.
static Timer timer_priv = {
    // L'adresse de base des registres du timer.
    .address  = 0x83000000,
    // Le masque des événements périodiques.
    .evt_mask = EVT_MASK(2),
    // Le contrôleur d'interruption qui gère les événements du timer
    .intc     = intc
};

Timer *const timer = &timer_priv;

// Le timer utilisé pour les communications SPI.
static Timer spi_timer_priv = {
    .address  = 0x85000000,
    .evt_mask = EVT_MASK(4),
    .intc     = intc
};

Timer *const spi_timer = &spi_timer_priv;

// Le contrôleur SPI.
static SPIMaster spi_master_priv = {
    // L'adresse de base des registres du contrôleur SPI.
    .address  = 0x86000000,
    // Le masque des événements de fin de trame.
    .evt_mask = EVT_MASK(5),
    // Le contrôleur d'interruption qui gère les événements du contrôleur SPI.
    .intc     = intc
};

SPIMaster *const spi_master = &spi_master_priv;

static SPIDevice acl_priv = {
    // Le contrôleur SPI utilisé pour communiquer avec l'accéléromètre.
    .spi            = spi_master,
    // Le timer utilisé pour mesurer les temps d'attente.
    .timer          = spi_timer,
    // La polarité de l'horloge SPI.
    .polarity       = 0,
    // La phase de l'horloge SPI.
    .phase          = 0,
    // La vitesse de communication, en périodes d'horloge par bit.
    .cycles_per_bit = CLK_FREQUENCY_HZ / 2000000, // 2 Mbit/sec
    // Le temps d'attente, en périodes d'horloge.
    .cycles_per_gap = CLK_FREQUENCY_HZ / 5000000  // 200ns (> 100 ns)
};

SPIDevice *const acl = &acl_priv;

