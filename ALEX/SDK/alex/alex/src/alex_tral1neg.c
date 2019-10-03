/******************************************************************************

   Project:	      Alex

   Filename:      alex_tral1.c

   Description:    Trip neg1_algoritm

   © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

/***************************** Include Files *********************************/
#include "alex_tral1neg.h"
#include "xparameters.h"
#include "xil_types.h"
#include "xil_cache.h"
#include "xdmaps.h"
#include "alex_param.h"
#include "alex_datlg.h"
#include "alex_time.h"
#include "xgpiops.h"
#include "trip_algorithm_axi.h"

/************************** Constant Definitions *****************************/
/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/
/************************** Function Prototypes ******************************/
/******************** Local variable definitions *****************************/
//filter coefficients LPF 12.5 kHz cutoff 3*sqrt(2) damping -- for 50 kHz measurement
#define lpf_b0 					0.125f
#define lpf_b1 					0.5f
#define lpf_b2 					0.5f
#define lpf_a1 					2.0f
#define lpf_a2 					0.5f

//filter coefficients LPF 25 kHz cutoff
//#define lpf_b0 					1.0f
//#define lpf_b1 					1.0f
//#define lpf_b2 					1.0f
//#define lpf_a1 					4.0f
//#define lpf_a2 					4.0f

/****************** Declaration of external linker variables *****************/
extern u32 __datlgvars_start;
extern u32 __datlgvars_end;

/******************** Global variable definitions ****************************/
int neg1_trip __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));

u32 ch8_lpf_out __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 ch9_lpf_out __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));

u32 neg1_alg1_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 neg1_alg2_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 neg1_alg3_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 neg1_alg4_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 neg1_alg5_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 neg1_alg6_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 neg1_alg7_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 neg1_alg8_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 neg1_alg9_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 neg1_alg10_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));

u32 manual_fault_detected __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 neg1_fault_detected __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));

//variables that do not really need to be logged, but have to be available next time the function is called.
//If a variable is needed later, it will otherwise risk overflow.
u32 ch8_alarm_counter __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));

u32 neg1_alg1_threshold;
double neg1_alg2_threshold;
u32 neg1_alg3_threshold;
double neg1_alg4_threshold;
double neg1_alg5_threshold;
u32 neg1_alg6_threshold;
u32 neg1_alg7_threshold;
u32 neg1_alg8_threshold;
u32 neg1_alg9_threshold;
u32 neg1_alg10_threshold;

int neg1_alg1_enable;
int neg1_alg2_enable;
int neg1_alg3_enable;
int neg1_alg4_enable;
int neg1_alg5_enable;
int neg1_alg6_enable;
int neg1_alg7_enable;
int neg1_alg8_enable;
int neg1_alg9_enable;
int neg1_alg10_enable;

u32 *manual1neg_trip_old = 0;

//for LPF
u32 ch8_lpf_out_old = 0;
u32 ch9_lpf_out_old = 0;

u32 ch8_lpf_in = 0;
u32 ch8_lpf_out = 0;
u32 ch8_lpf_x1 = 0;
u32 ch8_lpf_x2 = 0;

u32 ch9_lpf_in = 0;
u32 ch9_lpf_out = 0;
u32 ch9_lpf_x1 = 0;
u32 ch9_lpf_x2 = 0;

u32 ch8_dxdt;
int ch8_dvdt_alarm;
u32 ch9_beforefault;

//for neg1_alg3;
u32 neg1_s1 = 0;
u32 ch9_dxdt;
u32 ch9_dxdt_pos;

