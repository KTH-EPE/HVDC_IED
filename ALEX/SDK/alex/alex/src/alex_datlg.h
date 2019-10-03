/******************************************************************************

   Project:	      Alex

   Filename:      alex_datlg.h

   Description:   Header file for alex_datlg.c

   © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

#ifndef ALEX_DATLG_H		/* prevent circular inclusions */
#define ALEX_DATLG_H		/* by using protection macros */

/***************************** Include Files *********************************/
/************************** Constant Definitions *****************************/
/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
s32 Alex_DatalogInit(void);
s32 Alex_LogData(void);
void Alex_DatalogDmaDoneHandler(unsigned int Channel, 
                                XDmaPs_Cmd *DmaCmd, 
                                void *CallbackRef);
XDmaPs* Alex_GetXDmaPsPtr(void);

#endif
