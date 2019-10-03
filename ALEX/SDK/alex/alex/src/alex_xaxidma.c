/******************************************************************************

   Project:	      Alex

   Filename:      alex_xaxidma.c

   Description:   Initialization of Xilinx AXI DMA IP
                  Definition of the AXI DMA receive interrupt 

    © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

/***************************** Include Files *********************************/
#include "xaxidma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "xscugic.h"
#include "xgpiops.h"
#include "xil_printf.h"
#include "alex_param.h"
#include "alex_xaxidma.h"
#include "alex_hstsk.h"
#include "alex_bgtsk.h"

extern XGpioPs sGpioPs;     // Instance of the GpioPs. For debugging

/************************** Constant Definitions *****************************/
#define DMA_DEV_ID		        XPAR_AXIDMA_0_DEVICE_ID
#define RESET_TIMEOUT_COUNTER	10000  // Timeout loop counter for reset

/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
/******************** File local variable definitions ************************/
static XAxiDma sAxiDma;		  // Instance of the XAxiDma
volatile int sRxCnt;        // Debug variable
volatile int sError;        // Flag indicating that the error interrupt has been asserted

// The RxBuffer, placed in the datlgvars section
static u8 sRxBuffer[ALEX_PARAM_MAX_PKT_LEN] __attribute__((section(".datlgvars.pl")))  __attribute__((aligned(4)));

/******************************************************************************
*
* Alex_XAxidmaInit
*
*	Initialize the DMA engine
*	Setup of the Rx channel
*	Setup of the first AXI DMA transfer from PL to PS
*
* @param	None
*
* @return
*		- XST_SUCCESS if example finishes successfully
*		- XST_FAILURE if example fails.
*
* @note		None.
*
******************************************************************************/
int Alex_XAxidmaInit(void)
{
	int status;
	XAxiDma_Config* config;
	u8* rxBufferPtr = sRxBuffer;

	config = XAxiDma_LookupConfig(DMA_DEV_ID);
	if (!config)
	{
		xil_printf("No config found for %d\r\n", DMA_DEV_ID);
		return XST_FAILURE;
	}

	/* Initialize DMA engine */
	status = XAxiDma_CfgInitialize(&sAxiDma, config);

	if (status != XST_SUCCESS)
	{
		xil_printf("Initialization failed %d\r\n", status);
		return XST_FAILURE;
	}

	if(XAxiDma_HasSg(&sAxiDma))
	{
		xil_printf("Device configured as SG mode \r\n");
		return XST_FAILURE;
	}

	/* Disable all interrupts before setup */
	XAxiDma_IntrDisable(&sAxiDma, XAXIDMA_IRQ_ALL_MASK,
						XAXIDMA_DMA_TO_DEVICE);

	XAxiDma_IntrDisable(&sAxiDma, XAXIDMA_IRQ_ALL_MASK,
				        XAXIDMA_DEVICE_TO_DMA);

	/* Enable all interrupts */
	XAxiDma_IntrEnable(&sAxiDma, XAXIDMA_IRQ_ALL_MASK,
							XAXIDMA_DMA_TO_DEVICE);

	XAxiDma_IntrEnable(&sAxiDma, XAXIDMA_IRQ_ALL_MASK,
							XAXIDMA_DEVICE_TO_DMA);

	/* Initialize flags before start */
	sRxCnt = 0;
	sError = 0;

	// Setup the RX channel for the receival of the first package
	status = XAxiDma_SimpleTransfer(&sAxiDma,(u32)rxBufferPtr,
					                ALEX_PARAM_MAX_PKT_LEN, XAXIDMA_DEVICE_TO_DMA);

	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************
*
* This is the AXI DMA RX interrupt handler function
*
* It gets the interrupt status from the hardware, acknowledges it, and if any
* error happens, it resets the hardware.
*
* @param	callback is a pointer to RX channel of the DMA engine.
* @return	None.
* @note		None.
*
******************************************************************************/
void Alex_XAxidmaRxIntrHandler(void* callback)
{
	int status;
	u32 irqStatus;
	int timeOut;
  u8* rxBufferPtr = sRxBuffer;
	XAxiDma* axiDmaInst = (XAxiDma *)callback;
  
	// For debugging, when measuring the execution time of the interrupt
  // Note: Enabling the toggling of this pin in the begining and the end of the
  //       interrupt does somehow does somehow disturb the dmaps transfers
  // ToDo: Investigate why
	// XGpioPs_WritePin(&sGpioPs, 7, 1);

	/* Read pending interrupts */
	irqStatus = XAxiDma_IntrGetIrq(axiDmaInst, XAXIDMA_DEVICE_TO_DMA);

	/* Acknowledge pending interrupts */
	XAxiDma_IntrAckIrq(axiDmaInst, irqStatus, XAXIDMA_DEVICE_TO_DMA);

	/*
	 * If no interrupt is asserted, we do not do anything
	 */
	if (!(irqStatus & XAXIDMA_IRQ_ALL_MASK))
	{
		// ToDo: Handle this error.
		return;
	}

	// Invalidate the RX buffer after the completion of the DMA transfer
	Xil_DCacheInvalidateRange((UINTPTR)rxBufferPtr, ALEX_PARAM_MAX_PKT_LEN);

	/*
	 * If error interrupt is asserted, raise error flag, reset the
	 * hardware to recover from the error, and return with no further
	 * processing.
	 */
	if ((irqStatus & XAXIDMA_IRQ_ERROR_MASK))
	{

		sError = 1;

		// ToDo: Handle this error.

		/* Xilinx comment:
		 * Reset could fail and hang
		 * NEED a way to handle this or do not call it??
		 */
		XAxiDma_Reset(axiDmaInst);

		timeOut = RESET_TIMEOUT_COUNTER;

		while (timeOut) 
    {
			if(XAxiDma_ResetIsDone(axiDmaInst)) 
      {
				break;
			}

			timeOut -= 1;
		}
		return;
	}

	/*
	 * If completion interrupt is asserted, call the Alex high-speed task
	 */
	if ((irqStatus & XAXIDMA_IRQ_IOC_MASK))
	{

		// Call the Alex high-speed task function
		Alex_HighSpeedTask();

		// Increment the RX counter (for debugging only)
		sRxCnt++;
    
	}

	// Setup the RX channel for the receival of the next package
	status = XAxiDma_SimpleTransfer(&sAxiDma,(u32)rxBufferPtr,
					                ALEX_PARAM_MAX_PKT_LEN, XAXIDMA_DEVICE_TO_DMA);
	if (status != XST_SUCCESS)
	{
		// ToDo: Handle this error.
		return;
	}

	// For debugging, when measuring the execution time of the interrupt
	// XGpioPs_WritePin(&sGpioPs, 7, 0);

}

/*****************************************************************************
*
* Returns the to the active instance of the XAxiDma struct 
*
* @param	None
* @return	XAxeDma* | a pointer to active instance of the XAxiDma struct
* @note		None.
*
******************************************************************************/
XAxiDma* Alex_GetXAxiDmaPtr(void)
{
  return(&sAxiDma);
}
