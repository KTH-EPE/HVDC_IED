-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       transmitter_TB.vhd
--
--    Description:    VHDL testbench for transmitter.vhd
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;


-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity transmitter_TB is
--  Port ( );
end transmitter_TB;

architecture Behavioral of transmitter_TB is

  -- Component Declaration for the Unit Under Test (UUT)
  component transmitter  
    Port (    
      CLK      : in  STD_LOGIC; --100 MHz
      TX       : out  STD_LOGIC;
      TX_en    : out  STD_LOGIC;         
      eoc      : in  STD_LOGIC;
      ADC_DATA : in STD_LOGIC_VECTOR(15 downto 0) 
      );
  end component;
  
  -- Clock period definitions
  constant CLK_period : time := 10 ns;    

   --signals
  signal clk     : std_logic := '0';
  signal tx      : std_logic := '0';    
  signal tx_en   : std_logic := '0';     
  signal eoc     : std_logic := '0';   
  signal adc_data: std_logic_vector(15 downto 0) := "0101010101010101";
begin

	-- Instantiate the Unit Under Test (UUT)
  uut : transmitter port map (
    CLK   => clk,      
    TX    => tx,    
    TX_EN => tx_en,      
    EOC   => eoc,     
    ADC_DATA   => adc_data    
  );
  
   -- Clock process definitions
  CLK_process : process
  begin
    clk <= '0';
    wait for CLK_period / 2;
    clk <= '1';
    wait for CLK_period / 2;
  end process;

-- Stimulus process
  stim_proc: process
  begin
		EOC <= '0';
		wait for CLK_period/2;
		EOC <= '1';
		wait for 2000*CLK_period; -- 20us = 2000*10ns
		EOC <='0';
		wait for CLK_period/2;
    EOC <= '1';
    wait for 2000*CLK_period; -- 20us = 2000*10ns
    wait;
	end process;
  
end Behavioral;
