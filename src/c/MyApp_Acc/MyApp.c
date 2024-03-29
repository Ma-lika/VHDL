#include "Platform.h"
#include <Utilities/int_to_string.h>

static volatile unsigned tick;

__attribute__((interrupt("machine")))
void irq_handler(void) {
    // Appeler le gestionnaire d'interruptions du pilote de l'interface série.
    UART_irq_handler(uart);

    // Incrémenter le compteur tick à chaque interruption du timer.
    if (Timer_has_events(timer)) {
        Timer_clear_event(timer);
        tick ++;
    }
}

void main(void) {
    // Initialiser le pilote de l'interface série
    // et afficher un message de bienvenue.
    UART_init(uart);
    UART_puts(uart, "Accelerometer Demo.\n");

    // Configurer le timer pour demander des interruptions
    // 10 fois par seconde.
    Timer_init(timer);
    Timer_set_limit(timer, CLK_FREQUENCY_HZ / 10);
    Timer_enable_interrupts(timer);
    char s[INT_TO_STRING_LEN];

    // Initialiser le contrôleur SPI et le pilote de l'accéléromètre.
    Accelerometer_init(acl);

    AccelerometerState acl_state;

    tick = 0;
    unsigned tock = 0;

    // Exécuter jusqu'à ce que l'utilisateur presse une touche.
    while (!UART_has_data(uart)) {
        // Si une ou plusieurs interruptions du timer ont été détectées.
        if (tick != tock) {
            // Interroger l'accéléromètre.
            Accelerometer_update(acl, &acl_state);

            // Ici, vous pouvez utiliser les champs suivants :
            // acl_state.x : l'accélération selon l'axe X
            // acl_state.y : l'accélération selon l'axe Y
            // acl_state.z : l'accélération selon l'axe Z
            // acl_state.t : la température mesurée par l'accéléromètre
            int32_to_string(s, acl_state.x);
            UART_puts(uart, s);
            UART_putc(uart, '\n');
            int32_to_string(s, acl_state.y);
            UART_puts(uart, s);
            UART_putc(uart, '\n');
            int32_to_string(s, acl_state.z);
            UART_puts(uart, s);
            UART_putc(uart, '\n');
            int32_to_string(s, acl_state.t);
            UART_puts(uart, s);
            UART_putc(uart, '\n');
            
            UART_puts(uart, "fin\n");

            tock ++;
        }
    }
}

