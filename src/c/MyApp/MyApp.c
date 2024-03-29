#include "Platform.h"
#include <Utilities/int_to_string.h>

#define LED_ADRESS 0x89000000



static volatile unsigned tick;

__attribute__((interrupt("machine")))
void irq_handler(void) {
    // Appeler le gestionnaire d'interruptions du pilote de l'interface série.
    UART_irq_handler(uart);
    UART_irq_handler(sonar_uart);

    // Incrémenter le compteur tick à chaque interruption du timer.
    if (Timer_has_events(timer)) {
        Timer_clear_event(timer);
        tick ++;
    }
}

// Définir des couleurs {Rouge, Vert, Bleu}.
#define B {0,  0,  0}
#define C {31, 31, 0}
#define W {31, 31, 31}

// Définir la taille de l'objet à afficher.
#define SPRITE_WIDTH  7
#define SPRITE_HEIGHT 7
#define SPRITE_SIZE_PIX   (SPRITE_WIDTH * SPRITE_HEIGHT)
#define SPRITE_SIZE_BYTES (SPRITE_SIZE_PIX * 2)

// L'image, représentée par un tableau de couleurs.
static const OLEDColor sprite[SPRITE_SIZE_PIX] = {
    B, B, W, W, W, B, B,
    B, C, W, W, W, W, B,
    C, C, W, W, W, W, W,
    C, C, C, W, W, W, W,
    C, C, C, C, W, W, W,
    B, C, C, C, C, C, B,
    B, B, C, C, C, B, B
};

// Tableau qui recevra l'image compactée.
static uint8_t bitmap[SPRITE_SIZE_BYTES];

void main(void) {
    // Initialiser le pilote de l'interface série
    // et afficher un message de bienvenue.
    UART_init(uart);
    UART_puts(uart, "Sonar Demo.\n");
    UART_puts(uart, "Joystick Demo.\n");
    UART_puts(uart, "OLED Display Demo.\n");
    

    // Configurer le timer pour demander des interruptions
    // dix fois par seconde.
    Timer_init(timer);
    Timer_set_limit(timer, CLK_FREQUENCY_HZ / 20);
    Timer_enable_interrupts(timer);

    // Initialiser le récepteur série associé au sonar
    Sonar_init(sonar);
    
    Joystick_init(jstk);
    JoystickState jstk_state;
    
     OLED_init(oled);

    tick = 0;
    unsigned tock = 0;
    
    char s[INT_TO_STRING_LEN];
    
    // Définir les coordonnées initiales du sprite.
    uint8_t x1 = 0;
    uint8_t y1 = 0;
    uint8_t x2 = x1 + SPRITE_WIDTH  - 1;
    uint8_t y2 = x2 + SPRITE_HEIGHT - 1;

    // Encoder le sprite, puis l'afficher.
    OLED_set_bitmap(bitmap, SPRITE_SIZE_PIX, sprite);
    OLED_draw_bitmap(oled, x1, y1, x2, y2, SPRITE_SIZE_BYTES, bitmap);   

    // Exécuter jusqu'à ce que l'utilisateur presse une touche.
    while (!UART_has_data(uart)) {
        // Mettre à jour la distance mesurée par le sonar.
        uint8_t dist = Sonar_update(sonar);

        if (tick != tock) {
            // Ici, on peut faire quelque chose avec dist.
            // ...
            // Déclarer un tableau de caractères capable de recevoir
	    // la représentation décimale d'un nombre de 32 bits.
	    char s[INT_TO_STRING_LEN];
	          // Configurer la couleur de la LED du joystick.
            jstk_state.red   = 255;
            jstk_state.green = 0;
            jstk_state.blue  = 0;

            // Mettre à jour la couleur de la LED du joystick,
            // lire les coordonnées du joystick et l'état des boutons.
            Joystick_update(jstk, &jstk_state);

            // Ici, vous pouvez utiliser les champs suivants :
            // jstk_state.x       : la coordonnée X du joystick (0 à 1023)
            // jstk_state.y       : la coordonnée Y du joystick (0 à 1023)
            // jstk_state.trigger : vaut 1 si l'utilisateur presse la gachette
            // jstk_state.pressed : vaut 1 si l'utilisateur presse la manette
            
               // Calculer la nouvelle position du sprite.
            // Mettre à jour x1, y1, x2 et y2.

            // Déplacer le sprite.
            // Utiliser les fonctions OLED_copy et OLED_clear
            // pour copier ou effacer une région rectangulaire de l'écran.
	    
	    // Convertir un entier en chaîne.
	    int32_to_string(s, dist);
	    // Afficher le résultat, suivi d'un saut de ligne.
		//UART_puts(uart, s);
		//UART_putc(uart, '\n');
	    
	    //int32_to_string(s, jstk_state.x);

            UART_putc(uart,'s');
            UART_putc(uart, '\n');
            
            /*if(jstk_state. x <= 580 && jstk_state.x >= 480 && jstk_state. y <= 580 && jstk_state.y>= 480){
            jstk_state.red   = 0;
            jstk_state.green = 0;
            jstk_state.blue  = 0;
           }
            else if(jstk_state.y > 580){
            jstk_state.red   = 255;
            jstk_state.green = 0;
            jstk_state.blue  = 0;
            OLED_clear_all(oled);
                if(x1 < 96)
                    x1 = x1 + 1;
                OLED_draw_bitmap(oled, x1, y1, x2, y2, SPRITE_SIZE_BYTES, bitmap);
            }            
            
            else if(jstk_state. x > 580){
            jstk_state.red   = 0;
            jstk_state.green = 255;
            jstk_state.blue  = 0;
            OLED_clear_all(oled);
            if(y1 < 64)
                    y1 = y1 + 1;
                OLED_draw_bitmap(oled, x1, y1, x2, y2, SPRITE_SIZE_BYTES, bitmap);
            }
            else if(jstk_state.y < 480){
            jstk_state.red   = 0;
            jstk_state.green = 0;
            jstk_state.blue  = 255;
            OLED_clear_all(oled);
                if(x1 > 0)
                    x1 = x1 - 1;
                OLED_draw_bitmap(oled, x1, y1, x2, y2, SPRITE_SIZE_BYTES, bitmap);

            }
            
            else if(jstk_state.x < 480){
            jstk_state.red   = 255;
            jstk_state.green = 255;
            jstk_state.blue  = 255;
            OLED_clear_all(oled);
            if(y1 > 0)
                    y1 = y1 - 1;
                OLED_draw_bitmap(oled, x1, y1, x2, y2, SPRITE_SIZE_BYTES, bitmap);
            }
            

            
	*/

            
           
            // jstk_state.y       : la coordonnée Y du joystick (0 à 1023)
            // jstk_state.trigger : vaut 1 si l'utilisateur presse la gachette
            // jstk_state.pressed : vaut 1 si l'utilisateur presse la manette
            
            
            
            
            
            
            
            
            

            tock ++;    
            //x2 = x1 + SPRITE_WIDTH  - 1;
           //y2 = x2 + SPRITE_HEIGHT - 1;

        }
    }
    
    
}
