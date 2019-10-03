/******************************************************************************

   Project:	      Alex

   Filename:      alex_xaxidma.h

   Description:   Header file for alex_xaxidma.c

    © Copyright 2019 KTH. All rights reserved.

******************************************************************************/
#ifndef ALEX_XAXIDMA_H		/* prevent circular inclusions */
#define ALEX_XAXIDMA_H		/* by using protection macros */

/***************************** Include Files *********************************/
/************************** Constant Definitions *****************************/
/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
int Alex_XAxidmaInit(void);
void Alex_XAxidmaRxIntrHandler(void* callback);
XAxiDma* Alex_GetXAxiDmaPtr(void);

#endif
