-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       initialize.vhd
--
--    Description:    initialize transmitter
--
--    © Copyright 2019 KTH. All rights reserved.
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity initialize is
    Port ( pulse : out  STD_LOGIC;
           clock : in  STD_LOGIC);
end initialize;

architecture Behavioral of initialize is
signal pulse_not_done : BOOLEAN :=TRUE;

begin
process(clock)
	begin
	if rising_edge(clock) then
    if pulse_not_done then
      pulse <= '0';
      pulse_not_done <= FALSE;
    else 
      pulse <= '1';
    end if;  	
	end if;	
end process;
	
end Behavioral;

