-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       parity_checker.vhd
--
--    Description:    check the parity of the incoming word. 
--                    Parity is 1 if the number of 1s is odd, 0 if even
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.IED_config.ALL;

entity Reparity_checker is
	  Port ( 	clock : in STD_LOGIC;
		        parallel_in : in  STD_LOGIC_VECTOR (TX_RX_Length-3 downto 0); -- incoming parallel word, including parity bit at bit 0
	          busy : in STD_LOGIC;
            parallel_out : out  STD_LOGIC_VECTOR (TX_RX_Length-4 downto 0)); -- outgoing parallel word, without parity bit

end Reparity_checker;

architecture Behavioral of Reparity_checker is

signal parity_error : STD_LOGIC := '0';
signal parityValue : STD_LOGIC_VECTOR(TX_RX_Length- 4 downto 0);

begin
	parityValue(0) <= parallel_in(1);	
	
	parityLoop : for i in 1 to TX_RX_Length-4 generate 
		parityValue(i) <= parityValue(i-1) xor parallel_in(i+1);
	end generate;
	
	parity_error <= not((parityValue(TX_RX_Length-4)) xor parallel_in(0))and not(busy);
	
	process(clock)
    begin
	   --if busy = '0' and parity_error = '0' then
     if busy = '0' then
      parallel_out <= parallel_in(TX_RX_Length-3 downto 1);
     --else
      --parallel_out <= "1111111111111111";      
     end if;
  end process;	
end Behavioral;
