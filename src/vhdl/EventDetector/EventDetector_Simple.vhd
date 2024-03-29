
architecture Simple of EventDetector is
    -- Declarations
    signal src_reg : std_logic_vector(0 to 1) := "00";
begin
    -- Concurrent statements
    p_reg0 : process (clk_i)
    begin
        if rising_edge (clk_i) then
            src_reg(0) <= src_i;
            src_reg(1) <=src_reg(0);
        end if;
    end process p_reg0;
   
   --status_o <= src_reg(0);
   --on_evt_o <= src_reg(0) and not src_reg(1);
   --off_evt_o <= src_reg(1) and not src_reg(0);
   
   p_out : process (src_reg)
   begin
        if src_reg(0) = '1' and  src_reg(1) =  '0' then
            on_evt_o <= '1';
        else
            on_evt_o <= '0';
        end if;
        if src_reg(0) = '0' and  src_reg(1) =  '1' then
            off_evt_o <= '1';
        else
            off_evt_o <= '0';
        end if;
   end process p_out;
        
   status_o <= src_reg(0);
end Simple;
