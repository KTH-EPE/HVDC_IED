-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       receiver.vhd
--
--    Description:    Receiver for simple serial communication
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.IED_config.all;

entity receiver is
    Port(  
            CLK           : in  STD_LOGIC;
            RECEIVED_DATA : out STD_LOGIC_VECTOR(15 downto 0);
            RX            : in  STD_LOGIC;
            RX_en         : out STD_LOGIC 
        );
end receiver;

architecture Behavioral of receiver is

signal init_pulse   : STD_LOGIC;
signal parallel_out : STD_LOGIC_VECTOR(15 downto 0);

begin
  RX_en <= '0';
  
  receiver_communication : entity work.receiver_communication
	port map(  clock => CLK,
             parallel_out => RECEIVED_DATA,
             RX => RX,
			       init => init_pulse);
  
  initialize : entity work.initialize
  port map(  clock => CLK,                
             pulse => init_pulse);
end Behavioral;