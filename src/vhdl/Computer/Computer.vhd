
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.Virgule_pkg.all;
use work.Computer_pkg.all;

entity Computer is
    port(
    --ports computer
        clk_i        : in  std_logic;
        btn_center_i : in  std_logic;
        switches_i   : in  std_logic_vector(15 downto 0);
        leds_o       : out std_logic_vector(15 downto 0);
        --ports uart
        uart_rx_i    : in std_logic;
        uart_tx_o    : out std_logic;
        
        --ports destinés au sonar
         pmod_a1    : in std_logic;
         pmod_a2    : in std_logic;
         pmod_a3   : in std_logic;
         pmod_a4   : in std_logic; 
        --ports destinés à l'accelerateur
         pmod_b1    : out std_logic;
         pmod_b2    : out std_logic;
         pmod_b3   : in std_logic;
         pmod_b4   : out std_logic;
         --ports destinés au joy stick
         pmod_c1    : out std_logic;
         pmod_c2    : out std_logic;
         pmod_c3   : in std_logic;
         pmod_c4   : out std_logic;
         --ports destinés à l'ecran
         pmod_xadc1   : out std_logic;
         pmod_xadc2    : out std_logic;
         pmod_xadc3    : in std_logic;
         pmod_xadc4     : out std_logic;
         pmod_xadc7   : out std_logic;
         pmod_xadc8    : out std_logic;
         pmod_xadc9    : out std_logic;
         pmod_xadc10    : out std_logic
    );
end Computer;

architecture Structural of Computer is

    signal sync_reset    : std_logic;
    signal core_valid    : std_logic;
    signal core_ready    : std_logic;
    signal core_address  : word_t;
    signal core_rdata    : word_t;
    signal core_wdata    : word_t;
    signal core_write    : std_logic_vector(3 downto 0);
    signal core_irq      : std_logic;
    alias dev_address    : byte_t is core_address(31 downto 24);
 
    --signaux mémoire
    signal mem_valid     : std_logic;
    signal mem_ready     : std_logic; 
    signal mem_rdata     : word_t;
    
    --signaux io
    signal io_valid      : std_logic;
    signal io_ready      : std_logic;
    signal io_rdata      : word_t;
    
   --signaux uart
    signal uart_rx_evt : std_logic;
    signal uart_tx_evt: std_logic;
    signal uart_valid : std_logic;
    signal uart_ready : std_logic;
    signal uart_rdata : word_t;
    
    --signaux intc
    signal intc_events : word_t;
    signal intc_valid : std_logic;
    signal intc_ready : std_logic;
    signal intc_rdata : word_t;  
    
    --signaux timer principal
    signal timer_valid : std_logic;
    signal timer_ready : std_logic;
    signal timer_rdata : word_t;
    signal timer_events : word_t;
    
    --signaux sync
    signal sync_sonar_rx : std_logic;
    signal sync_uart_rx : std_logic;
    signal sync_spi_miso : std_logic;
    signal sync_spijoy_miso : std_logic;
    signal sync_spiecr_miso : std_logic;
    
    --signaux uart du sonar
    signal uart2_ready : std_logic;
    signal uart2_rdata : word_t;
    signal uart2_valid : std_logic;
    
    --signaux spi sonar
    signal spi_valid    : std_logic;
    signal spi_ready    : std_logic;
    signal spi_evt       : std_logic;
    signal spi_rdata     : word_t; 
    
    --signaux spi joystick
    signal spijoy_valid    : std_logic;
    signal spijoy_ready    : std_logic;
    signal spijoy_evt       : std_logic;
    signal spijoy_rdata     : word_t; 
    
    --signaux spi ecran
    signal spiecr_valid    : std_logic;
    signal spiecr_ready    : std_logic;
    signal spiecr_evt       : std_logic;
    signal spiecr_rdata     : word_t; 
    
    --signaux timer accélerateur
    signal timer2_valid : std_logic;
    signal timer2_ready : std_logic;
    signal timer2_rdata : word_t;   
        
    --signaux timer joystick
    signal timer3_valid : std_logic;
    signal timer3_ready : std_logic;
    signal timer3_rdata : word_t;   
      
    -- signaux timer ecran
    signal timer4_valid : std_logic;
    signal timer4_ready : std_logic;
    signal timer4_rdata  : word_t;   
   
    
