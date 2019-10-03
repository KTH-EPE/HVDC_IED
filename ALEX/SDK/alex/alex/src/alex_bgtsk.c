/******************************************************************************

   Project:	       Alex

   Filename:       alex_bgtsk.c

   Description:    The file implements the background task of the Alex 
                   application.

   © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

/***************************** Include Files *********************************/
#include "xgpiops.h"
#include "alex_param.h"
#include "alex_bgtsk.h"
#include "alex_hstsk.h"
#include "xuartps.h"

/************************** Constant Definitions *****************************/
#define LED_OFF    0
#define LED_ON     1

/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
/******************** Local variable definitions *****************************/
static XGpioPs sGpioPs;     // Instance of the GpioPs

/******************** Global variable definitions ****************************/
/*************************** Global functions ********************************/

/******************************************************************************
*
* Alex_BackgroundTaskInit
*
* Initialization of the Alex background task
*
* @param	None
*		- XST_SUCCESS if successful
*		- XST_FAILURE otherwise
* @note		None.
*
******************************************************************************/
s32 Alex_BackgroundTaskInit(void)
{
	int status;
	XGpioPs_Config* gpioConfig;

	/* Setup LED IO */
	gpioConfig = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
	status = XGpioPs_CfgInitialize(&sGpioPs, gpioConfig, gpioConfig->BaseAddr);

	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	XGpioPs_SetDirectionPin(&sGpioPs, 7, 1);

	return XST_SUCCESS;
}

/******************************************************************************
*
* Alex_BackgroundTask()
*
* The background task is intended for running  functions that are not time 
* critical.
* The background tasks is meant to run forever and never to return from.
*
* @param	None
* @return None
* @note		None.
*
******************************************************************************/
void Alex_BackgroundTask(void)
{
	while(1)
	{
		// Toggle Zedboard PS LED
    Alex_TogglePsLed();

    // Add the tasks that should run in the background here
    // Check if there has been a datalog read command received from MATLAB UART
    Alex_DatalogToMatlabUart();
	}
}

/******************************************************************************
*
* Alex_TogglePsLed()
*
* Toggles the PS LED
*
* @param	None
* @return None
* @note		None
*
******************************************************************************/
void Alex_TogglePsLed(void)
{
	static u32 hstskCntOld = 0;
	static int ledState = LED_OFF;

	if (Alex_GetHstskCnt() - hstskCntOld > ALEX_PARAM_LED_TOGGLE_CNT)
	{
		if (ledState == LED_OFF)
		{
			XGpioPs_WritePin(&sGpioPs, 7, 1);
			ledState = LED_ON;
		}
		else
		{
			XGpioPs_WritePin(&sGpioPs, 7, 0);
			ledState = LED_OFF;
		}
		hstskCntOld = Alex_GetHstskCnt();
	}
}







