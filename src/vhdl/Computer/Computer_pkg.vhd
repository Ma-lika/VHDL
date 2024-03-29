
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.Virgule_pkg.all;

package Computer_pkg is

    constant CLK_FREQUENCY_HZ : positive      := 100e6;

    constant MEM_ADDRESS      : byte_t        := x"00";
    
    constant MEM_CONTENT : word_vector_t := work.Loader_pkg.DATA;

    constant IO_ADDRESS       : byte_t        := x"80";
    
    constant INTC_ADDRESS :byte_t := x"81";
    constant UART_ADDRESS: byte_t := x"82";
    constant UART_BIT_RATE_HZ: positive := 115200;
    constant INTC_EVENTS_UART_RX : integer := 0;
    constant INTC_EVENTS_UART_TX : positive := 1;
    
    constant TIMER_ADDRESS :byte_t := x"83";
    constant INTC_EVENTS_TIMER : positive := 2;
    
    constant SONAR_UART_ADDRESS:byte_t := x"84";
    constant SONAR_UART_BIT_RATE_HZ : positive := 9600;
    constant INTC_EVENTS_SONAR_UART : positive := 3;
    
   constant SPI_TIMER_ADDRESS:byte_t := x"85";
   constant SPI_MASTER_ADDRESS:byte_t := x"86";
   constant INTC_EVENTS_SPI_TIMER : positive := 4;
   constant INTC_EVENTS_SPI_MASTER : positive := 5;
   
   constant SPI_TIMER_ADDRESS2:byte_t := x"87";
   constant SPI_MASTER_ADDRESS2:byte_t := x"88";
   constant INTC_EVENTS_SPI_TIMER2 : positive := 6;
   constant INTC_EVENTS_SPI_MASTER2 : positive := 7;
   
   constant SPI_TIMER_ADDRESS3:byte_t := x"89";
   constant SPI_MASTER_ADDRESS3:byte_t := x"8A";
   constant INTC_EVENTS_SPI_TIMER3 : positive := 8;
   constant INTC_EVENTS_SPI_MASTER3 : positive := 9;
    
    
    
end Computer_pkg;