begin  

--instanciation de l'entité de Timer2 (accélerateur)
timer2_inst : entity work. Timer(Behavioral)
    port map( clk_i => clk_i,
              reset_i =>sync_reset,
              valid_i => timer2_valid,
              ready_o => timer2_ready,
              write_i => core_write,
              wdata_i => core_wdata,
              rdata_o => timer2_rdata,
              evt_o => intc_events(INTC_EVENTS_SPI_TIMER),
              address_i => core_address(2));
              
 --instanciation de l'entité de Timer3 (le joystick)
timer3_inst : entity work. Timer(Behavioral)
    port map( clk_i => clk_i,
              reset_i =>sync_reset,
              valid_i => timer3_valid,
              ready_o => timer3_ready,
              write_i => core_write,
              wdata_i => core_wdata,
              rdata_o => timer3_rdata,
              evt_o => intc_events(INTC_EVENTS_SPI_TIMER2),
              address_i => core_address(2));

 --instanciation de l'entité de Timer4 (l'écran)
timer4_inst : entity work. Timer(Behavioral)
    port map( clk_i => clk_i,
              reset_i =>sync_reset,
              valid_i => timer4_valid,
              ready_o => timer4_ready,
              address_i => core_address(2),
              rdata_o => timer4_rdata,
              wdata_i => core_wdata,
              write_i => core_write,
              evt_o => intc_events(INTC_EVENTS_SPI_TIMER3));
              
 --instanciation de l'entité de SPIMaster (Accélerateur)
    spi_inst : entity work. spimaster(rtl)
    port map( clk_i => clk_i,
              reset_i => sync_reset,
              valid_i => spi_valid,
              ready_o => spi_ready,
              address_i => core_address(3 downto 2),
              write_i => core_write(0),
              wdata_i => core_wdata(7 downto 0),
              rdata_o => spi_rdata(7 downto 0),
              evt_o => intc_events(INTC_EVENTS_SPI_MASTER),
              miso_i => sync_spi_miso,
              mosi_o =>pmod_b2,
              sclk_o =>pmod_b4,
              cs_n_o =>pmod_b1);
           spi_rdata (31 downto 8) <= (others=>'0');
           
  --instanciation de l'entité de SPIMaster (Joystick)
    spijoy_inst: entity work. spimaster(rtl)
    port map( clk_i => clk_i,
              reset_i => sync_reset,
              valid_i => spijoy_valid,
              ready_o => spijoy_ready,
              address_i => core_address(3 downto 2),
              write_i => core_write(0),
              wdata_i => core_wdata(7 downto 0),
              rdata_o => spijoy_rdata(7 downto 0),
              evt_o => intc_events(INTC_EVENTS_SPI_MASTER2),
              miso_i => sync_spijoy_miso,
              mosi_o =>pmod_c2,
              sclk_o =>pmod_c4,
              cs_n_o =>pmod_c1);
      spijoy_rdata (31 downto 8) <= (others=>'0');
      
       --instanciation de l'entité de SPIMaster (l'écran)
    spiecr_inst : entity work. spimaster(rtl)
    port map( clk_i => clk_i,
              reset_i => sync_reset,
              valid_i => spiecr_valid,
              ready_o => spiecr_ready,
              address_i => core_address(3 downto 2),
              write_i => core_write(0),
              wdata_i => core_wdata(7 downto 0),
              rdata_o => spiecr_rdata(7 downto 0),
              evt_o => intc_events(INTC_EVENTS_SPI_MASTER3),
              miso_i => sync_spiecr_miso,
              mosi_o =>pmod_xadc2,
              sclk_o =>pmod_xadc4,
              cs_n_o =>pmod_xadc1);
   spiecr_rdata (31 downto 8) <= (others=>'0');
              
                          
--instanciation de l'entité du Timer principal
timer_inst : entity work. Timer(Behavioral)
    port map( clk_i => clk_i,
              reset_i =>sync_reset,
              valid_i => timer_valid,
              ready_o => timer_ready,
              write_i => core_write,
              wdata_i => core_wdata,
              rdata_o => timer_rdata,
              evt_o => intc_events(INTC_EVENTS_TIMER),
              address_i => core_address(2));
    

