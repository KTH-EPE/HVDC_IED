/******************************************************************************

    Project:	      Alex

    Filename:       alex_main.c

    Description:    Main routine for Alex, called after reset.

    © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

/***************************** Include Files *********************************/
#include <stdio.h>
#include "xparameters.h"
#include "xil_io.h"
#include "xaxidma.h"
#include "xdmaps.h"
#include "adc_getdata_axi.h"
#include "trip_algorithm_axi.h"
#include "platform.h"
#include "alex_xaxidma.h"
#include "alex_param.h"
#include "alex_datlg.h"
#include "alex_tral1pos.h"
#include "alex_tral2pos.h"
#include "alex_tral3pos.h"
#include "alex_tral1neg.h"
#include "alex_tral2neg.h"
#include "alex_tral3neg.h"
#include "alex_intc.h"
#include "alex_bgtsk.h"

/**************** Constant Definitions for xparameters.h *********************/
/* Defines to copy to xparameters.h since Vivado does not give is
   correct addresses to custom IPs (A known Vivado issue, apparently).

#define XPAR_TRIP_ALGORITHM_AXI_0_S00_AXI_BASEADDR 0x43C10000
#define XPAR_TRIP_ALGORITHM_AXI_0_S00_AXI_HIGHADDR 0x43C1FFFF
#define XPAR_ADC_GETDATA_AXI_0_S00_AXI_BASEADDR 0x43C00000
#define XPAR_ADC_GETDATA_AXI_0_S00_AXI_HIGHADDR 0x43C0FFFF

*/

/************************** Constant Definitions *****************************/
#define DELAY                 10000000   // Define delay length
#define ENABLE_ADC            1
#define DISABLE_ADC           0
#define INTERRUPT_MODE        1

/************************** Global functions ********************************/

/*****************************************************************************
*
* main
*
* This function implements the main routine of the Alex application software.
* As such it is responsible for initializing the hardware (PS and the ARM9 
* peripherals) and the different software blocks.
*
* @param	None
* @return None
* @note		None.
*
******************************************************************************/
int main()
{
  int status;

	init_platform();

	// Debug printout
  printf("Alex says hello!\n\r");

  // Initialize the background task
  status = Alex_BackgroundTaskInit();
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

  // Initialize datalogging
	status = Alex_DatalogInit();
  if (status != XST_SUCCESS)
  {
    return XST_FAILURE;
  }

	// Initialize trip algorithm
  Alex_TripAlgorithm1posInit();
  Alex_TripAlgorithm2posInit();
  Alex_TripAlgorithm3posInit();
  Alex_TripAlgorithm1negInit();
  Alex_TripAlgorithm2negInit();
  Alex_TripAlgorithm3negInit();

	// Initialize the ADC sampling interval in adc_getdata_axi IP
	ADC_GETDATA_AXI_mWriteReg(XPAR_ADC_GETDATA_AXI_0_S00_AXI_BASEADDR,
					  ADC_GETDATA_AXI_S00_AXI_SLV_REG1_OFFSET,
					  (u32)ALEX_PARAM_ADC_SAMPLE_PERIOD_CNT);

	// Initialize the size of the data to be sent, via DMA,
	// from the trip algorithms in PL to PS.
	TRIP_ALGORITHM_AXI_mWriteReg(XPAR_TRIP_ALGORITHM_AXI_0_S00_AXI_BASEADDR,
						 TRIP_ALGORITHM_AXI_S00_AXI_SLV_REG0_OFFSET,
						 ALEX_PARAM_DMA_SIZE);

  // Setup and initialize the Xilinx AXI DMA IP and the interrupt controller
  status = Alex_XAxidmaInit();
  if (status != XST_SUCCESS)
  {
    return XST_FAILURE;
  }
  
  // Initialize and setup the interrupt controller
  status = Alex_SetupIntrSystem();  
  if (status != XST_SUCCESS)
  {
    return XST_FAILURE;
  }


  // Release the ADC sampling. This will start the DMA transfers from PL to PS
  ADC_GETDATA_AXI_mWriteReg(XPAR_ADC_GETDATA_AXI_0_S00_AXI_BASEADDR,
	                        ADC_GETDATA_AXI_S00_AXI_SLV_REG0_OFFSET,
	                        (u32)ENABLE_ADC);

  // Call the background task.
  // Note: We never return from the background task. It runs "forever".
  Alex_BackgroundTask();
    
  return 0;
}