//for neg1_alg8
u32 neg1_current_sum = 0;
u32 neg1_current_sum_offset = 0;
/******************************************************************************
*
* Trip neg1_algorithm
*
* Example implementation of a trip neg1_algorithm.
*
* @param	None
* @return None
* @note		None.
*
******************************************************************************/
void Alex_TripAlgorithm1negInit(void)
{
	neg1_trip = 0;
	manual1neg_trip_old = 0;

	neg1_alg1_threshold = 800; //dvdt
	neg1_alg2_threshold = 1.2;
	neg1_alg3_threshold = 800;
	neg1_alg4_threshold = 0.8;
	neg1_alg5_threshold = 1.3;
	neg1_alg6_threshold = 30000;
	neg1_alg7_threshold = 30000;
	neg1_alg8_threshold = 1638;
	neg1_alg9_threshold = 30000;
	neg1_alg10_threshold = 30000;

	ch9_beforefault = 0;

	neg1_fault_detected = 0;

	neg1_alg1_crit = 0;
	neg1_alg2_crit = 0;
	neg1_alg3_crit = 0;
	neg1_alg4_crit = 0;
	neg1_alg5_crit = 0;
	neg1_alg6_crit = 0;
	neg1_alg7_crit = 0;
	neg1_alg8_crit = 0;
	neg1_alg9_crit = 0;
	neg1_alg10_crit = 0;

	neg1_alg1_enable = 1;
	neg1_alg2_enable = 0;
	neg1_alg3_enable = 0;
	neg1_alg4_enable = 0;
	neg1_alg5_enable = 0;
	neg1_alg6_enable = 0;
	neg1_alg7_enable = 0;
	neg1_alg8_enable = 0;
	neg1_alg9_enable = 0;
	neg1_alg10_enable= 0;

	ch8_dxdt = 0;
	ch9_dxdt = 0;
	ch9_dxdt_pos = 0;

	ch8_dvdt_alarm = 0;
	ch8_alarm_counter = 0;
}

