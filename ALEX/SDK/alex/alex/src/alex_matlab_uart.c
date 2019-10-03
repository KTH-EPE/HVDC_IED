/******************************************************************************

   Project:	      Alex

   Filename:      alex_matlab_uart.c

   Description:   The interface to send the ALEX datalog to MATLAB via UART

    © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

/***************************** Include Files *********************************/
#include "xil_types.h"
#include "xdmaps.h"
#include "alex_matlab_uart.h"
#include "alex_tral1pos.h"
#include "alex_tral1neg.h"
#include "alex_param.h"
#include "xplatform_info.h"
#include "xuartps.h"
#include "xil_exception.h"
#include "xil_printf.h"
/************************** Variable Definitions ***************************/
/************************** Constant Definitions *****************************/
#ifdef XPAR_INTC_0_DEVICE_ID
#include "xintc.h"
#else
#include "xscugic.h"
#endif
#ifdef XPAR_INTC_0_DEVICE_ID
#define INTC		XIntc
#define UART_DEVICE_ID		XPAR_XUARTPS_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID
#define UART_INT_IRQ_ID		XPAR_INTC_0_UARTPS_0_VEC_ID
#else
#define INTC		XScuGic
#define UART_DEVICE_ID		XPAR_XUARTPS_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define UART_INT_IRQ_ID		XPAR_XUARTPS_1_INTR
#endif
#define TEST_BUFFER_SIZE	1

XUartPs UartPs	;		/* Instance of the UART Device */
INTC InterruptController;	/* Instance of the Interrupt Controller */

/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
/******************** File local variable definitions ************************/
volatile int TotalReceivedCount;
volatile int TotalSentCount;
int TotalErrorCount;
int i;
int init = 0;
int send_data = 0;
int count_pause	= 0;

//char RecvBuffer[10] = "";
u32 SendBuffer[1];
/**************************** Local functions ********************************/
static int SetupInterruptSystem(INTC *IntcInstancePtr, XUartPs *UartInstancePtr, u16 UartIntrId);
void Handler(void *CallBackRef, u32 Event, unsigned int EventData);
int Alex_UartInit(INTC *IntcInstPtr, XUartPs *UartInstPtr, u16 DeviceId, u16 UartIntrId);
/*************************** Global functions ********************************/
/****************** Declaration of external linker variables *****************/
extern u32 __datlgvars_start;
extern u32 __datlgvars_end;
extern u32 __datlg_start;
extern u32 __datlg_end;


