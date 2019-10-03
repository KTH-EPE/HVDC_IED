-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       parity_generator.vhd
--
--    Description:    generate the parity bit. Parity is 1 if 
--                    the number of 1s is odd, 0 if even
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.IED_config.ALL;

entity parity_generator is
	 Port ( parallel_in : in  STD_LOGIC_VECTOR (TX_RX_Length-4 downto 0); -- incoming parallel word, without parity bit
          parallel_out : out  STD_LOGIC_VECTOR (TX_RX_Length-3 downto 0) --outgoing parallel word, including parity bit at bit 0
			  );
end parity_generator;

architecture Behavioral of parity_generator is
	signal parityValue : STD_LOGIC_VECTOR(TX_RX_Length-4 downto 0);
begin

  parityValue(0) <= parallel_in(0);
	parityLoop : for i in 1 to TX_RX_Length-4 generate 
		parityValue(i) <= parityValue(i-1) xor parallel_in(i);
	end generate;
	
	parallel_out(0) <= not(parityValue(TX_RX_Length - 4));
	parallel_out(TX_RX_Length-3 downto 1) <= parallel_in;
end Behavioral;
