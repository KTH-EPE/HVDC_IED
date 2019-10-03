/******************************************************************************

   Project:	      Alex

   Filename:      alex_param.h

   Description:   Header file containing parameter definitions

   © Copyright 2019 KTH. All rights reserved.

******************************************************************************/
#ifndef ALEX_PARAM_H		/* prevent circular inclusions */
#define ALEX_PARAM_H		/* by using protection macros */

/***************************** Include Files *********************************/
/************************** Constant Definitions *****************************/
#define FPGA0_FREQ                      100000000    // This definition can be found in ps7_init.h
                                                     // but SDK does not include this folder by default
                                                     // so I make a separate define for it. 

#define ALEX_PARAM_ADC_SAMPLE_FREQ       100000      // 2*Sample frequency for the ADC. there is a factor 2 in the vivado project resulting in 50kHz
// FPGA clock cycles per desired ADC sample period
#define ALEX_PARAM_ADC_SAMPLE_PERIOD_CNT (FPGA0_FREQ/ALEX_PARAM_ADC_SAMPLE_FREQ)
// Size of DMA transfer from PL to PS, number of 32-bit words
#define ALEX_PARAM_DMA_SIZE              64           
// Size of DMA transfer from PL to PS, number of bytes
#define ALEX_PARAM_MAX_PKT_LEN		      (64*4)       
// Datalog length in micro-seconds.
#define ALEX_PARAM_DATALOG_LENGTH_US     8000         
#define ALEX_PARAM_PRE_TRIGGER           40           // Length of datalog pre-trigger, in percent
// Number of high-speed sample counts per toggling of the PS LED
#define ALEX_PARAM_LED_TOGGLE_CNT        (ALEX_PARAM_ADC_SAMPLE_FREQ/2)

/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/

#endif
