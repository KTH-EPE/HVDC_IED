-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       FEdetector.vhd
--
--    Description:    falling edge detector. When a falling edge occurs 
--                    on the input, one clock cycle long pulse
--                    is produced on the FE output
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.IED_config.ALL;


entity ReFEdetector is
    Port ( clock : in STD_LOGIC;
		       init : in STD_LOGIC;
		       input : in  STD_LOGIC;           
           FE : inout  STD_LOGIC
           );
			  
end ReFEdetector;

architecture Behavioral of ReFEdetector is

signal input_old : STD_LOGIC := '0';
signal input_old_old : STD_LOGIC := '0';

begin 
process (clock)
	begin		
		if rising_edge(clock) then
			input_old <= input;
			input_old_old <=input_old;
		end if;		
	end process;	
	FE <= not(input) and (input_old or input_old_old);--make a little drawing to understand	
end Behavioral;
