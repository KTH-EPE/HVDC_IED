/******************************************************************************

   Project:	      Alex

   Filename:      alex_hstsk.c

   Description:   The high-speed task of the Alex application

    © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

/***************************** Include Files *********************************/
#include "xil_types.h"
#include "xdmaps.h"
#include "alex_hstsk.h"
#include "alex_tral1pos.h"
#include "alex_tral2pos.h"
#include "alex_tral3pos.h"
#include "alex_tral1neg.h"
#include "alex_tral2neg.h"
#include "alex_tral3neg.h"
#include "alex_datlg.h"

/************************** Constant Definitions *****************************/
/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
/******************** File local variable definitions ************************/
volatile u32 sHighSpeedTaskCnt;  // High speed task counter
volatile u32 counter = 0;
/**************************** Local functions ********************************/
/*************************** Global functions ********************************/

/*****************************************************************************
*
* Alex_HighSpeedTask
*
* This function implements the high-speed task of the Alex application software.
* As such it is called once every high-speed interrupt, when the DMA transfer from
* PL to PS has completed.
*
*	The function calls other Alex functions such as:
*	The trip algorithms
*	The data logging functions
*
* @param	None
* @return None
* @note		None.
*
******************************************************************************/
void Alex_HighSpeedTask(void)
{
  
  // Increment the high-speed task counter
  sHighSpeedTaskCnt++;


  Alex_TripAlgorithm1pos(); //link 1 pos pole
  Alex_TripAlgorithm2pos(); //link 2 pos pole
  Alex_TripAlgorithm3pos(); //link 3 pos pole
  Alex_TripAlgorithm1neg(); //link 1 neg pole
  Alex_TripAlgorithm2neg(); //link 2 neg pole
  Alex_TripAlgorithm3neg(); //link 3 neg pole

  // Add call to data logging functions
  Alex_LogData();

}

/*****************************************************************************
*
* Alex_GetHstskCnt
*
* Returns the value of the high-speed task counter
*
* @param	None
* @return The value of the high-speed task counter
* @note		None.
*
******************************************************************************/
u32 Alex_GetHstskCnt(void)
{
  return(sHighSpeedTaskCnt);
}
