-- ---------------------------------------------------------------------------
--
--    Project:	      Alex
--
--    Filename:       trip_algorithm_axi_M00_AXIS.vhd
--
--    Description:    Sends relevant Alex data from PL to PS
--                    via an AXI Stream. 
--
--    © Copyright 2019 KTH. All rights reserved.
--
-- ---------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.IED_config.all;

entity trip_algorithm_axi_v1_0_M00_AXIS is
	generic (
		-- Users to add parameters here

		-- User parameters ends
		-- Do not modify the parameters beyond this line

		-- Width of S_AXIS address bus. The slave accepts the read and write addresses of width C_M_AXIS_TDATA_WIDTH.
		C_M_AXIS_TDATA_WIDTH	: integer	:= 32
	);
	port (
		-- Users to add ports here
    M_SEND_DATA             : in std_logic;                     -- Order to send the data in the alg_to_ps array                                           
    M_ALG_TO_PS_DCH_ARRAY   : in alg_to_ps_dch_array;           -- Array of data channels to be transferred to PS
    M_ALG_TO_PS_SIZE        : in std_logic_vector(31 downto 0); -- Number of data channels to transmit. ToDo: replace "31" with constant 
		-- User ports ends
    
		-- Do not modify the ports beyond this line

		-- Global ports
		M_AXIS_ACLK	    : in std_logic;
		-- 
		M_AXIS_ARESETN	: in std_logic;
		-- Master Stream Ports. TVALID indicates that the master is driving a valid transfer, A transfer takes place when both TVALID and TREADY are asserted. 
		M_AXIS_TVALID	  : out std_logic;
		-- TDATA is the primary payload that is used to provide the data that is passing across the interface from the master.
		M_AXIS_TDATA	  : out std_logic_vector(C_M_AXIS_TDATA_WIDTH-1 downto 0);
		-- TSTRB is the byte qualifier that indicates whether the content of the associated byte of TDATA is processed as a data byte or a position byte.
		M_AXIS_TSTRB	  : out std_logic_vector((C_M_AXIS_TDATA_WIDTH/8)-1 downto 0);
		-- TLAST indicates the boundary of a packet.
		M_AXIS_TLAST	  : out std_logic;
		-- TREADY indicates that the slave can accept a transfer in the current cycle.
		M_AXIS_TREADY	  : in std_logic
	);
end trip_algorithm_axi_v1_0_M00_AXIS;

architecture implementation of trip_algorithm_axi_v1_0_M00_AXIS is
	                                                                                  
	-- Define the states of state machine                                             
	-- The control state machine oversees the writing of input streaming data to the FIFO,
	-- and outputs the streaming data from the FIFO                                   
	type state is ( IDLE,          -- This is the initial/idle state                    
	                SEND_STREAM);  -- In this state the                               
	                             -- stream data is output through M_AXIS_TDATA        
	-- State variable                                                                 
	signal  mst_exec_state : state;                                                   
	-- Example design FIFO read pointer                                               
	signal read_pointer : integer range 0 to ALG_TO_PS_CHANNELS;                               

	-- AXI Stream internal signals
	--streaming data valid
	signal axis_tvalid	: std_logic;
	--streaming data valid delayed by one clock cycle
	signal axis_tvalid_delay	: std_logic;
	--Last of the streaming data 
	signal axis_tlast	: std_logic;
	--Last of the streaming data delayed by one clock cycle
	signal axis_tlast_delay	: std_logic;
	--FIFO implementation signals
	signal stream_data_out	: std_logic_vector(C_M_AXIS_TDATA_WIDTH-1 downto 0);
	signal tx_en	: std_logic;
	--The master has issued all the streaming data stored in FIFO
	signal tx_done	: std_logic;


