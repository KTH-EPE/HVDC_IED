/******************************************************************************

   Project:	      Alex

   Filename:      alex_time.c

   Description:   Routines for handling the Alex time

   © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

/***************************** Include Files *********************************/
#include "xil_types.h"
#include "alex_time.h"

/************************** Constant Definitions *****************************/
/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
/******************** Local variable definitions *****************************/
/******************** Global variable definitions ****************************/
/*****************************************************************************
*
* Alex_GetTime
*
* Returns the current Alex time
*
* @param	None
* @return None
* @note		None.
*
******************************************************************************/
void Alex_GetTime(Xuint64* timePtr)
{
	timePtr->Lower = 123456;
	timePtr->Upper = 789;
}