/******************************************************************************
*
* Trip neg1_algorithm
*
* Example implementation of a trip neg1_algoritm.
*
* @param	None
* @return None
* @note		None.
*
******************************************************************************/
void Alex_TripAlgorithm1neg(void)
{
	int i = 0;
	u32 *manual1neg_trip = 0x1FF00000;
	u32 *ch8         = 0x1FF00000+36;   //voltage link1neg
	u32 *ch9         = 0x1FF00000+40;   //current link1neg
	u32 *enable_trip = 0x1FF00044;


	//data comes as unsigned integer range 0...32764...65528
	//                      representing -10.......0.....+10 V
	u32 zero_offset = 32764;
	//----------------------------------------------------------------------------------//
	//LOW PASS FILTER
	//----------------------------------------------------------------------------------//
	// Low pass filter ch8 and ch9
	// measurement currently at 50 kHz:
	//ch8 voltage
	ch8_lpf_in = (u32)*ch8;
	ch8_lpf_out = lpf_b0*ch8_lpf_in + ch8_lpf_x1;
	ch8_lpf_x1 = ch8_lpf_x1 + ch8_lpf_x2 + lpf_b1*ch8_lpf_in - lpf_a1*ch8_lpf_out;
	ch8_lpf_x2 = ch8_lpf_x2 + lpf_b2*ch8_lpf_in - lpf_a2*ch8_lpf_out;
	//ch9 current
	ch9_lpf_in = (u32)*ch9;
	ch9_lpf_out = lpf_b0*ch9_lpf_in + ch9_lpf_x1;
	ch9_lpf_x1 = ch9_lpf_x1 + ch9_lpf_x2 + lpf_b1*ch9_lpf_in - lpf_a1*ch9_lpf_out;
	ch9_lpf_x2 = ch9_lpf_x2 + lpf_b2*ch9_lpf_in - lpf_a2*ch9_lpf_out;

	//positive dvdt ch8 - because this is negative pole collapsing dv/dt means it rises
	if (ch8_lpf_out<ch8_lpf_out_old){ch8_dxdt = 0;}
	else{ch8_dxdt = (ch8_lpf_out-ch8_lpf_out_old);};
	ch8_lpf_out_old = ch8_lpf_out;

	//negative didt ch9 (ch9_dxdt) is default - because this is the negative pole and we look at TW
	//positive didt ch9 (ch9_dxdt_pos)
	if(ch9_lpf_out>ch9_lpf_out_old){
		ch9_dxdt = 0;
		ch9_dxdt_pos = ch9_lpf_out-ch9_lpf_out_old;
	}else{
		ch9_dxdt = ch9_lpf_out_old-ch9_lpf_out;
		ch9_dxdt_pos = 0;
	};
	ch9_lpf_out_old = ch9_lpf_out;

	//----------------------------------------------------------------------------------//
	//ALGORITHMS
	//----------------------------------------------------------------------------------//
	//neg1_alg1: dv/dt
	//neg1_alg2: dv/dt + current direction
	//neg1_alg3: TW extraction (local)
	//neg1_alg4: undervoltage
	//neg1_alg5: overcurrent
	//neg1_alg6: TW + current directional (double-ended)
	//neg1_alg7: TW + current differential (double-ended)
	//neg1_alg8: Busbar
	//neg1_alg9: backup: breaker failure
	//neg1_alg10:backup (local)

	//dv/dt - negative dxdt ch8
	if(neg1_alg1_enable>0 && (u32)*enable_trip>0){
		if (ch8_dxdt>neg1_alg1_threshold){
			neg1_fault_detected = neg1_fault_detected | 0x0001;
		}else{
			neg1_fault_detected = neg1_fault_detected & 0xFFFE;
		}
		//log data
		neg1_alg1_crit = ch8_dxdt;
	}

	//dv/dt + current direction
	if(neg1_alg2_enable && (u32)*enable_trip>0){
		if (ch8_dxdt>neg1_alg1_threshold){
			ch8_dvdt_alarm = 1;
		}
		//current direction change current ch9
		if(!ch8_dvdt_alarm){
			ch9_beforefault = ch9_lpf_out;
		}
		if(ch8_dvdt_alarm){
			//positive current gets more positive:
			if     ((ch9_lpf_out > (neg1_alg2_threshold*(ch9_beforefault-zero_offset)+zero_offset) ) && (ch9_beforefault >zero_offset)){
				neg1_fault_detected = neg1_fault_detected | 0x0002;
			}
			//negative current gets more positive:
			else if((ch9_lpf_out > (ch9_beforefault+((neg1_alg2_threshold-1)*(zero_offset-ch9_beforefault))))&& (ch9_beforefault < zero_offset)){
				neg1_fault_detected = neg1_fault_detected | 0x0002;
			}
			else{
				neg1_fault_detected = neg1_fault_detected & 0xFFFD;
			}
			if (ch9_lpf_out>ch9_beforefault){
				neg1_alg2_crit = ch9_lpf_out-ch9_beforefault;
			}else{
				neg1_alg2_crit = ch9_beforefault-ch9_lpf_out;
			}
			ch8_alarm_counter = ch8_alarm_counter + 1;
			if(ch8_alarm_counter >250){ //5ms
				ch8_dvdt_alarm = 0;
				ch8_alarm_counter = 0;
				neg1_fault_detected = neg1_fault_detected & 0xFFFD;
				neg1_alg2_crit = 0;
			}
		}
	}

	//TW extraction S1 = u'-R*i'
	if(neg1_alg3_enable && (u32)*enable_trip>0){
		//int R = 25;	//real part of characteristic impedance at 1MHz
		//divide current by 80 for current scaling
		//R = 25 for cable --> 25/80 = 0.3125
		//R = 270 for OHL  --> 270/80 = 3.3750
		if (ch9_dxdt_pos>0){
			if(ch9_dxdt_pos*3.3750 < ch8_dxdt){
				neg1_s1 = ch8_dxdt-ch9_dxdt_pos*3.3750;
			}else{
				neg1_s1 = 0;
			}
		}else{
			neg1_s1 = ch8_dxdt+(ch9_dxdt)*3.3750;
		}
		if (neg1_s1>neg1_alg3_threshold){
			neg1_fault_detected = neg1_fault_detected | 0x0004;
		}
		else{
			neg1_fault_detected = neg1_fault_detected & 0xFFFB;
		}
		//log data
		neg1_alg3_crit = neg1_s1;
	}

	//undervoltage
	//steady state = 4V from RTDS (32764+3274*0.5*4 = 39317)
	//               0V from RTDS (32764)
	//undervoltage 0.8 p.u. = 3.2V (<38006) if positive voltage
	//undervoltage                 (>27522) if negative voltage
	if(neg1_alg4_enable && (u32)*enable_trip>0){
			if(ch8_lpf_out >27522){
				ch8_alarm_counter = ch8_alarm_counter+256; //count 1 up
			}
			else{
				ch8_alarm_counter = ch8_alarm_counter & 0x00FF; //set first 16 bits to zero
				neg1_fault_detected = neg1_fault_detected & 0xFFF7;
			}
		neg1_alg4_crit = ch8_lpf_out;
		if((ch8_alarm_counter & 0xFF00) >(250*256)+255){  //check first 16 bits, so add 255 to everything //5ms = 250 samples
			ch8_alarm_counter = ch8_alarm_counter & 0x00FF; //set first 16 bits to zero
			neg1_fault_detected = neg1_fault_detected | 0x0008;
		}
	}

	//overcurrent
	//steady state = 1V from RTDS (32764+3274*0.5*1 = 34401)
	//               0V from RTDS (32764)
	//steady state = -1Vfrom RTDS (32764-3274*0.5*1 = 31127)
	//overcurrent 1.3 p.u. = 1.3V (>34892) in positive direction
	//overcurrent                 (<30656) in negative direction
	if(neg1_alg5_enable && (u32)*enable_trip>0){
		if(ch9_lpf_out > zero_offset){
			if(ch9_lpf_out > (zero_offset + (1637*neg1_alg5_threshold))){
				neg1_fault_detected = neg1_fault_detected | 0x0010;
			}else{
				neg1_fault_detected = neg1_fault_detected & 0xFFEF;
			}
		}else{
			if(ch9_lpf_out < (zero_offset - (1637*neg1_alg5_threshold))){
				neg1_fault_detected = neg1_fault_detected | 0x0010;
			}else{
				neg1_fault_detected = neg1_fault_detected & 0xFFEF;
			}
		}
		neg1_alg5_crit = ch9_lpf_out;
	}

	//if(neg1_alg6_enable && (u32)*enable_trip>0){
	//	neg1_fault_detected = neg1_fault_detected | 0x0020;
	//}else{neg1_fault_detected = neg1_fault_detected & 0xFFDF;}

	//if(neg1_alg7_enable && (u32)*enable_trip>0){
	//	//neg1_fault_detected = neg1_fault_detected | 0x0040;
	//}else{neg1_fault_detected = neg1_fault_detected & 0xFFBF;}

	//Busbar
	if(neg1_alg8_enable && (u32)*enable_trip>0){
		u32 *busbar_current = 0x1FF00000+32;
		u32 del_i1 = 0;
		u32 del_i2 = 0;
		u32 del_i3 = 0;
		u32 del_ibus = 0;

		neg1_current_sum = zero_offset;

		if (Alex_TripAlgorithm2neg_getCurrent()>zero_offset){
			del_i1 = Alex_TripAlgorithm2neg_getCurrent()-zero_offset;
			neg1_current_sum = neg1_current_sum + del_i1;
		}else{
			del_i1 = zero_offset-Alex_TripAlgorithm2neg_getCurrent();
			neg1_current_sum = neg1_current_sum - del_i1;
		}
		if (Alex_TripAlgorithm3neg_getCurrent()>zero_offset){
			del_i2 = Alex_TripAlgorithm3neg_getCurrent()-zero_offset;
			neg1_current_sum = neg1_current_sum + del_i2;
		}else{
			del_i2 = zero_offset-Alex_TripAlgorithm3neg_getCurrent();
			neg1_current_sum = neg1_current_sum - del_i2;
		}
		if (ch9_lpf_out>zero_offset){
			del_i3 = ch9_lpf_out-zero_offset;
			neg1_current_sum = neg1_current_sum + del_i3;
		}else{
			del_i3 = zero_offset-ch9_lpf_out;
			neg1_current_sum = neg1_current_sum - del_i3;
		}
		if ((u32)*busbar_current>zero_offset){
			del_ibus = (u32)*busbar_current-zero_offset;
			neg1_current_sum = neg1_current_sum + del_ibus;
		}else{
			del_ibus = zero_offset - (u32)*busbar_current;
			neg1_current_sum = neg1_current_sum - del_ibus;
		}
		if (neg1_current_sum>zero_offset){
			neg1_current_sum = neg1_current_sum-zero_offset;
		}else{
			neg1_current_sum = zero_offset - neg1_current_sum;
		}
		if (neg1_current_sum>(neg1_alg8_threshold)){
			neg1_fault_detected = neg1_fault_detected | 0x0080;
			TRIP_ALGORITHM_AXI_mWriteReg(XPAR_TRIP_ALGORITHM_AXI_0_S00_AXI_BASEADDR,
													TRIP_ALGORITHM_AXI_S00_AXI_SLV_REG1_OFFSET, 0x0F);
		}else{
			neg1_fault_detected = neg1_fault_detected & 0xFF7F;
		}
		neg1_alg8_crit = neg1_current_sum;
	}

	//breaker failure with external breaker failure signal
	//if(neg1_alg9_enable && (u32)*enable_trip>0){
	//	u32 *breaker_failure = 0x1FF00048;
	//	if(breaker_failure>0){
	//		neg1_fault_detected = neg1_fault_detected | 0x0100;
	//	}
	//}else{neg1_fault_detected = neg1_fault_detected & 0xFEFF;}

	//if(neg1_alg10_enable&& (u32)*enable_trip>0){
	//	neg1_fault_detected = neg1_fault_detected | 0x0200;
	//}else{neg1_fault_detected = neg1_fault_detected & 0xFDFF;}


	manual_fault_detected = 0;
	if ((u32)*enable_trip>0) {
		if(neg1_fault_detected>0){
			neg1_trip = 1;
			Alex_DatalogStop();
			u32 current_breaker_status = TRIP_ALGORITHM_AXI_mReadReg(XPAR_TRIP_ALGORITHM_AXI_0_S00_AXI_BASEADDR,TRIP_ALGORITHM_AXI_S00_AXI_SLV_REG1_OFFSET);
			u32 new_breaker_status = current_breaker_status | 0x03; //DC breaker 1+2
			TRIP_ALGORITHM_AXI_mWriteReg(XPAR_TRIP_ALGORITHM_AXI_0_S00_AXI_BASEADDR,TRIP_ALGORITHM_AXI_S00_AXI_SLV_REG1_OFFSET, new_breaker_status);
		}
	}

	//trip from FPGA (manual trip)
	if (((u32)*manual1neg_trip > 0)&&((u32)*enable_trip == 0)){
		Alex_DatalogStop();
		manual_fault_detected = 1;
	}
	else if (((u32)*manual1neg_trip == 0)&&((u32)*enable_trip == 0)){
		neg1_fault_detected = 0;
		manual_fault_detected = 0;
		Alex_DatalogRun();
		neg1_trip = 0;
		TRIP_ALGORITHM_AXI_mWriteReg(XPAR_TRIP_ALGORITHM_AXI_0_S00_AXI_BASEADDR,
									TRIP_ALGORITHM_AXI_S00_AXI_SLV_REG1_OFFSET, 0x00);
	}
}

