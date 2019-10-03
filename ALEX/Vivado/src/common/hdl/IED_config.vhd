--
--	Package File Template
--
--	Purpose: This package defines supplemental types, subtypes, 
--		 constants, and functions 
--
--   To use any of the example code shown below, uncomment the lines and modify as necessary
--

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.ALL;

package IED_config is

  constant data_bitlength       : integer := 16;
  constant nb_breakers          : integer := 8;
  constant PS_TO_ALG_DATA_WIDTH : integer := 32;
  constant ALG_TO_PS_DATA_WIDTH : integer := 32;
  
  --range +2pu to -2pu, 
  --first bit is sign
  --second bit is most significant
   
  constant pu100_bits: STD_LOGIC_VECTOR(data_bitlength-1 downto 0):="0011111111111111";
  constant pu095_bits: STD_LOGIC_VECTOR(data_bitlength-1 downto 0):="0011110011001011";
  constant pu090_bits: STD_LOGIC_VECTOR(data_bitlength-1 downto 0):="0011100110011001";
  constant pu085_bits: STD_LOGIC_VECTOR(data_bitlength-1 downto 0):="0011011001100110";
  constant pu080_bits: STD_LOGIC_VECTOR(data_bitlength-1 downto 0):="0011001100110010";

  constant threshold_100 : integer := to_integer(unsigned(pu100_bits));
  constant threshold_095 : integer := to_integer(unsigned(pu095_bits));
  constant threshold_090 : integer := to_integer(unsigned(pu090_bits));
  constant threshold_085 : integer := to_integer(unsigned(pu085_bits));
  constant threshold_080 : integer := to_integer(unsigned(pu080_bits));


  constant ADC_CHANNELS : integer := 16;
  type adc_dch_array is array(ADC_CHANNELS-1 downto 0) of std_logic_vector(data_bitlength-1 downto 0);
  constant ADC_CH_00_IDX : integer := 0;
  constant ADC_CH_01_IDX : integer := 1;
  constant ADC_CH_02_IDX : integer := 2;
  constant ADC_CH_03_IDX : integer := 3;
  constant ADC_CH_04_IDX : integer := 4;
  constant ADC_CH_05_IDX : integer := 5;
  constant ADC_CH_06_IDX : integer := 6;
  constant ADC_CH_07_IDX : integer := 7;
  constant ADC_CH_08_IDX : integer := 8;
  constant ADC_CH_09_IDX : integer := 9;
  constant ADC_CH_10_IDX : integer := 10;
  constant ADC_CH_11_IDX : integer := 11;
  constant ADC_CH_12_IDX : integer := 12;
  constant ADC_CH_13_IDX : integer := 13;
  constant ADC_CH_14_IDX : integer := 14;
  constant ADC_CH_15_IDX : integer := 15;
  

  constant ALG_TO_PS_CHANNELS : integer := 64;
  type alg_to_ps_dch_array is array(ALG_TO_PS_CHANNELS-1 downto 0) of std_logic_vector(ALG_TO_PS_DATA_WIDTH-1 downto 0);
  -- Definition of the contents of the ALG_TO_PS
  constant ALG_TO_PS_TRIP_IDX      : integer := 0;
  constant ALG_TO_PS_ADC_CH_00_IDX : integer := 1;
  constant ALG_TO_PS_ADC_CH_01_IDX : integer := 2;
  constant ALG_TO_PS_ADC_CH_02_IDX : integer := 3;
  constant ALG_TO_PS_ADC_CH_03_IDX : integer := 4;
  constant ALG_TO_PS_ADC_CH_04_IDX : integer := 5;
  constant ALG_TO_PS_ADC_CH_05_IDX : integer := 6;
  constant ALG_TO_PS_ADC_CH_06_IDX : integer := 7;
  constant ALG_TO_PS_ADC_CH_07_IDX : integer := 8;
  constant ALG_TO_PS_ADC_CH_08_IDX : integer := 9;
  constant ALG_TO_PS_ADC_CH_09_IDX : integer := 10;
  constant ALG_TO_PS_ADC_CH_10_IDX : integer := 11;
  constant ALG_TO_PS_ADC_CH_11_IDX : integer := 12;
  constant ALG_TO_PS_ADC_CH_12_IDX : integer := 13;
  constant ALG_TO_PS_ADC_CH_13_IDX : integer := 14;
  constant ALG_TO_PS_ADC_CH_14_IDX : integer := 15;
  constant ALG_TO_PS_ADC_CH_15_IDX : integer := 16;
  constant ALG_TO_PS_ENABLE_TRIP_IDX : integer := 17;
  constant ALG_TO_PS_REMOTE_END_IDX : integer := 18;
  
  constant PS_TO_ALG_CHANNELS : integer := 64;
  type ps_to_alg_dch_array is array(PS_TO_ALG_CHANNELS-1 downto 0) of std_logic_vector(PS_TO_ALG_DATA_WIDTH-1 downto 0);
  -- Definition of the contents of the PS_TO_ALG_CHANNELS
  constant PS_TO_ALG_ALG_TO_PS_SIZE_IDX : integer := 0;
  
 
   
  --TRANSMITTER/RECEIVER
  constant TX_RX_Length : integer := 19;--16 bits for the adc measurement + 1 startbit + 1 errorbit + 1 paritybit

  constant divider : integer := 6; -- has to be set up carefully so to ensure a match between both receiver and transmitter bit length

  -- functions' prototypes
  function minimum (left, right : integer) return integer;
  
end IED_config;

package body IED_config is

  function minimum (left, right : integer) return integer is
  begin  -- function minimum
  if left < right then return left;
  else return right;
  end if;
  end function;

  
  
end package body;
 

