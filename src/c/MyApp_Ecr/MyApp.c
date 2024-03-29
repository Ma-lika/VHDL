#include "Platform.h"
#include <Utilities/int_to_string.h>

#define LED_ADRESS 0x89000000



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
    //UART_puts(uart, "OLED Display Demo.\n");
    // Configurer le timer pour demander des interruptions
    // 20 fois par seconde.
    Timer_init(timer);
    Timer_set_limit(timer, CLK_FREQUENCY_HZ / 20);
    Timer_enable_interrupts(timer);

    // Initialiser le récepteur série associé au sonar
     OLED_init(oled);

    
    char s[INT_TO_STRING_LEN];
    
    // Définir les coordonnées initiales du sprite.
    uint8_t x1 = 0;
    uint8_t y1 = 0;
    uint8_t x2 = x1 + SPRITE_WIDTH  - 1;
    uint8_t y2 = x2 + SPRITE_HEIGHT - 1;
    
    OLEDColor red ;
    OLEDColor blue;
    OLEDColor green;

    // Encoder le sprite, puis l'afficher.
    OLED_set_bitmap(bitmap, SPRITE_SIZE_PIX, sprite);
    OLED_draw_bitmap(oled, x1, y1, x2, y2, SPRITE_SIZE_BYTES, bitmap);  
    
    
    
    // Initialiser le contrôleur SPI et le pilote du joystick.
    Joystick_init(jstk);

    JoystickState jstk_state;
     
    // Initialiser le contrôleur SPI et le pilote de l'accéléromètre.
    Accelerometer_init(acl);

    AccelerometerState acl_state;
    
    
    
    // Initialiser le récepteur série associé au sonar
    Sonar_init(sonar);
    
    
    tick = 0;
    unsigned tock = 0;
    // Exécuter jusqu'à ce que l'utilisateur presse une touche.
    while (!UART_has_data(uart)) {
    uint8_t dist = 0;//Sonar_update(sonar);
        if (tick != tock) {
            Joystick_update(jstk, &jstk_state);
            Accelerometer_update(acl, &acl_state);
            
            
            int32_to_string(s, jstk_state.x);
            
            
            if(jstk_state. x <= 580 && jstk_state.x >= 480 && jstk_state. y <= 580 && jstk_state.y>= 480){
		    jstk_state.red   = 0;
		    jstk_state.green = 0;
		    jstk_state.blue  = 0;
           }
            else if(jstk_state.y > 580){
		    jstk_state.red   = 255;
		    jstk_state.green = 0;
		    jstk_state.blue  = 0;
		    //OLED_clear_all(oled);
	            if(x1 < 96)
	            	x1 = x1 + 1;
    		    OLED_draw_bitmap(oled, x1, y1, x2, y2, SPRITE_SIZE_BYTES, bitmap);
            }            
            
            else if(jstk_state. x > 580){
		    jstk_state.red   = 0;
		    jstk_state.green = 255;
		    jstk_state.blue  = 0;
		    //OLED_clear_all(oled);
		    if(y1 < 64)
	            	y1 = y1 + 1;
    		    OLED_draw_bitmap(oled, x1, y1, x2, y2, SPRITE_SIZE_BYTES, bitmap);
            }
            else if(jstk_state.y < 480){
		    jstk_state.red   = 0;
		    jstk_state.green = 0;
		    jstk_state.blue  = 255;
		    //OLED_clear_all(oled);
	            if(x1 > 0)
	            	x1 = x1 - 1;
    		    OLED_draw_bitmap(oled, x1, y1, x2, y2, SPRITE_SIZE_BYTES, bitmap);

            }
            
            else if(jstk_state.x < 480){
		    jstk_state.red   = 255;
		    jstk_state.green = 255;
		    jstk_state.blue  = 255;
		    //OLED_clear_all(oled);
		    if(y1 > 0)
	            	y1 = y1 - 1;
    		    OLED_draw_bitmap(oled, x1, y1, x2, y2, SPRITE_SIZE_BYTES, bitmap);
            }
    	    x2 = x1 + SPRITE_WIDTH  - 1;
            y2 = x2 + SPRITE_HEIGHT - 1;
            tock ++;
            
            
            int32_to_string(s, acl_state.x);
            UART_puts(uart, "La position x : ");
            UART_puts(uart,s);
            UART_putc(uart, '\n');
            int32_to_string(s, dist);
            UART_puts(uart, "Le sonar : ");
            UART_puts(uart,s);
            UART_putc(uart, '\n');
            UART_putc(uart, '\n');

        }
    }       
}
