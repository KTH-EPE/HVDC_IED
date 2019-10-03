-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       MAX7301_in.vhd
--
--    Description:    Handles the interface towards a MAX7301.
--                    Configration of the MAX7301.
--                    Readout and storing of the MAX7301 pins (parallel to serial)
--                    
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity MAX7301_in is
  port(
    CLK  : in std_logic;  -- 100MHz clock            
    MAX_DOUT : in std_logic;  -- data coming from MAX
    SCK  : inout std_logic; -- clock for MAX to send data back, 25 MHz    
    CS   : out std_logic; -- chip select for MAX    
    MAX_DIN  : out std_logic; -- command bytes for MAX
    DATA : out std_logic_vector(27 downto 0) -- data read from MAX in parallel    
);
end MAX7301_in;

architecture Behavioral of MAX7301_in is  
  signal data_in  : std_logic_vector(27 downto 0):=(others => '0');   
  signal cnt   : integer range 0 to 3;   -- counter for clock division SCK  
  signal cnt1  : integer range 0 to 20; -- counter for clock division command bytes
  signal sck_in : std_logic:= '1';
  signal din_in : std_logic:= '0';
  signal config     : std_logic_vector(15 downto 0):="0000010000000001"; --take MAX out of shutdown mode
  signal read0to7   : std_logic_vector(15 downto 0):="1100010000000000"; --read signal  0- 7 (P 4-11)
  signal read8to15  : std_logic_vector(15 downto 0):="1100110000000000"; --read signal  8-15 (P12-19)
  signal read16to23 : std_logic_vector(15 downto 0):="1101010000000000"; --read signal 16-23 (P20-27)
  signal read24to27 : std_logic_vector(15 downto 0):="1101110000000000"; --read signal 24-27 (P28-31)
  type state_type is (PRE_INIT, READ_0to7, READ_8to15, READ_16to23, READ_24to27);--FSM states
  signal state : state_type := PRE_INIT;
   
begin
  MAX_DIN   <= din_in;  
  SCK   <= sck_in;  
  DATA  <= data_in;
  
  main : process (CLK)  
  
  begin  
    if (rising_edge(CLK)) then
      cnt <= cnt + 1;
      
      if (cnt = 2) then      
        
        case state is
          
          when PRE_INIT =>
            CS <= '0';            
            cnt1 <= cnt1+1;            
            if (cnt1<16) then
              din_in <= config(15-cnt1);               
            end if;           
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
            state <= READ_0to7;
            end if;            
                    
          when READ_0to7   =>     
            CS <= '0';            
            cnt1 <= cnt1+1;  
            if (cnt1<16) then          
              din_in <= read0to7(15-cnt1); --write command to MAX
            end if;
            if(cnt1>12) and (cnt1<=16) then
              data_in(40-cnt1) <= MAX_DOUT; --read DOUT from MAX containing values for pin 24-27 -- might contain an unwanted 1 from take out of shutdown              
            end if;            
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= READ_8to15;
            end if;                               
          
          when READ_8to15  =>
            CS <= '0';            
            cnt1 <= cnt1+1;   
            if (cnt1<16) then        
              din_in <= read8to15(15-cnt1);--write command to MAX
            end if;
            if(cnt1>8) and (cnt1<=16) then
              data_in(16-cnt1) <= MAX_DOUT; --read DOUT from MAX containing values for pin 0-7
            end if;            
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= READ_16to23;
            end if;            
            
          when READ_16to23 =>
            CS <= '0';            
            cnt1 <= cnt1+1;     
            if (cnt1<16) then       
              din_in <= read16to23(15-cnt1);--write command to MAX
            end if;
            if(cnt1>8) and (cnt1<=16) then
              data_in(24-cnt1) <= MAX_DOUT; --read DOUT from MAX containing values for pin 8-15
            end if;            
            if (cnt1 >= 16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= READ_24to27;
            end if;                      
          
          when READ_24to27 =>
            CS <= '0';            
            cnt1 <= cnt1+1;    
            if (cnt1<16) then        
              din_in <= read24to27(15-cnt1);--write command to MAX
            end if;
            if(cnt1>8) and (cnt1<=16) then
              data_in(32-cnt1) <= MAX_DOUT; --read DOUT from MAX containing values for pin 16-23
            end if;                   
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= READ_0to7;
            end if;          
        end case;       
      end if;
      
      if (cnt = 1) or (cnt = 3) then --update clk SCK for MAX, read data back
        if (cnt = 3) then
          cnt <= 0;
        end if;
        sck_in <= not sck_in; 
      end if;   
                     
    end if;
  end process main;
end Behavioral;