/******************************************************************************
*
* Update thresholds
*
* @param	u32 threshold, int neg1_algorithm
* @return None
* @note		None.
*
******************************************************************************/

void Alex_TripAlgorithm1neg_updateThresholds(u32 threshold, int algorithm){
	//neg1_alg1: dv/dt                                    	-range 0...320 kV
	//neg1_alg2: dv/dt + current direction					-range 1...5 p.u.
	//neg1_alg3: TW extraction (local)					    -range 0...320 kV
	//neg1_alg4: undervoltage					          	-range 0...1 p.u.
	//neg1_alg5: overcurrent								-range 1...5 p.u.
	//neg1_alg6: TW + current directional (double-ended)
	//neg1_alg7: TW + current differential (double-ended)
	//neg1_alg8: Busbar	                                    -range 0...1 kA
	//neg1_alg9: backup: breaker failure
	//neg1_alg10:backup (local)
	u32 range_1 = 6552; //320 kV = 4V = 6552 bits
	u32 range_2 = 4;
	u32 range_3 = 6552; //320 kV = 4V = 6552 bits
	u32 range_4 = 1;
	u32 range_5 = 4;
	//u32 range_6 = 1;
	//u32 range_7 = 1638;  //1 kA = 1V = 1638 bits
	u32 range_8 = 1638;  //1 kA = 1V = 1638 bits
	//u32 range_9 = 1;
	//u32 range_10= 1;

	switch(algorithm) {
	   case 1 :
	      neg1_alg1_threshold = threshold*0.02*range_1;	      break;
	   case 2 :
	   	  neg1_alg2_threshold = threshold*0.02*range_2+1;	 	  break;
	   case 3 :
	   	  neg1_alg3_threshold = threshold*0.02*range_3;	   	  break;
	   case 4 :
	   	  neg1_alg4_threshold = threshold*0.02*range_4;	   	  break;
	   case 5 :
	   	  neg1_alg5_threshold = threshold*0.02*range_5+1;	 	  break;
	   //case 6 :
	      //neg1_alg6_threshold = threshold*0.02*range_6;	      break;
	   //case 7 :
	      //neg1_alg7_threshold = threshold*0.02*range_7;	      break;
	   case 8 :
	      neg1_alg8_threshold = threshold*0.02*range_8;	      break;
	   //case 9 :
	   	  //neg1_alg9_threshold = threshold*0.02*range_9;	      break;
	   //case 10 :
	   	  //neg1_alg10_threshold= threshold*0.02*range_10;	    break;
	}
}
/******************************************************************************
*
* Enable neg1_algorithm
*
* @param	int neg1_algorithm, int enable
* @return None
* @note		None.
*
******************************************************************************/

void Alex_TripAlgorithm1neg_enableAlgorithm(int algorithm, int enable){
	switch(algorithm) {
	   case 1 :
	     neg1_alg1_enable = enable;	      break;
	   case 2 :
	  	 neg1_alg2_enable = enable;	      break;
	   case 3 :
	  	 neg1_alg3_enable = enable;	      break;
	   case 4 :
	  	 neg1_alg4_enable = enable;	      break;
	   case 5 :
	  	 neg1_alg5_enable = enable;	      break;
	   //case 6 :
	  	 //neg1_alg6_enable = enable;	      break;
	   //case 7 :
	  	 //neg1_alg7_enable = enable;	      break;
	   case 8 :
	  	 neg1_alg8_enable = enable;	      break;
	   //case 9 :
	   	 //neg1_alg9_enable = enable;	      break;
	   //case 10:
	   	 //neg1_alg10_enable = enable;	      break;
	}
}

/******************************************************************************
*
* Get Current for this link
*
* @param	None.
* @return u32 ch9_lpf_out
* @note		None.
*
******************************************************************************/

u32 Alex_TripAlgorithm1neg_getCurrent(){
	return ch9_lpf_out;
}
