/******************************************************************************

   Project:	       Alex

   Filename:       alex_bgtsk.h

   Description:    Header file for alex_bgtsk.c

    © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

#ifndef ALEX_BGTSK_H		/* prevent circular inclusions */
#define ALEX_BGTSK_H		/* by using protection macros */

/***************************** Include Files *********************************/
/************************** Constant Definitions *****************************/
/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
void Alex_BackgroundTask(void);
s32 Alex_BackgroundTaskInit(void);
void Alex_TogglePsLed(void);

#endif
