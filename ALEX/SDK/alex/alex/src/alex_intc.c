/******************************************************************************

   Project:	      Alex

   Filename:      alex_intc.c

   Description:   Routines for initialization of the interrupt controller

   © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

/***************************** Include Files *********************************/
#include "xaxidma.h"
#include "xdmaps.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "xscugic.h"
#include "xil_printf.h"
#include "alex_xaxidma.h"
#include "alex_datlg.h"

/************************** Constant Definitions *****************************/
/*
 * Device hardware build related constants.
 */
#define INTC_DEVICE_ID      XPAR_PS7_SCUGIC_0_DEVICE_ID

#define DMA_DEVICE_ID 			XPAR_XDMAPS_1_DEVICE_ID

#define DMA_DONE_INTR_0			XPAR_XDMAPS_0_DONE_INTR_0
#define DMA_DONE_INTR_1			XPAR_XDMAPS_0_DONE_INTR_1
#define DMA_DONE_INTR_2			XPAR_XDMAPS_0_DONE_INTR_2
#define DMA_DONE_INTR_3			XPAR_XDMAPS_0_DONE_INTR_3
#define DMA_DONE_INTR_4			XPAR_XDMAPS_0_DONE_INTR_4
#define DMA_DONE_INTR_5			XPAR_XDMAPS_0_DONE_INTR_5
#define DMA_DONE_INTR_6			XPAR_XDMAPS_0_DONE_INTR_6
#define DMA_DONE_INTR_7			XPAR_XDMAPS_0_DONE_INTR_7
#define DMA_FAULT_INTR			XPAR_XDMAPS_0_FAULT_INTR


/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
int Alex_SetupIntrSystem(void);

/******************** File local variable definitions ************************/
static XScuGic sIntc;	    /* Instance of the Interrupt Controller */

/*****************************************************************************/
/*
*
* This function setups the interrupt system so interrupts can occur for the
* DMA, it assumes INTC component exists in the hardware system.
*
* @param	intcInstancePtr is a pointer to the instance of the INTC.
* @param	axiDmaPtr is a pointer to the instance of the DMA engine
* @param	rxIntrId is the RX channel Interrupt ID.
*
* @return
*		- XST_SUCCESS if successful,
*		- XST_FAILURE.if not succesful
*
* @note		None.
*
******************************************************************************/
int Alex_SetupIntrSystem(void)
{
	int status;
	XScuGic_Config* intcConfig;
  XAxiDma*        axiDmaPtr        = Alex_GetXAxiDmaPtr();
  XDmaPs*         dmaPsPtr         = Alex_GetXDmaPsPtr();

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	intcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == intcConfig)
	{
		return XST_FAILURE;
	}

	status = XScuGic_CfgInitialize(&sIntc, intcConfig,
					                       intcConfig->CpuBaseAddress);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	// Initialize the interrupts related to AXI DMA
  
  XScuGic_SetPriorityTriggerType(&sIntc, XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR, 0xA0, 0x3);
	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	status = XScuGic_Connect(&sIntc, 
                           XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR,
				                   (Xil_InterruptHandler)Alex_XAxidmaRxIntrHandler,
				                   axiDmaPtr);
	if (status != XST_SUCCESS)
	{
		return status;
	}
	XScuGic_Enable(&sIntc, XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR);

	// Initialize the interrupts related to DMA PS
  
	// Connect the Fault ISR
	status = XScuGic_Connect(&sIntc,
				                   XPAR_XDMAPS_0_FAULT_INTR,
				                   (Xil_InterruptHandler)XDmaPs_FaultISR,
				                   (void *)dmaPsPtr);

	if (status != XST_SUCCESS)
  {
		return XST_FAILURE;
	}

	// Connect the Done ISR
	status = XScuGic_Connect(&sIntc,
				                   XPAR_XDMAPS_0_DONE_INTR_0,
				                   (Xil_InterruptHandler)XDmaPs_DoneISR_0,
				                   (void *)dmaPsPtr);

	if (status != XST_SUCCESS)
  {
		return XST_FAILURE;
  }

	XScuGic_Enable(&sIntc, XPAR_XDMAPS_0_FAULT_INTR);
	XScuGic_Enable(&sIntc, DMA_DONE_INTR_0);

	// Enable interrupts from the hardware
	Xil_ExceptionInit();
  
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			                        (Xil_ExceptionHandler)XScuGic_InterruptHandler, 
			                        (void *)&sIntc);

	Xil_ExceptionEnable();

	return XST_SUCCESS;
}
