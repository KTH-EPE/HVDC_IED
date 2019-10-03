/******************************************************************************

   Project:	      Alex

   Filename:      alex_matlab_uart.h

   Description:   Header file for alex_matlab_uart.c

    © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

#ifndef ALEX_MATLAB_UART_H		/* prevent circular inclusions */
#define ALEX_MATLAB_UART_H		/* by using protection macros */

/***************************** Include Files *********************************/
/************************** Constant Definitions *****************************/
/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
void Alex_DatalogToMatlabUart(void);
void Alex_UartInit_void(void);
#endif