/*****************************************************************************
*
* Alex_DatalogToMatlabUart
*
* This function implements the interface to send the ALEX datalog to MATLAB.
* As such it is called once the datalog is completed.
*
*	The function calls other Alex functions such as:
*	The data logging functions
*
* @param	None
* @return None
* @note		None.
*
******************************************************************************/
void Alex_DatalogToMatlabUart()
{
	char RecvBuffer[10] = "";
	char threshold_test[4] = "";
	u32 SendBuffer[1];
	int n = 0;

		/*
		 * Initialize the UART driver so that it's ready to use
		 * Look up the configuration in the config table and then initialize it.
		 */
	if (init == 0){
		XUartPs_Config *Config;
		init = 1;
		Config = XUartPs_LookupConfig(UART_DEVICE_ID);
			if (NULL == Config) {
				return XST_FAILURE;
			}
			int Status;
			Status = XUartPs_CfgInitialize(&UartPs, Config, Config->BaseAddress);
			if (Status != XST_SUCCESS) {
				return XST_FAILURE;
		}
	}

	n = XUartPs_Recv(&UartPs, RecvBuffer, 10);
	for (count_pause = 0; count_pause <10000; count_pause++){}
	if (n<=0){return;}

	//variables logged from FPGA:
	u32 offset_trip = 0;

	u32 offset_ch0 =  4;
	u32 offset_ch1 =  8;
	u32 offset_ch2 = 12;
	u32 offset_ch3 = 16;
	u32 offset_ch4 = 20;
	u32 offset_ch5 = 24;
	u32 offset_ch6 = 28;
	u32 offset_ch7 = 32;
	u32 offset_ch8 = 36;
	u32 offset_ch9 = 40;
	u32 offset_ch10 = 44;
	u32 offset_ch11 = 48;
	u32 offset_ch12 = 52;
	u32 offset_ch13 = 56;
	u32 offset_ch14 = 60;
	u32 offset_ch15 = 64;
	//u32 enable trip
	//u32 offset_remote = 72;

	//variables logged from SDK:

	//neg1
	u32 offset_ch8_lpf     					= (65)*4;	//65 - u32 ch8_lpf_out
	u32 offset_ch9_lpf     					= (66)*4; //66 - u32 ch9_lpf_out
	u32 offset_neg1_alg1_crit   			= (67)*4; //67 - u32 alg1_crit
	u32 offset_neg1_alg2_crit   			= (68)*4; //68 - u32 alg2_crit
	u32 offset_neg1_alg3_crit   			= (69)*4; //69 - u32 alg3_crit
	u32 offset_neg1_alg4_crit   			= (70)*4; //70 - u32 alg4_crit
	u32 offset_neg1_alg5_crit   			= (71)*4; //71 - u32 alg5_crit
	//u32 offset_neg1_alg6_crit 			= (72)*4; //72 - u32 alg6_crit
	//u32 offset_neg1_alg7_crit 			= (73)*4; //73 - u32 alg7_crit
	u32 offset_neg1_alg8_crit   			= (74)*4; //74 - u32 alg8_crit
	//u32 offset_neg1_alg9_crit 			= (75)*4; //75 - u32 alg9_crit
	//u32 offset_neg1_alg10_crit			= (76)*4; //76 - u32 alg10_crit
	u32 offset_manual_fault_detected        = (77)*4; //77 - u32 manual_fault_detected
	u32 offset_neg1_fault_detected          = (78)*4; //78 - u32 neg1_fault_detected
	//79 is for counter logging
	//80 is for trip1 logging

	//pos1
	u32 offset_ch0_lpf          					= (81)*4;
	u32 offset_ch1_lpf          					= (82)*4;
	//u32 offset_chr_delayed       					= (83)*4;
	u32 offset_chr_lpf          					= (84)*4;
	u32 offset_pos1_alg1_crit   					= (85)*4;
	u32 offset_pos1_alg2_crit   					= (86)*4;
	u32 offset_pos1_alg3_crit   					= (87)*4;
	u32 offset_pos1_alg4_crit   					= (88)*4;
	u32 offset_pos1_alg5_crit   					= (89)*4;
	u32 offset_pos1_alg6_crit   					= (90)*4;
	u32 offset_pos1_alg7_crit   					= (91)*4;
	u32 offset_pos1_alg8_crit   					= (92)*4;
	u32 offset_pos1_alg9_crit   					= (93)*4;
	u32 offset_pos1_alg10_crit  					= (94)*4;
	u32 offset_pos1_fault_detected                  = (95)*4;
	//96 is for counter logging
	//97 is for trip logging

	//neg2
	u32 offset_ch10_lpf          					= (98)*4;
	u32 offset_ch11_lpf          					= (99)*4;
	u32 offset_neg2_alg1_crit   					= (100)*4;
	u32 offset_neg2_alg2_crit   					= (101)*4;
	u32 offset_neg2_alg3_crit   					= (102)*4;
	u32 offset_neg2_alg4_crit   					= (103)*4;
	u32 offset_neg2_alg5_crit   					= (104)*4;
	//u32 offset_neg2_alg6_crit   					= (105)*4;
	//u32 offset_neg2_alg7_crit   					= (106)*4;
	u32 offset_neg2_alg8_crit   					= (107)*4;
	//u32 offset_neg2_alg9_crit   					= (108)*4;
	//u32 offset_neg2_alg10_crit  					= (109)*4;
	u32 offset_neg2_fault_detected                  = (110)*4;
	//111 is for counter logging
	//112 is for trip logging

	//pos2
	u32 offset_ch2_lpf          					= (113)*4;
	u32 offset_ch3_lpf          					= (114)*4;
	u32 offset_pos2_alg1_crit   					= (115)*4;
	u32 offset_pos2_alg2_crit   					= (116)*4;
	u32 offset_pos2_alg3_crit   					= (117)*4;
	u32 offset_pos2_alg4_crit   					= (118)*4;
	u32 offset_pos2_alg5_crit   					= (119)*4;
	//u32 offset_pos2_alg6_crit   					= (120)*4;
	//u32 offset_pos2_alg7_crit   					= (121)*4;
	u32 offset_pos2_alg8_crit   					= (122)*4;
	//u32 offset_pos2_alg9_crit   					= (123)*4;
	//u32 offset_pos2_alg10_crit  					= (124)*4;
	u32 offset_pos2_fault_detected                  = (125)*4;
	//126 is for counter logging
	//127 is for trip logging

	//neg3
	u32 offset_ch12_lpf          					= (128)*4;
	u32 offset_ch13_lpf          					= (129)*4;
	u32 offset_neg3_alg1_crit   					= (130)*4;
	u32 offset_neg3_alg2_crit   					= (131)*4;
	u32 offset_neg3_alg3_crit   					= (132)*4;
	u32 offset_neg3_alg4_crit   					= (133)*4;
	u32 offset_neg3_alg5_crit   					= (134)*4;
	//u32 offset_neg3_alg6_crit   					= (135)*4;
	//u32 offset_neg3_alg7_crit   					= (136)*4;
	u32 offset_neg3_alg8_crit   					= (137)*4;
	//u32 offset_neg3_alg9_crit   					= (138)*4;
	//u32 offset_neg3_alg10_crit  					= (139)*4;
	u32 offset_neg3_fault_detected                  = (140)*4;
	//141 is for counter logging
	//142 is for trip logging

	//pos3
	u32 offset_ch4_lpf          					= (143)*4;
	u32 offset_ch5_lpf          					= (144)*4;
	u32 offset_pos3_alg1_crit   					= (145)*4;
	u32 offset_pos3_alg2_crit   					= (146)*4;
	u32 offset_pos3_alg3_crit   					= (147)*4;
	u32 offset_pos3_alg4_crit   					= (148)*4;
	u32 offset_pos3_alg5_crit   					= (149)*4;
	//u32 offset_pos3_alg6_crit   					= (150)*4;
	//u32 offset_pos3_alg7_crit   					= (151)*4;
	u32 offset_pos3_alg8_crit   					= (152)*4;
	//u32 offset_pos3_alg9_crit   					= (153)*4;
	//u32 offset_pos3_alg10_crit  					= (154)*4;
	u32 offset_pos3_fault_detected                  = (155)*4;
	//156 is for counter logging
	//157 is for trip logging

	if (!strncmp(RecvBuffer, "fi0", 3)){Alex_SendData(offset_ch0_lpf);return;}
	else if (!strncmp(RecvBuffer, "fi1", 3)){Alex_SendData(offset_ch1_lpf);return;}
	else if (!strncmp(RecvBuffer, "fir", 3)){Alex_SendData(offset_chr_lpf);return;}
	else if (!strncmp(RecvBuffer, "fi2", 3)){Alex_SendData(offset_ch2_lpf);return;}
	else if (!strncmp(RecvBuffer, "fi3", 3)){Alex_SendData(offset_ch3_lpf);return;}
	else if (!strncmp(RecvBuffer, "fi4", 3)){Alex_SendData(offset_ch4_lpf);return;}
	else if (!strncmp(RecvBuffer, "fi5", 3)){Alex_SendData(offset_ch5_lpf);return;}
	//else if (!strncmp(RecvBuffer, "fi6", 3)){Alex_SendData(offset_ch6_lpf);return;}
	//else if (!strncmp(RecvBuffer, "fi7", 3)){Alex_SendData(offset_ch7_lpf);return;}
	else if (!strncmp(RecvBuffer, "fi8", 3)){Alex_SendData(offset_ch8_lpf);return;}
	else if (!strncmp(RecvBuffer, "fi9", 3)){Alex_SendData(offset_ch9_lpf);return;}
	else if (!strncmp(RecvBuffer, "fia", 3)){Alex_SendData(offset_ch10_lpf);return;}
	else if (!strncmp(RecvBuffer, "fib", 3)){Alex_SendData(offset_ch11_lpf);return;}
	else if (!strncmp(RecvBuffer, "fic", 3)){Alex_SendData(offset_ch12_lpf);return;}
	else if (!strncmp(RecvBuffer, "fid", 3)){Alex_SendData(offset_ch13_lpf);return;}
	//else if (!strncmp(RecvBuffer, "fie", 3)){Alex_SendData(offset_ch14_lpf);return;}
	//else if (!strncmp(RecvBuffer, "fif", 3)){Alex_SendData(offset_ch15_lpf);return;}

	else if (!strncmp(RecvBuffer, "ch0", 3)){Alex_SendData(offset_ch0);return;}
	else if (!strncmp(RecvBuffer, "ch1", 3)){Alex_SendData(offset_ch1);return;}
	else if (!strncmp(RecvBuffer, "ch2", 3)){Alex_SendData(offset_ch2);return;}
	else if (!strncmp(RecvBuffer, "ch3", 3)){Alex_SendData(offset_ch3);return;}
	else if (!strncmp(RecvBuffer, "ch4", 3)){Alex_SendData(offset_ch4);return;}
	else if (!strncmp(RecvBuffer, "ch5", 3)){Alex_SendData(offset_ch5);return;}
	else if (!strncmp(RecvBuffer, "ch6", 3)){Alex_SendData(offset_ch6);return;}
	else if (!strncmp(RecvBuffer, "ch7", 3)){Alex_SendData(offset_ch7);return;}
	else if (!strncmp(RecvBuffer, "ch8", 3)){Alex_SendData(offset_ch8);return;}
	else if (!strncmp(RecvBuffer, "ch9", 3)){Alex_SendData(offset_ch9);return;}
	else if (!strncmp(RecvBuffer, "cha", 3)){Alex_SendData(offset_ch10);return;}
	else if (!strncmp(RecvBuffer, "chb", 3)){Alex_SendData(offset_ch11);return;}
	else if (!strncmp(RecvBuffer, "chc", 3)){Alex_SendData(offset_ch12);return;}
	else if (!strncmp(RecvBuffer, "chd", 3)){Alex_SendData(offset_ch13);return;}
	else if (!strncmp(RecvBuffer, "che", 3)){Alex_SendData(offset_ch14);return;}
	else if (!strncmp(RecvBuffer, "chf", 3)){Alex_SendData(offset_ch15);return;}
	//else if (!strncmp(RecvBuffer, "chr", 3)){Alex_SendData(offset_chr_delayed);return;}


	else if (!strncmp(RecvBuffer, "c11", 3)){Alex_SendData(offset_pos1_alg1_crit);return;}
	else if (!strncmp(RecvBuffer, "c12", 3)){Alex_SendData(offset_pos1_alg2_crit);return;}
	else if (!strncmp(RecvBuffer, "c13", 3)){Alex_SendData(offset_pos1_alg3_crit);return;}
	else if (!strncmp(RecvBuffer, "c14", 3)){Alex_SendData(offset_pos1_alg4_crit);return;}
	else if (!strncmp(RecvBuffer, "c15", 3)){Alex_SendData(offset_pos1_alg5_crit);return;}
	//else if (!strncmp(RecvBuffer, "c16", 3)){Alex_SendData(offset_pos1_alg6_crit);return;}
	//else if (!strncmp(RecvBuffer, "c17", 3)){Alex_SendData(offset_pos1_alg7_crit);return;}
	else if (!strncmp(RecvBuffer, "c18", 3)){Alex_SendData(offset_pos1_alg8_crit);return;}
	//else if (!strncmp(RecvBuffer, "c19", 3)){Alex_SendData(offset_pos1_alg9_crit);return;}
	//else if (!strncmp(RecvBuffer, "c1a", 3)){Alex_SendData(offset_pos1_alg10_crit);return;}
	else if (!strncmp(RecvBuffer, "c21", 3)){Alex_SendData(offset_pos2_alg1_crit);return;}
	else if (!strncmp(RecvBuffer, "c22", 3)){Alex_SendData(offset_pos2_alg2_crit);return;}
	else if (!strncmp(RecvBuffer, "c23", 3)){Alex_SendData(offset_pos2_alg3_crit);return;}
	else if (!strncmp(RecvBuffer, "c24", 3)){Alex_SendData(offset_pos2_alg4_crit);return;}
	else if (!strncmp(RecvBuffer, "c25", 3)){Alex_SendData(offset_pos2_alg5_crit);return;}
	//else if (!strncmp(RecvBuffer, "c26", 3)){Alex_SendData(offset_pos2_alg6_crit);return;}
	//else if (!strncmp(RecvBuffer, "c27", 3)){Alex_SendData(offset_pos2_alg7_crit);return;}
	else if (!strncmp(RecvBuffer, "c28", 3)){Alex_SendData(offset_pos2_alg8_crit);return;}
	//else if (!strncmp(RecvBuffer, "c29", 3)){Alex_SendData(offset_pos2_alg9_crit);return;}
	//else if (!strncmp(RecvBuffer, "c2a", 3)){Alex_SendData(offset_pos2_alg10_crit);return;}
	else if (!strncmp(RecvBuffer, "c31", 3)){Alex_SendData(offset_pos3_alg1_crit);return;}
	else if (!strncmp(RecvBuffer, "c32", 3)){Alex_SendData(offset_pos3_alg2_crit);return;}
	else if (!strncmp(RecvBuffer, "c33", 3)){Alex_SendData(offset_pos3_alg3_crit);return;}
	else if (!strncmp(RecvBuffer, "c34", 3)){Alex_SendData(offset_pos3_alg4_crit);return;}
	else if (!strncmp(RecvBuffer, "c35", 3)){Alex_SendData(offset_pos3_alg5_crit);return;}
	//else if (!strncmp(RecvBuffer, "c36", 3)){Alex_SendData(offset_pos3_alg6_crit);return;}
	//else if (!strncmp(RecvBuffer, "c37", 3)){Alex_SendData(offset_pos3_alg7_crit);return;}
	else if (!strncmp(RecvBuffer, "c38", 3)){Alex_SendData(offset_pos3_alg8_crit);return;}
	//else if (!strncmp(RecvBuffer, "c39", 3)){Alex_SendData(offset_pos3_alg9_crit);return;}
	//else if (!strncmp(RecvBuffer, "c3a", 3)){Alex_SendData(offset_pos3_alg10_crit);return;}


	else if (!strncmp(RecvBuffer, "c41", 3)){Alex_SendData(offset_neg1_alg1_crit);return;}
	else if (!strncmp(RecvBuffer, "c42", 3)){Alex_SendData(offset_neg1_alg2_crit);return;}
	else if (!strncmp(RecvBuffer, "c43", 3)){Alex_SendData(offset_neg1_alg3_crit);return;}
	else if (!strncmp(RecvBuffer, "c44", 3)){Alex_SendData(offset_neg1_alg4_crit);return;}
	else if (!strncmp(RecvBuffer, "c45", 3)){Alex_SendData(offset_neg1_alg5_crit);return;}
	//else if (!strncmp(RecvBuffer, "c46", 3)){Alex_SendData(offset_neg1_alg6_crit);return;}
	//else if (!strncmp(RecvBuffer, "c47", 3)){Alex_SendData(offset_neg1_alg7_crit);return;}
	else if (!strncmp(RecvBuffer, "c48", 3)){Alex_SendData(offset_neg1_alg8_crit);return;}
	//else if (!strncmp(RecvBuffer, "c49", 3)){Alex_SendData(offset_neg1_alg9_crit);return;}
	//else if (!strncmp(RecvBuffer, "c4a", 3)){Alex_SendData(offset_neg1_alg10_crit);return;}
	else if (!strncmp(RecvBuffer, "c51", 3)){Alex_SendData(offset_neg2_alg1_crit);return;}
	else if (!strncmp(RecvBuffer, "c52", 3)){Alex_SendData(offset_neg2_alg2_crit);return;}
	else if (!strncmp(RecvBuffer, "c53", 3)){Alex_SendData(offset_neg2_alg3_crit);return;}
	else if (!strncmp(RecvBuffer, "c54", 3)){Alex_SendData(offset_neg2_alg4_crit);return;}
	else if (!strncmp(RecvBuffer, "c55", 3)){Alex_SendData(offset_neg2_alg5_crit);return;}
	//else if (!strncmp(RecvBuffer, "c56", 3)){Alex_SendData(offset_neg2_alg6_crit);return;}
	//else if (!strncmp(RecvBuffer, "c57", 3)){Alex_SendData(offset_neg2_alg7_crit);return;}
	else if (!strncmp(RecvBuffer, "c58", 3)){Alex_SendData(offset_neg2_alg8_crit);return;}
	//else if (!strncmp(RecvBuffer, "c59", 3)){Alex_SendData(offset_neg2_alg9_crit);return;}
	//else if (!strncmp(RecvBuffer, "c5a", 3)){Alex_SendData(offset_neg2_alg10_crit);return;}
	else if (!strncmp(RecvBuffer, "c61", 3)){Alex_SendData(offset_neg3_alg1_crit);return;}
	else if (!strncmp(RecvBuffer, "c62", 3)){Alex_SendData(offset_neg3_alg2_crit);return;}
	else if (!strncmp(RecvBuffer, "c63", 3)){Alex_SendData(offset_neg3_alg3_crit);return;}
	else if (!strncmp(RecvBuffer, "c64", 3)){Alex_SendData(offset_neg3_alg4_crit);return;}
	else if (!strncmp(RecvBuffer, "c65", 3)){Alex_SendData(offset_neg3_alg5_crit);return;}
	//else if (!strncmp(RecvBuffer, "c66", 3)){Alex_SendData(offset_neg3_alg6_crit);return;}
	//else if (!strncmp(RecvBuffer, "c67", 3)){Alex_SendData(offset_neg3_alg7_crit);return;}
	else if (!strncmp(RecvBuffer, "c68", 3)){Alex_SendData(offset_neg3_alg8_crit);return;}
	//else if (!strncmp(RecvBuffer, "c69", 3)){Alex_SendData(offset_neg3_alg9_crit);return;}
	//else if (!strncmp(RecvBuffer, "c6a", 3)){Alex_SendData(offset_neg3_alg10_crit);return;}

	else if (!strncmp(RecvBuffer, "fmm", 3)){Alex_SendData(offset_manual_fault_detected);return;}
	else if (!strncmp(RecvBuffer, "f1n", 3)){Alex_SendData(offset_neg1_fault_detected);return;}
	else if (!strncmp(RecvBuffer, "f1p", 3)){Alex_SendData(offset_pos1_fault_detected);return;}
	else if (!strncmp(RecvBuffer, "f2n", 3)){Alex_SendData(offset_neg2_fault_detected);return;}
	else if (!strncmp(RecvBuffer, "f2p", 3)){Alex_SendData(offset_pos2_fault_detected);return;}
	else if (!strncmp(RecvBuffer, "f3n", 3)){Alex_SendData(offset_neg3_fault_detected);return;}
	else if (!strncmp(RecvBuffer, "f3p", 3)){Alex_SendData(offset_pos3_fault_detected);return;}

	//turn algorithm on/off, send  some data back that will be ignored.
	else if (!strncmp(RecvBuffer, "on1", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(1,1);Alex_TripAlgorithm1neg_enableAlgorithm(1,1);
		Alex_TripAlgorithm2pos_enableAlgorithm(1,1);Alex_TripAlgorithm2neg_enableAlgorithm(1,1);
		Alex_TripAlgorithm3pos_enableAlgorithm(1,1);Alex_TripAlgorithm3neg_enableAlgorithm(1,1);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "of1", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(1,0);Alex_TripAlgorithm1neg_enableAlgorithm(1,0);
		Alex_TripAlgorithm2pos_enableAlgorithm(1,0);Alex_TripAlgorithm2neg_enableAlgorithm(1,0);
		Alex_TripAlgorithm3pos_enableAlgorithm(1,0);Alex_TripAlgorithm3neg_enableAlgorithm(1,0);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "on2", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(2,1);Alex_TripAlgorithm1neg_enableAlgorithm(2,1);
		Alex_TripAlgorithm2pos_enableAlgorithm(2,1);Alex_TripAlgorithm2neg_enableAlgorithm(2,1);
		Alex_TripAlgorithm3pos_enableAlgorithm(2,1);Alex_TripAlgorithm3neg_enableAlgorithm(2,1);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "of2", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(2,0);Alex_TripAlgorithm1neg_enableAlgorithm(2,0);
		Alex_TripAlgorithm2pos_enableAlgorithm(2,0);Alex_TripAlgorithm2neg_enableAlgorithm(2,0);
		Alex_TripAlgorithm3pos_enableAlgorithm(2,0);Alex_TripAlgorithm3neg_enableAlgorithm(2,0);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "on3", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(3,1);Alex_TripAlgorithm1neg_enableAlgorithm(3,1);
		Alex_TripAlgorithm2pos_enableAlgorithm(3,1);Alex_TripAlgorithm2neg_enableAlgorithm(3,1);
		Alex_TripAlgorithm3pos_enableAlgorithm(3,1);Alex_TripAlgorithm3neg_enableAlgorithm(3,1);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "of3", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(3,0);Alex_TripAlgorithm1neg_enableAlgorithm(3,0);
		Alex_TripAlgorithm2pos_enableAlgorithm(3,0);Alex_TripAlgorithm2neg_enableAlgorithm(3,0);
		Alex_TripAlgorithm3pos_enableAlgorithm(3,0);Alex_TripAlgorithm3neg_enableAlgorithm(3,0);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "on4", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(4,1);Alex_TripAlgorithm1neg_enableAlgorithm(4,1);
		Alex_TripAlgorithm2pos_enableAlgorithm(4,1);Alex_TripAlgorithm2neg_enableAlgorithm(4,1);
		Alex_TripAlgorithm3pos_enableAlgorithm(4,1);Alex_TripAlgorithm3neg_enableAlgorithm(4,1);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "of4", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(4,0);Alex_TripAlgorithm1neg_enableAlgorithm(4,0);
		Alex_TripAlgorithm2pos_enableAlgorithm(4,0);Alex_TripAlgorithm2neg_enableAlgorithm(4,0);
		Alex_TripAlgorithm3pos_enableAlgorithm(4,0);Alex_TripAlgorithm3neg_enableAlgorithm(4,0);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "on5", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(5,1);Alex_TripAlgorithm1neg_enableAlgorithm(5,1);
		Alex_TripAlgorithm2pos_enableAlgorithm(5,1);Alex_TripAlgorithm2neg_enableAlgorithm(5,1);
		Alex_TripAlgorithm3pos_enableAlgorithm(5,1);Alex_TripAlgorithm3neg_enableAlgorithm(5,1);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "of5", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(5,0);Alex_TripAlgorithm1neg_enableAlgorithm(5,0);
		Alex_TripAlgorithm2pos_enableAlgorithm(5,0);Alex_TripAlgorithm2neg_enableAlgorithm(5,0);
		Alex_TripAlgorithm3pos_enableAlgorithm(5,0);Alex_TripAlgorithm3neg_enableAlgorithm(5,0);
		Alex_Confirm();return;}
	/*else if (!strncmp(RecvBuffer, "on6", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(6,1);Alex_TripAlgorithm1neg_enableAlgorithm(6,1);
		Alex_TripAlgorithm2pos_enableAlgorithm(6,1);Alex_TripAlgorithm2neg_enableAlgorithm(6,1);
		Alex_TripAlgorithm3pos_enableAlgorithm(6,1);Alex_TripAlgorithm3neg_enableAlgorithm(6,1);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "of6", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(6,0);Alex_TripAlgorithm1neg_enableAlgorithm(6,0);
		Alex_TripAlgorithm2pos_enableAlgorithm(6,0);Alex_TripAlgorithm2neg_enableAlgorithm(6,0);
		Alex_TripAlgorithm3pos_enableAlgorithm(6,0);Alex_TripAlgorithm3neg_enableAlgorithm(6,0);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "on7", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(7,1);Alex_TripAlgorithm1neg_enableAlgorithm(7,1);
		Alex_TripAlgorithm2pos_enableAlgorithm(7,1);Alex_TripAlgorithm2neg_enableAlgorithm(7,1);
		Alex_TripAlgorithm3pos_enableAlgorithm(7,1);Alex_TripAlgorithm3neg_enableAlgorithm(7,1);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "of7", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(7,0);Alex_TripAlgorithm1neg_enableAlgorithm(7,0);
		Alex_TripAlgorithm2pos_enableAlgorithm(7,0);Alex_TripAlgorithm2neg_enableAlgorithm(7,0);
		Alex_TripAlgorithm3pos_enableAlgorithm(7,0);Alex_TripAlgorithm3neg_enableAlgorithm(7,0);
		Alex_Confirm();return;}*/
	else if (!strncmp(RecvBuffer, "on8", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(8,1);Alex_TripAlgorithm1neg_enableAlgorithm(8,1);
		Alex_TripAlgorithm2pos_enableAlgorithm(8,1);Alex_TripAlgorithm2neg_enableAlgorithm(8,1);
		Alex_TripAlgorithm3pos_enableAlgorithm(8,1);Alex_TripAlgorithm3neg_enableAlgorithm(8,1);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "of8", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(8,0);Alex_TripAlgorithm1neg_enableAlgorithm(8,0);
		Alex_TripAlgorithm2pos_enableAlgorithm(8,0);Alex_TripAlgorithm2neg_enableAlgorithm(8,0);
		Alex_TripAlgorithm3pos_enableAlgorithm(8,0);Alex_TripAlgorithm3neg_enableAlgorithm(8,0);
		Alex_Confirm();return;}
	/*else if (!strncmp(RecvBuffer, "on9", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(9,1);Alex_TripAlgorithm1neg_enableAlgorithm(9,1);
		Alex_TripAlgorithm2pos_enableAlgorithm(9,1);Alex_TripAlgorithm2neg_enableAlgorithm(9,1);
		Alex_TripAlgorithm3pos_enableAlgorithm(9,1);Alex_TripAlgorithm3neg_enableAlgorithm(9,1);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "of9", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(9,0);Alex_TripAlgorithm1neg_enableAlgorithm(9,0);
		Alex_TripAlgorithm2pos_enableAlgorithm(9,0);Alex_TripAlgorithm2neg_enableAlgorithm(9,0);
		Alex_TripAlgorithm3pos_enableAlgorithm(9,0);Alex_TripAlgorithm3neg_enableAlgorithm(9,0);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ona", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(10,1);Alex_TripAlgorithm1neg_enableAlgorithm(10,1);
		Alex_TripAlgorithm2pos_enableAlgorithm(10,1);Alex_TripAlgorithm2neg_enableAlgorithm(10,1);
		Alex_TripAlgorithm3pos_enableAlgorithm(10,1);Alex_TripAlgorithm3neg_enableAlgorithm(10,1);
		Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ofa", 3)){
		Alex_TripAlgorithm1pos_enableAlgorithm(10,0);Alex_TripAlgorithm1neg_enableAlgorithm(10,0);
		Alex_TripAlgorithm2pos_enableAlgorithm(10,0);Alex_TripAlgorithm2neg_enableAlgorithm(10,0);
		Alex_TripAlgorithm3pos_enableAlgorithm(10,0);Alex_TripAlgorithm3neg_enableAlgorithm(10,0);
		Alex_Confirm();return;}*/

	//set line length
	/*else if (!strncmp(RecvBuffer, "llA", 3)){Alex_TripAlgorithm1pos_updateLength(0);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llB", 3)){Alex_TripAlgorithm1pos_updateLength(1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llC", 3)){Alex_TripAlgorithm1pos_updateLength(2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llD", 3)){Alex_TripAlgorithm1pos_updateLength(3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llE", 3)){Alex_TripAlgorithm1pos_updateLength(4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llF", 3)){Alex_TripAlgorithm1pos_updateLength(5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llG", 3)){Alex_TripAlgorithm1pos_updateLength(6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llH", 3)){Alex_TripAlgorithm1pos_updateLength(7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llI", 3)){Alex_TripAlgorithm1pos_updateLength(8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llJ", 3)){Alex_TripAlgorithm1pos_updateLength(9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llK", 3)){Alex_TripAlgorithm1pos_updateLength(10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llL", 3)){Alex_TripAlgorithm1pos_updateLength(11);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llM", 3)){Alex_TripAlgorithm1pos_updateLength(12);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llN", 3)){Alex_TripAlgorithm1pos_updateLength(13);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llO", 3)){Alex_TripAlgorithm1pos_updateLength(14);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llP", 3)){Alex_TripAlgorithm1pos_updateLength(15);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llQ", 3)){Alex_TripAlgorithm1pos_updateLength(16);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llR", 3)){Alex_TripAlgorithm1pos_updateLength(17);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llS", 3)){Alex_TripAlgorithm1pos_updateLength(18);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llT", 3)){Alex_TripAlgorithm1pos_updateLength(19);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llU", 3)){Alex_TripAlgorithm1pos_updateLength(20);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llV", 3)){Alex_TripAlgorithm1pos_updateLength(21);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llW", 3)){Alex_TripAlgorithm1pos_updateLength(22);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llX", 3)){Alex_TripAlgorithm1pos_updateLength(23);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llY", 3)){Alex_TripAlgorithm1pos_updateLength(24);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llZ", 3)){Alex_TripAlgorithm1pos_updateLength(25);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "lla", 3)){Alex_TripAlgorithm1pos_updateLength(26);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llb", 3)){Alex_TripAlgorithm1pos_updateLength(27);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llc", 3)){Alex_TripAlgorithm1pos_updateLength(28);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "lld", 3)){Alex_TripAlgorithm1pos_updateLength(29);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "lle", 3)){Alex_TripAlgorithm1pos_updateLength(30);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llf", 3)){Alex_TripAlgorithm1pos_updateLength(31);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llg", 3)){Alex_TripAlgorithm1pos_updateLength(32);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llh", 3)){Alex_TripAlgorithm1pos_updateLength(33);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "lli", 3)){Alex_TripAlgorithm1pos_updateLength(34);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llj", 3)){Alex_TripAlgorithm1pos_updateLength(35);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llk", 3)){Alex_TripAlgorithm1pos_updateLength(36);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "lll", 3)){Alex_TripAlgorithm1pos_updateLength(37);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llm", 3)){Alex_TripAlgorithm1pos_updateLength(38);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "lln", 3)){Alex_TripAlgorithm1pos_updateLength(39);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llo", 3)){Alex_TripAlgorithm1pos_updateLength(40);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llp", 3)){Alex_TripAlgorithm1pos_updateLength(41);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llq", 3)){Alex_TripAlgorithm1pos_updateLength(42);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llr", 3)){Alex_TripAlgorithm1pos_updateLength(43);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "lls", 3)){Alex_TripAlgorithm1pos_updateLength(44);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llt", 3)){Alex_TripAlgorithm1pos_updateLength(45);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llu", 3)){Alex_TripAlgorithm1pos_updateLength(46);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llv", 3)){Alex_TripAlgorithm1pos_updateLength(47);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llw", 3)){Alex_TripAlgorithm1pos_updateLength(48);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "llx", 3)){Alex_TripAlgorithm1pos_updateLength(49);Alex_Confirm();return;}*/


	//set algorithm thresholds: Alex_TripAlgorithm1pos_updateThresholds(u32 threshold, int algorithm)
	//first letter "t" marks that it is a threshold to be set
	//second letter marks the algorithm to be set 1-a for alg1-alg10 in pos1
	//				    						  b-k  for alg1-alg10 in pos2
	//								              l-u  for alg1-alg10 in pos3
	//
	//third letter marks the threshold
	else if (!strncmp(RecvBuffer, "t1A", 3)){Alex_TripAlgorithm1pos_updateThresholds(0, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1B", 3)){Alex_TripAlgorithm1pos_updateThresholds(1, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1C", 3)){Alex_TripAlgorithm1pos_updateThresholds(2, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1D", 3)){Alex_TripAlgorithm1pos_updateThresholds(3, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1E", 3)){Alex_TripAlgorithm1pos_updateThresholds(4, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1F", 3)){Alex_TripAlgorithm1pos_updateThresholds(5, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1G", 3)){Alex_TripAlgorithm1pos_updateThresholds(6, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1H", 3)){Alex_TripAlgorithm1pos_updateThresholds(7, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1I", 3)){Alex_TripAlgorithm1pos_updateThresholds(8, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1J", 3)){Alex_TripAlgorithm1pos_updateThresholds(9, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1K", 3)){Alex_TripAlgorithm1pos_updateThresholds(10, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1L", 3)){Alex_TripAlgorithm1pos_updateThresholds(11, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1M", 3)){Alex_TripAlgorithm1pos_updateThresholds(12, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1N", 3)){Alex_TripAlgorithm1pos_updateThresholds(13, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1O", 3)){Alex_TripAlgorithm1pos_updateThresholds(14, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1P", 3)){Alex_TripAlgorithm1pos_updateThresholds(15, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1Q", 3)){Alex_TripAlgorithm1pos_updateThresholds(16, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1R", 3)){Alex_TripAlgorithm1pos_updateThresholds(17, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1S", 3)){Alex_TripAlgorithm1pos_updateThresholds(18, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1T", 3)){Alex_TripAlgorithm1pos_updateThresholds(19, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1U", 3)){Alex_TripAlgorithm1pos_updateThresholds(20, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1V", 3)){Alex_TripAlgorithm1pos_updateThresholds(21, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1W", 3)){Alex_TripAlgorithm1pos_updateThresholds(22, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1X", 3)){Alex_TripAlgorithm1pos_updateThresholds(23, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1Y", 3)){Alex_TripAlgorithm1pos_updateThresholds(24, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1Z", 3)){Alex_TripAlgorithm1pos_updateThresholds(25, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1a", 3)){Alex_TripAlgorithm1pos_updateThresholds(26, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1b", 3)){Alex_TripAlgorithm1pos_updateThresholds(27, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1c", 3)){Alex_TripAlgorithm1pos_updateThresholds(28, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1d", 3)){Alex_TripAlgorithm1pos_updateThresholds(29, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1e", 3)){Alex_TripAlgorithm1pos_updateThresholds(30, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1f", 3)){Alex_TripAlgorithm1pos_updateThresholds(31, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1g", 3)){Alex_TripAlgorithm1pos_updateThresholds(32, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1h", 3)){Alex_TripAlgorithm1pos_updateThresholds(33, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1i", 3)){Alex_TripAlgorithm1pos_updateThresholds(34, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1j", 3)){Alex_TripAlgorithm1pos_updateThresholds(35, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1k", 3)){Alex_TripAlgorithm1pos_updateThresholds(36, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1l", 3)){Alex_TripAlgorithm1pos_updateThresholds(37, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1m", 3)){Alex_TripAlgorithm1pos_updateThresholds(38, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1n", 3)){Alex_TripAlgorithm1pos_updateThresholds(39, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1o", 3)){Alex_TripAlgorithm1pos_updateThresholds(40, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1p", 3)){Alex_TripAlgorithm1pos_updateThresholds(41, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1q", 3)){Alex_TripAlgorithm1pos_updateThresholds(42, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1r", 3)){Alex_TripAlgorithm1pos_updateThresholds(43, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1s", 3)){Alex_TripAlgorithm1pos_updateThresholds(44, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1t", 3)){Alex_TripAlgorithm1pos_updateThresholds(45, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1u", 3)){Alex_TripAlgorithm1pos_updateThresholds(46, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1v", 3)){Alex_TripAlgorithm1pos_updateThresholds(47, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1w", 3)){Alex_TripAlgorithm1pos_updateThresholds(48, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t1x", 3)){Alex_TripAlgorithm1pos_updateThresholds(49, 1);Alex_Confirm();return;}

	else if (!strncmp(RecvBuffer, "t2A", 3)){Alex_TripAlgorithm1pos_updateThresholds(0, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2B", 3)){Alex_TripAlgorithm1pos_updateThresholds(1, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2C", 3)){Alex_TripAlgorithm1pos_updateThresholds(2, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2D", 3)){Alex_TripAlgorithm1pos_updateThresholds(3, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2E", 3)){Alex_TripAlgorithm1pos_updateThresholds(4, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2F", 3)){Alex_TripAlgorithm1pos_updateThresholds(5, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2G", 3)){Alex_TripAlgorithm1pos_updateThresholds(6, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2H", 3)){Alex_TripAlgorithm1pos_updateThresholds(7, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2I", 3)){Alex_TripAlgorithm1pos_updateThresholds(8, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2J", 3)){Alex_TripAlgorithm1pos_updateThresholds(9, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2K", 3)){Alex_TripAlgorithm1pos_updateThresholds(10, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2L", 3)){Alex_TripAlgorithm1pos_updateThresholds(11, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2M", 3)){Alex_TripAlgorithm1pos_updateThresholds(12, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2N", 3)){Alex_TripAlgorithm1pos_updateThresholds(13, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2O", 3)){Alex_TripAlgorithm1pos_updateThresholds(14, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2P", 3)){Alex_TripAlgorithm1pos_updateThresholds(15, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2Q", 3)){Alex_TripAlgorithm1pos_updateThresholds(16, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2R", 3)){Alex_TripAlgorithm1pos_updateThresholds(17, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2S", 3)){Alex_TripAlgorithm1pos_updateThresholds(18, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2T", 3)){Alex_TripAlgorithm1pos_updateThresholds(19, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2U", 3)){Alex_TripAlgorithm1pos_updateThresholds(20, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2V", 3)){Alex_TripAlgorithm1pos_updateThresholds(21, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2W", 3)){Alex_TripAlgorithm1pos_updateThresholds(22, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2X", 3)){Alex_TripAlgorithm1pos_updateThresholds(23, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2Y", 3)){Alex_TripAlgorithm1pos_updateThresholds(24, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2Z", 3)){Alex_TripAlgorithm1pos_updateThresholds(25, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2a", 3)){Alex_TripAlgorithm1pos_updateThresholds(26, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2b", 3)){Alex_TripAlgorithm1pos_updateThresholds(27, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2c", 3)){Alex_TripAlgorithm1pos_updateThresholds(28, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2d", 3)){Alex_TripAlgorithm1pos_updateThresholds(29, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2e", 3)){Alex_TripAlgorithm1pos_updateThresholds(30, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2f", 3)){Alex_TripAlgorithm1pos_updateThresholds(31, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2g", 3)){Alex_TripAlgorithm1pos_updateThresholds(32, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2h", 3)){Alex_TripAlgorithm1pos_updateThresholds(33, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2i", 3)){Alex_TripAlgorithm1pos_updateThresholds(34, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2j", 3)){Alex_TripAlgorithm1pos_updateThresholds(35, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2k", 3)){Alex_TripAlgorithm1pos_updateThresholds(36, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2l", 3)){Alex_TripAlgorithm1pos_updateThresholds(37, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2m", 3)){Alex_TripAlgorithm1pos_updateThresholds(38, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2n", 3)){Alex_TripAlgorithm1pos_updateThresholds(39, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2o", 3)){Alex_TripAlgorithm1pos_updateThresholds(40, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2p", 3)){Alex_TripAlgorithm1pos_updateThresholds(41, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2q", 3)){Alex_TripAlgorithm1pos_updateThresholds(42, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2r", 3)){Alex_TripAlgorithm1pos_updateThresholds(43, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2s", 3)){Alex_TripAlgorithm1pos_updateThresholds(44, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2t", 3)){Alex_TripAlgorithm1pos_updateThresholds(45, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2u", 3)){Alex_TripAlgorithm1pos_updateThresholds(46, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2v", 3)){Alex_TripAlgorithm1pos_updateThresholds(47, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2w", 3)){Alex_TripAlgorithm1pos_updateThresholds(48, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t2x", 3)){Alex_TripAlgorithm1pos_updateThresholds(49, 2);Alex_Confirm();return;}

	else if (!strncmp(RecvBuffer, "t3A", 3)){Alex_TripAlgorithm1pos_updateThresholds(0, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3B", 3)){Alex_TripAlgorithm1pos_updateThresholds(1, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3C", 3)){Alex_TripAlgorithm1pos_updateThresholds(2, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3D", 3)){Alex_TripAlgorithm1pos_updateThresholds(3, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3E", 3)){Alex_TripAlgorithm1pos_updateThresholds(4, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3F", 3)){Alex_TripAlgorithm1pos_updateThresholds(5, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3G", 3)){Alex_TripAlgorithm1pos_updateThresholds(6, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3H", 3)){Alex_TripAlgorithm1pos_updateThresholds(7, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3I", 3)){Alex_TripAlgorithm1pos_updateThresholds(8, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3J", 3)){Alex_TripAlgorithm1pos_updateThresholds(9, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3K", 3)){Alex_TripAlgorithm1pos_updateThresholds(10, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3L", 3)){Alex_TripAlgorithm1pos_updateThresholds(11, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3M", 3)){Alex_TripAlgorithm1pos_updateThresholds(12, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3N", 3)){Alex_TripAlgorithm1pos_updateThresholds(13, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3O", 3)){Alex_TripAlgorithm1pos_updateThresholds(14, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3P", 3)){Alex_TripAlgorithm1pos_updateThresholds(15, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3Q", 3)){Alex_TripAlgorithm1pos_updateThresholds(16, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3R", 3)){Alex_TripAlgorithm1pos_updateThresholds(17, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3S", 3)){Alex_TripAlgorithm1pos_updateThresholds(18, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3T", 3)){Alex_TripAlgorithm1pos_updateThresholds(19, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3U", 3)){Alex_TripAlgorithm1pos_updateThresholds(20, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3V", 3)){Alex_TripAlgorithm1pos_updateThresholds(21, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3W", 3)){Alex_TripAlgorithm1pos_updateThresholds(22, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3X", 3)){Alex_TripAlgorithm1pos_updateThresholds(23, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3Y", 3)){Alex_TripAlgorithm1pos_updateThresholds(24, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3Z", 3)){Alex_TripAlgorithm1pos_updateThresholds(25, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3a", 3)){Alex_TripAlgorithm1pos_updateThresholds(26, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3b", 3)){Alex_TripAlgorithm1pos_updateThresholds(27, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3c", 3)){Alex_TripAlgorithm1pos_updateThresholds(28, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3d", 3)){Alex_TripAlgorithm1pos_updateThresholds(29, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3e", 3)){Alex_TripAlgorithm1pos_updateThresholds(30, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3f", 3)){Alex_TripAlgorithm1pos_updateThresholds(31, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3g", 3)){Alex_TripAlgorithm1pos_updateThresholds(32, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3h", 3)){Alex_TripAlgorithm1pos_updateThresholds(33, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3i", 3)){Alex_TripAlgorithm1pos_updateThresholds(34, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3j", 3)){Alex_TripAlgorithm1pos_updateThresholds(35, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3k", 3)){Alex_TripAlgorithm1pos_updateThresholds(36, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3l", 3)){Alex_TripAlgorithm1pos_updateThresholds(37, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3m", 3)){Alex_TripAlgorithm1pos_updateThresholds(38, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3n", 3)){Alex_TripAlgorithm1pos_updateThresholds(39, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3o", 3)){Alex_TripAlgorithm1pos_updateThresholds(40, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3p", 3)){Alex_TripAlgorithm1pos_updateThresholds(41, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3q", 3)){Alex_TripAlgorithm1pos_updateThresholds(42, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3r", 3)){Alex_TripAlgorithm1pos_updateThresholds(43, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3s", 3)){Alex_TripAlgorithm1pos_updateThresholds(44, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3t", 3)){Alex_TripAlgorithm1pos_updateThresholds(45, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3u", 3)){Alex_TripAlgorithm1pos_updateThresholds(46, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3v", 3)){Alex_TripAlgorithm1pos_updateThresholds(47, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3w", 3)){Alex_TripAlgorithm1pos_updateThresholds(48, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t3x", 3)){Alex_TripAlgorithm1pos_updateThresholds(49, 3);Alex_Confirm();return;}

	/*else if (!strncmp(RecvBuffer, "t4A", 3)){Alex_TripAlgorithm1pos_updateThresholds(0, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4B", 3)){Alex_TripAlgorithm1pos_updateThresholds(1, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4C", 3)){Alex_TripAlgorithm1pos_updateThresholds(2, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4D", 3)){Alex_TripAlgorithm1pos_updateThresholds(3, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4E", 3)){Alex_TripAlgorithm1pos_updateThresholds(4, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4F", 3)){Alex_TripAlgorithm1pos_updateThresholds(5, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4G", 3)){Alex_TripAlgorithm1pos_updateThresholds(6, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4H", 3)){Alex_TripAlgorithm1pos_updateThresholds(7, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4I", 3)){Alex_TripAlgorithm1pos_updateThresholds(8, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4J", 3)){Alex_TripAlgorithm1pos_updateThresholds(9, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4K", 3)){Alex_TripAlgorithm1pos_updateThresholds(10, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4L", 3)){Alex_TripAlgorithm1pos_updateThresholds(11, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4M", 3)){Alex_TripAlgorithm1pos_updateThresholds(12, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4N", 3)){Alex_TripAlgorithm1pos_updateThresholds(13, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4O", 3)){Alex_TripAlgorithm1pos_updateThresholds(14, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4P", 3)){Alex_TripAlgorithm1pos_updateThresholds(15, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4Q", 3)){Alex_TripAlgorithm1pos_updateThresholds(16, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4R", 3)){Alex_TripAlgorithm1pos_updateThresholds(17, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4S", 3)){Alex_TripAlgorithm1pos_updateThresholds(18, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4T", 3)){Alex_TripAlgorithm1pos_updateThresholds(19, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4U", 3)){Alex_TripAlgorithm1pos_updateThresholds(20, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4V", 3)){Alex_TripAlgorithm1pos_updateThresholds(21, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4W", 3)){Alex_TripAlgorithm1pos_updateThresholds(22, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4X", 3)){Alex_TripAlgorithm1pos_updateThresholds(23, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4Y", 3)){Alex_TripAlgorithm1pos_updateThresholds(24, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4Z", 3)){Alex_TripAlgorithm1pos_updateThresholds(25, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4a", 3)){Alex_TripAlgorithm1pos_updateThresholds(26, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4b", 3)){Alex_TripAlgorithm1pos_updateThresholds(27, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4c", 3)){Alex_TripAlgorithm1pos_updateThresholds(28, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4d", 3)){Alex_TripAlgorithm1pos_updateThresholds(29, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4e", 3)){Alex_TripAlgorithm1pos_updateThresholds(30, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4f", 3)){Alex_TripAlgorithm1pos_updateThresholds(31, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4g", 3)){Alex_TripAlgorithm1pos_updateThresholds(32, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4h", 3)){Alex_TripAlgorithm1pos_updateThresholds(33, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4i", 3)){Alex_TripAlgorithm1pos_updateThresholds(34, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4j", 3)){Alex_TripAlgorithm1pos_updateThresholds(35, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4k", 3)){Alex_TripAlgorithm1pos_updateThresholds(36, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4l", 3)){Alex_TripAlgorithm1pos_updateThresholds(37, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4m", 3)){Alex_TripAlgorithm1pos_updateThresholds(38, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4n", 3)){Alex_TripAlgorithm1pos_updateThresholds(39, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4o", 3)){Alex_TripAlgorithm1pos_updateThresholds(40, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4p", 3)){Alex_TripAlgorithm1pos_updateThresholds(41, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4q", 3)){Alex_TripAlgorithm1pos_updateThresholds(42, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4r", 3)){Alex_TripAlgorithm1pos_updateThresholds(43, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4s", 3)){Alex_TripAlgorithm1pos_updateThresholds(44, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4t", 3)){Alex_TripAlgorithm1pos_updateThresholds(45, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4u", 3)){Alex_TripAlgorithm1pos_updateThresholds(46, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4v", 3)){Alex_TripAlgorithm1pos_updateThresholds(47, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4w", 3)){Alex_TripAlgorithm1pos_updateThresholds(48, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t4x", 3)){Alex_TripAlgorithm1pos_updateThresholds(49, 4);Alex_Confirm();return;}*/

	else if (!strncmp(RecvBuffer, "t5A", 3)){Alex_TripAlgorithm1pos_updateThresholds(0, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5B", 3)){Alex_TripAlgorithm1pos_updateThresholds(1, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5C", 3)){Alex_TripAlgorithm1pos_updateThresholds(2, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5D", 3)){Alex_TripAlgorithm1pos_updateThresholds(3, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5E", 3)){Alex_TripAlgorithm1pos_updateThresholds(4, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5F", 3)){Alex_TripAlgorithm1pos_updateThresholds(5, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5G", 3)){Alex_TripAlgorithm1pos_updateThresholds(6, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5H", 3)){Alex_TripAlgorithm1pos_updateThresholds(7, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5I", 3)){Alex_TripAlgorithm1pos_updateThresholds(8, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5J", 3)){Alex_TripAlgorithm1pos_updateThresholds(9, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5K", 3)){Alex_TripAlgorithm1pos_updateThresholds(10, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5L", 3)){Alex_TripAlgorithm1pos_updateThresholds(11, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5M", 3)){Alex_TripAlgorithm1pos_updateThresholds(12, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5N", 3)){Alex_TripAlgorithm1pos_updateThresholds(13, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5O", 3)){Alex_TripAlgorithm1pos_updateThresholds(14, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5P", 3)){Alex_TripAlgorithm1pos_updateThresholds(15, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5Q", 3)){Alex_TripAlgorithm1pos_updateThresholds(16, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5R", 3)){Alex_TripAlgorithm1pos_updateThresholds(17, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5S", 3)){Alex_TripAlgorithm1pos_updateThresholds(18, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5T", 3)){Alex_TripAlgorithm1pos_updateThresholds(19, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5U", 3)){Alex_TripAlgorithm1pos_updateThresholds(20, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5V", 3)){Alex_TripAlgorithm1pos_updateThresholds(21, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5W", 3)){Alex_TripAlgorithm1pos_updateThresholds(22, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5X", 3)){Alex_TripAlgorithm1pos_updateThresholds(23, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5Y", 3)){Alex_TripAlgorithm1pos_updateThresholds(24, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5Z", 3)){Alex_TripAlgorithm1pos_updateThresholds(25, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5a", 3)){Alex_TripAlgorithm1pos_updateThresholds(26, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5b", 3)){Alex_TripAlgorithm1pos_updateThresholds(27, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5c", 3)){Alex_TripAlgorithm1pos_updateThresholds(28, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5d", 3)){Alex_TripAlgorithm1pos_updateThresholds(29, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5e", 3)){Alex_TripAlgorithm1pos_updateThresholds(30, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5f", 3)){Alex_TripAlgorithm1pos_updateThresholds(31, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5g", 3)){Alex_TripAlgorithm1pos_updateThresholds(32, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5h", 3)){Alex_TripAlgorithm1pos_updateThresholds(33, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5i", 3)){Alex_TripAlgorithm1pos_updateThresholds(34, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5j", 3)){Alex_TripAlgorithm1pos_updateThresholds(35, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5k", 3)){Alex_TripAlgorithm1pos_updateThresholds(36, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5l", 3)){Alex_TripAlgorithm1pos_updateThresholds(37, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5m", 3)){Alex_TripAlgorithm1pos_updateThresholds(38, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5n", 3)){Alex_TripAlgorithm1pos_updateThresholds(39, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5o", 3)){Alex_TripAlgorithm1pos_updateThresholds(40, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5p", 3)){Alex_TripAlgorithm1pos_updateThresholds(41, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5q", 3)){Alex_TripAlgorithm1pos_updateThresholds(42, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5r", 3)){Alex_TripAlgorithm1pos_updateThresholds(43, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5s", 3)){Alex_TripAlgorithm1pos_updateThresholds(44, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5t", 3)){Alex_TripAlgorithm1pos_updateThresholds(45, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5u", 3)){Alex_TripAlgorithm1pos_updateThresholds(46, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5v", 3)){Alex_TripAlgorithm1pos_updateThresholds(47, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5w", 3)){Alex_TripAlgorithm1pos_updateThresholds(48, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t5x", 3)){Alex_TripAlgorithm1pos_updateThresholds(49, 5);Alex_Confirm();return;}

	/*else if (!strncmp(RecvBuffer, "t6A", 3)){Alex_TripAlgorithm1pos_updateThresholds(0, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6B", 3)){Alex_TripAlgorithm1pos_updateThresholds(1, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6C", 3)){Alex_TripAlgorithm1pos_updateThresholds(2, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6D", 3)){Alex_TripAlgorithm1pos_updateThresholds(3, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6E", 3)){Alex_TripAlgorithm1pos_updateThresholds(4, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6F", 3)){Alex_TripAlgorithm1pos_updateThresholds(5, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6G", 3)){Alex_TripAlgorithm1pos_updateThresholds(6, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6H", 3)){Alex_TripAlgorithm1pos_updateThresholds(7, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6I", 3)){Alex_TripAlgorithm1pos_updateThresholds(8, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6J", 3)){Alex_TripAlgorithm1pos_updateThresholds(9, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6K", 3)){Alex_TripAlgorithm1pos_updateThresholds(10, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6L", 3)){Alex_TripAlgorithm1pos_updateThresholds(11, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6M", 3)){Alex_TripAlgorithm1pos_updateThresholds(12, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6N", 3)){Alex_TripAlgorithm1pos_updateThresholds(13, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6O", 3)){Alex_TripAlgorithm1pos_updateThresholds(14, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6P", 3)){Alex_TripAlgorithm1pos_updateThresholds(15, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6Q", 3)){Alex_TripAlgorithm1pos_updateThresholds(16, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6R", 3)){Alex_TripAlgorithm1pos_updateThresholds(17, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6S", 3)){Alex_TripAlgorithm1pos_updateThresholds(18, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6T", 3)){Alex_TripAlgorithm1pos_updateThresholds(19, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6U", 3)){Alex_TripAlgorithm1pos_updateThresholds(20, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6V", 3)){Alex_TripAlgorithm1pos_updateThresholds(21, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6W", 3)){Alex_TripAlgorithm1pos_updateThresholds(22, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6X", 3)){Alex_TripAlgorithm1pos_updateThresholds(23, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6Y", 3)){Alex_TripAlgorithm1pos_updateThresholds(24, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6Z", 3)){Alex_TripAlgorithm1pos_updateThresholds(25, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6a", 3)){Alex_TripAlgorithm1pos_updateThresholds(26, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6b", 3)){Alex_TripAlgorithm1pos_updateThresholds(27, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6c", 3)){Alex_TripAlgorithm1pos_updateThresholds(28, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6d", 3)){Alex_TripAlgorithm1pos_updateThresholds(29, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6e", 3)){Alex_TripAlgorithm1pos_updateThresholds(30, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6f", 3)){Alex_TripAlgorithm1pos_updateThresholds(31, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6g", 3)){Alex_TripAlgorithm1pos_updateThresholds(32, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6h", 3)){Alex_TripAlgorithm1pos_updateThresholds(33, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6i", 3)){Alex_TripAlgorithm1pos_updateThresholds(34, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6j", 3)){Alex_TripAlgorithm1pos_updateThresholds(35, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6k", 3)){Alex_TripAlgorithm1pos_updateThresholds(36, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6l", 3)){Alex_TripAlgorithm1pos_updateThresholds(37, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6m", 3)){Alex_TripAlgorithm1pos_updateThresholds(38, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6n", 3)){Alex_TripAlgorithm1pos_updateThresholds(39, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6o", 3)){Alex_TripAlgorithm1pos_updateThresholds(40, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6p", 3)){Alex_TripAlgorithm1pos_updateThresholds(41, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6q", 3)){Alex_TripAlgorithm1pos_updateThresholds(42, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6r", 3)){Alex_TripAlgorithm1pos_updateThresholds(43, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6s", 3)){Alex_TripAlgorithm1pos_updateThresholds(44, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6t", 3)){Alex_TripAlgorithm1pos_updateThresholds(45, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6u", 3)){Alex_TripAlgorithm1pos_updateThresholds(46, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6v", 3)){Alex_TripAlgorithm1pos_updateThresholds(47, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6w", 3)){Alex_TripAlgorithm1pos_updateThresholds(48, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t6x", 3)){Alex_TripAlgorithm1pos_updateThresholds(49, 6);Alex_Confirm();return;}*/

	/*else if (!strncmp(RecvBuffer, "t7A", 3)){Alex_TripAlgorithm1pos_updateThresholds(0, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7B", 3)){Alex_TripAlgorithm1pos_updateThresholds(1, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7C", 3)){Alex_TripAlgorithm1pos_updateThresholds(2, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7D", 3)){Alex_TripAlgorithm1pos_updateThresholds(3, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7E", 3)){Alex_TripAlgorithm1pos_updateThresholds(4, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7F", 3)){Alex_TripAlgorithm1pos_updateThresholds(5, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7G", 3)){Alex_TripAlgorithm1pos_updateThresholds(6, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7H", 3)){Alex_TripAlgorithm1pos_updateThresholds(7, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7I", 3)){Alex_TripAlgorithm1pos_updateThresholds(8, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7J", 3)){Alex_TripAlgorithm1pos_updateThresholds(9, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7K", 3)){Alex_TripAlgorithm1pos_updateThresholds(10, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7L", 3)){Alex_TripAlgorithm1pos_updateThresholds(11, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7M", 3)){Alex_TripAlgorithm1pos_updateThresholds(12, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7N", 3)){Alex_TripAlgorithm1pos_updateThresholds(13, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7O", 3)){Alex_TripAlgorithm1pos_updateThresholds(14, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7P", 3)){Alex_TripAlgorithm1pos_updateThresholds(15, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7Q", 3)){Alex_TripAlgorithm1pos_updateThresholds(16, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7R", 3)){Alex_TripAlgorithm1pos_updateThresholds(17, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7S", 3)){Alex_TripAlgorithm1pos_updateThresholds(18, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7T", 3)){Alex_TripAlgorithm1pos_updateThresholds(19, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7U", 3)){Alex_TripAlgorithm1pos_updateThresholds(20, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7V", 3)){Alex_TripAlgorithm1pos_updateThresholds(21, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7W", 3)){Alex_TripAlgorithm1pos_updateThresholds(22, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7X", 3)){Alex_TripAlgorithm1pos_updateThresholds(23, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7Y", 3)){Alex_TripAlgorithm1pos_updateThresholds(24, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7Z", 3)){Alex_TripAlgorithm1pos_updateThresholds(25, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7a", 3)){Alex_TripAlgorithm1pos_updateThresholds(26, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7b", 3)){Alex_TripAlgorithm1pos_updateThresholds(27, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7c", 3)){Alex_TripAlgorithm1pos_updateThresholds(28, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7d", 3)){Alex_TripAlgorithm1pos_updateThresholds(29, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7e", 3)){Alex_TripAlgorithm1pos_updateThresholds(30, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7f", 3)){Alex_TripAlgorithm1pos_updateThresholds(31, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7g", 3)){Alex_TripAlgorithm1pos_updateThresholds(32, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7h", 3)){Alex_TripAlgorithm1pos_updateThresholds(33, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7i", 3)){Alex_TripAlgorithm1pos_updateThresholds(34, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7j", 3)){Alex_TripAlgorithm1pos_updateThresholds(35, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7k", 3)){Alex_TripAlgorithm1pos_updateThresholds(36, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7l", 3)){Alex_TripAlgorithm1pos_updateThresholds(37, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7m", 3)){Alex_TripAlgorithm1pos_updateThresholds(38, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7n", 3)){Alex_TripAlgorithm1pos_updateThresholds(39, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7o", 3)){Alex_TripAlgorithm1pos_updateThresholds(40, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7p", 3)){Alex_TripAlgorithm1pos_updateThresholds(41, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7q", 3)){Alex_TripAlgorithm1pos_updateThresholds(42, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7r", 3)){Alex_TripAlgorithm1pos_updateThresholds(43, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7s", 3)){Alex_TripAlgorithm1pos_updateThresholds(44, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7t", 3)){Alex_TripAlgorithm1pos_updateThresholds(45, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7u", 3)){Alex_TripAlgorithm1pos_updateThresholds(46, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7v", 3)){Alex_TripAlgorithm1pos_updateThresholds(47, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7w", 3)){Alex_TripAlgorithm1pos_updateThresholds(48, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t7x", 3)){Alex_TripAlgorithm1pos_updateThresholds(49, 7);Alex_Confirm();return;}*/

	else if (!strncmp(RecvBuffer, "t8A", 3)){Alex_TripAlgorithm1pos_updateThresholds(0, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8B", 3)){Alex_TripAlgorithm1pos_updateThresholds(1, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8C", 3)){Alex_TripAlgorithm1pos_updateThresholds(2, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8D", 3)){Alex_TripAlgorithm1pos_updateThresholds(3, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8E", 3)){Alex_TripAlgorithm1pos_updateThresholds(4, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8F", 3)){Alex_TripAlgorithm1pos_updateThresholds(5, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8G", 3)){Alex_TripAlgorithm1pos_updateThresholds(6, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8H", 3)){Alex_TripAlgorithm1pos_updateThresholds(7, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8I", 3)){Alex_TripAlgorithm1pos_updateThresholds(8, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8J", 3)){Alex_TripAlgorithm1pos_updateThresholds(9, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8K", 3)){Alex_TripAlgorithm1pos_updateThresholds(10, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8L", 3)){Alex_TripAlgorithm1pos_updateThresholds(11, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8M", 3)){Alex_TripAlgorithm1pos_updateThresholds(12, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8N", 3)){Alex_TripAlgorithm1pos_updateThresholds(13, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8O", 3)){Alex_TripAlgorithm1pos_updateThresholds(14, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8P", 3)){Alex_TripAlgorithm1pos_updateThresholds(15, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8Q", 3)){Alex_TripAlgorithm1pos_updateThresholds(16, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8R", 3)){Alex_TripAlgorithm1pos_updateThresholds(17, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8S", 3)){Alex_TripAlgorithm1pos_updateThresholds(18, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8T", 3)){Alex_TripAlgorithm1pos_updateThresholds(19, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8U", 3)){Alex_TripAlgorithm1pos_updateThresholds(20, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8V", 3)){Alex_TripAlgorithm1pos_updateThresholds(21, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8W", 3)){Alex_TripAlgorithm1pos_updateThresholds(22, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8X", 3)){Alex_TripAlgorithm1pos_updateThresholds(23, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8Y", 3)){Alex_TripAlgorithm1pos_updateThresholds(24, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8Z", 3)){Alex_TripAlgorithm1pos_updateThresholds(25, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8a", 3)){Alex_TripAlgorithm1pos_updateThresholds(26, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8b", 3)){Alex_TripAlgorithm1pos_updateThresholds(27, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8c", 3)){Alex_TripAlgorithm1pos_updateThresholds(28, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8d", 3)){Alex_TripAlgorithm1pos_updateThresholds(29, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8e", 3)){Alex_TripAlgorithm1pos_updateThresholds(30, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8f", 3)){Alex_TripAlgorithm1pos_updateThresholds(31, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8g", 3)){Alex_TripAlgorithm1pos_updateThresholds(32, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8h", 3)){Alex_TripAlgorithm1pos_updateThresholds(33, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8i", 3)){Alex_TripAlgorithm1pos_updateThresholds(34, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8j", 3)){Alex_TripAlgorithm1pos_updateThresholds(35, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8k", 3)){Alex_TripAlgorithm1pos_updateThresholds(36, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8l", 3)){Alex_TripAlgorithm1pos_updateThresholds(37, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8m", 3)){Alex_TripAlgorithm1pos_updateThresholds(38, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8n", 3)){Alex_TripAlgorithm1pos_updateThresholds(39, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8o", 3)){Alex_TripAlgorithm1pos_updateThresholds(40, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8p", 3)){Alex_TripAlgorithm1pos_updateThresholds(41, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8q", 3)){Alex_TripAlgorithm1pos_updateThresholds(42, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8r", 3)){Alex_TripAlgorithm1pos_updateThresholds(43, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8s", 3)){Alex_TripAlgorithm1pos_updateThresholds(44, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8t", 3)){Alex_TripAlgorithm1pos_updateThresholds(45, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8u", 3)){Alex_TripAlgorithm1pos_updateThresholds(46, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8v", 3)){Alex_TripAlgorithm1pos_updateThresholds(47, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8w", 3)){Alex_TripAlgorithm1pos_updateThresholds(48, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t8x", 3)){Alex_TripAlgorithm1pos_updateThresholds(49, 8);Alex_Confirm();return;}

	/*else if (!strncmp(RecvBuffer, "t9A", 3)){Alex_TripAlgorithm1pos_updateThresholds(0, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9B", 3)){Alex_TripAlgorithm1pos_updateThresholds(1, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9C", 3)){Alex_TripAlgorithm1pos_updateThresholds(2, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9D", 3)){Alex_TripAlgorithm1pos_updateThresholds(3, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9E", 3)){Alex_TripAlgorithm1pos_updateThresholds(4, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9F", 3)){Alex_TripAlgorithm1pos_updateThresholds(5, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9G", 3)){Alex_TripAlgorithm1pos_updateThresholds(6, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9H", 3)){Alex_TripAlgorithm1pos_updateThresholds(7, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9I", 3)){Alex_TripAlgorithm1pos_updateThresholds(8, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9J", 3)){Alex_TripAlgorithm1pos_updateThresholds(9, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9K", 3)){Alex_TripAlgorithm1pos_updateThresholds(10, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9L", 3)){Alex_TripAlgorithm1pos_updateThresholds(11, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9M", 3)){Alex_TripAlgorithm1pos_updateThresholds(12, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9N", 3)){Alex_TripAlgorithm1pos_updateThresholds(13, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9O", 3)){Alex_TripAlgorithm1pos_updateThresholds(14, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9P", 3)){Alex_TripAlgorithm1pos_updateThresholds(15, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9Q", 3)){Alex_TripAlgorithm1pos_updateThresholds(16, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9R", 3)){Alex_TripAlgorithm1pos_updateThresholds(17, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9S", 3)){Alex_TripAlgorithm1pos_updateThresholds(18, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9T", 3)){Alex_TripAlgorithm1pos_updateThresholds(19, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9U", 3)){Alex_TripAlgorithm1pos_updateThresholds(20, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9V", 3)){Alex_TripAlgorithm1pos_updateThresholds(21, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9W", 3)){Alex_TripAlgorithm1pos_updateThresholds(22, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9X", 3)){Alex_TripAlgorithm1pos_updateThresholds(23, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9Y", 3)){Alex_TripAlgorithm1pos_updateThresholds(24, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9Z", 3)){Alex_TripAlgorithm1pos_updateThresholds(25, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9a", 3)){Alex_TripAlgorithm1pos_updateThresholds(26, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9b", 3)){Alex_TripAlgorithm1pos_updateThresholds(27, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9c", 3)){Alex_TripAlgorithm1pos_updateThresholds(28, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9d", 3)){Alex_TripAlgorithm1pos_updateThresholds(29, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9e", 3)){Alex_TripAlgorithm1pos_updateThresholds(30, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9f", 3)){Alex_TripAlgorithm1pos_updateThresholds(31, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9g", 3)){Alex_TripAlgorithm1pos_updateThresholds(32, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9h", 3)){Alex_TripAlgorithm1pos_updateThresholds(33, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9i", 3)){Alex_TripAlgorithm1pos_updateThresholds(34, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9j", 3)){Alex_TripAlgorithm1pos_updateThresholds(35, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9k", 3)){Alex_TripAlgorithm1pos_updateThresholds(36, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9l", 3)){Alex_TripAlgorithm1pos_updateThresholds(37, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9m", 3)){Alex_TripAlgorithm1pos_updateThresholds(38, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9n", 3)){Alex_TripAlgorithm1pos_updateThresholds(39, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9o", 3)){Alex_TripAlgorithm1pos_updateThresholds(40, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9p", 3)){Alex_TripAlgorithm1pos_updateThresholds(41, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9q", 3)){Alex_TripAlgorithm1pos_updateThresholds(42, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9r", 3)){Alex_TripAlgorithm1pos_updateThresholds(43, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9s", 3)){Alex_TripAlgorithm1pos_updateThresholds(44, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9t", 3)){Alex_TripAlgorithm1pos_updateThresholds(45, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9u", 3)){Alex_TripAlgorithm1pos_updateThresholds(46, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9v", 3)){Alex_TripAlgorithm1pos_updateThresholds(47, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9w", 3)){Alex_TripAlgorithm1pos_updateThresholds(48, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "t9x", 3)){Alex_TripAlgorithm1pos_updateThresholds(49, 9);Alex_Confirm();return;}*/

	/*else if (!strncmp(RecvBuffer, "taA", 3)){Alex_TripAlgorithm1pos_updateThresholds(0, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taB", 3)){Alex_TripAlgorithm1pos_updateThresholds(1, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taC", 3)){Alex_TripAlgorithm1pos_updateThresholds(2, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taD", 3)){Alex_TripAlgorithm1pos_updateThresholds(3, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taE", 3)){Alex_TripAlgorithm1pos_updateThresholds(4, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taF", 3)){Alex_TripAlgorithm1pos_updateThresholds(5, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taG", 3)){Alex_TripAlgorithm1pos_updateThresholds(6, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taH", 3)){Alex_TripAlgorithm1pos_updateThresholds(7, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taI", 3)){Alex_TripAlgorithm1pos_updateThresholds(8, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taJ", 3)){Alex_TripAlgorithm1pos_updateThresholds(9, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taK", 3)){Alex_TripAlgorithm1pos_updateThresholds(10, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taL", 3)){Alex_TripAlgorithm1pos_updateThresholds(11, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taM", 3)){Alex_TripAlgorithm1pos_updateThresholds(12, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taN", 3)){Alex_TripAlgorithm1pos_updateThresholds(13, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taO", 3)){Alex_TripAlgorithm1pos_updateThresholds(14, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taP", 3)){Alex_TripAlgorithm1pos_updateThresholds(15, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taQ", 3)){Alex_TripAlgorithm1pos_updateThresholds(16, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taR", 3)){Alex_TripAlgorithm1pos_updateThresholds(17, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taS", 3)){Alex_TripAlgorithm1pos_updateThresholds(18, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taT", 3)){Alex_TripAlgorithm1pos_updateThresholds(19, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taU", 3)){Alex_TripAlgorithm1pos_updateThresholds(20, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taV", 3)){Alex_TripAlgorithm1pos_updateThresholds(21, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taW", 3)){Alex_TripAlgorithm1pos_updateThresholds(22, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taX", 3)){Alex_TripAlgorithm1pos_updateThresholds(23, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taY", 3)){Alex_TripAlgorithm1pos_updateThresholds(24, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taZ", 3)){Alex_TripAlgorithm1pos_updateThresholds(25, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taa", 3)){Alex_TripAlgorithm1pos_updateThresholds(26, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tab", 3)){Alex_TripAlgorithm1pos_updateThresholds(27, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tac", 3)){Alex_TripAlgorithm1pos_updateThresholds(28, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tad", 3)){Alex_TripAlgorithm1pos_updateThresholds(29, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tae", 3)){Alex_TripAlgorithm1pos_updateThresholds(30, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taf", 3)){Alex_TripAlgorithm1pos_updateThresholds(31, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tag", 3)){Alex_TripAlgorithm1pos_updateThresholds(32, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tah", 3)){Alex_TripAlgorithm1pos_updateThresholds(33, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tai", 3)){Alex_TripAlgorithm1pos_updateThresholds(34, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taj", 3)){Alex_TripAlgorithm1pos_updateThresholds(35, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tak", 3)){Alex_TripAlgorithm1pos_updateThresholds(36, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tal", 3)){Alex_TripAlgorithm1pos_updateThresholds(37, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tam", 3)){Alex_TripAlgorithm1pos_updateThresholds(38, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tan", 3)){Alex_TripAlgorithm1pos_updateThresholds(39, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tao", 3)){Alex_TripAlgorithm1pos_updateThresholds(40, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tap", 3)){Alex_TripAlgorithm1pos_updateThresholds(41, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taq", 3)){Alex_TripAlgorithm1pos_updateThresholds(42, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tar", 3)){Alex_TripAlgorithm1pos_updateThresholds(43, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tas", 3)){Alex_TripAlgorithm1pos_updateThresholds(44, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tat", 3)){Alex_TripAlgorithm1pos_updateThresholds(45, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tau", 3)){Alex_TripAlgorithm1pos_updateThresholds(46, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tav", 3)){Alex_TripAlgorithm1pos_updateThresholds(47, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "taw", 3)){Alex_TripAlgorithm1pos_updateThresholds(48, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tax", 3)){Alex_TripAlgorithm1pos_updateThresholds(49, 10);Alex_Confirm();return;}*/



	else if (!strncmp(RecvBuffer, "tbA", 3)){Alex_TripAlgorithm2pos_updateThresholds(0, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbB", 3)){Alex_TripAlgorithm2pos_updateThresholds(1, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbC", 3)){Alex_TripAlgorithm2pos_updateThresholds(2, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbD", 3)){Alex_TripAlgorithm2pos_updateThresholds(3, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbE", 3)){Alex_TripAlgorithm2pos_updateThresholds(4, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbF", 3)){Alex_TripAlgorithm2pos_updateThresholds(5, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbG", 3)){Alex_TripAlgorithm2pos_updateThresholds(6, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbH", 3)){Alex_TripAlgorithm2pos_updateThresholds(7, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbI", 3)){Alex_TripAlgorithm2pos_updateThresholds(8, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbJ", 3)){Alex_TripAlgorithm2pos_updateThresholds(9, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbK", 3)){Alex_TripAlgorithm2pos_updateThresholds(10, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbL", 3)){Alex_TripAlgorithm2pos_updateThresholds(11, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbM", 3)){Alex_TripAlgorithm2pos_updateThresholds(12, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbN", 3)){Alex_TripAlgorithm2pos_updateThresholds(13, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbO", 3)){Alex_TripAlgorithm2pos_updateThresholds(14, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbP", 3)){Alex_TripAlgorithm2pos_updateThresholds(15, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbQ", 3)){Alex_TripAlgorithm2pos_updateThresholds(16, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbR", 3)){Alex_TripAlgorithm2pos_updateThresholds(17, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbS", 3)){Alex_TripAlgorithm2pos_updateThresholds(18, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbT", 3)){Alex_TripAlgorithm2pos_updateThresholds(19, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbU", 3)){Alex_TripAlgorithm2pos_updateThresholds(20, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbV", 3)){Alex_TripAlgorithm2pos_updateThresholds(21, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbW", 3)){Alex_TripAlgorithm2pos_updateThresholds(22, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbX", 3)){Alex_TripAlgorithm2pos_updateThresholds(23, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbY", 3)){Alex_TripAlgorithm2pos_updateThresholds(24, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbZ", 3)){Alex_TripAlgorithm2pos_updateThresholds(25, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tba", 3)){Alex_TripAlgorithm2pos_updateThresholds(26, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbb", 3)){Alex_TripAlgorithm2pos_updateThresholds(27, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbc", 3)){Alex_TripAlgorithm2pos_updateThresholds(28, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbd", 3)){Alex_TripAlgorithm2pos_updateThresholds(29, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbe", 3)){Alex_TripAlgorithm2pos_updateThresholds(30, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbf", 3)){Alex_TripAlgorithm2pos_updateThresholds(31, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbg", 3)){Alex_TripAlgorithm2pos_updateThresholds(32, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbh", 3)){Alex_TripAlgorithm2pos_updateThresholds(33, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbi", 3)){Alex_TripAlgorithm2pos_updateThresholds(34, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbj", 3)){Alex_TripAlgorithm2pos_updateThresholds(35, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbk", 3)){Alex_TripAlgorithm2pos_updateThresholds(36, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbl", 3)){Alex_TripAlgorithm2pos_updateThresholds(37, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbm", 3)){Alex_TripAlgorithm2pos_updateThresholds(38, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbn", 3)){Alex_TripAlgorithm2pos_updateThresholds(39, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbo", 3)){Alex_TripAlgorithm2pos_updateThresholds(40, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbp", 3)){Alex_TripAlgorithm2pos_updateThresholds(41, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbq", 3)){Alex_TripAlgorithm2pos_updateThresholds(42, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbr", 3)){Alex_TripAlgorithm2pos_updateThresholds(43, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbs", 3)){Alex_TripAlgorithm2pos_updateThresholds(44, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbt", 3)){Alex_TripAlgorithm2pos_updateThresholds(45, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbu", 3)){Alex_TripAlgorithm2pos_updateThresholds(46, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbv", 3)){Alex_TripAlgorithm2pos_updateThresholds(47, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbw", 3)){Alex_TripAlgorithm2pos_updateThresholds(48, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tbx", 3)){Alex_TripAlgorithm2pos_updateThresholds(49, 1);Alex_Confirm();return;}

	else if (!strncmp(RecvBuffer, "tcA", 3)){Alex_TripAlgorithm2pos_updateThresholds(0, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcB", 3)){Alex_TripAlgorithm2pos_updateThresholds(1, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcC", 3)){Alex_TripAlgorithm2pos_updateThresholds(2, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcD", 3)){Alex_TripAlgorithm2pos_updateThresholds(3, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcE", 3)){Alex_TripAlgorithm2pos_updateThresholds(4, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcF", 3)){Alex_TripAlgorithm2pos_updateThresholds(5, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcG", 3)){Alex_TripAlgorithm2pos_updateThresholds(6, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcH", 3)){Alex_TripAlgorithm2pos_updateThresholds(7, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcI", 3)){Alex_TripAlgorithm2pos_updateThresholds(8, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcJ", 3)){Alex_TripAlgorithm2pos_updateThresholds(9, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcK", 3)){Alex_TripAlgorithm2pos_updateThresholds(10, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcL", 3)){Alex_TripAlgorithm2pos_updateThresholds(11, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcM", 3)){Alex_TripAlgorithm2pos_updateThresholds(12, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcN", 3)){Alex_TripAlgorithm2pos_updateThresholds(13, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcO", 3)){Alex_TripAlgorithm2pos_updateThresholds(14, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcP", 3)){Alex_TripAlgorithm2pos_updateThresholds(15, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcQ", 3)){Alex_TripAlgorithm2pos_updateThresholds(16, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcR", 3)){Alex_TripAlgorithm2pos_updateThresholds(17, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcS", 3)){Alex_TripAlgorithm2pos_updateThresholds(18, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcT", 3)){Alex_TripAlgorithm2pos_updateThresholds(19, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcU", 3)){Alex_TripAlgorithm2pos_updateThresholds(20, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcV", 3)){Alex_TripAlgorithm2pos_updateThresholds(21, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcW", 3)){Alex_TripAlgorithm2pos_updateThresholds(22, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcX", 3)){Alex_TripAlgorithm2pos_updateThresholds(23, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcY", 3)){Alex_TripAlgorithm2pos_updateThresholds(24, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcZ", 3)){Alex_TripAlgorithm2pos_updateThresholds(25, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tca", 3)){Alex_TripAlgorithm2pos_updateThresholds(26, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcb", 3)){Alex_TripAlgorithm2pos_updateThresholds(27, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcc", 3)){Alex_TripAlgorithm2pos_updateThresholds(28, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcd", 3)){Alex_TripAlgorithm2pos_updateThresholds(29, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tce", 3)){Alex_TripAlgorithm2pos_updateThresholds(30, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcf", 3)){Alex_TripAlgorithm2pos_updateThresholds(31, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcg", 3)){Alex_TripAlgorithm2pos_updateThresholds(32, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tch", 3)){Alex_TripAlgorithm2pos_updateThresholds(33, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tci", 3)){Alex_TripAlgorithm2pos_updateThresholds(34, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcj", 3)){Alex_TripAlgorithm2pos_updateThresholds(35, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tck", 3)){Alex_TripAlgorithm2pos_updateThresholds(36, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcl", 3)){Alex_TripAlgorithm2pos_updateThresholds(37, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcm", 3)){Alex_TripAlgorithm2pos_updateThresholds(38, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcn", 3)){Alex_TripAlgorithm2pos_updateThresholds(39, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tco", 3)){Alex_TripAlgorithm2pos_updateThresholds(40, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcp", 3)){Alex_TripAlgorithm2pos_updateThresholds(41, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcq", 3)){Alex_TripAlgorithm2pos_updateThresholds(42, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcr", 3)){Alex_TripAlgorithm2pos_updateThresholds(43, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcs", 3)){Alex_TripAlgorithm2pos_updateThresholds(44, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tct", 3)){Alex_TripAlgorithm2pos_updateThresholds(45, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcu", 3)){Alex_TripAlgorithm2pos_updateThresholds(46, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcv", 3)){Alex_TripAlgorithm2pos_updateThresholds(47, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcw", 3)){Alex_TripAlgorithm2pos_updateThresholds(48, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tcx", 3)){Alex_TripAlgorithm2pos_updateThresholds(49, 2);Alex_Confirm();return;}

	else if (!strncmp(RecvBuffer, "tdA", 3)){Alex_TripAlgorithm2pos_updateThresholds(0, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdB", 3)){Alex_TripAlgorithm2pos_updateThresholds(1, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdC", 3)){Alex_TripAlgorithm2pos_updateThresholds(2, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdD", 3)){Alex_TripAlgorithm2pos_updateThresholds(3, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdE", 3)){Alex_TripAlgorithm2pos_updateThresholds(4, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdF", 3)){Alex_TripAlgorithm2pos_updateThresholds(5, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdG", 3)){Alex_TripAlgorithm2pos_updateThresholds(6, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdH", 3)){Alex_TripAlgorithm2pos_updateThresholds(7, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdI", 3)){Alex_TripAlgorithm2pos_updateThresholds(8, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdJ", 3)){Alex_TripAlgorithm2pos_updateThresholds(9, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdK", 3)){Alex_TripAlgorithm2pos_updateThresholds(10, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdL", 3)){Alex_TripAlgorithm2pos_updateThresholds(11, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdM", 3)){Alex_TripAlgorithm2pos_updateThresholds(12, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdN", 3)){Alex_TripAlgorithm2pos_updateThresholds(13, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdO", 3)){Alex_TripAlgorithm2pos_updateThresholds(14, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdP", 3)){Alex_TripAlgorithm2pos_updateThresholds(15, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdQ", 3)){Alex_TripAlgorithm2pos_updateThresholds(16, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdR", 3)){Alex_TripAlgorithm2pos_updateThresholds(17, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdS", 3)){Alex_TripAlgorithm2pos_updateThresholds(18, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdT", 3)){Alex_TripAlgorithm2pos_updateThresholds(19, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdU", 3)){Alex_TripAlgorithm2pos_updateThresholds(20, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdV", 3)){Alex_TripAlgorithm2pos_updateThresholds(21, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdW", 3)){Alex_TripAlgorithm2pos_updateThresholds(22, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdX", 3)){Alex_TripAlgorithm2pos_updateThresholds(23, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdY", 3)){Alex_TripAlgorithm2pos_updateThresholds(24, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdZ", 3)){Alex_TripAlgorithm2pos_updateThresholds(25, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tda", 3)){Alex_TripAlgorithm2pos_updateThresholds(26, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdb", 3)){Alex_TripAlgorithm2pos_updateThresholds(27, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdc", 3)){Alex_TripAlgorithm2pos_updateThresholds(28, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdd", 3)){Alex_TripAlgorithm2pos_updateThresholds(29, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tde", 3)){Alex_TripAlgorithm2pos_updateThresholds(30, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdf", 3)){Alex_TripAlgorithm2pos_updateThresholds(31, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdg", 3)){Alex_TripAlgorithm2pos_updateThresholds(32, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdh", 3)){Alex_TripAlgorithm2pos_updateThresholds(33, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdi", 3)){Alex_TripAlgorithm2pos_updateThresholds(34, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdj", 3)){Alex_TripAlgorithm2pos_updateThresholds(35, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdk", 3)){Alex_TripAlgorithm2pos_updateThresholds(36, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdl", 3)){Alex_TripAlgorithm2pos_updateThresholds(37, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdm", 3)){Alex_TripAlgorithm2pos_updateThresholds(38, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdn", 3)){Alex_TripAlgorithm2pos_updateThresholds(39, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdo", 3)){Alex_TripAlgorithm2pos_updateThresholds(40, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdp", 3)){Alex_TripAlgorithm2pos_updateThresholds(41, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdq", 3)){Alex_TripAlgorithm2pos_updateThresholds(42, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdr", 3)){Alex_TripAlgorithm2pos_updateThresholds(43, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tds", 3)){Alex_TripAlgorithm2pos_updateThresholds(44, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdt", 3)){Alex_TripAlgorithm2pos_updateThresholds(45, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdu", 3)){Alex_TripAlgorithm2pos_updateThresholds(46, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdv", 3)){Alex_TripAlgorithm2pos_updateThresholds(47, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdw", 3)){Alex_TripAlgorithm2pos_updateThresholds(48, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tdx", 3)){Alex_TripAlgorithm2pos_updateThresholds(49, 3);Alex_Confirm();return;}

	/*else if (!strncmp(RecvBuffer, "teA", 3)){Alex_TripAlgorithm2pos_updateThresholds(0, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teB", 3)){Alex_TripAlgorithm2pos_updateThresholds(1, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teC", 3)){Alex_TripAlgorithm2pos_updateThresholds(2, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teD", 3)){Alex_TripAlgorithm2pos_updateThresholds(3, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teE", 3)){Alex_TripAlgorithm2pos_updateThresholds(4, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teF", 3)){Alex_TripAlgorithm2pos_updateThresholds(5, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teG", 3)){Alex_TripAlgorithm2pos_updateThresholds(6, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teH", 3)){Alex_TripAlgorithm2pos_updateThresholds(7, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teI", 3)){Alex_TripAlgorithm2pos_updateThresholds(8, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teJ", 3)){Alex_TripAlgorithm2pos_updateThresholds(9, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teK", 3)){Alex_TripAlgorithm2pos_updateThresholds(10, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teL", 3)){Alex_TripAlgorithm2pos_updateThresholds(11, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teM", 3)){Alex_TripAlgorithm2pos_updateThresholds(12, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teN", 3)){Alex_TripAlgorithm2pos_updateThresholds(13, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teO", 3)){Alex_TripAlgorithm2pos_updateThresholds(14, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teP", 3)){Alex_TripAlgorithm2pos_updateThresholds(15, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teQ", 3)){Alex_TripAlgorithm2pos_updateThresholds(16, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teR", 3)){Alex_TripAlgorithm2pos_updateThresholds(17, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teS", 3)){Alex_TripAlgorithm2pos_updateThresholds(18, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teT", 3)){Alex_TripAlgorithm2pos_updateThresholds(19, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teU", 3)){Alex_TripAlgorithm2pos_updateThresholds(20, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teV", 3)){Alex_TripAlgorithm2pos_updateThresholds(21, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teW", 3)){Alex_TripAlgorithm2pos_updateThresholds(22, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teX", 3)){Alex_TripAlgorithm2pos_updateThresholds(23, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teY", 3)){Alex_TripAlgorithm2pos_updateThresholds(24, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teZ", 3)){Alex_TripAlgorithm2pos_updateThresholds(25, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tea", 3)){Alex_TripAlgorithm2pos_updateThresholds(26, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teb", 3)){Alex_TripAlgorithm2pos_updateThresholds(27, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tec", 3)){Alex_TripAlgorithm2pos_updateThresholds(28, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ted", 3)){Alex_TripAlgorithm2pos_updateThresholds(29, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tee", 3)){Alex_TripAlgorithm2pos_updateThresholds(30, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tef", 3)){Alex_TripAlgorithm2pos_updateThresholds(31, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teg", 3)){Alex_TripAlgorithm2pos_updateThresholds(32, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teh", 3)){Alex_TripAlgorithm2pos_updateThresholds(33, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tei", 3)){Alex_TripAlgorithm2pos_updateThresholds(34, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tej", 3)){Alex_TripAlgorithm2pos_updateThresholds(35, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tek", 3)){Alex_TripAlgorithm2pos_updateThresholds(36, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tel", 3)){Alex_TripAlgorithm2pos_updateThresholds(37, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tem", 3)){Alex_TripAlgorithm2pos_updateThresholds(38, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ten", 3)){Alex_TripAlgorithm2pos_updateThresholds(39, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teo", 3)){Alex_TripAlgorithm2pos_updateThresholds(40, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tep", 3)){Alex_TripAlgorithm2pos_updateThresholds(41, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teq", 3)){Alex_TripAlgorithm2pos_updateThresholds(42, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ter", 3)){Alex_TripAlgorithm2pos_updateThresholds(43, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tes", 3)){Alex_TripAlgorithm2pos_updateThresholds(44, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tet", 3)){Alex_TripAlgorithm2pos_updateThresholds(45, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "teu", 3)){Alex_TripAlgorithm2pos_updateThresholds(46, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tev", 3)){Alex_TripAlgorithm2pos_updateThresholds(47, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tew", 3)){Alex_TripAlgorithm2pos_updateThresholds(48, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tex", 3)){Alex_TripAlgorithm2pos_updateThresholds(49, 4);Alex_Confirm();return;}*/

	else if (!strncmp(RecvBuffer, "tfA", 3)){Alex_TripAlgorithm2pos_updateThresholds(0, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfB", 3)){Alex_TripAlgorithm2pos_updateThresholds(1, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfC", 3)){Alex_TripAlgorithm2pos_updateThresholds(2, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfD", 3)){Alex_TripAlgorithm2pos_updateThresholds(3, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfE", 3)){Alex_TripAlgorithm2pos_updateThresholds(4, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfF", 3)){Alex_TripAlgorithm2pos_updateThresholds(5, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfG", 3)){Alex_TripAlgorithm2pos_updateThresholds(6, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfH", 3)){Alex_TripAlgorithm2pos_updateThresholds(7, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfI", 3)){Alex_TripAlgorithm2pos_updateThresholds(8, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfJ", 3)){Alex_TripAlgorithm2pos_updateThresholds(9, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfK", 3)){Alex_TripAlgorithm2pos_updateThresholds(10, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfL", 3)){Alex_TripAlgorithm2pos_updateThresholds(11, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfM", 3)){Alex_TripAlgorithm2pos_updateThresholds(12, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfN", 3)){Alex_TripAlgorithm2pos_updateThresholds(13, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfO", 3)){Alex_TripAlgorithm2pos_updateThresholds(14, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfP", 3)){Alex_TripAlgorithm2pos_updateThresholds(15, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfQ", 3)){Alex_TripAlgorithm2pos_updateThresholds(16, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfR", 3)){Alex_TripAlgorithm2pos_updateThresholds(17, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfS", 3)){Alex_TripAlgorithm2pos_updateThresholds(18, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfT", 3)){Alex_TripAlgorithm2pos_updateThresholds(19, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfU", 3)){Alex_TripAlgorithm2pos_updateThresholds(20, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfV", 3)){Alex_TripAlgorithm2pos_updateThresholds(21, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfW", 3)){Alex_TripAlgorithm2pos_updateThresholds(22, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfX", 3)){Alex_TripAlgorithm2pos_updateThresholds(23, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfY", 3)){Alex_TripAlgorithm2pos_updateThresholds(24, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfZ", 3)){Alex_TripAlgorithm2pos_updateThresholds(25, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfa", 3)){Alex_TripAlgorithm2pos_updateThresholds(26, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfb", 3)){Alex_TripAlgorithm2pos_updateThresholds(27, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfc", 3)){Alex_TripAlgorithm2pos_updateThresholds(28, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfd", 3)){Alex_TripAlgorithm2pos_updateThresholds(29, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfe", 3)){Alex_TripAlgorithm2pos_updateThresholds(30, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tff", 3)){Alex_TripAlgorithm2pos_updateThresholds(31, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfg", 3)){Alex_TripAlgorithm2pos_updateThresholds(32, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfh", 3)){Alex_TripAlgorithm2pos_updateThresholds(33, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfi", 3)){Alex_TripAlgorithm2pos_updateThresholds(34, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfj", 3)){Alex_TripAlgorithm2pos_updateThresholds(35, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfk", 3)){Alex_TripAlgorithm2pos_updateThresholds(36, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfl", 3)){Alex_TripAlgorithm2pos_updateThresholds(37, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfm", 3)){Alex_TripAlgorithm2pos_updateThresholds(38, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfn", 3)){Alex_TripAlgorithm2pos_updateThresholds(39, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfo", 3)){Alex_TripAlgorithm2pos_updateThresholds(40, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfp", 3)){Alex_TripAlgorithm2pos_updateThresholds(41, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfq", 3)){Alex_TripAlgorithm2pos_updateThresholds(42, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfr", 3)){Alex_TripAlgorithm2pos_updateThresholds(43, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfs", 3)){Alex_TripAlgorithm2pos_updateThresholds(44, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tft", 3)){Alex_TripAlgorithm2pos_updateThresholds(45, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfu", 3)){Alex_TripAlgorithm2pos_updateThresholds(46, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfv", 3)){Alex_TripAlgorithm2pos_updateThresholds(47, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfw", 3)){Alex_TripAlgorithm2pos_updateThresholds(48, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tfx", 3)){Alex_TripAlgorithm2pos_updateThresholds(49, 5);Alex_Confirm();return;}

	/*else if (!strncmp(RecvBuffer, "tgA", 3)){Alex_TripAlgorithm2pos_updateThresholds(0, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgB", 3)){Alex_TripAlgorithm2pos_updateThresholds(1, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgC", 3)){Alex_TripAlgorithm2pos_updateThresholds(2, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgD", 3)){Alex_TripAlgorithm2pos_updateThresholds(3, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgE", 3)){Alex_TripAlgorithm2pos_updateThresholds(4, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgF", 3)){Alex_TripAlgorithm2pos_updateThresholds(5, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgG", 3)){Alex_TripAlgorithm2pos_updateThresholds(6, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgH", 3)){Alex_TripAlgorithm2pos_updateThresholds(7, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgI", 3)){Alex_TripAlgorithm2pos_updateThresholds(8, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgJ", 3)){Alex_TripAlgorithm2pos_updateThresholds(9, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgK", 3)){Alex_TripAlgorithm2pos_updateThresholds(10, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgL", 3)){Alex_TripAlgorithm2pos_updateThresholds(11, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgM", 3)){Alex_TripAlgorithm2pos_updateThresholds(12, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgN", 3)){Alex_TripAlgorithm2pos_updateThresholds(13, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgO", 3)){Alex_TripAlgorithm2pos_updateThresholds(14, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgP", 3)){Alex_TripAlgorithm2pos_updateThresholds(15, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgQ", 3)){Alex_TripAlgorithm2pos_updateThresholds(16, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgR", 3)){Alex_TripAlgorithm2pos_updateThresholds(17, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgS", 3)){Alex_TripAlgorithm2pos_updateThresholds(18, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgT", 3)){Alex_TripAlgorithm2pos_updateThresholds(19, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgU", 3)){Alex_TripAlgorithm2pos_updateThresholds(20, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgV", 3)){Alex_TripAlgorithm2pos_updateThresholds(21, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgW", 3)){Alex_TripAlgorithm2pos_updateThresholds(22, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgX", 3)){Alex_TripAlgorithm2pos_updateThresholds(23, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgY", 3)){Alex_TripAlgorithm2pos_updateThresholds(24, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgZ", 3)){Alex_TripAlgorithm2pos_updateThresholds(25, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tga", 3)){Alex_TripAlgorithm2pos_updateThresholds(26, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgb", 3)){Alex_TripAlgorithm2pos_updateThresholds(27, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgc", 3)){Alex_TripAlgorithm2pos_updateThresholds(28, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgd", 3)){Alex_TripAlgorithm2pos_updateThresholds(29, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tge", 3)){Alex_TripAlgorithm2pos_updateThresholds(30, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgf", 3)){Alex_TripAlgorithm2pos_updateThresholds(31, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgg", 3)){Alex_TripAlgorithm2pos_updateThresholds(32, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgh", 3)){Alex_TripAlgorithm2pos_updateThresholds(33, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgi", 3)){Alex_TripAlgorithm2pos_updateThresholds(34, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgj", 3)){Alex_TripAlgorithm2pos_updateThresholds(35, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgk", 3)){Alex_TripAlgorithm2pos_updateThresholds(36, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgl", 3)){Alex_TripAlgorithm2pos_updateThresholds(37, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgm", 3)){Alex_TripAlgorithm2pos_updateThresholds(38, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgn", 3)){Alex_TripAlgorithm2pos_updateThresholds(39, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgo", 3)){Alex_TripAlgorithm2pos_updateThresholds(40, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgp", 3)){Alex_TripAlgorithm2pos_updateThresholds(41, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgq", 3)){Alex_TripAlgorithm2pos_updateThresholds(42, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgr", 3)){Alex_TripAlgorithm2pos_updateThresholds(43, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgs", 3)){Alex_TripAlgorithm2pos_updateThresholds(44, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgt", 3)){Alex_TripAlgorithm2pos_updateThresholds(45, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgu", 3)){Alex_TripAlgorithm2pos_updateThresholds(46, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgv", 3)){Alex_TripAlgorithm2pos_updateThresholds(47, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgw", 3)){Alex_TripAlgorithm2pos_updateThresholds(48, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tgx", 3)){Alex_TripAlgorithm2pos_updateThresholds(49, 6);Alex_Confirm();return;}*/

	/*else if (!strncmp(RecvBuffer, "thA", 3)){Alex_TripAlgorithm2pos_updateThresholds(0, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thB", 3)){Alex_TripAlgorithm2pos_updateThresholds(1, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thC", 3)){Alex_TripAlgorithm2pos_updateThresholds(2, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thD", 3)){Alex_TripAlgorithm2pos_updateThresholds(3, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thE", 3)){Alex_TripAlgorithm2pos_updateThresholds(4, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thF", 3)){Alex_TripAlgorithm2pos_updateThresholds(5, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thG", 3)){Alex_TripAlgorithm2pos_updateThresholds(6, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thH", 3)){Alex_TripAlgorithm2pos_updateThresholds(7, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thI", 3)){Alex_TripAlgorithm2pos_updateThresholds(8, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thJ", 3)){Alex_TripAlgorithm2pos_updateThresholds(9, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thK", 3)){Alex_TripAlgorithm2pos_updateThresholds(10, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thL", 3)){Alex_TripAlgorithm2pos_updateThresholds(11, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thM", 3)){Alex_TripAlgorithm2pos_updateThresholds(12, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thN", 3)){Alex_TripAlgorithm2pos_updateThresholds(13, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thO", 3)){Alex_TripAlgorithm2pos_updateThresholds(14, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thP", 3)){Alex_TripAlgorithm2pos_updateThresholds(15, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thQ", 3)){Alex_TripAlgorithm2pos_updateThresholds(16, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thR", 3)){Alex_TripAlgorithm2pos_updateThresholds(17, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thS", 3)){Alex_TripAlgorithm2pos_updateThresholds(18, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thT", 3)){Alex_TripAlgorithm2pos_updateThresholds(19, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thU", 3)){Alex_TripAlgorithm2pos_updateThresholds(20, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thV", 3)){Alex_TripAlgorithm2pos_updateThresholds(21, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thW", 3)){Alex_TripAlgorithm2pos_updateThresholds(22, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thX", 3)){Alex_TripAlgorithm2pos_updateThresholds(23, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thY", 3)){Alex_TripAlgorithm2pos_updateThresholds(24, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thZ", 3)){Alex_TripAlgorithm2pos_updateThresholds(25, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tha", 3)){Alex_TripAlgorithm2pos_updateThresholds(26, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thb", 3)){Alex_TripAlgorithm2pos_updateThresholds(27, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thc", 3)){Alex_TripAlgorithm2pos_updateThresholds(28, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thd", 3)){Alex_TripAlgorithm2pos_updateThresholds(29, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "the", 3)){Alex_TripAlgorithm2pos_updateThresholds(30, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thf", 3)){Alex_TripAlgorithm2pos_updateThresholds(31, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thg", 3)){Alex_TripAlgorithm2pos_updateThresholds(32, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thh", 3)){Alex_TripAlgorithm2pos_updateThresholds(33, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thi", 3)){Alex_TripAlgorithm2pos_updateThresholds(34, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thj", 3)){Alex_TripAlgorithm2pos_updateThresholds(35, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thk", 3)){Alex_TripAlgorithm2pos_updateThresholds(36, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thl", 3)){Alex_TripAlgorithm2pos_updateThresholds(37, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thm", 3)){Alex_TripAlgorithm2pos_updateThresholds(38, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thn", 3)){Alex_TripAlgorithm2pos_updateThresholds(39, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tho", 3)){Alex_TripAlgorithm2pos_updateThresholds(40, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thp", 3)){Alex_TripAlgorithm2pos_updateThresholds(41, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thq", 3)){Alex_TripAlgorithm2pos_updateThresholds(42, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thr", 3)){Alex_TripAlgorithm2pos_updateThresholds(43, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ths", 3)){Alex_TripAlgorithm2pos_updateThresholds(44, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tht", 3)){Alex_TripAlgorithm2pos_updateThresholds(45, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thu", 3)){Alex_TripAlgorithm2pos_updateThresholds(46, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thv", 3)){Alex_TripAlgorithm2pos_updateThresholds(47, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thw", 3)){Alex_TripAlgorithm2pos_updateThresholds(48, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "thx", 3)){Alex_TripAlgorithm2pos_updateThresholds(49, 7);Alex_Confirm();return;}*/

	else if (!strncmp(RecvBuffer, "tiA", 3)){Alex_TripAlgorithm2pos_updateThresholds(0, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiB", 3)){Alex_TripAlgorithm2pos_updateThresholds(1, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiC", 3)){Alex_TripAlgorithm2pos_updateThresholds(2, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiD", 3)){Alex_TripAlgorithm2pos_updateThresholds(3, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiE", 3)){Alex_TripAlgorithm2pos_updateThresholds(4, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiF", 3)){Alex_TripAlgorithm2pos_updateThresholds(5, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiG", 3)){Alex_TripAlgorithm2pos_updateThresholds(6, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiH", 3)){Alex_TripAlgorithm2pos_updateThresholds(7, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiI", 3)){Alex_TripAlgorithm2pos_updateThresholds(8, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiJ", 3)){Alex_TripAlgorithm2pos_updateThresholds(9, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiK", 3)){Alex_TripAlgorithm2pos_updateThresholds(10, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiL", 3)){Alex_TripAlgorithm2pos_updateThresholds(11, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiM", 3)){Alex_TripAlgorithm2pos_updateThresholds(12, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiN", 3)){Alex_TripAlgorithm2pos_updateThresholds(13, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiO", 3)){Alex_TripAlgorithm2pos_updateThresholds(14, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiP", 3)){Alex_TripAlgorithm2pos_updateThresholds(15, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiQ", 3)){Alex_TripAlgorithm2pos_updateThresholds(16, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiR", 3)){Alex_TripAlgorithm2pos_updateThresholds(17, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiS", 3)){Alex_TripAlgorithm2pos_updateThresholds(18, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiT", 3)){Alex_TripAlgorithm2pos_updateThresholds(19, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiU", 3)){Alex_TripAlgorithm2pos_updateThresholds(20, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiV", 3)){Alex_TripAlgorithm2pos_updateThresholds(21, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiW", 3)){Alex_TripAlgorithm2pos_updateThresholds(22, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiX", 3)){Alex_TripAlgorithm2pos_updateThresholds(23, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiY", 3)){Alex_TripAlgorithm2pos_updateThresholds(24, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiZ", 3)){Alex_TripAlgorithm2pos_updateThresholds(25, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tia", 3)){Alex_TripAlgorithm2pos_updateThresholds(26, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tib", 3)){Alex_TripAlgorithm2pos_updateThresholds(27, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tic", 3)){Alex_TripAlgorithm2pos_updateThresholds(28, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tid", 3)){Alex_TripAlgorithm2pos_updateThresholds(29, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tie", 3)){Alex_TripAlgorithm2pos_updateThresholds(30, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tif", 3)){Alex_TripAlgorithm2pos_updateThresholds(31, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tig", 3)){Alex_TripAlgorithm2pos_updateThresholds(32, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tih", 3)){Alex_TripAlgorithm2pos_updateThresholds(33, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tii", 3)){Alex_TripAlgorithm2pos_updateThresholds(34, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tij", 3)){Alex_TripAlgorithm2pos_updateThresholds(35, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tik", 3)){Alex_TripAlgorithm2pos_updateThresholds(36, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "til", 3)){Alex_TripAlgorithm2pos_updateThresholds(37, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tim", 3)){Alex_TripAlgorithm2pos_updateThresholds(38, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tin", 3)){Alex_TripAlgorithm2pos_updateThresholds(39, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tio", 3)){Alex_TripAlgorithm2pos_updateThresholds(40, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tip", 3)){Alex_TripAlgorithm2pos_updateThresholds(41, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiq", 3)){Alex_TripAlgorithm2pos_updateThresholds(42, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tir", 3)){Alex_TripAlgorithm2pos_updateThresholds(43, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tis", 3)){Alex_TripAlgorithm2pos_updateThresholds(44, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tit", 3)){Alex_TripAlgorithm2pos_updateThresholds(45, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiu", 3)){Alex_TripAlgorithm2pos_updateThresholds(46, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiv", 3)){Alex_TripAlgorithm2pos_updateThresholds(47, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tiw", 3)){Alex_TripAlgorithm2pos_updateThresholds(48, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tix", 3)){Alex_TripAlgorithm2pos_updateThresholds(49, 8);Alex_Confirm();return;}

	/*else if (!strncmp(RecvBuffer, "tjA", 3)){Alex_TripAlgorithm2pos_updateThresholds(0, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjB", 3)){Alex_TripAlgorithm2pos_updateThresholds(1, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjC", 3)){Alex_TripAlgorithm2pos_updateThresholds(2, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjD", 3)){Alex_TripAlgorithm2pos_updateThresholds(3, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjE", 3)){Alex_TripAlgorithm2pos_updateThresholds(4, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjF", 3)){Alex_TripAlgorithm2pos_updateThresholds(5, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjG", 3)){Alex_TripAlgorithm2pos_updateThresholds(6, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjH", 3)){Alex_TripAlgorithm2pos_updateThresholds(7, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjI", 3)){Alex_TripAlgorithm2pos_updateThresholds(8, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjJ", 3)){Alex_TripAlgorithm2pos_updateThresholds(9, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjK", 3)){Alex_TripAlgorithm2pos_updateThresholds(10, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjL", 3)){Alex_TripAlgorithm2pos_updateThresholds(11, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjM", 3)){Alex_TripAlgorithm2pos_updateThresholds(12, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjN", 3)){Alex_TripAlgorithm2pos_updateThresholds(13, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjO", 3)){Alex_TripAlgorithm2pos_updateThresholds(14, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjP", 3)){Alex_TripAlgorithm2pos_updateThresholds(15, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjQ", 3)){Alex_TripAlgorithm2pos_updateThresholds(16, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjR", 3)){Alex_TripAlgorithm2pos_updateThresholds(17, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjS", 3)){Alex_TripAlgorithm2pos_updateThresholds(18, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjT", 3)){Alex_TripAlgorithm2pos_updateThresholds(19, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjU", 3)){Alex_TripAlgorithm2pos_updateThresholds(20, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjV", 3)){Alex_TripAlgorithm2pos_updateThresholds(21, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjW", 3)){Alex_TripAlgorithm2pos_updateThresholds(22, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjX", 3)){Alex_TripAlgorithm2pos_updateThresholds(23, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjY", 3)){Alex_TripAlgorithm2pos_updateThresholds(24, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjZ", 3)){Alex_TripAlgorithm2pos_updateThresholds(25, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tja", 3)){Alex_TripAlgorithm2pos_updateThresholds(26, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjb", 3)){Alex_TripAlgorithm2pos_updateThresholds(27, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjc", 3)){Alex_TripAlgorithm2pos_updateThresholds(28, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjd", 3)){Alex_TripAlgorithm2pos_updateThresholds(29, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tje", 3)){Alex_TripAlgorithm2pos_updateThresholds(30, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjf", 3)){Alex_TripAlgorithm2pos_updateThresholds(31, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjg", 3)){Alex_TripAlgorithm2pos_updateThresholds(32, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjh", 3)){Alex_TripAlgorithm2pos_updateThresholds(33, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tji", 3)){Alex_TripAlgorithm2pos_updateThresholds(34, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjj", 3)){Alex_TripAlgorithm2pos_updateThresholds(35, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjk", 3)){Alex_TripAlgorithm2pos_updateThresholds(36, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjl", 3)){Alex_TripAlgorithm2pos_updateThresholds(37, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjm", 3)){Alex_TripAlgorithm2pos_updateThresholds(38, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjn", 3)){Alex_TripAlgorithm2pos_updateThresholds(39, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjo", 3)){Alex_TripAlgorithm2pos_updateThresholds(40, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjp", 3)){Alex_TripAlgorithm2pos_updateThresholds(41, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjq", 3)){Alex_TripAlgorithm2pos_updateThresholds(42, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjr", 3)){Alex_TripAlgorithm2pos_updateThresholds(43, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjs", 3)){Alex_TripAlgorithm2pos_updateThresholds(44, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjt", 3)){Alex_TripAlgorithm2pos_updateThresholds(45, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tju", 3)){Alex_TripAlgorithm2pos_updateThresholds(46, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjv", 3)){Alex_TripAlgorithm2pos_updateThresholds(47, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjw", 3)){Alex_TripAlgorithm2pos_updateThresholds(48, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tjx", 3)){Alex_TripAlgorithm2pos_updateThresholds(49, 9);Alex_Confirm();return;}*/

	/*else if (!strncmp(RecvBuffer, "tkA", 3)){Alex_TripAlgorithm2pos_updateThresholds(0, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkB", 3)){Alex_TripAlgorithm2pos_updateThresholds(1, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkC", 3)){Alex_TripAlgorithm2pos_updateThresholds(2, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkD", 3)){Alex_TripAlgorithm2pos_updateThresholds(3, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkE", 3)){Alex_TripAlgorithm2pos_updateThresholds(4, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkF", 3)){Alex_TripAlgorithm2pos_updateThresholds(5, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkG", 3)){Alex_TripAlgorithm2pos_updateThresholds(6, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkH", 3)){Alex_TripAlgorithm2pos_updateThresholds(7, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkI", 3)){Alex_TripAlgorithm2pos_updateThresholds(8, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkJ", 3)){Alex_TripAlgorithm2pos_updateThresholds(9, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkK", 3)){Alex_TripAlgorithm2pos_updateThresholds(10, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkL", 3)){Alex_TripAlgorithm2pos_updateThresholds(11, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkM", 3)){Alex_TripAlgorithm2pos_updateThresholds(12, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkN", 3)){Alex_TripAlgorithm2pos_updateThresholds(13, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkO", 3)){Alex_TripAlgorithm2pos_updateThresholds(14, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkP", 3)){Alex_TripAlgorithm2pos_updateThresholds(15, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkQ", 3)){Alex_TripAlgorithm2pos_updateThresholds(16, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkR", 3)){Alex_TripAlgorithm2pos_updateThresholds(17, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkS", 3)){Alex_TripAlgorithm2pos_updateThresholds(18, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkT", 3)){Alex_TripAlgorithm2pos_updateThresholds(19, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkU", 3)){Alex_TripAlgorithm2pos_updateThresholds(20, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkV", 3)){Alex_TripAlgorithm2pos_updateThresholds(21, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkW", 3)){Alex_TripAlgorithm2pos_updateThresholds(22, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkX", 3)){Alex_TripAlgorithm2pos_updateThresholds(23, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkY", 3)){Alex_TripAlgorithm2pos_updateThresholds(24, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkZ", 3)){Alex_TripAlgorithm2pos_updateThresholds(25, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tka", 3)){Alex_TripAlgorithm2pos_updateThresholds(26, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkb", 3)){Alex_TripAlgorithm2pos_updateThresholds(27, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkc", 3)){Alex_TripAlgorithm2pos_updateThresholds(28, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkd", 3)){Alex_TripAlgorithm2pos_updateThresholds(29, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tke", 3)){Alex_TripAlgorithm2pos_updateThresholds(30, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkf", 3)){Alex_TripAlgorithm2pos_updateThresholds(31, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkg", 3)){Alex_TripAlgorithm2pos_updateThresholds(32, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkh", 3)){Alex_TripAlgorithm2pos_updateThresholds(33, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tki", 3)){Alex_TripAlgorithm2pos_updateThresholds(34, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkj", 3)){Alex_TripAlgorithm2pos_updateThresholds(35, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkk", 3)){Alex_TripAlgorithm2pos_updateThresholds(36, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkl", 3)){Alex_TripAlgorithm2pos_updateThresholds(37, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkm", 3)){Alex_TripAlgorithm2pos_updateThresholds(38, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkn", 3)){Alex_TripAlgorithm2pos_updateThresholds(39, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tko", 3)){Alex_TripAlgorithm2pos_updateThresholds(40, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkp", 3)){Alex_TripAlgorithm2pos_updateThresholds(41, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkq", 3)){Alex_TripAlgorithm2pos_updateThresholds(42, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkr", 3)){Alex_TripAlgorithm2pos_updateThresholds(43, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tks", 3)){Alex_TripAlgorithm2pos_updateThresholds(44, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkt", 3)){Alex_TripAlgorithm2pos_updateThresholds(45, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tku", 3)){Alex_TripAlgorithm2pos_updateThresholds(46, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkv", 3)){Alex_TripAlgorithm2pos_updateThresholds(47, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkw", 3)){Alex_TripAlgorithm2pos_updateThresholds(48, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tkx", 3)){Alex_TripAlgorithm2pos_updateThresholds(49, 10);Alex_Confirm();return;}*/

	else if (!strncmp(RecvBuffer, "tlA", 3)){Alex_TripAlgorithm3pos_updateThresholds(0, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlB", 3)){Alex_TripAlgorithm3pos_updateThresholds(1, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlC", 3)){Alex_TripAlgorithm3pos_updateThresholds(2, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlD", 3)){Alex_TripAlgorithm3pos_updateThresholds(3, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlE", 3)){Alex_TripAlgorithm3pos_updateThresholds(4, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlF", 3)){Alex_TripAlgorithm3pos_updateThresholds(5, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlG", 3)){Alex_TripAlgorithm3pos_updateThresholds(6, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlH", 3)){Alex_TripAlgorithm3pos_updateThresholds(7, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlI", 3)){Alex_TripAlgorithm3pos_updateThresholds(8, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlJ", 3)){Alex_TripAlgorithm3pos_updateThresholds(9, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlK", 3)){Alex_TripAlgorithm3pos_updateThresholds(10, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlL", 3)){Alex_TripAlgorithm3pos_updateThresholds(11, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlM", 3)){Alex_TripAlgorithm3pos_updateThresholds(12, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlN", 3)){Alex_TripAlgorithm3pos_updateThresholds(13, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlO", 3)){Alex_TripAlgorithm3pos_updateThresholds(14, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlP", 3)){Alex_TripAlgorithm3pos_updateThresholds(15, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlQ", 3)){Alex_TripAlgorithm3pos_updateThresholds(16, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlR", 3)){Alex_TripAlgorithm3pos_updateThresholds(17, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlS", 3)){Alex_TripAlgorithm3pos_updateThresholds(18, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlT", 3)){Alex_TripAlgorithm3pos_updateThresholds(19, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlU", 3)){Alex_TripAlgorithm3pos_updateThresholds(20, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlV", 3)){Alex_TripAlgorithm3pos_updateThresholds(21, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlW", 3)){Alex_TripAlgorithm3pos_updateThresholds(22, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlX", 3)){Alex_TripAlgorithm3pos_updateThresholds(23, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlY", 3)){Alex_TripAlgorithm3pos_updateThresholds(24, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlZ", 3)){Alex_TripAlgorithm3pos_updateThresholds(25, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tla", 3)){Alex_TripAlgorithm3pos_updateThresholds(26, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlb", 3)){Alex_TripAlgorithm3pos_updateThresholds(27, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlc", 3)){Alex_TripAlgorithm3pos_updateThresholds(28, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tld", 3)){Alex_TripAlgorithm3pos_updateThresholds(29, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tle", 3)){Alex_TripAlgorithm3pos_updateThresholds(30, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlf", 3)){Alex_TripAlgorithm3pos_updateThresholds(31, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlg", 3)){Alex_TripAlgorithm3pos_updateThresholds(32, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlh", 3)){Alex_TripAlgorithm3pos_updateThresholds(33, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tli", 3)){Alex_TripAlgorithm3pos_updateThresholds(34, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlj", 3)){Alex_TripAlgorithm3pos_updateThresholds(35, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlk", 3)){Alex_TripAlgorithm3pos_updateThresholds(36, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tll", 3)){Alex_TripAlgorithm3pos_updateThresholds(37, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlm", 3)){Alex_TripAlgorithm3pos_updateThresholds(38, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tln", 3)){Alex_TripAlgorithm3pos_updateThresholds(39, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlo", 3)){Alex_TripAlgorithm3pos_updateThresholds(40, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlp", 3)){Alex_TripAlgorithm3pos_updateThresholds(41, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlq", 3)){Alex_TripAlgorithm3pos_updateThresholds(42, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlr", 3)){Alex_TripAlgorithm3pos_updateThresholds(43, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tls", 3)){Alex_TripAlgorithm3pos_updateThresholds(44, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlt", 3)){Alex_TripAlgorithm3pos_updateThresholds(45, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlu", 3)){Alex_TripAlgorithm3pos_updateThresholds(46, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlv", 3)){Alex_TripAlgorithm3pos_updateThresholds(47, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlw", 3)){Alex_TripAlgorithm3pos_updateThresholds(48, 1);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tlx", 3)){Alex_TripAlgorithm3pos_updateThresholds(49, 1);Alex_Confirm();return;}

	else if (!strncmp(RecvBuffer, "tmA", 3)){Alex_TripAlgorithm3pos_updateThresholds(0, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmB", 3)){Alex_TripAlgorithm3pos_updateThresholds(1, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmC", 3)){Alex_TripAlgorithm3pos_updateThresholds(2, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmD", 3)){Alex_TripAlgorithm3pos_updateThresholds(3, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmE", 3)){Alex_TripAlgorithm3pos_updateThresholds(4, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmF", 3)){Alex_TripAlgorithm3pos_updateThresholds(5, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmG", 3)){Alex_TripAlgorithm3pos_updateThresholds(6, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmH", 3)){Alex_TripAlgorithm3pos_updateThresholds(7, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmI", 3)){Alex_TripAlgorithm3pos_updateThresholds(8, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmJ", 3)){Alex_TripAlgorithm3pos_updateThresholds(9, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmK", 3)){Alex_TripAlgorithm3pos_updateThresholds(10, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmL", 3)){Alex_TripAlgorithm3pos_updateThresholds(11, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmM", 3)){Alex_TripAlgorithm3pos_updateThresholds(12, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmN", 3)){Alex_TripAlgorithm3pos_updateThresholds(13, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmO", 3)){Alex_TripAlgorithm3pos_updateThresholds(14, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmP", 3)){Alex_TripAlgorithm3pos_updateThresholds(15, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmQ", 3)){Alex_TripAlgorithm3pos_updateThresholds(16, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmR", 3)){Alex_TripAlgorithm3pos_updateThresholds(17, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmS", 3)){Alex_TripAlgorithm3pos_updateThresholds(18, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmT", 3)){Alex_TripAlgorithm3pos_updateThresholds(19, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmU", 3)){Alex_TripAlgorithm3pos_updateThresholds(20, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmV", 3)){Alex_TripAlgorithm3pos_updateThresholds(21, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmW", 3)){Alex_TripAlgorithm3pos_updateThresholds(22, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmX", 3)){Alex_TripAlgorithm3pos_updateThresholds(23, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmY", 3)){Alex_TripAlgorithm3pos_updateThresholds(24, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmZ", 3)){Alex_TripAlgorithm3pos_updateThresholds(25, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tma", 3)){Alex_TripAlgorithm3pos_updateThresholds(26, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmb", 3)){Alex_TripAlgorithm3pos_updateThresholds(27, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmc", 3)){Alex_TripAlgorithm3pos_updateThresholds(28, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmd", 3)){Alex_TripAlgorithm3pos_updateThresholds(29, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tme", 3)){Alex_TripAlgorithm3pos_updateThresholds(30, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmf", 3)){Alex_TripAlgorithm3pos_updateThresholds(31, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmg", 3)){Alex_TripAlgorithm3pos_updateThresholds(32, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmh", 3)){Alex_TripAlgorithm3pos_updateThresholds(33, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmi", 3)){Alex_TripAlgorithm3pos_updateThresholds(34, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmj", 3)){Alex_TripAlgorithm3pos_updateThresholds(35, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmk", 3)){Alex_TripAlgorithm3pos_updateThresholds(36, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tml", 3)){Alex_TripAlgorithm3pos_updateThresholds(37, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmm", 3)){Alex_TripAlgorithm3pos_updateThresholds(38, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmn", 3)){Alex_TripAlgorithm3pos_updateThresholds(39, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmo", 3)){Alex_TripAlgorithm3pos_updateThresholds(40, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmp", 3)){Alex_TripAlgorithm3pos_updateThresholds(41, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmq", 3)){Alex_TripAlgorithm3pos_updateThresholds(42, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmr", 3)){Alex_TripAlgorithm3pos_updateThresholds(43, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tms", 3)){Alex_TripAlgorithm3pos_updateThresholds(44, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmt", 3)){Alex_TripAlgorithm3pos_updateThresholds(45, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmu", 3)){Alex_TripAlgorithm3pos_updateThresholds(46, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmv", 3)){Alex_TripAlgorithm3pos_updateThresholds(47, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmw", 3)){Alex_TripAlgorithm3pos_updateThresholds(48, 2);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tmx", 3)){Alex_TripAlgorithm3pos_updateThresholds(49, 2);Alex_Confirm();return;}

	else if (!strncmp(RecvBuffer, "tnA", 3)){Alex_TripAlgorithm3pos_updateThresholds(0, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnB", 3)){Alex_TripAlgorithm3pos_updateThresholds(1, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnC", 3)){Alex_TripAlgorithm3pos_updateThresholds(2, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnD", 3)){Alex_TripAlgorithm3pos_updateThresholds(3, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnE", 3)){Alex_TripAlgorithm3pos_updateThresholds(4, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnF", 3)){Alex_TripAlgorithm3pos_updateThresholds(5, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnG", 3)){Alex_TripAlgorithm3pos_updateThresholds(6, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnH", 3)){Alex_TripAlgorithm3pos_updateThresholds(7, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnI", 3)){Alex_TripAlgorithm3pos_updateThresholds(8, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnJ", 3)){Alex_TripAlgorithm3pos_updateThresholds(9, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnK", 3)){Alex_TripAlgorithm3pos_updateThresholds(10, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnL", 3)){Alex_TripAlgorithm3pos_updateThresholds(11, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnM", 3)){Alex_TripAlgorithm3pos_updateThresholds(12, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnN", 3)){Alex_TripAlgorithm3pos_updateThresholds(13, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnO", 3)){Alex_TripAlgorithm3pos_updateThresholds(14, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnP", 3)){Alex_TripAlgorithm3pos_updateThresholds(15, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnQ", 3)){Alex_TripAlgorithm3pos_updateThresholds(16, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnR", 3)){Alex_TripAlgorithm3pos_updateThresholds(17, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnS", 3)){Alex_TripAlgorithm3pos_updateThresholds(18, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnT", 3)){Alex_TripAlgorithm3pos_updateThresholds(19, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnU", 3)){Alex_TripAlgorithm3pos_updateThresholds(20, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnV", 3)){Alex_TripAlgorithm3pos_updateThresholds(21, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnW", 3)){Alex_TripAlgorithm3pos_updateThresholds(22, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnX", 3)){Alex_TripAlgorithm3pos_updateThresholds(23, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnY", 3)){Alex_TripAlgorithm3pos_updateThresholds(24, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnZ", 3)){Alex_TripAlgorithm3pos_updateThresholds(25, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tna", 3)){Alex_TripAlgorithm3pos_updateThresholds(26, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnb", 3)){Alex_TripAlgorithm3pos_updateThresholds(27, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnc", 3)){Alex_TripAlgorithm3pos_updateThresholds(28, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnd", 3)){Alex_TripAlgorithm3pos_updateThresholds(29, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tne", 3)){Alex_TripAlgorithm3pos_updateThresholds(30, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnf", 3)){Alex_TripAlgorithm3pos_updateThresholds(31, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tng", 3)){Alex_TripAlgorithm3pos_updateThresholds(32, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnh", 3)){Alex_TripAlgorithm3pos_updateThresholds(33, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tni", 3)){Alex_TripAlgorithm3pos_updateThresholds(34, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnj", 3)){Alex_TripAlgorithm3pos_updateThresholds(35, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnk", 3)){Alex_TripAlgorithm3pos_updateThresholds(36, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnl", 3)){Alex_TripAlgorithm3pos_updateThresholds(37, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnm", 3)){Alex_TripAlgorithm3pos_updateThresholds(38, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnn", 3)){Alex_TripAlgorithm3pos_updateThresholds(39, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tno", 3)){Alex_TripAlgorithm3pos_updateThresholds(40, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnp", 3)){Alex_TripAlgorithm3pos_updateThresholds(41, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnq", 3)){Alex_TripAlgorithm3pos_updateThresholds(42, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnr", 3)){Alex_TripAlgorithm3pos_updateThresholds(43, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tns", 3)){Alex_TripAlgorithm3pos_updateThresholds(44, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnt", 3)){Alex_TripAlgorithm3pos_updateThresholds(45, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnu", 3)){Alex_TripAlgorithm3pos_updateThresholds(46, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnv", 3)){Alex_TripAlgorithm3pos_updateThresholds(47, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnw", 3)){Alex_TripAlgorithm3pos_updateThresholds(48, 3);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tnx", 3)){Alex_TripAlgorithm3pos_updateThresholds(49, 3);Alex_Confirm();return;}

	/*else if (!strncmp(RecvBuffer, "toA", 3)){Alex_TripAlgorithm3pos_updateThresholds(0, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toB", 3)){Alex_TripAlgorithm3pos_updateThresholds(1, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toC", 3)){Alex_TripAlgorithm3pos_updateThresholds(2, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toD", 3)){Alex_TripAlgorithm3pos_updateThresholds(3, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toE", 3)){Alex_TripAlgorithm3pos_updateThresholds(4, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toF", 3)){Alex_TripAlgorithm3pos_updateThresholds(5, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toG", 3)){Alex_TripAlgorithm3pos_updateThresholds(6, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toH", 3)){Alex_TripAlgorithm3pos_updateThresholds(7, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toI", 3)){Alex_TripAlgorithm3pos_updateThresholds(8, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toJ", 3)){Alex_TripAlgorithm3pos_updateThresholds(9, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toK", 3)){Alex_TripAlgorithm3pos_updateThresholds(10, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toL", 3)){Alex_TripAlgorithm3pos_updateThresholds(11, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toM", 3)){Alex_TripAlgorithm3pos_updateThresholds(12, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toN", 3)){Alex_TripAlgorithm3pos_updateThresholds(13, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toO", 3)){Alex_TripAlgorithm3pos_updateThresholds(14, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toP", 3)){Alex_TripAlgorithm3pos_updateThresholds(15, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toQ", 3)){Alex_TripAlgorithm3pos_updateThresholds(16, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toR", 3)){Alex_TripAlgorithm3pos_updateThresholds(17, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toS", 3)){Alex_TripAlgorithm3pos_updateThresholds(18, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toT", 3)){Alex_TripAlgorithm3pos_updateThresholds(19, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toU", 3)){Alex_TripAlgorithm3pos_updateThresholds(20, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toV", 3)){Alex_TripAlgorithm3pos_updateThresholds(21, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toW", 3)){Alex_TripAlgorithm3pos_updateThresholds(22, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toX", 3)){Alex_TripAlgorithm3pos_updateThresholds(23, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toY", 3)){Alex_TripAlgorithm3pos_updateThresholds(24, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toZ", 3)){Alex_TripAlgorithm3pos_updateThresholds(25, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toa", 3)){Alex_TripAlgorithm3pos_updateThresholds(26, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tob", 3)){Alex_TripAlgorithm3pos_updateThresholds(27, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toc", 3)){Alex_TripAlgorithm3pos_updateThresholds(28, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tod", 3)){Alex_TripAlgorithm3pos_updateThresholds(29, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toe", 3)){Alex_TripAlgorithm3pos_updateThresholds(30, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tof", 3)){Alex_TripAlgorithm3pos_updateThresholds(31, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tog", 3)){Alex_TripAlgorithm3pos_updateThresholds(32, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toh", 3)){Alex_TripAlgorithm3pos_updateThresholds(33, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toi", 3)){Alex_TripAlgorithm3pos_updateThresholds(34, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toj", 3)){Alex_TripAlgorithm3pos_updateThresholds(35, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tok", 3)){Alex_TripAlgorithm3pos_updateThresholds(36, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tol", 3)){Alex_TripAlgorithm3pos_updateThresholds(37, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tom", 3)){Alex_TripAlgorithm3pos_updateThresholds(38, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ton", 3)){Alex_TripAlgorithm3pos_updateThresholds(39, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "too", 3)){Alex_TripAlgorithm3pos_updateThresholds(40, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "top", 3)){Alex_TripAlgorithm3pos_updateThresholds(41, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "toq", 3)){Alex_TripAlgorithm3pos_updateThresholds(42, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tor", 3)){Alex_TripAlgorithm3pos_updateThresholds(43, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tos", 3)){Alex_TripAlgorithm3pos_updateThresholds(44, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tot", 3)){Alex_TripAlgorithm3pos_updateThresholds(45, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tou", 3)){Alex_TripAlgorithm3pos_updateThresholds(46, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tov", 3)){Alex_TripAlgorithm3pos_updateThresholds(47, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tow", 3)){Alex_TripAlgorithm3pos_updateThresholds(48, 4);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tox", 3)){Alex_TripAlgorithm3pos_updateThresholds(49, 4);Alex_Confirm();return;}*/

	else if (!strncmp(RecvBuffer, "tpA", 3)){Alex_TripAlgorithm3pos_updateThresholds(0, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpB", 3)){Alex_TripAlgorithm3pos_updateThresholds(1, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpC", 3)){Alex_TripAlgorithm3pos_updateThresholds(2, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpD", 3)){Alex_TripAlgorithm3pos_updateThresholds(3, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpE", 3)){Alex_TripAlgorithm3pos_updateThresholds(4, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpF", 3)){Alex_TripAlgorithm3pos_updateThresholds(5, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpG", 3)){Alex_TripAlgorithm3pos_updateThresholds(6, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpH", 3)){Alex_TripAlgorithm3pos_updateThresholds(7, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpI", 3)){Alex_TripAlgorithm3pos_updateThresholds(8, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpJ", 3)){Alex_TripAlgorithm3pos_updateThresholds(9, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpK", 3)){Alex_TripAlgorithm3pos_updateThresholds(10, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpL", 3)){Alex_TripAlgorithm3pos_updateThresholds(11, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpM", 3)){Alex_TripAlgorithm3pos_updateThresholds(12, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpN", 3)){Alex_TripAlgorithm3pos_updateThresholds(13, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpO", 3)){Alex_TripAlgorithm3pos_updateThresholds(14, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpP", 3)){Alex_TripAlgorithm3pos_updateThresholds(15, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpQ", 3)){Alex_TripAlgorithm3pos_updateThresholds(16, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpR", 3)){Alex_TripAlgorithm3pos_updateThresholds(17, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpS", 3)){Alex_TripAlgorithm3pos_updateThresholds(18, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpT", 3)){Alex_TripAlgorithm3pos_updateThresholds(19, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpU", 3)){Alex_TripAlgorithm3pos_updateThresholds(20, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpV", 3)){Alex_TripAlgorithm3pos_updateThresholds(21, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpW", 3)){Alex_TripAlgorithm3pos_updateThresholds(22, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpX", 3)){Alex_TripAlgorithm3pos_updateThresholds(23, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpY", 3)){Alex_TripAlgorithm3pos_updateThresholds(24, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpZ", 3)){Alex_TripAlgorithm3pos_updateThresholds(25, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpa", 3)){Alex_TripAlgorithm3pos_updateThresholds(26, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpb", 3)){Alex_TripAlgorithm3pos_updateThresholds(27, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpc", 3)){Alex_TripAlgorithm3pos_updateThresholds(28, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpd", 3)){Alex_TripAlgorithm3pos_updateThresholds(29, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpe", 3)){Alex_TripAlgorithm3pos_updateThresholds(30, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpf", 3)){Alex_TripAlgorithm3pos_updateThresholds(31, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpg", 3)){Alex_TripAlgorithm3pos_updateThresholds(32, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tph", 3)){Alex_TripAlgorithm3pos_updateThresholds(33, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpi", 3)){Alex_TripAlgorithm3pos_updateThresholds(34, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpj", 3)){Alex_TripAlgorithm3pos_updateThresholds(35, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpk", 3)){Alex_TripAlgorithm3pos_updateThresholds(36, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpl", 3)){Alex_TripAlgorithm3pos_updateThresholds(37, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpm", 3)){Alex_TripAlgorithm3pos_updateThresholds(38, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpn", 3)){Alex_TripAlgorithm3pos_updateThresholds(39, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpo", 3)){Alex_TripAlgorithm3pos_updateThresholds(40, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpp", 3)){Alex_TripAlgorithm3pos_updateThresholds(41, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpq", 3)){Alex_TripAlgorithm3pos_updateThresholds(42, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpr", 3)){Alex_TripAlgorithm3pos_updateThresholds(43, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tps", 3)){Alex_TripAlgorithm3pos_updateThresholds(44, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpt", 3)){Alex_TripAlgorithm3pos_updateThresholds(45, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpu", 3)){Alex_TripAlgorithm3pos_updateThresholds(46, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpv", 3)){Alex_TripAlgorithm3pos_updateThresholds(47, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpw", 3)){Alex_TripAlgorithm3pos_updateThresholds(48, 5);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tpx", 3)){Alex_TripAlgorithm3pos_updateThresholds(49, 5);Alex_Confirm();return;}

	/*else if (!strncmp(RecvBuffer, "tqA", 3)){Alex_TripAlgorithm3pos_updateThresholds(0, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqB", 3)){Alex_TripAlgorithm3pos_updateThresholds(1, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqC", 3)){Alex_TripAlgorithm3pos_updateThresholds(2, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqD", 3)){Alex_TripAlgorithm3pos_updateThresholds(3, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqE", 3)){Alex_TripAlgorithm3pos_updateThresholds(4, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqF", 3)){Alex_TripAlgorithm3pos_updateThresholds(5, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqG", 3)){Alex_TripAlgorithm3pos_updateThresholds(6, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqH", 3)){Alex_TripAlgorithm3pos_updateThresholds(7, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqI", 3)){Alex_TripAlgorithm3pos_updateThresholds(8, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqJ", 3)){Alex_TripAlgorithm3pos_updateThresholds(9, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqK", 3)){Alex_TripAlgorithm3pos_updateThresholds(10, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqL", 3)){Alex_TripAlgorithm3pos_updateThresholds(11, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqM", 3)){Alex_TripAlgorithm3pos_updateThresholds(12, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqN", 3)){Alex_TripAlgorithm3pos_updateThresholds(13, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqO", 3)){Alex_TripAlgorithm3pos_updateThresholds(14, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqP", 3)){Alex_TripAlgorithm3pos_updateThresholds(15, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqQ", 3)){Alex_TripAlgorithm3pos_updateThresholds(16, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqR", 3)){Alex_TripAlgorithm3pos_updateThresholds(17, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqS", 3)){Alex_TripAlgorithm3pos_updateThresholds(18, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqT", 3)){Alex_TripAlgorithm3pos_updateThresholds(19, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqU", 3)){Alex_TripAlgorithm3pos_updateThresholds(20, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqV", 3)){Alex_TripAlgorithm3pos_updateThresholds(21, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqW", 3)){Alex_TripAlgorithm3pos_updateThresholds(22, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqX", 3)){Alex_TripAlgorithm3pos_updateThresholds(23, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqY", 3)){Alex_TripAlgorithm3pos_updateThresholds(24, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqZ", 3)){Alex_TripAlgorithm3pos_updateThresholds(25, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqa", 3)){Alex_TripAlgorithm3pos_updateThresholds(26, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqb", 3)){Alex_TripAlgorithm3pos_updateThresholds(27, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqc", 3)){Alex_TripAlgorithm3pos_updateThresholds(28, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqd", 3)){Alex_TripAlgorithm3pos_updateThresholds(29, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqe", 3)){Alex_TripAlgorithm3pos_updateThresholds(30, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqf", 3)){Alex_TripAlgorithm3pos_updateThresholds(31, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqg", 3)){Alex_TripAlgorithm3pos_updateThresholds(32, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqh", 3)){Alex_TripAlgorithm3pos_updateThresholds(33, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqi", 3)){Alex_TripAlgorithm3pos_updateThresholds(34, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqj", 3)){Alex_TripAlgorithm3pos_updateThresholds(35, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqk", 3)){Alex_TripAlgorithm3pos_updateThresholds(36, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tql", 3)){Alex_TripAlgorithm3pos_updateThresholds(37, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqm", 3)){Alex_TripAlgorithm3pos_updateThresholds(38, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqn", 3)){Alex_TripAlgorithm3pos_updateThresholds(39, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqo", 3)){Alex_TripAlgorithm3pos_updateThresholds(40, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqp", 3)){Alex_TripAlgorithm3pos_updateThresholds(41, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqq", 3)){Alex_TripAlgorithm3pos_updateThresholds(42, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqr", 3)){Alex_TripAlgorithm3pos_updateThresholds(43, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqs", 3)){Alex_TripAlgorithm3pos_updateThresholds(44, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqt", 3)){Alex_TripAlgorithm3pos_updateThresholds(45, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqu", 3)){Alex_TripAlgorithm3pos_updateThresholds(46, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqv", 3)){Alex_TripAlgorithm3pos_updateThresholds(47, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqw", 3)){Alex_TripAlgorithm3pos_updateThresholds(48, 6);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tqx", 3)){Alex_TripAlgorithm3pos_updateThresholds(49, 6);Alex_Confirm();return;}*/

	/*else if (!strncmp(RecvBuffer, "trA", 3)){Alex_TripAlgorithm3pos_updateThresholds(0, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trB", 3)){Alex_TripAlgorithm3pos_updateThresholds(1, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trC", 3)){Alex_TripAlgorithm3pos_updateThresholds(2, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trD", 3)){Alex_TripAlgorithm3pos_updateThresholds(3, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trE", 3)){Alex_TripAlgorithm3pos_updateThresholds(4, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trF", 3)){Alex_TripAlgorithm3pos_updateThresholds(5, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trG", 3)){Alex_TripAlgorithm3pos_updateThresholds(6, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trH", 3)){Alex_TripAlgorithm3pos_updateThresholds(7, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trI", 3)){Alex_TripAlgorithm3pos_updateThresholds(8, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trJ", 3)){Alex_TripAlgorithm3pos_updateThresholds(9, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trK", 3)){Alex_TripAlgorithm3pos_updateThresholds(10, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trL", 3)){Alex_TripAlgorithm3pos_updateThresholds(11, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trM", 3)){Alex_TripAlgorithm3pos_updateThresholds(12, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trN", 3)){Alex_TripAlgorithm3pos_updateThresholds(13, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trO", 3)){Alex_TripAlgorithm3pos_updateThresholds(14, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trP", 3)){Alex_TripAlgorithm3pos_updateThresholds(15, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trQ", 3)){Alex_TripAlgorithm3pos_updateThresholds(16, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trR", 3)){Alex_TripAlgorithm3pos_updateThresholds(17, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trS", 3)){Alex_TripAlgorithm3pos_updateThresholds(18, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trT", 3)){Alex_TripAlgorithm3pos_updateThresholds(19, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trU", 3)){Alex_TripAlgorithm3pos_updateThresholds(20, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trV", 3)){Alex_TripAlgorithm3pos_updateThresholds(21, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trW", 3)){Alex_TripAlgorithm3pos_updateThresholds(22, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trX", 3)){Alex_TripAlgorithm3pos_updateThresholds(23, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trY", 3)){Alex_TripAlgorithm3pos_updateThresholds(24, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trZ", 3)){Alex_TripAlgorithm3pos_updateThresholds(25, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tra", 3)){Alex_TripAlgorithm3pos_updateThresholds(26, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trb", 3)){Alex_TripAlgorithm3pos_updateThresholds(27, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trc", 3)){Alex_TripAlgorithm3pos_updateThresholds(28, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trd", 3)){Alex_TripAlgorithm3pos_updateThresholds(29, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tre", 3)){Alex_TripAlgorithm3pos_updateThresholds(30, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trf", 3)){Alex_TripAlgorithm3pos_updateThresholds(31, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trg", 3)){Alex_TripAlgorithm3pos_updateThresholds(32, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trh", 3)){Alex_TripAlgorithm3pos_updateThresholds(33, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tri", 3)){Alex_TripAlgorithm3pos_updateThresholds(34, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trj", 3)){Alex_TripAlgorithm3pos_updateThresholds(35, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trk", 3)){Alex_TripAlgorithm3pos_updateThresholds(36, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trl", 3)){Alex_TripAlgorithm3pos_updateThresholds(37, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trm", 3)){Alex_TripAlgorithm3pos_updateThresholds(38, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trn", 3)){Alex_TripAlgorithm3pos_updateThresholds(39, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tro", 3)){Alex_TripAlgorithm3pos_updateThresholds(40, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trp", 3)){Alex_TripAlgorithm3pos_updateThresholds(41, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trq", 3)){Alex_TripAlgorithm3pos_updateThresholds(42, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trr", 3)){Alex_TripAlgorithm3pos_updateThresholds(43, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trs", 3)){Alex_TripAlgorithm3pos_updateThresholds(44, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trt", 3)){Alex_TripAlgorithm3pos_updateThresholds(45, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tru", 3)){Alex_TripAlgorithm3pos_updateThresholds(46, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trv", 3)){Alex_TripAlgorithm3pos_updateThresholds(47, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trw", 3)){Alex_TripAlgorithm3pos_updateThresholds(48, 7);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "trx", 3)){Alex_TripAlgorithm3pos_updateThresholds(49, 7);Alex_Confirm();return;}*/

	else if (!strncmp(RecvBuffer, "tsA", 3)){Alex_TripAlgorithm3pos_updateThresholds(0, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsB", 3)){Alex_TripAlgorithm3pos_updateThresholds(1, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsC", 3)){Alex_TripAlgorithm3pos_updateThresholds(2, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsD", 3)){Alex_TripAlgorithm3pos_updateThresholds(3, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsE", 3)){Alex_TripAlgorithm3pos_updateThresholds(4, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsF", 3)){Alex_TripAlgorithm3pos_updateThresholds(5, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsG", 3)){Alex_TripAlgorithm3pos_updateThresholds(6, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsH", 3)){Alex_TripAlgorithm3pos_updateThresholds(7, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsI", 3)){Alex_TripAlgorithm3pos_updateThresholds(8, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsJ", 3)){Alex_TripAlgorithm3pos_updateThresholds(9, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsK", 3)){Alex_TripAlgorithm3pos_updateThresholds(10, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsL", 3)){Alex_TripAlgorithm3pos_updateThresholds(11, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsM", 3)){Alex_TripAlgorithm3pos_updateThresholds(12, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsN", 3)){Alex_TripAlgorithm3pos_updateThresholds(13, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsO", 3)){Alex_TripAlgorithm3pos_updateThresholds(14, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsP", 3)){Alex_TripAlgorithm3pos_updateThresholds(15, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsQ", 3)){Alex_TripAlgorithm3pos_updateThresholds(16, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsR", 3)){Alex_TripAlgorithm3pos_updateThresholds(17, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsS", 3)){Alex_TripAlgorithm3pos_updateThresholds(18, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsT", 3)){Alex_TripAlgorithm3pos_updateThresholds(19, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsU", 3)){Alex_TripAlgorithm3pos_updateThresholds(20, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsV", 3)){Alex_TripAlgorithm3pos_updateThresholds(21, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsW", 3)){Alex_TripAlgorithm3pos_updateThresholds(22, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsX", 3)){Alex_TripAlgorithm3pos_updateThresholds(23, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsY", 3)){Alex_TripAlgorithm3pos_updateThresholds(24, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsZ", 3)){Alex_TripAlgorithm3pos_updateThresholds(25, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsa", 3)){Alex_TripAlgorithm3pos_updateThresholds(26, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsb", 3)){Alex_TripAlgorithm3pos_updateThresholds(27, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsc", 3)){Alex_TripAlgorithm3pos_updateThresholds(28, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsd", 3)){Alex_TripAlgorithm3pos_updateThresholds(29, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tse", 3)){Alex_TripAlgorithm3pos_updateThresholds(30, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsf", 3)){Alex_TripAlgorithm3pos_updateThresholds(31, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsg", 3)){Alex_TripAlgorithm3pos_updateThresholds(32, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsh", 3)){Alex_TripAlgorithm3pos_updateThresholds(33, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsi", 3)){Alex_TripAlgorithm3pos_updateThresholds(34, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsj", 3)){Alex_TripAlgorithm3pos_updateThresholds(35, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsk", 3)){Alex_TripAlgorithm3pos_updateThresholds(36, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsl", 3)){Alex_TripAlgorithm3pos_updateThresholds(37, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsm", 3)){Alex_TripAlgorithm3pos_updateThresholds(38, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsn", 3)){Alex_TripAlgorithm3pos_updateThresholds(39, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tso", 3)){Alex_TripAlgorithm3pos_updateThresholds(40, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsp", 3)){Alex_TripAlgorithm3pos_updateThresholds(41, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsq", 3)){Alex_TripAlgorithm3pos_updateThresholds(42, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsr", 3)){Alex_TripAlgorithm3pos_updateThresholds(43, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tss", 3)){Alex_TripAlgorithm3pos_updateThresholds(44, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tst", 3)){Alex_TripAlgorithm3pos_updateThresholds(45, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsu", 3)){Alex_TripAlgorithm3pos_updateThresholds(46, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsv", 3)){Alex_TripAlgorithm3pos_updateThresholds(47, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsw", 3)){Alex_TripAlgorithm3pos_updateThresholds(48, 8);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tsx", 3)){Alex_TripAlgorithm3pos_updateThresholds(49, 8);Alex_Confirm();return;}

	/*else if (!strncmp(RecvBuffer, "ttA", 3)){Alex_TripAlgorithm3pos_updateThresholds(0, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttB", 3)){Alex_TripAlgorithm3pos_updateThresholds(1, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttC", 3)){Alex_TripAlgorithm3pos_updateThresholds(2, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttD", 3)){Alex_TripAlgorithm3pos_updateThresholds(3, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttE", 3)){Alex_TripAlgorithm3pos_updateThresholds(4, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttF", 3)){Alex_TripAlgorithm3pos_updateThresholds(5, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttG", 3)){Alex_TripAlgorithm3pos_updateThresholds(6, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttH", 3)){Alex_TripAlgorithm3pos_updateThresholds(7, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttI", 3)){Alex_TripAlgorithm3pos_updateThresholds(8, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttJ", 3)){Alex_TripAlgorithm3pos_updateThresholds(9, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttK", 3)){Alex_TripAlgorithm3pos_updateThresholds(10, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttL", 3)){Alex_TripAlgorithm3pos_updateThresholds(11, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttM", 3)){Alex_TripAlgorithm3pos_updateThresholds(12, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttN", 3)){Alex_TripAlgorithm3pos_updateThresholds(13, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttO", 3)){Alex_TripAlgorithm3pos_updateThresholds(14, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttP", 3)){Alex_TripAlgorithm3pos_updateThresholds(15, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttQ", 3)){Alex_TripAlgorithm3pos_updateThresholds(16, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttR", 3)){Alex_TripAlgorithm3pos_updateThresholds(17, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttS", 3)){Alex_TripAlgorithm3pos_updateThresholds(18, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttT", 3)){Alex_TripAlgorithm3pos_updateThresholds(19, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttU", 3)){Alex_TripAlgorithm3pos_updateThresholds(20, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttV", 3)){Alex_TripAlgorithm3pos_updateThresholds(21, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttW", 3)){Alex_TripAlgorithm3pos_updateThresholds(22, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttX", 3)){Alex_TripAlgorithm3pos_updateThresholds(23, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttY", 3)){Alex_TripAlgorithm3pos_updateThresholds(24, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttZ", 3)){Alex_TripAlgorithm3pos_updateThresholds(25, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tta", 3)){Alex_TripAlgorithm3pos_updateThresholds(26, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttb", 3)){Alex_TripAlgorithm3pos_updateThresholds(27, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttc", 3)){Alex_TripAlgorithm3pos_updateThresholds(28, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttd", 3)){Alex_TripAlgorithm3pos_updateThresholds(29, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tte", 3)){Alex_TripAlgorithm3pos_updateThresholds(30, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttf", 3)){Alex_TripAlgorithm3pos_updateThresholds(31, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttg", 3)){Alex_TripAlgorithm3pos_updateThresholds(32, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tth", 3)){Alex_TripAlgorithm3pos_updateThresholds(33, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tti", 3)){Alex_TripAlgorithm3pos_updateThresholds(34, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttj", 3)){Alex_TripAlgorithm3pos_updateThresholds(35, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttk", 3)){Alex_TripAlgorithm3pos_updateThresholds(36, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttl", 3)){Alex_TripAlgorithm3pos_updateThresholds(37, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttm", 3)){Alex_TripAlgorithm3pos_updateThresholds(38, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttn", 3)){Alex_TripAlgorithm3pos_updateThresholds(39, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tto", 3)){Alex_TripAlgorithm3pos_updateThresholds(40, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttp", 3)){Alex_TripAlgorithm3pos_updateThresholds(41, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttq", 3)){Alex_TripAlgorithm3pos_updateThresholds(42, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttr", 3)){Alex_TripAlgorithm3pos_updateThresholds(43, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tts", 3)){Alex_TripAlgorithm3pos_updateThresholds(44, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttt", 3)){Alex_TripAlgorithm3pos_updateThresholds(45, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttu", 3)){Alex_TripAlgorithm3pos_updateThresholds(46, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttv", 3)){Alex_TripAlgorithm3pos_updateThresholds(47, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttw", 3)){Alex_TripAlgorithm3pos_updateThresholds(48, 9);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "ttx", 3)){Alex_TripAlgorithm3pos_updateThresholds(49, 9);Alex_Confirm();return;}

	else if (!strncmp(RecvBuffer, "tuA", 3)){Alex_TripAlgorithm3pos_updateThresholds(0, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuB", 3)){Alex_TripAlgorithm3pos_updateThresholds(1, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuC", 3)){Alex_TripAlgorithm3pos_updateThresholds(2, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuD", 3)){Alex_TripAlgorithm3pos_updateThresholds(3, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuE", 3)){Alex_TripAlgorithm3pos_updateThresholds(4, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuF", 3)){Alex_TripAlgorithm3pos_updateThresholds(5, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuG", 3)){Alex_TripAlgorithm3pos_updateThresholds(6, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuH", 3)){Alex_TripAlgorithm3pos_updateThresholds(7, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuI", 3)){Alex_TripAlgorithm3pos_updateThresholds(8, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuJ", 3)){Alex_TripAlgorithm3pos_updateThresholds(9, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuK", 3)){Alex_TripAlgorithm3pos_updateThresholds(10, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuL", 3)){Alex_TripAlgorithm3pos_updateThresholds(11, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuM", 3)){Alex_TripAlgorithm3pos_updateThresholds(12, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuN", 3)){Alex_TripAlgorithm3pos_updateThresholds(13, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuO", 3)){Alex_TripAlgorithm3pos_updateThresholds(14, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuP", 3)){Alex_TripAlgorithm3pos_updateThresholds(15, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuQ", 3)){Alex_TripAlgorithm3pos_updateThresholds(16, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuR", 3)){Alex_TripAlgorithm3pos_updateThresholds(17, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuS", 3)){Alex_TripAlgorithm3pos_updateThresholds(18, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuT", 3)){Alex_TripAlgorithm3pos_updateThresholds(19, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuU", 3)){Alex_TripAlgorithm3pos_updateThresholds(20, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuV", 3)){Alex_TripAlgorithm3pos_updateThresholds(21, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuW", 3)){Alex_TripAlgorithm3pos_updateThresholds(22, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuX", 3)){Alex_TripAlgorithm3pos_updateThresholds(23, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuY", 3)){Alex_TripAlgorithm3pos_updateThresholds(24, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuZ", 3)){Alex_TripAlgorithm3pos_updateThresholds(25, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tua", 3)){Alex_TripAlgorithm3pos_updateThresholds(26, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tub", 3)){Alex_TripAlgorithm3pos_updateThresholds(27, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuc", 3)){Alex_TripAlgorithm3pos_updateThresholds(28, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tud", 3)){Alex_TripAlgorithm3pos_updateThresholds(29, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tue", 3)){Alex_TripAlgorithm3pos_updateThresholds(30, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuf", 3)){Alex_TripAlgorithm3pos_updateThresholds(31, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tug", 3)){Alex_TripAlgorithm3pos_updateThresholds(32, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuh", 3)){Alex_TripAlgorithm3pos_updateThresholds(33, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tui", 3)){Alex_TripAlgorithm3pos_updateThresholds(34, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuj", 3)){Alex_TripAlgorithm3pos_updateThresholds(35, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuk", 3)){Alex_TripAlgorithm3pos_updateThresholds(36, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tul", 3)){Alex_TripAlgorithm3pos_updateThresholds(37, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tum", 3)){Alex_TripAlgorithm3pos_updateThresholds(38, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tun", 3)){Alex_TripAlgorithm3pos_updateThresholds(39, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuo", 3)){Alex_TripAlgorithm3pos_updateThresholds(40, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tup", 3)){Alex_TripAlgorithm3pos_updateThresholds(41, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuq", 3)){Alex_TripAlgorithm3pos_updateThresholds(42, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tur", 3)){Alex_TripAlgorithm3pos_updateThresholds(43, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tus", 3)){Alex_TripAlgorithm3pos_updateThresholds(44, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tut", 3)){Alex_TripAlgorithm3pos_updateThresholds(45, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuu", 3)){Alex_TripAlgorithm3pos_updateThresholds(46, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuv", 3)){Alex_TripAlgorithm3pos_updateThresholds(47, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tuw", 3)){Alex_TripAlgorithm3pos_updateThresholds(48, 10);Alex_Confirm();return;}
	else if (!strncmp(RecvBuffer, "tux", 3)){Alex_TripAlgorithm3pos_updateThresholds(49, 10);Alex_Confirm();return;}*/
}
/******************************************************************************
*
* Send data
*
* @param	u32 offset_signal
* @return None
* @note		None.
*
******************************************************************************/
void Alex_SendData(u32 offset_signal){

	u32      *sHeaderDatalogSize    = 0x1FF00274;
	u32      *sHeaderNumSigs        = 0x1FF00278;
	u32      *sHeaderFirstIdx       = 0x1FF0027C;
	u32      *sHeaderSampleFreq     = 0x1FF00280;
	Xuint64  *sHeaderLastSampleTime = 0x1FF00284;

	u32 i = 0;
	u32 offset_log = *sHeaderFirstIdx;
	u32 numSigs = *sHeaderNumSigs;
	u32 start = 0x1FF00300;
	u32 sDatalogVarsSize = (u32)(&__datlgvars_end - &__datlgvars_start); //65
	u32 sEndIdx = (ALEX_PARAM_DATALOG_LENGTH_US * ALEX_PARAM_ADC_SAMPLE_FREQ) / 1000000 - 1;
	u32 sMaxIdx = ( (u32)(&__datlg_end - &__datlg_start)) / sDatalogVarsSize - 1;
	u32 length = sEndIdx*(u32)4*numSigs;
	u32 end = start + length;

	int count_pause = 0;
	u32 *data;
	for (i=0; i<sEndIdx; i++){
		data = start + offset_signal + (i+offset_log-sEndIdx)*4*numSigs;
		if(data >= start){
			//printf("Ch1 start %d: adress %p with value %lu  \n\r", i, (void *)ch1, (unsigned long)*ch1);
		}
		else{
			data = end + offset_signal + (i+offset_log-sEndIdx+1)*4*numSigs;
			//printf("Ch1 end %d: adress %p with value %lu  \n\r", i, (void *)ch1, (unsigned long)*ch1);
		}
		SendBuffer[0] = (u32)*data;
		XUartPs_Send(&UartPs, SendBuffer, 4);
		for (count_pause = 0; count_pause <20000; count_pause++){} //previous 20000,10000
	}
}

/******************************************************************************
*
* Alex_Confirm(): Send dummy data
*
* @param	None.
* @return None.
* @note		None.
*
******************************************************************************/

void Alex_Confirm(){
	int i = 0;
	int count_pause = 0;
	for (i=0; i<100; i++){
		SendBuffer[0] = 0;
		XUartPs_Send(&UartPs, SendBuffer, 4);
		for (count_pause = 0; count_pause <50000; count_pause++){}
	}
}

/**************************************************************************/
/**
*
* This function is the handler which performs processing to handle data events
* from the device.  It is called from an interrupt context. so the amount of
* processing should be minimal.
*
* This handler provides an example of how to handle data for the device and
* is application specific.
*
* @param	CallBackRef contains a callback reference from the driver,
*		in this case it is the instance pointer for the XUartPs driver.
* @param	Event contains the specific kind of event that has occurred.
* @param	EventData contains the number of bytes sent or received for sent
*		and receive events.
*
* @return	None.
*
* @note		None.
*
***************************************************************************/
void Handler(void *CallBackRef, u32 Event, unsigned int EventData)
{
	/* All of the data has been sent */
	if (Event == XUARTPS_EVENT_SENT_DATA) {
		TotalSentCount = EventData;
	}

	/* All of the data has been received */
	if (Event == XUARTPS_EVENT_RECV_DATA) {
		TotalReceivedCount = EventData;
	}

	/*
	 * Data was received, but not the expected number of bytes, a
	 * timeout just indicates the data stopped for 8 character times
	 */
	if (Event == XUARTPS_EVENT_RECV_TOUT) {
		TotalReceivedCount = EventData;
	}

	/*
	 * Data was received with an error, keep the data but determine
	 * what kind of errors occurred
	 */
	if (Event == XUARTPS_EVENT_RECV_ERROR) {
		TotalReceivedCount = EventData;
		TotalErrorCount++;
	}

	/*
	 * Data was received with an parity or frame or break error, keep the data
	 * but determine what kind of errors occurred. Specific to Zynq Ultrascale+
	 * MP.
	 */
	if (Event == XUARTPS_EVENT_PARE_FRAME_BRKE) {
		TotalReceivedCount = EventData;
		TotalErrorCount++;
	}

	/*
	 * Data was received with an overrun error, keep the data but determine
	 * what kind of errors occurred. Specific to Zynq Ultrascale+ MP.
	 */
	if (Event == XUARTPS_EVENT_RECV_ORERR) {
		TotalReceivedCount = EventData;
		TotalErrorCount++;
	}
}


/*****************************************************************************/
/**
*
* This function sets up the interrupt system so interrupts can occur for the
* Uart. This function is application-specific. The user should modify this
* function to fit the application.
*
* @param	IntcInstancePtr is a pointer to the instance of the INTC.
* @param	UartInstancePtr contains a pointer to the instance of the UART
*		driver which is going to be connected to the interrupt
*		controller.
* @param	UartIntrId is the interrupt Id and is typically
*		XPAR_<UARTPS_instance>_INTR value from xparameters.h.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
****************************************************************************/
static int SetupInterruptSystem(INTC *IntcInstancePtr,
				XUartPs *UartInstancePtr,
				u16 UartIntrId)
{
	int Status;

#ifdef XPAR_INTC_0_DEVICE_ID
#ifndef TESTAPP_GEN
	/*
	 * Initialize the interrupt controller driver so that it's ready to
	 * use.
	 */
	Status = XIntc_Initialize(IntcInstancePtr, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif
	/*
	 * Connect the handler that will be called when an interrupt
	 * for the device occurs, the handler defined above performs the
	 * specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(IntcInstancePtr, UartIntrId,
		(XInterruptHandler) XUartPs_InterruptHandler, UartInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

#ifndef TESTAPP_GEN
	/*
	 * Start the interrupt controller so interrupts are enabled for all
	 * devices that cause interrupts.
	 */
	Status = XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif
	/*
	 * Enable the interrupt for uart
	 */
	XIntc_Enable(IntcInstancePtr, UartIntrId);

	#ifndef TESTAPP_GEN
	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler) XIntc_InterruptHandler,
				IntcInstancePtr);
	#endif
#else
#ifndef TESTAPP_GEN
	XScuGic_Config *IntcConfig; /* Config for interrupt controller */

	/* Initialize the interrupt controller driver */
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the interrupt controller interrupt handler to the
	 * hardware interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler) XScuGic_InterruptHandler,
				IntcInstancePtr);
#endif

	/*
	 * Connect a device driver handler that will be called when an
	 * interrupt for the device occurs, the device driver handler
	 * performs the specific interrupt processing for the device
	 */
	Status = XScuGic_Connect(IntcInstancePtr, UartIntrId,
				  (Xil_ExceptionHandler) XUartPs_InterruptHandler,
				  (void *) UartInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Enable the interrupt for the device */
	XScuGic_Enable(IntcInstancePtr, UartIntrId);

#endif
#ifndef TESTAPP_GEN
	/* Enable interrupts */
	 Xil_ExceptionEnable();
#endif

	return XST_SUCCESS;
}


/*****************************************************************************
*
* Void initialization of Uart_PS, calls Alex_UartInit.
*
* @param	None
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
* @note		None.
*
******************************************************************************/
void Alex_UartInit_void(){
	int status;
	status = Alex_UartInit(&InterruptController, &UartPs, UART_DEVICE_ID, UART_INT_IRQ_ID);
  if (status != XST_SUCCESS)
  {
  	return XST_FAILURE;
  }
}

/*****************************************************************************
*
* Initialization of Uart_PS
*
* @param	None
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
* @note		None.
*
******************************************************************************/
int Alex_UartInit(INTC *IntcInstPtr, XUartPs *UartInstPtr, u16 DeviceId, u16 UartIntrId)
{
	int Status;
	XUartPs_Config *Config;
	u32 IntrMask;

	if (XGetPlatform_Info() == XPLAT_ZYNQ) { //
#ifdef XPAR_XUARTPS_1_DEVICE_ID
		DeviceId = XPAR_XUARTPS_1_DEVICE_ID;
#endif
	}

	/*
	 * Initialize the UART driver so that it's ready to use
	 * Look up the configuration in the config table, then initialize it.
	 */
	Config = XUartPs_LookupConfig(DeviceId);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XUartPs_CfgInitialize(UartInstPtr, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Check hardware build */
	Status = XUartPs_SelfTest(UartInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the UART to the interrupt subsystem such that interrupts
	 * can occur. This function is application specific.
	 */
	Status = SetupInterruptSystem(IntcInstPtr, UartInstPtr, UartIntrId);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Setup the handlers for the UART that will be called from the
	 * interrupt context when data has been sent and received, specify
	 * a pointer to the UART driver instance as the callback reference
	 * so the handlers are able to access the instance data
	 */
	XUartPs_SetHandler(UartInstPtr, (XUartPs_Handler)Handler, UartInstPtr);

	/*
	 * Enable the interrupt of the UART so interrupts will occur, setup
	 * a local loopback so data that is sent will be received.
	 */
	IntrMask =
		XUARTPS_IXR_TOUT | XUARTPS_IXR_PARITY | XUARTPS_IXR_FRAMING |
		XUARTPS_IXR_OVER | XUARTPS_IXR_TXEMPTY | XUARTPS_IXR_RXFULL |
		XUARTPS_IXR_RXOVR;

	if (UartInstPtr->Platform == XPLAT_ZYNQ) {
		IntrMask |= XUARTPS_IXR_RBRK;
	}

	XUartPs_SetInterruptMask(UartInstPtr, IntrMask);

	//XUartPs_SetOperMode(UartInstPtr, XUARTPS_OPER_MODE_LOCAL_LOOP);
	XUartPs_SetOperMode(UartInstPtr, XUARTPS_OPER_MODE_NORMAL);

	/*
	 * Set the receiver timeout. If it is not set, and the last few bytes
	 * of data do not trigger the over-water or full interrupt, the bytes
	 * will not be received. By default it is disabled.
	 *
	 * The setting of 8 will timeout after 8 x 4 = 32 character times.
	 * Increase the time out value if baud rate is high, decrease it if
	 * baud rate is low.
	 */
	XUartPs_SetRecvTimeout(UartInstPtr, 32);

	/* Set the UART in Normal Mode */
	XUartPs_SetOperMode(UartInstPtr, XUARTPS_OPER_MODE_NORMAL);

	return XST_SUCCESS;
}
