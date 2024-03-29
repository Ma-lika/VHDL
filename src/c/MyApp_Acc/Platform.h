#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <InterruptController/InterruptController.h>
#include <UART/UART.h>
#include <Timer/Timer.h>
#include <SPI/SPI.h>
#include <SPI/Accelerometer.h>

#define CLK_FREQUENCY_HZ     100000000

extern InterruptController *const intc;
extern UART                *const uart;
extern Timer               *const timer;
extern Timer               *const spi_timer;
extern SPIMaster           *const spi_master;
extern SPIDevice           *const acl;

#endif