--instanciation de l'entité de VInterruptController
intc_inst : entity work. VInterruptController(Behavioral)
    port map( clk_i => clk_i,
              reset_i =>sync_reset,
              events_i => intc_events,
              valid_i => intc_valid,
              ready_o => intc_ready,
              address_i => core_address(2),
              write_i => core_write,
              wdata_i => core_wdata,
              rdata_o => intc_rdata,
              irq_o => core_irq);
              
              
--instanciation de l'entité de UART
uart_inst : entity work.UART(Structural)
    generic map(CLK_FREQUENCY_HZ => CLK_FREQUENCY_HZ,
                BIT_RATE_HZ => UART_BIT_RATE_HZ)
    port map( clk_i => clk_i,
              reset_i => sync_reset,
              valid_i => uart_valid,
              ready_o => uart_ready,
              write_i => core_write(0),
              wdata_i => core_wdata(7 downto 0),
              rdata_o => uart_rdata(7 downto 0),
              tx_o => uart_tx_o,
              rx_i => sync_uart_rx,
              tx_evt_o => intc_events(INTC_EVENTS_UART_TX),
              rx_evt_o => intc_events(INTC_EVENTS_UART_RX));   
     uart_rdata (31 downto 8) <= (others=>'0');
     
     --instanciation de l'entité de UART2 (sonar)
uart2_inst : entity work.UART(Structural)
    generic map(CLK_FREQUENCY_HZ => CLK_FREQUENCY_HZ,
                BIT_RATE_HZ => SONAR_UART_BIT_RATE_HZ)
    port map( clk_i => clk_i,
              reset_i => sync_reset,
              valid_i => uart2_valid,
              ready_o => uart2_ready,
              write_i => core_write(0),
              wdata_i => core_wdata(7 downto 0),
              rdata_o => uart2_rdata(7 downto 0),
              tx_o => open,
              rx_i => sync_sonar_rx,
              tx_evt_o => open,
              rx_evt_o => intc_events(INTC_EVENTS_SONAR_UART));   
      uart2_rdata (31 downto 8) <= (others=>'0');

--instanciation de l'entité de Virgule
core_inst : entity work. Virgule(rtl)
    port map( clk_i => clk_i,
              reset_i => sync_reset,
              valid_o => core_valid,
              ready_i => core_ready,
              address_o => core_address,
              write_o => core_write,
              wdata_o => core_wdata,
              rdata_i => core_rdata,
              irq_i => core_irq);
              
 --instanciation de l'entité de VMemory
 mem_inst: entity work. VMemory(Behavioral)
    generic map( CONTENT => MEM_CONTENT)
    port map( clk_i => clk_i,
              reset_i => sync_reset,
              valid_i => mem_valid,
              ready_o => mem_ready,
              address_i => core_address(31 downto 2),
              write_i => core_write,
              wdata_i => core_wdata,
              rdata_o => mem_rdata);

