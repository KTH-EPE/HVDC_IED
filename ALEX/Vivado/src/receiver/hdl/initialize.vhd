-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       initialize.vhd
--
--    Description:    Initialize receiver
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- make the output 1 clock cycle long when the push button is pushed, no matter the
-- push's length. The output (pulse) is normally 1, it goes to 0 for one clock cyle
-- when pb goes from 0 to 1
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

