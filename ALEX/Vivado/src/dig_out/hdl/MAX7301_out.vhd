-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       MAX7301_out.vhd
--
--    Description:    Handles the interface from a MAX7301.
--                    Configuration of the MAX7301.
--                    Writing data to the MAX7301 pins
--                    
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity MAX7301_out is
  port(
    CLK  : in std_logic;  -- 100MHz clock            
    MAX_DOUT : in std_logic;  -- data coming from MAX
    SCK  : inout std_logic; -- clock for MAX to send data back, 25 MHz    
    CS   : out std_logic; -- chip select for MAX    
    MAX_DIN  : out std_logic; -- command bytes for MAX
    DATA : in std_logic_vector(27 downto 0) -- data to be sent to MAX in parallel       
);
end MAX7301_out;

architecture Behavioral of MAX7301_out is  
  signal data_out  : std_logic_vector(27 downto 0):=(others => '0');   
  signal cnt   : integer range 0 to 3;   -- counter for clock division SCK  
  signal cnt1  : integer range 0 to 20;  -- counter for clock division command bytes
  signal cnt2  : integer range 0 to 3;   -- counter for initialisation and configuration
  signal sck_in : std_logic:= '1';
  signal din_in : std_logic:= '0';
  signal config        : std_logic_vector(15 downto 0):="0000010000000001"; --take MAX out of shutdown mode
  signal configP4to7   : std_logic_vector(15 downto 0):="0000100101010101"; --configure P 4- 7 as active low output
  signal configP8to11  : std_logic_vector(15 downto 0):="0000101001010101"; --configure P8 -11 as active low output
  signal configP12to15 : std_logic_vector(15 downto 0):="0000101101010101"; --configure P12-15 as active low output
  signal configP16to19 : std_logic_vector(15 downto 0):="0000110001010101"; --configure P16-19 as active low output
  signal configP20to23 : std_logic_vector(15 downto 0):="0000110101010101"; --configure P20-23 as active low output
  signal configP24to27 : std_logic_vector(15 downto 0):="0000111001010101"; --configure P24-27 as active low output
  signal configP28to31 : std_logic_vector(15 downto 0):="0000111101010101"; --configure P28-31 as active low output
  signal write0to7   : std_logic_vector(7 downto 0):="01000100"; --write command  1- 8 (P 4-11)
  signal write8to15  : std_logic_vector(7 downto 0):="01001100"; --write command  9-16 (P12-19)
  signal write16to23 : std_logic_vector(7 downto 0):="01010100"; --write command 17-24 (P20-27)
  signal write24to27 : std_logic_vector(7 downto 0):="01011100"; --write command 25-28 (P28-31)
  type state_type is (PRE_INIT, CONFIG_4to7, CONFIG_8to11, CONFIG_12to15, CONFIG_16to19, CONFIG_20to23, CONFIG_24to27, CONFIG_28to31, WRITE_0to7, WRITE_8to15, WRITE_16to23, WRITE_24to27);--FSM states
  signal state : state_type := PRE_INIT;
   
begin
  MAX_DIN   <= din_in;  
  SCK   <= sck_in;  
  
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
              cnt2 <= cnt2+1;
              state <= CONFIG_4to7;
            end if;            
            
          when CONFIG_4to7 =>
            CS <= '0';            
            cnt1 <= cnt1+1;            
            if (cnt1<16) then
              din_in <= configP4to7(15-cnt1);               
            end if;           
            if (cnt1>=16) then        
              CS <= '1';
            end if;
              if (cnt1 = 20) then
            cnt1 <= 0;
              state <= CONFIG_8to11;
            end if;
 
          when CONFIG_8to11 =>
            CS <= '0';            
            cnt1 <= cnt1+1;            
            if (cnt1<16) then
              din_in <= configP8to11(15-cnt1);               
            end if;           
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= CONFIG_12to15;
            end if;                               
                                    
          when CONFIG_12to15 =>
            CS <= '0';            
            cnt1 <= cnt1+1;            
            if (cnt1<16) then
              din_in <= configP12to15(15-cnt1);               
            end if;           
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= CONFIG_16to19;
            end if;
                                                
          when CONFIG_16to19 =>
            CS <= '0';            
            cnt1 <= cnt1+1;            
            if (cnt1<16) then
              din_in <= configP16to19(15-cnt1);               
              end if;           
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= CONFIG_20to23;
            end if;
                                                            
          when CONFIG_20to23 =>
            CS <= '0';            
            cnt1 <= cnt1+1;            
            if (cnt1<16) then
              din_in <= configP20to23(15-cnt1);               
            end if;           
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= CONFIG_24to27;
            end if;
                                                                        
          when CONFIG_24to27 =>
            CS <= '0';            
            cnt1 <= cnt1+1;            
            if (cnt1<16) then
              din_in <= configP24to27(15-cnt1);               
            end if;           
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= CONFIG_28to31;
            end if;  
            
          when CONFIG_28to31 =>
            CS <= '0';            
            cnt1 <= cnt1+1;            
            if (cnt1<16) then
              din_in <= configP28to31(15-cnt1);               
            end if;           
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= WRITE_0to7;              
            end if;              
            
          when WRITE_0to7   =>     
            CS <= '0';            
            cnt1 <= cnt1+1;  
            if (cnt1<8) then          
              din_in <= write0to7(7-cnt1); --write command to MAX
            end if;
            if(cnt1>=8) and (cnt1<16) then               
              din_in <= data(15-cnt1); --write data to MAX containing values for pin 0-7
            end if;            
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= WRITE_8to15;
            end if;                               
          
          when WRITE_8to15  =>
            CS <= '0';            
            cnt1 <= cnt1+1;   
            if (cnt1<8) then        
              din_in <= write8to15(7-cnt1);--write command to MAX
            end if;
            if(cnt1>=8) and (cnt1<16) then              
              din_in <= data(23-cnt1); --write data to MAX containing values for pin 8-15
            end if;            
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= WRITE_16to23;
            end if;            
            
          when WRITE_16to23 =>
            CS <= '0';            
            cnt1 <= cnt1+1;     
            if (cnt1<8) then       
              din_in <= write16to23(7-cnt1);--write command to MAX
            end if;
            if(cnt1>=8) and (cnt1<16) then              
              din_in <= data(31-cnt1); --write data to MAX containing values for pin 16-23
            end if;            
            if (cnt1 >= 16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;
              state <= WRITE_24to27;
            end if;                      
          
          when WRITE_24to27 =>
            CS <= '0';            
            cnt1 <= cnt1+1;    
            if (cnt1<8) then        
              din_in <= write24to27(7-cnt1);--write command to MAX
            end if;
            if (cnt1>8) and (cnt1<=12) then
              din_in <= '0';
            end if;
            if(cnt1>=12) and (cnt1<16) then
              din_in <= data(39-cnt1); --write data to MAX containing values for pin 24-27
            end if;                  
            if (cnt1>=16) then        
              CS <= '1';
            end if;
            if (cnt1 = 20) then
              cnt1 <= 0;              
              if cnt2<3 then
                state <= CONFIG_4to7;--PRE_INIT;
              else 
                state <= WRITE_0to7;
              end if;              
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