-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       re_detector.vhd
--
--    Description:    rising edge detector. 
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.IED_config.ALL;


entity REdetector is
    Port (  clock : in STD_LOGIC;
		        init : in STD_LOGIC;
		        input : in  STD_LOGIC;
            send_ready : out  STD_LOGIC
			 );
			  
end REdetector;

architecture Behavioral of REdetector is

signal input_old : STD_LOGIC := '0';
signal input_old_old : STD_LOGIC := '0';
signal cnt : integer range 0 to 300 := 0;
signal RE : STD_LOGIC := '0';

begin 
process (clock)
	begin
		
		if rising_edge(clock) then
			input_old <= input;
			input_old_old <=input_old;
			RE <= (input) and (not(input_old and input_old_old));
			
			if RE = '1' or cnt > 0 then        
        if cnt < (divider*TX_RX_Length) then
          cnt <= cnt + 1;
          send_ready <= '1';
        else
          cnt <= 0;
          send_ready <= '0';
        end if;
      else
        cnt <= 0;
        send_ready <= '0';
      end if;			
		end if;		
	end process;	
end Behavioral;


