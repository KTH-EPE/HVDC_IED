-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       trip_algorithm_axi.vhd
--
--    Description:    A container for trip algorithms. 
--                    It receives configurable parameters from PS via AXI Lite.
--                    It receives ADC samples from adc_getdata_axi.
--                    It sends ADC samples and data from the trip algorithms to PS
--                    via an AXI Stream. 
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.IED_config.all;

entity trip_algorithm_axi_v1_0 is
	generic (
		-- Users to add parameters here

		-- User parameters ends
		-- Do not modify the parameters beyond this line


		-- Parameters of Axi Slave Bus Interface S00_AXI
		C_S00_AXI_DATA_WIDTH	: integer	:= 32;
		C_S00_AXI_ADDR_WIDTH	: integer	:= 8;

		-- Parameters of Axi Master Bus Interface M00_AXIS
		C_M00_AXIS_TDATA_WIDTH	: integer	:= 32
	);
	port (
		-- Users to add ports here
    eoc        : in  std_logic;     -- "End of conversion"                                           
    adc_dch_00 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_01 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_02 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_03 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_04 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_05 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_06 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_07 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_08 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_09 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_10 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_11 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_12 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_13 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_14 : in  std_logic_vector(data_bitlength-1 downto 0);    
    adc_dch_15 : in  std_logic_vector(data_bitlength-1 downto 0);    
    remote_end : in  std_logic_vector(data_bitlength-1 downto 0);
    manual_trip: in std_logic;
    enable_trip: in std_logic;
    logic_in   : in std_logic_vector(23 downto 0);
		logic_out  : out std_logic_vector(27 downto 0);
		-- User ports ends
    
		-- Do not modify the ports beyond this line


		-- Ports of Axi Slave Bus Interface S00_AXI
		s00_axi_aclk	  : in std_logic;
		s00_axi_aresetn	: in std_logic;
		s00_axi_awaddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_awprot	: in std_logic_vector(2 downto 0);
		s00_axi_awvalid	: in std_logic;
		s00_axi_awready	: out std_logic;
		s00_axi_wdata	  : in std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_wstrb	  : in std_logic_vector((C_S00_AXI_DATA_WIDTH/8)-1 downto 0);
		s00_axi_wvalid	: in std_logic;
		s00_axi_wready	: out std_logic;
		s00_axi_bresp	  : out std_logic_vector(1 downto 0);
		s00_axi_bvalid	: out std_logic;
		s00_axi_bready	: in std_logic;
		s00_axi_araddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_arprot	: in std_logic_vector(2 downto 0);
		s00_axi_arvalid	: in std_logic;
		s00_axi_arready	: out std_logic;
		s00_axi_rdata	  : out std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_rresp	  : out std_logic_vector(1 downto 0);
		s00_axi_rvalid	: out std_logic;
		s00_axi_rready	: in std_logic;

		-- Ports of Axi Master Bus Interface M00_AXIS
		m00_axis_aclk	    : in std_logic;
		m00_axis_aresetn	: in std_logic;
		m00_axis_tvalid	  : out std_logic;
		m00_axis_tdata	  : out std_logic_vector(C_M00_AXIS_TDATA_WIDTH-1 downto 0);
		m00_axis_tstrb	  : out std_logic_vector((C_M00_AXIS_TDATA_WIDTH/8)-1 downto 0);
		m00_axis_tlast	  : out std_logic;
		m00_axis_tready	  : in std_logic
	);
end trip_algorithm_axi_v1_0;

