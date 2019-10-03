-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       MAX7301_in_TB.vhd
--
--    Description:    VHDL testbench for MAX7301.
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

entity MAX7301_in_TB is
--  Port ( );
end MAX7301_in_TB;

architecture Behavioral of MAX7301_in_TB is

 component MAX7301_in  
    Port (    
       CLK  : in std_logic;  -- 100MHz clock            
       DOUT : in std_logic;  -- data coming from MAX
       SCK  : inout std_logic; -- clock for MAX to send data back, 25 MHz    
       CS   : out std_logic; -- chip select for MAX    
       DIN  : out std_logic; -- command bytes for MAX
       DATA : out std_logic_vector(27 downto 0) -- data read from MAX in parallel     
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
  signal data: std_logic_vector(27 downto 0) := (others => '0');
 
begin

	-- Instantiate the Unit Under Test (UUT)
  uut : MAX7301_in port map (
    CLK  => clk,      
    DOUT => dout,
    SCK  => sck,
    CS   => cs,    
    DIN  => din,
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
