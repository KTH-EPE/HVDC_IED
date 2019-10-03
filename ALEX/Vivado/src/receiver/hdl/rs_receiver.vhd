-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       rs_receiver.vhd
--
--    Description:    RS485 transceiver overall structure. 
--                    This can be use for any kind of UART based communication
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.IED_config.ALL;

entity rs_receiver is
    Port( 
		   clock        : in  STD_LOGIC;
       parallel_out : out  STD_LOGIC_VECTOR (TX_RX_Length-4 downto 0);-- word without parity bit
       RX           : in  STD_LOGIC;
       init         : in STD_LOGIC);
end rs_receiver;

architecture Behavioral of rs_receiver is

signal parallel_1 : STD_LOGIC_VECTOR(TX_RX_Length-3 downto 0); -- parallel from serial2parallel to parity checker
signal busy_internal : STD_LOGIC:='0';
signal FE_i : STD_LOGIC:='0';

begin

ReFEdetector : entity work.ReFEdetector
	port map(
				clock => clock,
				init => init,
				input => RX,
				FE => FE_i);	

Reserial2parallel : entity work.Reserial2parallel	
	port map(
				clock => clock,
				init => init,
				serial_in => RX,
				FE => FE_i,
				busy => busy_internal,
				parallel_out => parallel_1);				

Reparity_checker : entity work.Reparity_checker	
	port map(
				clock => clock,
				parallel_in =>parallel_1,
				busy => busy_internal,
				parallel_out => parallel_out
				);
				
end Behavioral;
