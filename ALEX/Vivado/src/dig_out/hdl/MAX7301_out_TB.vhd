-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       MAX7301_out_TB.vhd
--
--    Description:    VHDL Testbench for MAX7301_out.vhd.
--                    
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity MAX7301_out_TB is
--  Port ( );
end MAX7301_out_TB;

architecture Behavioral of MAX7301_out_TB is

 component MAX7301_out  
    Port (    
       CLK  : in std_logic;  -- 100MHz clock            
       MAX_DOUT : in std_logic;  -- data coming from MAX
       SCK  : inout std_logic; -- clock for MAX to send data back, 25 MHz    
       CS   : out std_logic; -- chip select for MAX    
       MAX_DIN  : out std_logic; -- command bytes for MAX
       DATA : in std_logic_vector(27 downto 0) -- data to be sent to MAX in parallel     
      );
  end component;
  
  -- Clock period definitions
  constant CLK_period : time := 10 ns;    

   --signals
  signal clk  : std_logic := '0';
  signal dout  : std_logic := '0';    
  signal sck : std_logic := '0';     
  signal cs  : std_logic := '1';   
  signal din  : std_logic := '0'; 
  signal data: std_logic_vector(27 downto 0) := "0110011001100110011001100110";
 
begin

	-- Instantiate the Unit Under Test (UUT)
  uut : MAX7301_out port map (
    CLK  => clk,      
    MAX_DOUT => dout,
    SCK  => sck,
    CS   => cs,    
    MAX_DIN  => din,
    DATA => data    
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
		DOUT <= '1';
		wait for 200*CLK_period;
		--SDO <='0';
		--wait for CLK_period;
		--SDO <='1';
    --wait for 3*CLK_period;
    --SDO <='0';
		--wait for 50*CLK_period;
		--BUSY <='0';
	end process;


end Behavioral;
