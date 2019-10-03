-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       transmitter_communication.vhd
--
--    Description:    transmitter communication
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.IED_config.ALL;

entity transmitter_communication is
    Port ( clock          : in  STD_LOGIC;
           TX             : out  STD_LOGIC;                      
		       init           : in STD_LOGIC;
		       start_to_send  : in STD_LOGIC;
		       adc_data       : in STD_LOGIC_VECTOR(data_bitlength-1 downto 0)
			  );
end transmitter_communication;

architecture Behavioral of transmitter_communication is

signal parallel_2 : STD_LOGIC_VECTOR(TX_RX_Length-3 downto 0); -- parallel from parity maker to parallel2serial 

begin  				
   parallel2serial : entity work.parallel2serial	 
	 port map(
				clock => clock,
				parallel_in => parallel_2,
				start2send => start_to_send,
				serial_out => TX);

  parity_generator : entity work.parity_generator	 
	 port map(
				parallel_in => adc_data,
				parallel_out => parallel_2
	 );		
end Behavioral;

