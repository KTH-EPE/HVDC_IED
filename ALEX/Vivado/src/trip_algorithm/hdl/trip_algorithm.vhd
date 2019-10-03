-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       trip_algorithm.vhd
--
--    Description:    Algorithm logic for detecting a fault on the line
--                    and ordering a trip of the DC-breakers
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.IED_config.all;

entity trip_algorithm is
  port (  
    clk                 : in  std_logic;
    reset_l             : in  std_logic;
    ps_to_alg_data      : in  ps_to_alg_dch_array;        
    adc_data            : in  adc_dch_array; -- Array of ADC channels
    remote_end          : in  std_logic_vector(data_bitlength-1 downto 0);     
    trigger             : in  std_logic;     
    trip_from_SDK       : in  std_logic_vector(31 downto 0);                       
    trip                : out std_logic_vector(27 downto 0);
    alg_to_ps_data      : out alg_to_ps_dch_array; -- Array of data channels to be transferred to PS
    send_data_to_ps     : out std_logic;
    manual_trip         : in  std_logic; 
    enable_trip         : in  std_logic;
    logic_in            : in std_logic_vector(23 downto 0)                                 
  );
end trip_algorithm;

architecture Behavioral of trip_algorithm is

  signal number       : integer := threshold_100;
  signal threshold    : integer := threshold_080;
  signal trip_init    : std_logic_vector(nb_breakers-1 downto 0) := "00000000";
  signal trip_i       : std_logic_vector(nb_breakers-1 downto 0) := "00000000";
  signal trip_i_v     : std_logic_vector(nb_breakers-1 downto 0) := "00000000";
  signal buf          : std_logic_vector(31 downto 0) := (others => '0');

  constant zeroes1    : std_logic_vector(ALG_TO_PS_DATA_WIDTH-nb_breakers-1 downto 0):= (others => '0');
  constant zeroes3    : std_logic_vector(ALG_TO_PS_DATA_WIDTH-1-1 downto 0):= (others => '0');
  constant zeroes2    : std_logic_vector(ALG_TO_PS_DATA_WIDTH-data_bitlength-1 downto 0):= (others => '0');
  constant zeroes4    : std_logic_vector(19 downto 0):= (others => '0');
    
  begin
  
    process(clk)
    variable trip_i_v : std_logic_vector(nb_breakers-1 downto 0) := "00000000";
    begin
      if rising_edge(clk) then
	      if(reset_l = '0') then                                                                 
          trip_i           <= trip_init;
          alg_to_ps_data   <= (others => (others => '0'));
          send_data_to_ps  <= '0';      
        else          
          if (trigger = '1') then
            
            if (manual_trip = '1') then
              trip_i_v := "00000001"; -- Note: must use variable to update alg_to_ps_data(ALG_TO_PS_TRIP_IDX in this clock cycle.
            elsif (manual_trip = '0') then              
              if (enable_trip = '1') then
                buf <= trip_from_SDK;
                trip_i_v := buf(7 downto 0);
              elsif (enable_trip = '0') then
                trip_i_v := "00000000";
              end if;
            end if;
            
            
            
            trip_i <= trip_i_v;                    
            -- Data to transfer to PS        
            alg_to_ps_data(ALG_TO_PS_TRIP_IDX)      <= zeroes1 & trip_i;
            alg_to_ps_data(ALG_TO_PS_ADC_CH_00_IDX) <= zeroes2 & adc_data(0); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_01_IDX) <= zeroes2 & adc_data(1); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_02_IDX) <= zeroes2 & adc_data(2); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_03_IDX) <= zeroes2 & adc_data(3); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_04_IDX) <= zeroes2 & adc_data(4); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_05_IDX) <= zeroes2 & adc_data(5); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_06_IDX) <= zeroes2 & adc_data(6); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_07_IDX) <= zeroes2 & adc_data(7); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_08_IDX) <= zeroes2 & adc_data(8); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_09_IDX) <= zeroes2 & adc_data(9); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_10_IDX) <= zeroes2 & adc_data(10); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_11_IDX) <= zeroes2 & adc_data(11); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_12_IDX) <= zeroes2 & adc_data(12); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_13_IDX) <= zeroes2 & adc_data(13); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_14_IDX) <= zeroes2 & adc_data(14); 
            alg_to_ps_data(ALG_TO_PS_ADC_CH_15_IDX) <= zeroes2 & adc_data(15); 
            alg_to_ps_data(ALG_TO_PS_ENABLE_TRIP_IDX) <= zeroes3 & enable_trip;
            alg_to_ps_data(ALG_TO_PS_REMOTE_END_IDX) <= zeroes2 & remote_end;        
            send_data_to_ps  <= '1';                            
          else                                                  
            send_data_to_ps  <= '0';                            
          end if;
                    
        end if;
      end if;
  end process;
  
  -- Concurrent
  trip <= zeroes4 & trip_i;                                        

end Behavioral;




