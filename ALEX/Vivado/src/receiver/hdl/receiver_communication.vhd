-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       receiver_communication.vhd
--
--    Description:    
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.IED_config.ALL;

entity receiver_communication is
    Port ( clock : in  STD_LOGIC;
           parallel_out : out STD_LOGIC_VECTOR(data_bitlength-1 downto 0);
           RX : in  STD_LOGIC;
		       init : in STD_LOGIC);
end receiver_communication;

architecture Behavioral of receiver_communication is

begin
  rs_receiver : entity work.rs_receiver
	port map(	  clock => clock,
				      init => init,
				      parallel_out => parallel_out,
              RX => RX);
end Behavioral;

