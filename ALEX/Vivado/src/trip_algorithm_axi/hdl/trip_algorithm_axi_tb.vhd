-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       trip_algorithm_axi_axi_tb.vhd
--
--    Description:    Testbench for trip_algorithm_axi
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

entity trip_algorithm_axi_tb is
  generic
  (
    C_S00_AXI_DATA_WIDTH   : integer := 32;
    C_S00_AXI_ADDR_WIDTH   : integer := 8;

		C_M00_AXIS_TDATA_WIDTH : integer := 32
  );
end trip_algorithm_axi_tb;

architecture Behavioral of trip_algorithm_axi_tb is

  -- Component Declaration for the Unit Under Test (UUT)
  
  component trip_algorithm_axi_v1_0  
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
      trip_cmd   : out std_logic_vector(nb_breakers-1 downto 0);
      button_trip : in std_logic;
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
  end component;
  
  -- Clock period definitions
  constant CLK_period : time := 10 ns;    
  constant ADC_period : time := 10000 ns;    

   --Inputs and outputs
  signal EOC              : std_logic                                                 := '0';   
  signal ADC_DCH_00       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_01       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_02       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_03       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_04       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_05       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_06       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_07       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_08       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_09       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_10       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_11       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_12       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_13       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_14       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal ADC_DCH_15       : std_logic_vector(data_bitlength-1 downto 0)               := (others => '0');     
  signal TRIP_CMD         : std_logic_vector(nb_breakers-1 downto 0)                  := (others => '0'); 
  signal BUTTON_TRIP      : std_logic                                                 := '0';
	signal S00_AXI_ACLK	    : std_logic                                                 := '0';
	signal S00_AXI_ARESETN	: std_logic                                                 := '0';
	signal S00_AXI_AWADDR	  : std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0)         := (others => '0');
	signal S00_AXI_AWPROT	  : std_logic_vector(2 downto 0)                              := (others => '0');
	signal S00_AXI_AWVALID	: std_logic                                                 := '0';
	signal S00_AXI_AWREADY	: std_logic                                                 := '0';
	signal S00_AXI_WDATA	  : std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0)         := (others => '0');
	signal S00_AXI_WSTRB	  : std_logic_vector((C_S00_AXI_DATA_WIDTH/8)-1 downto 0)     := (others => '0');
	signal S00_AXI_WVALID	  : std_logic                                                 := '0';
	signal S00_AXI_WREADY	  : std_logic                                                 := '0';
	signal S00_AXI_BRESP	  : std_logic_vector(1 downto 0)                              := (others => '0');
	signal S00_AXI_BVALID	  : std_logic                                                 := '0';
	signal S00_AXI_BREADY	  : std_logic                                                 := '0';
	signal S00_AXI_ARADDR	  : std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0)         := (others => '0');
	signal S00_AXI_ARPROT	  : std_logic_vector(2 downto 0)                              := (others => '0');
	signal S00_AXI_ARVALID	: std_logic                                                 := '0';
	signal S00_AXI_ARREADY	: std_logic                                                 := '0';
	signal S00_AXI_RDATA	  : std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0)         := (others => '0');
	signal S00_AXI_RRESP	  : std_logic_vector(1 downto 0)                              := (others => '0');
	signal S00_AXI_RVALID	  : std_logic                                                 := '0';
	signal S00_AXI_RREADY	  : std_logic                                                 := '0';
  signal M00_AXIS_ACLK	  : std_logic                                                 := '0';
  signal M00_AXIS_ARESETN	: std_logic                                                 := '0';
  signal M00_AXIS_TVALID	: std_logic                                                 := '0';
  signal M00_AXIS_TDATA	  : std_logic_vector(C_M00_AXIS_TDATA_WIDTH-1 downto 0)       := (others => '0');
  signal M00_AXIS_TSTRB	  : std_logic_vector((C_M00_AXIS_TDATA_WIDTH/8)-1 downto 0)   := (others => '0');
  signal M00_AXIS_TLAST	  : std_logic                                                 := '0';
  signal M00_AXIS_TREADY	: std_logic                                                 := '0';
  
  signal sendIt           : std_logic                                                 := '0';
  signal readIt           : std_logic                                                 := '0';
  
                                                                                     
