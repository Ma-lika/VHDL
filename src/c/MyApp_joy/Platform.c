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

static UART sonar_uart_priv = {
    .address     = 0x84000000,
    .rx_evt_mask = EVT_MASK(3),
    .tx_evt_mask = EVT_MASK_OFF, // Pas d'événement en émission sur cette interface.
    .intc        = intc
};

// Le timer utilisé pour les communications SPI.
static Timer spi_timer_priv = {
    .address  = 0x87000000,
    .evt_mask = EVT_MASK(6),
    .intc     = intc
};

UART *const sonar_uart = &sonar_uart_priv;

Timer *const spi_timer = &spi_timer_priv;

static Sonar sonar_priv = {
    .uart = sonar_uart
};

// Le contrôleur SPI.
static SPIMaster spi_master_priv = {
    // L'adresse de base des registres du contrôleur SPI.
    .address  = 0x88000000,
    // Le masque des événements de fin de trame.
    .evt_mask = EVT_MASK(7),
    // Le contrôleur d'interruption qui gère les événements du contrôleur SPI.
    .intc     = intc
    
};

Sonar *const sonar = &sonar_priv;

SPIMaster *const spi_master = &spi_master_priv;

static SPIDevice spi_dev_priv = {
    .spi            = spi_master,
    .timer          = spi_timer,
    .polarity       = 1,
    .phase          = 1,
    .cycles_per_bit = CLK_FREQUENCY_HZ / 5000000, // 5 Mbit/sec
    .cycles_per_gap = CLK_FREQUENCY_HZ / 5000000  // 200 ns
};

SPIDevice *const spi_dev = &spi_dev_priv;

static SPIDevice jstk_priv = {
    // Le contrôleur SPI utilisé pour communiquer avec le joystick.
    .spi            = spi_master,
    // Le timer utilisé pour mesurer les temps d'attente.
    .timer          = spi_timer,
    // La polarité de l'horloge SPI.
    .polarity       = 0,
    // La phase de l'horloge SPI.
    .phase          = 0,
    // La vitesse de communication, en périodes d'horloge par bit.
    .cycles_per_bit = CLK_FREQUENCY_HZ / 1000000, // 1 Mbit/sec
    // Le temps d'attente, en périodes d'horloge.
    .cycles_per_gap = CLK_FREQUENCY_HZ / 25000    // 40us (> 25 us)

};

SPIDevice *const jstk = &jstk_priv;

static OLED oled_priv = {
    .spi        = spi_dev,
    .address    = 0x80000002,
    .cycles_1ms = CLK_FREQUENCY_HZ / 1000
};

OLED *const oled = &oled_priv;


