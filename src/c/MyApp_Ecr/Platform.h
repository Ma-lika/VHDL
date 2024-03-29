#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <SPI/OLED.h>
#include <InterruptController/InterruptController.h>
#include <UART/UART.h>
#include <Timer/Timer.h>
#include <SPI/SPI.h>
#include <SPI/Accelerometer.h>
#include <SPI/Joystick.h>
#include <UART/Sonar.h>

#define CLK_FREQUENCY_HZ     100000000

extern InterruptController *const intc;
extern UART                *const uart;
extern Timer               *const timer;
extern Timer     	    *const spi_timer;
extern SPIMaster 	    *const spi_master;
extern SPIDevice           *const spi_dev;
extern OLED                *const oled;
extern UART 		    *const sonar_uart;



extern Timer                *const spi_timer_joy;
extern SPIMaster            *const spi_master_joy;
extern SPIDevice            *const jstk;


extern Sonar                *const sonar;




extern Timer     	     *const spi_timer_acc;
extern SPIMaster 	     *const spi_master_acc;
extern SPIDevice            *const acl;


#endif