--instanciation de l'entité de InputSynchronizer              
sync_inst: entity work. InputSynchronizer(Behavioral)
    generic map( WIDTH => 22)
    port map ( clk_i => clk_i,
                data_i(0) => btn_center_i,
               data_i(16 downto 1) => switches_i,
               data_i(17) => uart_rx_i,
               data_i(18) => pmod_a3,
               data_i(19) => pmod_b3,
               data_i(20) => pmod_c3,
               data_i(21) => pmod_xadc3,
               data_o (0) => sync_reset,
               data_o (16 downto 1) => io_rdata(15 downto 0),
               data_o(17) => sync_uart_rx,
               data_o(18) => sync_sonar_rx,
               data_o(19) => sync_spi_miso,
               data_o(20) => sync_spijoy_miso,
               data_o(21) => sync_spiecr_miso);         
    io_rdata (31 downto 16) <= (others=>'0');
   
     -- liaisons memoire , IO au processeur   
    mem_valid <= core_valid when dev_address = MEM_ADDRESS else '0';
    io_valid <= core_valid when dev_address = IO_ADDRESS else '0';
    
   --liaisons UART au processeur 
   uart_valid <= core_valid when dev_address = UART_ADDRESS else '0';
   --liaisons UART(sonar) au processeur 
   uart2_valid <= core_valid when dev_address = SONAR_UART_ADDRESS else '0';
   --INTC au processeur
   intc_valid <= core_valid when dev_address = INTC_ADDRESS else '0';
   --liaisons Timer principal au processeur
   timer_valid <= core_valid when dev_address = TIMER_ADDRESS else '0';
   --liaisons Timer (Accélérateur) au processeur
   timer2_valid <= core_valid when dev_address = SPI_TIMER_ADDRESS else '0';
   --liaisons Spi (Accélérateur) au processeur
   spi_valid  <= core_valid when dev_address = SPI_MASTER_ADDRESS else '0';
   --liaisons Spi (Joystick) au processeur
   spijoy_valid  <= core_valid when dev_address = SPI_MASTER_ADDRESS2 else '0';
   --liaisons Timer (Joystick) au processeur
   timer3_valid <= core_valid when dev_address = SPI_TIMER_ADDRESS2 else '0';
   
   --liaisons Spi (Ecran) au processeur
   spiecr_valid  <= core_valid when dev_address = SPI_MASTER_ADDRESS3 else '0';
   --liaisons Timer (Ecran) au processeur
   timer4_valid <= core_valid when dev_address = SPI_TIMER_ADDRESS3 else '0';
   
   intc_events(31 downto 10) <= (others=>'0');
   
   --multiplexer
        with dev_address select 
            core_rdata <= mem_rdata when MEM_ADDRESS,
                          io_rdata when IO_ADDRESS,
                          uart_rdata when UART_ADDRESS,
                          uart2_rdata when SONAR_UART_ADDRESS,
                          intc_rdata when INTC_ADDRESS,
                          timer_rdata when TIMER_ADDRESS,
                          timer2_rdata when SPI_TIMER_ADDRESS,
                          timer3_rdata when SPI_TIMER_ADDRESS2,
                          timer4_rdata when SPI_TIMER_ADDRESS3,
                          spi_rdata when SPI_MASTER_ADDRESS,
                          spijoy_rdata when SPI_MASTER_ADDRESS2,
                          spiecr_rdata when SPI_MASTER_ADDRESS3,
                          (others => '0') when others;
                          
        with dev_address select
            core_ready <= mem_ready when MEM_ADDRESS,
                          timer_ready when TIMER_ADDRESS,
                          timer2_ready when SPI_TIMER_ADDRESS,
                          timer3_ready when SPI_TIMER_ADDRESS2,
                          timer4_ready when SPI_TIMER_ADDRESS3,
                          uart_ready when  UART_ADDRESS,
                          uart2_ready when SONAR_UART_ADDRESS,
                          intc_ready when INTC_ADDRESS,
                          spi_ready when SPI_MASTER_ADDRESS,
                          spijoy_ready when SPI_MASTER_ADDRESS2,
                          spiecr_ready when SPI_MASTER_ADDRESS3,
                          core_valid when others;
            
            
             
  -- bascule pour leds de 7 à 0                        
  p_leds_o7 : process (clk_i, sync_reset)
  begin
    if sync_reset = '1' then 
        leds_o (7 downto 0) <= (others => '0');
    elsif rising_edge(clk_i) then
        if io_valid ='1' and core_write(0) = '1' then
            leds_o (7 downto 0) <= core_wdata(7 downto 0);
        end if;
    end if;
end process p_leds_o7;

--bascule pour leds de 15 à 8
p_leds_o15 : process (clk_i, sync_reset)
  begin
    if sync_reset = '1' then 
        leds_o (15 downto 8) <= (others => '0');
    elsif rising_edge(clk_i) then
        if io_valid ='1' and core_write(1) = '1' then
            leds_o (15 downto 8) <= core_wdata(15 downto 8);
        end if;
    end if;
end process p_leds_o15;

p_reg1: process(clk_i, sync_reset)
    begin
    if sync_reset = '1' then
    pmod_xadc7 <= '1';
    pmod_xadc8 <= '1';
    pmod_xadc9 <= '1';
    pmod_xadc10 <= '1';
    else if rising_edge(clk_i) then
        if core_write(2) = '1' and io_valid = '1' then
           pmod_xadc7 <= core_wdata(16);
           pmod_xadc8 <= core_wdata(17);
           pmod_xadc9 <= core_wdata(18);
           pmod_xadc10 <= core_wdata(19);
        end if;
      end if;
    end if;
 end process p_reg1;


       
            
   
     -- Concurrent statements
end Structural;
