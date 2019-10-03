-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       serial2parallel.vhd
--
--    Description:    Serial to parallel block. 
--                    Samples the incoming frame and produce the parallel output
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.IED_config.ALL;

entity Reserial2parallel is				
	Port ( 	clock : in STD_LOGIC;
		      init : in STD_LOGIC;
		      serial_in : in  STD_LOGIC;
		      FE : in STD_LOGIC:='0';
		      busy : out STD_LOGIC := '0';-- the parallel output is changing, so we can't trust it
			    parallel_out : out STD_LOGIC_VECTOR(TX_RX_Length-3 downto 0));
				
end Reserial2parallel;

architecture Behavioral of Reserial2parallel is

-- counter used to define the sampling period. 
-- Initialized so that the first bit is sampled
-- right in the middle, no matter the shift induced by the falling edge
-- detector beforehand

constant ctr0 : integer := minimum(-divider/2+3,0);
signal ctr : integer range ctr0 to divider :=ctr0;

-- counter used to count the number of bits, it increases each time
-- a new bit is sampled

signal counter : integer range 0 to TX_RX_Length-2 :=0;

-- latched to one when the start bit is detected, 
-- goes to zero when the incoming frame is over

signal new_frame : STD_LOGIC:='0';
signal not_new_frame : STD_LOGIC:='1';
signal reset_latch : STD_LOGIC:='0';

begin

not_new_frame <= FE nor new_frame;

	process(clock)
		begin
		
			if rising_edge(clock) then
				new_frame <= reset_latch nor not_new_frame; 
				
				if new_frame = '1' then  
				
					if ctr = 0 and counter = 0 then--check the start bit
						if serial_in ='0' then-- if it is actually a start bit and not a parasitic
							ctr <= ctr+1;
						else -- if it is not a start bit 
							new_frame <= '0';
							reset_latch <= '1';
							ctr <= divider;
						end if;
						
					elsif ctr = divider then-- it's time to sample
						
						 
						if counter < TX_RX_Length-2 then
							--parallel_out(TX_RX_Length-3-counter) <= serial_in;
              parallel_out(counter) <= serial_in;
							ctr <= 1;
							counter <= counter + 1;
							if counter = TX_RX_Length-3 then							
								busy <= '0';
							else
								busy <= '1';
							end if;
																 
						--elsif counter = TX_RX_Length-1 then
						elsif counter = TX_RX_Length-2 then
							ctr <= 1;
							counter <= 0;
							reset_latch <= '1';
							busy <= '0';							
					   end if;					
					else						
						ctr <= ctr + 1;					
					end if;
					
				elsif reset_latch = '1' and new_frame = '0' then				
					if ctr = divider then
						reset_latch <= '0';
						ctr <=ctr0;
					else					
						ctr <= ctr + 1;						
					end if;					
				end if;
				
			-- initialization			
				if init = '0' then
					ctr  <=ctr0;
					counter <= 0;
					new_frame <='0';
					reset_latch <='0';
					busy <= '0';
				end if;				
			end if;
			
		end process;
end Behavioral;

