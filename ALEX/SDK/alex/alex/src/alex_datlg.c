/******************************************************************************

   Project:	      Alex

   Filename:      alex_datlg.c

   Description:    Data logging

   © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

/***************************** Include Files *********************************/
#include "xparameters.h"
#include "xil_types.h"
#include "xil_cache.h"
#include "xdmaps.h"
#include "alex_param.h"
#include "alex_datlg.h"
#include "alex_time.h"

/****************** Declaration of external linker variables *****************/
extern u32 __datlgvars_start;
extern u32 __datlgvars_end;
extern u32 __datlg_start;
extern u32 __datlg_end;

/************************** Constant Definitions *****************************/
#define DMA_DEVICE_ID 			XPAR_XDMAPS_1_DEVICE_ID
#define DMA_CHANNEL_0 			0

/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
/******************** Local variable definitions *****************************/
static XDmaPs sDmaPs;
static u32    sDatalogVarsSize;   // Size, in bytes, of the datalog variables
static u32    sIdx;               // Datalog index

static u32    sMaxIdx;       // The maximum possible datalog index,
                             // based on the size of the memory available
                             // for datalogging.

static u32    sEndIdx;       // The maximum index of the datalog, based
                             // on the desired length of the log.

static u32    sPostTriggerSamples; // Number of samples to continue logging
                                   // after a trig.

static volatile u32  sDmaDone;   // Flag indicating that the DMA transfer is done

static u32    sDmaDoneCnt;       // Number of successful DMA transfers. For debugging.

static u32    sStop;             // Flag indicating that the datalogging shall be stopped

static u32    sPostTriggerCnt;   // Counter used for stopping the datalogging a configurable
                                 // time (given by the desired pre-trigger) after the stop order.

static u32    sRunning;          // Flag indicating that the datalogging is active

static u32    sHasWrappedAround; // Flag indicating that the datalogging has wrapped around at least once

// Datalog header information
static u32      sHeaderDatalogSize    __attribute__((section(".datlgheadervars")))  __attribute__((aligned(4)));
static u32      sHeaderNumSigs        __attribute__((section(".datlgheadervars")))  __attribute__((aligned(4)));
static u32      sHeaderFirstIdx       __attribute__((section(".datlgheadervars")))  __attribute__((aligned(4)));
static u32      sHeaderSampleFreq     __attribute__((section(".datlgheadervars")))  __attribute__((aligned(4)));
static Xuint64  sHeaderLastSampleTime __attribute__((section(".datlgheadervars")))  __attribute__((aligned(4)));

/******************** Global variable definitions ****************************/
/****************************** Local functions ******************************/

/*****************************************************************************
*
* DatalogDmaDoneHandler.
*
* @param	Channel is the Channel number.
* @param	DmaCmd is the Dma Command.
* @param	CallbackRef is the callback reference data.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void DatalogDmaDoneHandler(unsigned int Channel,
                                  XDmaPs_Cmd* dmaCmd,
                                  void* dmaDonePtr)
{
  // Indicate that the DMA transfer has been successful
	*((u32*)(dmaDonePtr)) = 1;
	sDmaDoneCnt++;
}

/******************************************************************************
*
* Alex_DatalogInit
*
* Initialization of the datalogging.
*
* @param	None
* @return XST_SUCCESS if the initialization was successful, otherwise 
*         XST_FAILURE.
* @note		None.
*
******************************************************************************/
s32 Alex_DatalogInit(void)
{
	XDmaPs_Config* dmaCfg;
	XDmaPs*        dmaPsPtr = &sDmaPs;
  s32            status;
	/*
	 * Initialize the DMA Driver
	 */
	dmaCfg = XDmaPs_LookupConfig(DMA_DEVICE_ID);
	if (dmaCfg == NULL)
	{
		return XST_FAILURE;
	}

	status = XDmaPs_CfgInitialize(dmaPsPtr,
				   dmaCfg,
				   dmaCfg->BaseAddress);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}  

	// Initialize the dmaDone flag and the dmaDone counter
	sDmaDone = 1;
	sDmaDoneCnt = 0;

	// Set the Done interrupt handler
	XDmaPs_SetDoneHandler(dmaPsPtr, DMA_CHANNEL_0, DatalogDmaDoneHandler,
					              (void *)(&sDmaDone));

  // Calculate the size of the datalog variables section
	sDatalogVarsSize = (u32)(&__datlgvars_end - &__datlgvars_start);

	// Initialize the datalog index
	sIdx = 0;

	// Calculate the maximum number of samples that can be stored in the datalog
	sMaxIdx = ( (u32)(&__datlg_end - &__datlg_start)) / sDatalogVarsSize - 1;

	// Calculate the maximum index needed for the desired datalog length
  sEndIdx = (ALEX_PARAM_DATALOG_LENGTH_US * ALEX_PARAM_ADC_SAMPLE_FREQ) / 1000000 - 1;

  // Make sure that the desired datalog lenght can be accommodated within the available
  // datalog memory.
  if(sEndIdx > sMaxIdx)
	{
		return XST_FAILURE;
	}

  // Calculate, based on the value of the pre-trigger,
  // how many sample that shall be stored after the datalog has been trigged
  sPostTriggerSamples = (100-ALEX_PARAM_PRE_TRIGGER) * (sEndIdx + 1) / 100;

  if(sPostTriggerSamples > sMaxIdx)
	{
		// ToDo: Implement fault handling functionality
		//       here or in the calling function.
		return XST_FAILURE;
	}

	// Write some datalog header information already at "init"
  sHeaderDatalogSize = (u32)(&__datlg_end - &__datlg_start);
	sHeaderNumSigs = (u32)(&__datlgvars_end - &__datlgvars_start);
	sHeaderSampleFreq = ALEX_PARAM_ADC_SAMPLE_FREQ;
  
  // Activate the datalogging from startup
  sRunning = 1; 

	return XST_SUCCESS;

}

