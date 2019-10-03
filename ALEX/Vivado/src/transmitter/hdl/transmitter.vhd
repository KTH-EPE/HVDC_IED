-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       transmitter.vhd
--
--    Description:    transmitter
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.IED_config.all;

entity transmitter is
    Port(  
            CLK      : in  STD_LOGIC;
            TX       : out  STD_LOGIC;
            TX_en    : out  STD_LOGIC;         
            eoc      : in  STD_LOGIC;
            ADC_DATA : in STD_LOGIC_VECTOR(data_bitlength-1 downto 0)
        );
end transmitter;

architecture Behavioral of transmitter is

signal init_pulse : STD_LOGIC;
signal start2send_adc : STD_LOGIC:= '0';
signal eoc_re : STD_LOGIC;

begin
  TX_en <= '1';
	
  transmitter_communication : entity work.transmitter_communication
	 port map( clock => CLK,
             TX => TX,             
			       init => init_pulse,
			       start_to_send => start2send_adc,
			       adc_data => ADC_DATA
	);

  adc_re_detector : entity work.REdetector
	 port map( clock => CLK,
			       init => init_pulse,
			       input => eoc,
             send_ready => start2send_adc		
	 );
	
  initialize : entity work.initialize
   port map(  clock => CLK,                
              pulse => init_pulse
   );
	
end Behavioral;



