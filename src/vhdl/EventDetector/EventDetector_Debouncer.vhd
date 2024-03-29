
architecture Debouncer of EventDetector is
    -- Declarations
    signal on_timer_reg, off_timer_reg : Integer range 0 to DURATION;
    
    type state_t is (OFF_STABLE, OFF_UNSTABLE, OFF_EVENT, ON_STABLE, ON_UNSTABLE, ON_EVENT);
    signal state_reg, state_next : state_t; 
begin
    -- Concurrent statements
    p_state_next : process (state_reg, src_i, on_timer_reg, off_timer_reg)
    begin
        case state_reg is 
        
            when OFF_STABLE =>
                if src_i='1' then
                    state_next <= ON_EVENT;
                else
                    state_next <= OFF_STABLE;
                end if;
                
            when ON_EVENT =>
                state_next <= ON_UNSTABLE;
                
            When ON_UNSTABLE =>
                if src_i = '1' then
                    if on_timer_reg=DURATION then
                        state_next <= ON_STABLE;
                    end if;
                end if;
                
            when ON_STABLE =>
                if src_i ='0' then
                    state_next <= OFF_EVENT;
                end if;
                
            when OFF_EVENT => 
                state_next <= OFF_UNSTABLE;
                
            when OFF_UNSTABLE =>
                if src_i ='0' then
                    if off_timer_reg = DURATION then
                        state_next <= OFF_STABLE;
                    end if;
                end if;
        end case;
    end process p_state_next;
    
    
    
    p_on_timer_reg : process (src_i, clk_i)
    begin
        if rising_edge (clk_i) then
            if src_i = '0' then
                on_timer_reg <= 0;
            else
                if off_timer_reg < DURATION then
                    off_timer_reg <= off_timer_reg +1;
                end if;
            end if;
        end if;
    end process p_on_timer_reg;
    
    p_off_timer_reg : process(src_i, clk_i)
    begin
        if rising_edge  (clk_i) then
            if src_i = '1' then
                on_timer_reg <= on_timer_reg + 1 ;
            else
                off_timer_reg <= 0;
            end if;
        end if;
    end process p_off_timer_reg;
    
    p_state_reg : process (clk_i)
    begin
        if rising_edge (clk_i) then
            state_reg <= state_next;
        end if;
    end process p_state_reg;
    
    on_evt_o <= '1' when state_reg = ON_EVENT else '0';
    off_evt_o <= '1' when state_reg = OFF_EVENT else '0';
    
    with state_reg select
    status_o <= '1' when ON_EVENT | ON_UNSTABLE | ON_STABLE,
                '0' when others;
    
    
end Debouncer;
