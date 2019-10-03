/******************************************************************************

   Project:	      Alex

   Filename:      alex_hstsk.h

   Description:   Header file for alex_hstsk.c

    © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

#ifndef ALEX_HSTSK_H		/* prevent circular inclusions */
#define ALEX_HSTSK_H		/* by using protection macros */

/***************************** Include Files *********************************/
/************************** Constant Definitions *****************************/
/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
void Alex_HighSpeedTask(void);
u32 Alex_GetHstskCnt(void);

#endif
