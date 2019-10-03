-----------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       parallel2serial.vhd
--
--    Description:    parallel to serial conversion
--
--    © Copyright 2019 KTH. All rights reserved.
--
-----------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.IED_config.ALL;

entity parallel2serial is	
    Port ( 
				clock : in STD_LOGIC;
				parallel_in : in  STD_LOGIC_VECTOR (TX_RX_Length-3  downto 0); -- parity bit is included
				start2send : in  STD_LOGIC;
				serial_out : out  STD_LOGIC				
				);
end parallel2serial;

architecture Behavioral of parallel2serial is

signal ctr : integer range 1 to divider := divider;-- counter used to define the sampling period
-- counter used to count the number of bits, it increases each time a new bit is sent
signal counter : integer range 0 to TX_RX_Length:=0;

begin

process(clock)
	begin	
		if rising_edge(clock) then 
			if start2send = '1' then
				if ctr = divider then
				   if counter = 0 then 
				     serial_out <= '0'; --startbot
				     counter <= counter + 1;
				     ctr <= 1;
				   elsif counter < TX_RX_Length-1 then
				     serial_out <= parallel_in(counter-1);
				     counter <= counter + 1;
				     ctr <= 1;
			     elsif counter = TX_RX_Length-1 then
             serial_out <= '1';
             counter <= counter + 1;
             ctr <= 1;
				   else 
					   counter <= 0;
					   serial_out <= '1';
				   end if;
				else
				  ctr <= ctr + 1;   
				end if;				
			else 
				counter <= 0;
        ctr <= divider;
        serial_out <= '1';			
			end if;
		end if; 
	end process;
	
end Behavioral;