architecture arch_imp of trip_algorithm_axi_v1_0 is

  -- Signals for internal connections
  signal ps_to_alg_data_i  : ps_to_alg_dch_array;    
  signal send_data_to_ps_i : std_logic;
  signal alg_to_ps_data_i  : alg_to_ps_dch_array;
  signal alg_to_ps_size_i  : std_logic_vector(31 downto 0);
  signal adc_data_i        : adc_dch_array;
  signal remote_end_i      : std_logic_vector(data_bitlength-1 downto 0);
  signal eoc_i             : std_logic;                                           
  signal manual_trip_i     : std_logic;
  signal enable_trip_i     : std_logic;  
  signal logic_in_i        : std_logic_vector(23 downto 0);
  signal trip_from_SDK_i   : std_logic_vector(31 downto 0);  
  
	-- component declaration
	component trip_algorithm_axi_v1_0_S00_AXI is
		generic (
		C_S_AXI_DATA_WIDTH	: integer	:= 32;
		C_S_AXI_ADDR_WIDTH	: integer	:= 8
		);
		port (
    S_PS_TO_ALG_DCH_ARRAY : inout ps_to_alg_dch_array;    --ilka. vorher out
		S_AXI_ACLK	          : in std_logic;
		S_AXI_ARESETN	        : in std_logic;
		S_AXI_AWADDR	        : in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_AWPROT	        : in std_logic_vector(2 downto 0);
		S_AXI_AWVALID	        : in std_logic;
		S_AXI_AWREADY	        : out std_logic;
		S_AXI_WDATA	          : in std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_WSTRB	          : in std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
		S_AXI_WVALID	        : in std_logic;
		S_AXI_WREADY	        : out std_logic;
		S_AXI_BRESP	          : out std_logic_vector(1 downto 0);
		S_AXI_BVALID	        : out std_logic;
		S_AXI_BREADY	        : in std_logic;
		S_AXI_ARADDR	        : in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_ARPROT	        : in std_logic_vector(2 downto 0);
		S_AXI_ARVALID	        : in std_logic;
		S_AXI_ARREADY	        : out std_logic;
		S_AXI_RDATA	          : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_RRESP	          : out std_logic_vector(1 downto 0);
		S_AXI_RVALID	        : out std_logic;
		S_AXI_RREADY	        : in std_logic
		);
	end component trip_algorithm_axi_v1_0_S00_AXI;

	component trip_algorithm_axi_v1_0_M00_AXIS is
		generic (
		C_M_AXIS_TDATA_WIDTH	: integer	:= 32
		);
		port (
    M_SEND_DATA           : in std_logic;                                                          
    M_ALG_TO_PS_DCH_ARRAY : in alg_to_ps_dch_array;           
    M_ALG_TO_PS_SIZE      : in std_logic_vector(31 downto 0); 
		M_AXIS_ACLK	          : in std_logic;
		M_AXIS_ARESETN	      : in std_logic;
		M_AXIS_TVALID	        : out std_logic;
		M_AXIS_TDATA	        : out std_logic_vector(C_M_AXIS_TDATA_WIDTH-1 downto 0);
		M_AXIS_TSTRB	        : out std_logic_vector((C_M_AXIS_TDATA_WIDTH/8)-1 downto 0);
		M_AXIS_TLAST	        : out std_logic;
		M_AXIS_TREADY	        : in std_logic
  );
  end component trip_algorithm_axi_v1_0_M00_AXIS;
  
  component trip_algorithm is
    port (  
      clk                 : in  std_logic;
      reset_l             : in  std_logic;      
      ps_to_alg_data      : in  ps_to_alg_dch_array;        
      adc_data            : in  adc_dch_array;     
      remote_end          : in std_logic_vector(data_bitlength-1 downto 0);
      trigger             : in  std_logic;  
      manual_trip         : in std_logic;   
      enable_trip         : in std_logic;      
      trip_from_SDK       : in  std_logic_vector(31 downto 0);
      logic_in            : in std_logic_vector(23 downto 0);                        
      trip                : out std_logic_vector(27 downto 0);
      alg_to_ps_data      : out alg_to_ps_dch_array;
      send_data_to_ps     : out std_logic                                  
    );
  end component trip_algorithm;
  
begin

-- Instantiation of Axi Bus Interface S00_AXI
trip_algorithm_axi_v1_0_S00_AXI_inst : trip_algorithm_axi_v1_0_S00_AXI
	generic map (
		C_S_AXI_DATA_WIDTH	=> C_S00_AXI_DATA_WIDTH,
		C_S_AXI_ADDR_WIDTH	=> C_S00_AXI_ADDR_WIDTH
	)
	port map (
		  S_PS_TO_ALG_DCH_ARRAY => ps_to_alg_data_i,
      S_AXI_ACLK	          => s00_axi_aclk,
		  S_AXI_ARESETN	        => s00_axi_aresetn,
		  S_AXI_AWADDR	        => s00_axi_awaddr,
		  S_AXI_AWPROT	        => s00_axi_awprot,
		  S_AXI_AWVALID	        => s00_axi_awvalid,
		  S_AXI_AWREADY	        => s00_axi_awready,
		  S_AXI_WDATA	          => s00_axi_wdata,
		  S_AXI_WSTRB	          => s00_axi_wstrb,
		  S_AXI_WVALID	        => s00_axi_wvalid,
		  S_AXI_WREADY	        => s00_axi_wready,
		  S_AXI_BRESP	          => s00_axi_bresp,
		  S_AXI_BVALID	        => s00_axi_bvalid,
		  S_AXI_BREADY	        => s00_axi_bready,
		  S_AXI_ARADDR	        => s00_axi_araddr,
		  S_AXI_ARPROT	        => s00_axi_arprot,
		  S_AXI_ARVALID	        => s00_axi_arvalid,
		  S_AXI_ARREADY	        => s00_axi_arready,
		  S_AXI_RDATA 	        => s00_axi_rdata,
		  S_AXI_RRESP	          => s00_axi_rresp,
		  S_AXI_RVALID	        => s00_axi_rvalid,
		  S_AXI_RREADY	        => s00_axi_rready
	);