begin
	-- I/O Connections assignments

	M_AXIS_TVALID	<= axis_tvalid_delay;
	M_AXIS_TDATA	<= stream_data_out;
	M_AXIS_TLAST	<= axis_tlast_delay;
	M_AXIS_TSTRB	<= (others => '1');

	-- Control state machine implementation                                               
	process(M_AXIS_ACLK)                                                                        
	begin                                                                                       
	  if (rising_edge (M_AXIS_ACLK)) then                                                       
	    if(M_AXIS_ARESETN = '0') then                                                           
	      -- Synchronous reset (active low)                                                     
	      mst_exec_state      <= IDLE;                                                          
	    else                                                                                    
	      case (mst_exec_state) is                                                              
	        when IDLE     =>                                                                    
	          -- The slave starts accepting tdata when                                          
	          -- there tvalid is asserted to mark the                                           
	          -- presence of valid streaming data                                               
            if (M_SEND_DATA = '1') then
	            mst_exec_state <= SEND_STREAM;                                                 
	          end if;                                                                         
	                                                                                            
	        when SEND_STREAM  =>                                                                
	          -- The example design streaming master functionality starts                       
	          -- when the master drives output tdata from the FIFO and the slave                
	          -- has finished storing the S_AXIS_TDATA                                          
	          if (tx_done = '1') then                                                           
	            mst_exec_state <= IDLE;                                                         
	          else                                                                              
	            mst_exec_state <= SEND_STREAM;                                                  
	          end if;                                                                           
	                                                                                            
	        when others    =>                                                                   
	          mst_exec_state <= IDLE;                                                           
	                                                                                            
	      end case;                                                                             
	    end if;                                                                                 
	  end if;                                                                                   
	end process;                                                                                


	--tvalid generation
	--axis_tvalid is asserted when the control state machine's state is SEND_STREAM and
	--number of output streaming data is less than the NUMBER_OF_OUTPUT_WORDS.
	axis_tvalid <= '1' when ( (to_integer(signed(M_ALG_TO_PS_SIZE)) > 0) and 
	                          (mst_exec_state = SEND_STREAM) and
	                          (read_pointer < to_integer(signed(M_ALG_TO_PS_SIZE))) )
	                   else '0';
	                                                                                               
	-- AXI tlast generation                                                                        
	-- axis_tlast is asserted number of output streaming data is NUMBER_OF_OUTPUT_WORDS-1          
	-- (0 to NUMBER_OF_OUTPUT_WORDS-1)                                                             
	axis_tlast <= '1' when ( (to_integer(signed(M_ALG_TO_PS_SIZE)) > 0) and
	                         (read_pointer = to_integer(signed(M_ALG_TO_PS_SIZE))-1) )
	                  else '0';                     
	                                                                                               
	-- Delay the axis_tvalid and axis_tlast signal by one clock cycle                              
	-- to match the latency of M_AXIS_TDATA                                                        
	process(M_AXIS_ACLK)                                                                           
	begin                                                                                          
	  if (rising_edge (M_AXIS_ACLK)) then                                                          
	    if(M_AXIS_ARESETN = '0') then                                                              
	      axis_tvalid_delay <= '0';                                                                
	      axis_tlast_delay <= '0';                                                                 
	    else                                                                                       
	      axis_tvalid_delay <= axis_tvalid;                                                        
	      axis_tlast_delay <= axis_tlast;                                                          
	    end if;                                                                                    
	  end if;                                                                                      
	end process;                                                                                   


	--read_pointer pointer

	process(M_AXIS_ACLK)                                                       
	begin                                                                            
	  if (rising_edge (M_AXIS_ACLK)) then                                            
	    if(M_AXIS_ARESETN = '0') then                                                
	      read_pointer <= 0;                                                         
	      tx_done  <= '0';                                                           
	    else 
	      if (to_integer(signed(M_ALG_TO_PS_SIZE)) > 0) then                                                                        
          if (read_pointer < to_integer(signed(M_ALG_TO_PS_SIZE))) then                         
            if (tx_en = '1') then                                                    
              -- read pointer is incremented after every read from the FIFO          
              -- when FIFO read signal is enabled.                                   
              read_pointer <= read_pointer + 1;                                      
              tx_done <= '0';                                                        
            end if;                                                                  
          elsif ((read_pointer = to_integer(signed(M_ALG_TO_PS_SIZE))) and (tx_done = '0')) then                         
            -- tx_done is asserted when NUMBER_OF_OUTPUT_WORDS numbers of streaming data
            -- has been out.                                                         
            tx_done <= '1';
          elsif ((read_pointer = to_integer(signed(M_ALG_TO_PS_SIZE))) and (tx_done = '1')) then
              read_pointer <= 0; 
              tx_done <= '0';                                                                                                                          
          end  if;
        end if;                                                                   
	    end  if;                                                                     
	  end  if;                                                                       
	end process;                                                                     


	--FIFO read enable generation 

	tx_en <= M_AXIS_TREADY and axis_tvalid;                                   
	                                                                                
	-- FIFO Implementation                                                          
	                                                                                
	-- Streaming output data is read from FIFO                                      
	  process(M_AXIS_ACLK)                                                          
	  begin                                                                         
	    if (rising_edge (M_AXIS_ACLK)) then                                         
	      if(M_AXIS_ARESETN = '0') then                                             
	        stream_data_out <= (others => '0');  
	      elsif (tx_en = '1') then -- && M_AXIS_TSTRB(byte_index)                   
	        stream_data_out <= M_ALG_TO_PS_DCH_ARRAY(read_pointer);
	      end if;                                                                   
	    end if;                                                                    
	  end process;                                                                 

	-- Add user logic here

	-- User logic ends

end implementation;