/*****************************************************************************
* Alex_LogData
*
* Transfers the data that should be logged to the datalog
*
* @param	None
* @return None
* @note		None.
*
******************************************************************************/
s32 Alex_LogData(void)
{
	s32           status;
	XDmaPs_Cmd    dmaCmd;
	u32           tmp;

  if (sRunning == 1)
  {

		// Stop the datalogging, after the post trig samples have elapsed
		if(sStop)
		{
			sPostTriggerCnt++;
			if(sPostTriggerCnt == sPostTriggerSamples)
			{
				// Save datalog header information
				tmp = sIdx + 1;
				if(tmp > sEndIdx)
				{
					tmp = 0;
				}
				sHeaderFirstIdx = tmp;
				Alex_GetTime((Xuint64*)(&sHeaderLastSampleTime));
				sRunning = 0;
			}
		}

		// Check that the previous DMA transfer was successful
		if ( sDmaDone != 1)
		{
			// ToDo: Implement fault handling functionality
			//       here or in the calling function.
			return XST_FAILURE;
		}

		// Initiate a transfer of the current sample of logged signals to the datalog
		memset(&dmaCmd, 0, sizeof(XDmaPs_Cmd));
		dmaCmd.ChanCtrl.SrcBurstSize = 4;
		dmaCmd.ChanCtrl.SrcBurstLen = 4;
		dmaCmd.ChanCtrl.SrcInc = 1;
		dmaCmd.ChanCtrl.DstBurstSize = 4;
		dmaCmd.ChanCtrl.DstBurstLen = 4;
		dmaCmd.ChanCtrl.DstInc = 1;
		dmaCmd.BD.SrcAddr = (u32)(&__datlgvars_start);
		dmaCmd.BD.DstAddr = (u32)(&__datlg_start + sIdx*sDatalogVarsSize);
		dmaCmd.BD.Length = sDatalogVarsSize * 4;  // "4" = U32 to U8

		sDmaDone = 0;
		status = XDmaPs_Start(&sDmaPs, DMA_CHANNEL_0, &dmaCmd, 0);
		if (status != XST_SUCCESS)
		{
			// ToDo: Implement fault handling functionality
			//       here or in the calling function.
			return XST_FAILURE;
		}

		// Increment datalog index counter and handle wrap around
		sIdx ++;
		if(sIdx > sEndIdx)
		{
			sIdx = 0;
			sHasWrappedAround = 1;
		}

  } // if (sRunning ...

	return XST_SUCCESS;
}

/*****************************************************************************
*
* Returns the to the active instance of the XDmaPs struct 
*
* @param	None
* @return	XAxeDma* | a pointer to active instance of the XDmaPs struct
* @note		None.
*
******************************************************************************/
XDmaPs* Alex_GetXDmaPsPtr(void)
{
  return(&sDmaPs);
}

/*****************************************************************************
* Alex_DatalogStop
*
* Stop the datalogging (after the post trigger samples have elapsed)
*
* @param	None
* @return None
* @note		This function should be called from the same task as
*         Alex_LogData is called.
*
******************************************************************************/
void Alex_DatalogStop(void)
{
  if(sHasWrappedAround)
  {
  	sStop = 1;
  }
}

/*****************************************************************************
* Alex_DatalogRun
*
* Trigs a datalog
*
* @param	None
* @return None
* @note		This function should be called from the same task as
*         Alex_LogData is called.
*
******************************************************************************/
void Alex_DatalogRun(void)
{
  sIdx = 0;
  sStop = 0;
  sPostTriggerCnt = 0;
  sHasWrappedAround = 0;
  sDmaDone = 1;
  sRunning = 1;
}
