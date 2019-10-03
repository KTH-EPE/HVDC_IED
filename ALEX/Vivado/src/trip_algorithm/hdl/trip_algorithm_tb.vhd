-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       trip_algorithm_tb.vhd
--
--    Description:    Testbench for trip_algoritm
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;
use work.IED_config.all;

entity trip_algorithm_tb is
--  Port ( );
end trip_algorithm_tb;

architecture Behavioral of trip_algorithm_tb is

  -- Component Declaration for the Unit Under Test (UUT)
 
  component trip_algorithm
  port (
    clk                 : in  std_logic;
    reset_l             : in  std_logic;
    ps_to_alg_data      : in  ps_to_alg_dch_array;        
    adc_data            : in  adc_dch_array; -- Array of ADC channels     
    trigger             : in  std_logic;                              
    trip                : out std_logic_vector(nb_breakers-1 downto 0);
    alg_to_ps_data      : out alg_to_ps_dch_array; -- Array of data channels to be transferred to PS
    send_data_to_ps     : out std_logic;
    button_trip         : in std_logic                                       
  );
  end component;

  -- Inputs
  signal CLK              : std_logic := '0';
  signal RESET_L          : std_logic := '0';
  signal PS_TO_ALG_DATA   : ps_to_alg_dch_array := (others => (others => '0'));        
  signal ADC_DATA         : adc_dch_array := (others => (others => '0'));
  signal TRIGGER          : std_logic := '0';
  signal BUTTON_TRIP      : std_logic := '0';
  
  -- Outputs
  signal TRIP             : std_logic_vector(nb_breakers-1 downto 0);
  signal ALG_TO_PS_DATA   : alg_to_ps_dch_array; 
  signal SEND_DATA_TO_PS  : std_logic;                                       
  
  -- Clock period definitions
  constant CLK_period : time := 10 ns;    
  constant ADC_period : time := 1000*CLK_period;

begin

  -- Instantiate the Unit Under Test (UUT)
  uut: trip_algorithm port map (
    clk                 => CLK,             
    reset_l             => RESET_L,         
    ps_to_alg_data      => PS_TO_ALG_DATA,  
    adc_data            => ADC_DATA,        
    trigger             => TRIGGER,         
    trip                => TRIP,            
    alg_to_ps_data      => ALG_TO_PS_DATA,  
    send_data_to_ps     => SEND_DATA_TO_PS,
    button_trip         => BUTTON_TRIP 
  );

  -- Clock process definitions
  CLK_process :process
  begin
    CLK <= '0';
    wait for CLK_period/2;
    CLK <= '1';
    wait for CLK_period/2;
  end process;

  -- Stimulus process
  stim_proc: process
  begin
    
    wait for 1*CLK_period/2;
      RESET_L <= '0';
    
    wait for 1*CLK_period;
      RESET_L <= '1';

    wait for 1*CLK_period;
      ADC_DATA(0) <= std_logic_vector(to_unsigned(0, data_bitlength));  
      ADC_DATA(1) <= std_logic_vector(to_unsigned(1, data_bitlength)); 
      ADC_DATA(2) <= std_logic_vector(to_unsigned(2, data_bitlength)); 
      ADC_DATA(3) <= std_logic_vector(to_unsigned(3, data_bitlength)); 
      ADC_DATA(4) <= std_logic_vector(to_unsigned(4, data_bitlength)); 
      ADC_DATA(5) <= std_logic_vector(to_unsigned(5, data_bitlength)); 
      ADC_DATA(6) <= std_logic_vector(to_unsigned(6, data_bitlength)); 
      ADC_DATA(7) <= std_logic_vector(to_unsigned(7, data_bitlength)); 
      ADC_DATA(8)<= std_logic_vector(to_unsigned(8, data_bitlength));
      ADC_DATA(9)<= std_logic_vector(to_unsigned(9, data_bitlength));
      ADC_DATA(10)<= std_logic_vector(to_unsigned(10, data_bitlength)); 
      ADC_DATA(11)<= std_logic_vector(to_unsigned(11, data_bitlength)); 
      ADC_DATA(12) <= std_logic_vector(to_unsigned(12, data_bitlength)); 
      ADC_DATA(13) <= std_logic_vector(to_unsigned(13, data_bitlength)); 
      ADC_DATA(14) <= std_logic_vector(to_unsigned(14, data_bitlength)); 
      ADC_DATA(15) <= std_logic_vector(to_unsigned(15, data_bitlength));      
    wait for 1*CLK_period;
      TRIGGER <= '1';
    wait for 1*CLK_period;
      TRIGGER <= '0';           
    wait for ADC_period - CLK_period;
    BUTTON_TRIP <= '1';
    wait for 1*CLK_period;
      ADC_DATA(0) <= std_logic_vector(to_unsigned(0, data_bitlength));  
      ADC_DATA(1) <= std_logic_vector(to_unsigned(1, data_bitlength)); 
      ADC_DATA(2) <= std_logic_vector(to_unsigned(2, data_bitlength)); 
      ADC_DATA(3) <= std_logic_vector(to_unsigned(3, data_bitlength)); 
      ADC_DATA(4) <= std_logic_vector(to_unsigned(4, data_bitlength)); 
      ADC_DATA(5) <= std_logic_vector(to_unsigned(5, data_bitlength)); 
      ADC_DATA(6) <= std_logic_vector(to_unsigned(6, data_bitlength)); 
      ADC_DATA(7) <= std_logic_vector(to_unsigned(7, data_bitlength)); 
      ADC_DATA(8)<= std_logic_vector(to_unsigned(8, data_bitlength));
      ADC_DATA(9)<= std_logic_vector(to_unsigned(9, data_bitlength));
      ADC_DATA(10)<= std_logic_vector(to_unsigned(10, data_bitlength)); 
      ADC_DATA(11)<= std_logic_vector(to_unsigned(11, data_bitlength)); 
      ADC_DATA(12) <= std_logic_vector(to_unsigned(12, data_bitlength)); 
      ADC_DATA(13) <= std_logic_vector(to_unsigned(13, data_bitlength)); 
      ADC_DATA(14) <= std_logic_vector(to_unsigned(14, data_bitlength)); 
      ADC_DATA(15) <= std_logic_vector(to_unsigned(15, data_bitlength));      
    wait for 1*CLK_period;
      TRIGGER <= '1';
    wait for 1*CLK_period;
      TRIGGER <= '0';           
    wait for ADC_period - CLK_period;
    
    wait; -- Wait forever           
  end process;

end Behavioral;
