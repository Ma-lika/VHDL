#ifndef PLATFORM_H_
#define PLATFORM_H_
#include <SPI/OLED.h>

#include <InterruptController/InterruptController.h>
#include <UART/UART.h>
#include <Timer/Timer.h>
#include <UART/Sonar.h>
#include <SPI/Joystick.h>
#include <SPI/SPI.h>

#define CLK_FREQUENCY_HZ     100000000

extern InterruptController *const intc;
extern UART                *const uart;
extern Timer               *const timer;
extern UART                *const sonar_uart;
extern Sonar               *const sonar;
extern SPIDevice           *const jstk;
extern Timer     	    *const spi_timer;
extern SPIMaster 	    *const spi_master;
extern SPIDevice           *const spi_dev;
extern OLED                *const oled;

#endif

