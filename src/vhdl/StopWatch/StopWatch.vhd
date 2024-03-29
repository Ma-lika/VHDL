
library ieee;
use ieee.std_logic_1164.all;

entity StopWatch is
    port(
        clk_i             : in  std_logic;
        btn_center_i      : in  std_logic;
        switches_i        : in  std_logic_vector(15 downto 0);
        leds_o            : out std_logic_vector(15 downto 0);
        disp_segments_n_o : out std_logic_vector(0 to 6);
        disp_point_n_o    : out std_logic;
        disp_select_n_o   : out std_logic_vector(3 downto 0)
    );
end StopWatch;

   architecture Structural of StopWatch is
 signal cycle_5ms : std_logic;
 signal cycle_20ms: std_logic;
 signal cycle_100ms : std_logic;
 signal inc_100ms , cycle_1sec , cycle_10sec , cycle_1min : std_logic;
 signal digit_index : integer range 0 to 3;
 signal digit_100ms : integer range 0 to 9;
 signal digit_1sec : integer range 0 to 9;
 signal digit_10sec : integer range 0 to 5;
 signal digit_1min : integer range 0 to 9;
 signal digit : natural range 0 to 9;
 signal segments : std_logic_vector(0 to 6);
 
    -- Declarations
begin
divider_5ms_inst : entity work.CounterModN(Behavioral) 
    generic map( N => 500000)
    port map( inc_i => '1',
              clk_i => clk_i,
              reset_i => '0',
              cycle_o => cycle_5ms,
              value_o => open);
              
divider_20ms_inst : entity work.CounterModN(Behavioral) 
    generic map( N => 4)
    port map( inc_i => cycle_5ms,
              clk_i => clk_i,
              reset_i => '0',
              cycle_o => cycle_20ms,
              value_o => digit_index);
divider_100ms_inst : entity work.CounterModN(Behavioral) 
    generic map( N => 5)
    port map( inc_i => cycle_20ms,
              clk_i => clk_i,
              reset_i => '0',
              cycle_o => cycle_100ms,
              value_o => open);
              
counter_10x100ms_inst : entity work.CounterModN(Behavioral) 
    generic map( N => 10)
    port map( inc_i => inc_100ms,
              clk_i => clk_i,
              reset_i => btn_center_i,
              cycle_o => cycle_1sec,
              value_o => digit_100ms);
              
counter_10x1sec_inst : entity work.CounterModN(Behavioral) 
    generic map( N => 10)
    port map( inc_i => cycle_1sec,
              clk_i => clk_i,
              reset_i => btn_center_i,
              cycle_o => cycle_10sec,
              value_o => digit_1sec);
              
              
counter_6x10sec_inst : entity work.CounterModN(Behavioral) 
    generic map( N => 6)
    port map( inc_i => cycle_10sec,
              clk_i => clk_i,
              reset_i => btn_center_i,
              cycle_o => cycle_1min,
              value_o => digit_10sec);
              
counter_10x1min_inst : entity work.CounterModN(Behavioral) 
    generic map( N => 10)
    port map( inc_i => cycle_1min,
              clk_i => clk_i,
              reset_i => btn_center_i,
              cycle_o => open,
              value_o => digit_1min);
              
decoder_inst: entity work.SegmentDecoder(TruthTable)
port map(digit_i => digit,
         segments_o => segments);
              
              
    -- Concurrent statements
inc_100ms <= cycle_100ms and switches_i(15);

with digit_index select
     digit <= digit_100ms when 0,
              digit_1sec when 1,
              digit_10sec when 2,
              digit_1min when 3;
              
disp_segments_n_o <= not segments;

disp_point_n_o <= '1';

disp_select_n_o(0) <='1' when digit_index /= 0 else '0';
disp_select_n_o(1) <='1'when digit_index /= 1 else '0';
disp_select_n_o(2) <='1' when digit_index /= 2 else '0';
disp_select_n_o(3) <='1' when digit_index /= 3 else '0';
     
     
              
    leds_o <= "0000000000000000";
end Structural;