-- Instantiation of Axi Bus Interface M00_AXIS
trip_algorithm_axi_v1_0_M00_AXIS_inst : trip_algorithm_axi_v1_0_M00_AXIS
	generic map (
		  C_M_AXIS_TDATA_WIDTH	=> C_M00_AXIS_TDATA_WIDTH
	)
	port map (
      M_SEND_DATA           => send_data_to_ps_i,                                                          
      M_ALG_TO_PS_DCH_ARRAY => alg_to_ps_data_i,           
      M_ALG_TO_PS_SIZE      => alg_to_ps_size_i, 
		  M_AXIS_ACLK	          => m00_axis_aclk,
		  M_AXIS_ARESETN	      => m00_axis_aresetn,
		  M_AXIS_TVALID	        => m00_axis_tvalid,
		  M_AXIS_TDATA	        => m00_axis_tdata,
		  M_AXIS_TSTRB	        => m00_axis_tstrb,
		  M_AXIS_TLAST	        => m00_axis_tlast,
		  M_AXIS_TREADY	        => m00_axis_tready
	  );

  trip_algorithm_inst : trip_algorithm 
    port map (  
      clk                 => s00_axi_aclk,
      reset_l             => s00_axi_aresetn,
      ps_to_alg_data      => ps_to_alg_data_i,        
      adc_data            => adc_data_i,
      remote_end          => remote_end_i,
      trip_from_SDK       => trip_from_SDK_i,     
      trigger             => eoc_i,                              
      trip                => logic_out,
      logic_in            => logic_in_i,
      alg_to_ps_data      => alg_to_ps_data_i,
      send_data_to_ps     => send_data_to_ps_i,
      manual_trip         => manual_trip_i,
      enable_trip         => enable_trip_i
	);

	-- Add user logic here
  eoc_i <= eoc;
  alg_to_ps_size_i          <=  ps_to_alg_data_i(PS_TO_ALG_ALG_TO_PS_SIZE_IDX);
  trip_from_SDK_i           <= ps_to_alg_data_i(1);
  adc_data_i(ADC_CH_00_IDX) <= adc_dch_00;       
  adc_data_i(ADC_CH_01_IDX) <= adc_dch_01;       
  adc_data_i(ADC_CH_02_IDX) <= adc_dch_02;       
  adc_data_i(ADC_CH_03_IDX) <= adc_dch_03;       
  adc_data_i(ADC_CH_04_IDX) <= adc_dch_04;       
  adc_data_i(ADC_CH_05_IDX) <= adc_dch_05;       
  adc_data_i(ADC_CH_06_IDX) <= adc_dch_06;       
  adc_data_i(ADC_CH_07_IDX) <= adc_dch_07;       
  adc_data_i(ADC_CH_08_IDX) <= adc_dch_08;       
  adc_data_i(ADC_CH_09_IDX) <= adc_dch_09;       
  adc_data_i(ADC_CH_10_IDX) <= adc_dch_10;       
  adc_data_i(ADC_CH_11_IDX) <= adc_dch_11;       
  adc_data_i(ADC_CH_12_IDX) <= adc_dch_12;       
  adc_data_i(ADC_CH_13_IDX) <= adc_dch_13;       
  adc_data_i(ADC_CH_14_IDX) <= adc_dch_14;       
  adc_data_i(ADC_CH_15_IDX) <= adc_dch_15;   
  remote_end_i              <= remote_end;       
  manual_trip_i             <= manual_trip;
  enable_trip_i             <= enable_trip;
  logic_in_i                <= logic_in;
	-- User logic ends

end arch_imp;
