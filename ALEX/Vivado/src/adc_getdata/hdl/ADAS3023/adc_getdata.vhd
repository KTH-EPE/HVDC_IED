-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       adc_getdata.vhd
--
--    Description:    Handles the interface towards an ADC of type TI AD3023.
--                    Configration of the ADCs.
--                    Start of conversion.
--                    Readout and storing of the ADC results until all channels
--                    have been read.
--                    Signaling that new ADC data is available.
--                    
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use work.IED_config.all;

entity adc_getdata is
  port (
    clk                  : in std_logic;                      -- Input clock
    reset_l              : in std_logic;                      -- reset, active low    
    enable               : in std_logic;                      -- enable - enables the start of the conversions from AXI
    enable_slider        : in std_logic;                      -- enable - from slider to avoid startup-sequence channel switching
    sample_time_interval : in  std_logic_vector(31 downto 0); -- Nr of clk cycles per sampling interval
    eoc                  : out std_logic;                     -- "End of conversion"                                           
    dch_array            : out adc_dch_array;                 -- Array of ADC chcnnels
    --ADC1
    sdo                  : in std_logic;                      -- ADC SDO    
    busy                 : in std_logic;                      -- ADC is busy
    cnv                  : out std_logic;                     -- ADC conversion start         
    sck                  : inout std_logic;                   -- ADC external clock send data back    
    reset                : out std_logic;                     -- ADC reset pin
    csn                  : out std_logic;                     -- ADC
    pd                   : out std_logic;                     -- ADC power down    
    din                  : out std_logic;                     -- ADC input data for configuration
    --ADC2
    sdo_1                : in std_logic;                      -- ADC SDO    
    busy_1               : in std_logic;                      -- ADC is busy
    cnv_1                : out std_logic;                     -- ADC conversion start         
    sck_1                : inout std_logic;                   -- ADC external clock send data back    
    reset_1              : out std_logic;                     -- ADC reset pin
    csn_1                : out std_logic;                     -- ADC
    pd_1                 : out std_logic;                     -- ADC power down    
    din_1                : out std_logic                      -- ADC input data for configuration
  );
end adc_getdata;

architecture Behavioral of adc_getdata is
	
	signal data_in    : std_logic_vector(127 downto 0):=(others => '0');-- data from the ADC1  
	signal data_in_1  : std_logic_vector(127 downto 0):=(others => '0');-- data from the ADC2
  signal cnt  : integer range 0 to 20000;  
  signal cnt1  : integer range 0 to 256;  
  signal cnt2  : integer range 0 to 128;
  signal cnt_enable  : integer range 0 to 200000;  
  type state_type is (IDLE, SEND_SCK);
  signal state : state_type := IDLE;  -- initial state    
	signal sim_adc_data_value : std_logic_vector(data_bitlength-1 downto 0) := (others=>'0'); -- Simulated adc-data  
  signal enable_slider_on : std_logic := '0';
  signal enable_sampling : std_logic := '0';
  
begin

  main : process (CLK)  
    begin  
      if (rising_edge(CLK)) then
        if (enable_slider = '1') then
          enable_slider_on <= '1';
        end if;
        
        if (enable_slider_on = '1') then
          if (cnt_enable < 199999) then
            cnt_enable <= cnt_enable +1;
          else
            enable_sampling <= '1';
          end if;
          --ADC1
          RESET <= '0';
          PD    <= '0';
          DIN   <= '1';
          --ADC2
          RESET_1 <= '0';
          PD_1    <= '0';
          DIN_1   <= '1';          
        else 
          --ADC1
          RESET <= '1';
          PD    <= '1';
          DIN   <= '0';
        --ADC2
          RESET_1 <= '1';
          PD_1    <= '1';
          DIN_1   <= '0';
        end if;
        
        
        cnt <= cnt + 1;
        if (cnt = sample_time_interval+sample_time_interval) then
          cnt <= 0;
          eoc <= '0';
              --ADC1 -- need to add artificial offset due to ADC startup sequence
              dch_array(1) <= not(data_in(127)) & data_in(126 downto 112);
              dch_array(2) <= not(data_in(111)) & data_in(110 downto 96);
              dch_array(3) <= not(data_in(95)) & data_in(94 downto 80);
              dch_array(4) <= not(data_in(79)) & data_in(78 downto 64);
              dch_array(5) <= not(data_in(63)) & data_in(62 downto 48);
              dch_array(6) <= not(data_in(47)) & data_in(46 downto 32);
              dch_array(7) <= not(data_in(31)) & data_in(30 downto 16);
              dch_array(0) <= not(data_in(15)) & data_in(14 downto 0);
              --ADC2 -- need to add artificial offset due to ADC startup sequence
              dch_array(10) <= not(data_in_1(127)) & data_in_1(126 downto 112);
              dch_array(11) <= not(data_in_1(111)) & data_in_1(110 downto 96);
              dch_array(12) <= not(data_in_1(95)) & data_in_1(94 downto 80);
              dch_array(13) <= not(data_in_1(79)) & data_in_1(78 downto 64);
              dch_array(14) <= not(data_in_1(63)) & data_in_1(62 downto 48);
              dch_array(15) <= not(data_in_1(47)) & data_in_1(46 downto 32);
              dch_array(8) <= not(data_in_1(31)) & data_in_1(30 downto 16);
              dch_array(9) <= not(data_in_1(15)) & data_in_1(14 downto 0);
              
        end if;
        case state is
        when IDLE =>
          eoc <= '0';
          --ADC1
          CSN <= '1';
          SCK <= '1';
          CNV <= '0'; 
          --ADC2
          CSN_1 <= '1';
          SCK_1 <= '1';
          CNV_1 <= '0';  
          if (cnt > 400 and cnt < 410)  then
            CNV <= '1';
            CNV_1 <= '1';
          elsif (cnt > sample_time_interval+sample_time_interval-32 and enable_sampling = '1') then
            state <= SEND_SCK;
            CSN <= '0'; 
            CSN_1 <= '0';
          end if; 
        when SEND_SCK =>        
          if (cnt2<128 and SCK ='1') then
            SCK <= '0';
            SCK_1 <= '0';
            eoc <= '0';
          else 
            SCK <= SCK xor '1';
            SCK_1 <= SCK_1 xor '1';
            eoc <= '0';
          end if;
          if (SCK = '0' and cnt2<128) then 
            data_in(127-cnt2) <= SDO;
            data_in_1(127-cnt2) <= SDO_1;
            cnt2 <= cnt2 + 1;
            eoc <= '0';
          end if;
          cnt1 <= cnt1 + 1;        
          if (cnt1 = 256) then
            cnt1 <= 0;
            cnt2 <= 0;
            state <= IDLE;
            eoc <= '1';
          end if;               
        end case;              
      end if;
    end process main;    
end Behavioral;