begin

	-- Instantiate the Unit Under Test (UUT)
  uut : trip_algorithm_axi_v1_0 
    generic map (
		  C_S00_AXI_DATA_WIDTH	 =>  32,
		  C_S00_AXI_ADDR_WIDTH   =>  8,
		  C_M00_AXIS_TDATA_WIDTH =>  32      
    )
    port map (
      eoc              => EOC,                                   
      adc_dch_00       => ADC_DCH_00,
      adc_dch_01       => ADC_DCH_01,
      adc_dch_02       => ADC_DCH_02,
      adc_dch_03       => ADC_DCH_03,
      adc_dch_04       => ADC_DCH_04,
      adc_dch_05       => ADC_DCH_05,
      adc_dch_06       => ADC_DCH_06,
      adc_dch_07       => ADC_DCH_07,
      adc_dch_08       => ADC_DCH_08,
      adc_dch_09       => ADC_DCH_09,
      adc_dch_10       => ADC_DCH_10,
      adc_dch_11       => ADC_DCH_11,
      adc_dch_12       => ADC_DCH_12,
      adc_dch_13       => ADC_DCH_13,
      adc_dch_14       => ADC_DCH_14,
      adc_dch_15       => ADC_DCH_15,                       
      trip_cmd         => TRIP_CMD,   
      button_trip      => BUTTON_TRIP,   
		  s00_axi_aclk	   => S00_AXI_ACLK,	 
		  s00_axi_aresetn	 => S00_AXI_ARESETN,
		  s00_axi_awaddr	 => S00_AXI_AWADDR,	
		  s00_axi_awprot	 => S00_AXI_AWPROT,	
		  s00_axi_awvalid	 => S00_AXI_AWVALID,
		  s00_axi_awready	 => S00_AXI_AWREADY,
		  s00_axi_wdata	   => S00_AXI_WDATA,	 
		  s00_axi_wstrb	   => S00_AXI_WSTRB,	 
		  s00_axi_wvalid	 => S00_AXI_WVALID,	
		  s00_axi_wready	 => S00_AXI_WREADY,	
		  s00_axi_bresp	   => S00_AXI_BRESP,	 
		  s00_axi_bvalid	 => S00_AXI_BVALID,	
		  s00_axi_bready	 => S00_AXI_BREADY,	
		  s00_axi_araddr	 => S00_AXI_ARADDR,	
		  s00_axi_arprot	 => S00_AXI_ARPROT,	
		  s00_axi_arvalid	 => S00_AXI_ARVALID,
		  s00_axi_arready	 => S00_AXI_ARREADY,
		  s00_axi_rdata	   => S00_AXI_RDATA,	 
		  s00_axi_rresp	   => S00_AXI_RRESP,	 
		  s00_axi_rvalid	 => S00_AXI_RVALID,	
		  s00_axi_rready	 => S00_AXI_RREADY,
		  m00_axis_aclk	   => M00_AXIS_ACLK,	  
		  m00_axis_aresetn => M00_AXIS_ARESETN,
		  m00_axis_tvalid	 => M00_AXIS_TVALID,	
		  m00_axis_tdata	 => M00_AXIS_TDATA,	
		  m00_axis_tstrb	 => M00_AXIS_TSTRB,	
		  m00_axis_tlast	 => M00_AXIS_TLAST,	
		  m00_axis_tready	 => M00_AXIS_TREADY	
      	              
    );                 
  
   -- Clock process definitions
  CLK_process : process
  begin
    S00_AXI_ACLK <= '0';
    M00_AXIS_ACLK <= '0';    
    wait for CLK_period / 2;
    S00_AXI_ACLK <= '1';
    M00_AXIS_ACLK <= '1';    
    wait for CLK_period / 2;
  end process;
  
  -- Initiate process which simulates a master wanting to write.
  -- This process is blocked on a "Send Flag" (sendIt).
  -- When the flag goes to 1, the process exits the wait state and
  -- execute a write transaction.
  send : process
  begin
    S00_AXI_AWVALID<='0';
    S00_AXI_WVALID<='0';
    S00_AXI_BREADY<='0';
    loop
      wait until sendIt = '1';
      wait until S00_AXI_ACLK= '0';
        S00_AXI_AWVALID<='1';
        S00_AXI_WVALID<='1';
      wait until (S00_AXI_AWREADY and S00_AXI_WREADY) = '1';  --Client ready to read address/data        
        S00_AXI_BREADY<='1';
      wait until S00_AXI_BVALID = '1';  -- Write result valid
        assert S00_AXI_BRESP = "00" report "AXI data not written" severity failure;
        S00_AXI_AWVALID<='0';
        S00_AXI_WVALID<='0';
        S00_AXI_BREADY<='1';
      wait until S00_AXI_BVALID = '0';  -- All finished
        S00_AXI_BREADY<='0';
    end loop;
  end process send;
  

  -- Initiate process which simulates a master wanting to read.
  -- This process is blocked on a "Read Flag" (readIt).
  -- When the flag goes to 1, the process exits the wait state and
  -- execute a read transaction.
  read : process
  begin
    S00_AXI_ARVALID<='0';
    S00_AXI_RREADY<='0';
     loop
       wait until readIt = '1';
       wait until S00_AXI_ACLK= '0';
         S00_AXI_ARVALID<='1';
         S00_AXI_RREADY<='1';
       wait until (S00_AXI_RVALID and S00_AXI_ARREADY) = '1';  --Client provided data
        assert S00_AXI_RRESP = "00" report "AXI data not written" severity failure;
         S00_AXI_ARVALID<='0';
        S00_AXI_RREADY<='0';
     end loop;
  end process read;

  -- Testbench
  tb : process
  begin

    wait for 1*CLK_period/2;
      S00_AXI_ARESETN<='0';
      M00_AXIS_ARESETN<='0';
      sendIt<='0';
          
    wait for 1*CLK_period;
      S00_AXI_ARESETN<='1';
      M00_AXIS_ARESETN<='1';
      
    -- Send PS_TO_ALG_M_ALG_TO_PS_SIZE to the component via the AXI Lite interface 
    wait for 2*CLK_period;
      -- Write to awaddr = 0 should end up slv_reg0, which is "PS_TO_ALG_M_ALG_TO_PS_SIZE" 
      S00_AXI_AWADDR<=x"00";
      --S00_AXI_WDATA<=x"00000040"; -- 0x40 = 64
      S00_AXI_WDATA<=x"00000011"; -- 0x11 = 17
      S00_AXI_WSTRB<=b"1111";
      sendIt<='1';                    --Start AXI Write to Slave
      wait for 1 ns; sendIt<='0';     --Clear Start Send Flag
    wait until S00_AXI_BVALID = '1';
    wait until S00_AXI_BVALID = '0';  --AXI Write finished
      S00_AXI_WSTRB<=b"0000";
      
    wait for 2*CLK_period;
      M00_AXIS_TREADY <= '1';           -- The AXI Stream slave indicates that it is ready to accept a transfer
    
    wait for 2*CLK_period;
      -- Generate input data from adc_getdata_axi
      ADC_DCH_00 <= std_logic_vector(to_unsigned(0, data_bitlength));  
      ADC_DCH_01 <= std_logic_vector(to_unsigned(1, data_bitlength)); 
      ADC_DCH_02 <= std_logic_vector(to_unsigned(2, data_bitlength)); 
      ADC_DCH_03 <= std_logic_vector(to_unsigned(3, data_bitlength)); 
      ADC_DCH_04 <= std_logic_vector(to_unsigned(4, data_bitlength)); 
      ADC_DCH_05 <= std_logic_vector(to_unsigned(5, data_bitlength)); 
      ADC_DCH_06 <= std_logic_vector(to_unsigned(6, data_bitlength)); 
      ADC_DCH_07 <= std_logic_vector(to_unsigned(7, data_bitlength)); 
      ADC_DCH_08 <= std_logic_vector(to_unsigned(8, data_bitlength));
      ADC_DCH_09 <= std_logic_vector(to_unsigned(9, data_bitlength));
      ADC_DCH_10 <= std_logic_vector(to_unsigned(10, data_bitlength)); 
      ADC_DCH_11 <= std_logic_vector(to_unsigned(11, data_bitlength)); 
      ADC_DCH_12 <= std_logic_vector(to_unsigned(12, data_bitlength)); 
      ADC_DCH_13 <= std_logic_vector(to_unsigned(13, data_bitlength)); 
      ADC_DCH_14 <= std_logic_vector(to_unsigned(14, data_bitlength)); 
      ADC_DCH_15 <= std_logic_vector(to_unsigned(15, data_bitlength));  
      EOC <= '1';              
    
    wait for 1*CLK_period;
      EOC <= '0';
    
    wait for ADC_period;
      EOC <= '1';  
      BUTTON_TRIP <= '1';            
    
    wait for 1*CLK_period;
      EOC <= '0';
         
    wait for ADC_period;
        EOC <= '1';              
      
      wait for 1*CLK_period;
        EOC <= '0';

    wait; -- will wait forever
  end process tb;    
    
end Behavioral;
