/******************************************************************************

   Project:	      Alex

   Filename:      alex_tral1.c

   Description:    Trip pos1_algoritm

   © Copyright 2019 KTH. All rights reserved.

******************************************************************************/

/***************************** Include Files *********************************/
#include "alex_tral1pos.h"
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
int pos1_trip __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));

u32 ch0_lpf_out __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 ch1_lpf_out __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 chr_delayed __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 chr_lpf_out __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));

u32 pos1_alg1_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 pos1_alg2_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 pos1_alg3_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 pos1_alg4_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 pos1_alg5_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 pos1_alg6_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 pos1_alg7_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 pos1_alg8_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 pos1_alg9_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));
u32 pos1_alg10_crit __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));

u32 pos1_fault_detected __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));


//variables that do not really need to be logged, but have to be available next time the function is called.
//If a variable is needed later, it will otherwise risk overflow.
//ch0_alarm_counter is used as counter for alg2 and alg4. alg2 uses the first 16 bits, alg4 uses the second 16 bits
u32 ch0_alarm_counter __attribute__((section(".datlgvars")))  __attribute__((aligned(4)));

u32 pos1_alg1_threshold;
double pos1_alg2_threshold;
u32 pos1_alg3_threshold;
double pos1_alg4_threshold;
double pos1_alg5_threshold;
u32 pos1_alg6_threshold;
u32 pos1_alg7_threshold;
u32 pos1_alg7_length;
u32 pos1_alg8_threshold;
u32 pos1_alg9_threshold;
u32 pos1_alg10_threshold;

int pos1_alg1_enable;
int pos1_alg2_enable;
int pos1_alg3_enable;
int pos1_alg4_enable;
int pos1_alg5_enable;
int pos1_alg6_enable;
int pos1_alg7_enable;
int pos1_alg8_enable;
int pos1_alg9_enable;
int pos1_alg10_enable;

u32 *manual1pos_trip_old = 0;

//for LPF
u32 ch0_lpf_out_old = 0;
u32 ch1_lpf_out_old = 0;
u32 chr_lpf_out_old = 0;

u32 ch0_lpf_in = 0;
u32 ch0_lpf_out = 0;
u32 ch0_lpf_x1 = 0;
u32 ch0_lpf_x2 = 0;

u32 ch1_lpf_in = 0;
u32 ch1_lpf_out = 0;
u32 ch1_lpf_x1 = 0;
u32 ch1_lpf_x2 = 0;

u32 chr_lpf_in = 0;
u32 chr_lpf_out = 0;
u32 chr_lpf_x1 = 0;
u32 chr_lpf_x2 = 0;

u32 chr_delayed = 0;

u32 ch0_dxdt;
int ch0_dvdt_alarm;
u32 ch1_beforefault;

//assume 200km/ms speed in optic fibre, so need delay buffer for max 200km i.e. 1ms i.e. 50 samples at 50kHz
//u32 chr_buffer[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//u32 chr_buffer_old[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//for pos1_alg3;
u32 pos1_s1 = 0;
u32 ch1_dxdt;
u32 ch1_dxdt_neg;

//for pos1_alg7
u32 pos1_current_diff = 0;

//for pos1_alg8
u64 pos1_current_sum = 0;
u64 pos1_current_sum_offset = 0;


/******************************************************************************
*
* Trip pos1_algorithm
*
* Example implementation of a trip pos1_algorithm.
*
* @param	None
* @return None
* @note		None.
*
******************************************************************************/
void Alex_TripAlgorithm1posInit(void)
{
	pos1_trip = 0;
	manual1pos_trip_old = 0;

	pos1_alg1_threshold = 800; //dvdt
	pos1_alg2_threshold = 1.2;
	pos1_alg3_threshold = 800;
	pos1_alg4_threshold = 0.8;
	pos1_alg5_threshold = 1.3;
	pos1_alg6_threshold = 30000;
	pos1_alg7_threshold = 1638;
	pos1_alg7_length = 25;
	pos1_alg8_threshold = 1638;
	pos1_alg9_threshold = 30000;
	pos1_alg10_threshold = 30000;

	ch1_beforefault = 0;

	pos1_fault_detected = 0;

	pos1_alg1_crit = 0;
	pos1_alg2_crit = 0;
	pos1_alg3_crit = 0;
	pos1_alg4_crit = 0;
	pos1_alg5_crit = 0;
	pos1_alg6_crit = 0;
	pos1_alg7_crit = 0;
	pos1_alg8_crit = 0;
	pos1_alg9_crit = 0;
	pos1_alg10_crit = 0;

	pos1_alg1_enable = 1;
	pos1_alg2_enable = 0;
	pos1_alg3_enable = 0;
	pos1_alg4_enable = 0;
	pos1_alg5_enable = 0;
	pos1_alg6_enable = 0;
	pos1_alg7_enable = 0;
	pos1_alg8_enable = 0;
	pos1_alg9_enable = 0;
	pos1_alg10_enable= 0;

	ch0_dxdt = 0;
	ch1_dxdt = 0;
	ch1_dxdt_neg = 0;

	ch0_dvdt_alarm = 0;
	ch0_alarm_counter = 0;
}

/******************************************************************************
*
* Trip pos1_algorithm
*
* Example implementation of a trip pos1_algoritm.
*
* @param	None
* @return None
* @note		None.
*
******************************************************************************/
void Alex_TripAlgorithm1pos(void)
{
	int i = 0;

	u32 *manual1pos_trip = 0x1FF00000;
	u32 *ch0         = 0x1FF00000+4;   //voltage link1pos
	u32 *ch1         = 0x1FF00000+8;   //current link1pos
	u32 *enable_trip = 0x1FF00044;
	u32 *chr_inst    = 0x1FF00000+72;	//remote line end current

	//chr_delayed = chr_buffer[pos1_alg7_length];

	//data comes as unsigned integer range 0...32764...65528
	//                      representing -10.......0.....+10 V
	u32 zero_offset = 32764;
	//----------------------------------------------------------------------------------//
	//LOW PASS FILTER
	//----------------------------------------------------------------------------------//
	// Low pass filter ch0 and ch1
	// measurement currently at 50 kHz:
	//ch0 voltage
	ch0_lpf_in = (u32)*ch0;
	ch0_lpf_out = lpf_b0*ch0_lpf_in + ch0_lpf_x1;
	ch0_lpf_x1 = ch0_lpf_x1 + ch0_lpf_x2 + lpf_b1*ch0_lpf_in - lpf_a1*ch0_lpf_out;
	ch0_lpf_x2 = ch0_lpf_x2 + lpf_b2*ch0_lpf_in - lpf_a2*ch0_lpf_out;
	//ch1 current
	ch1_lpf_in = (u32)*ch1;
	ch1_lpf_out = lpf_b0*ch1_lpf_in + ch1_lpf_x1;
	ch1_lpf_x1 = ch1_lpf_x1 + ch1_lpf_x2 + lpf_b1*ch1_lpf_in - lpf_a1*ch1_lpf_out;
	ch1_lpf_x2 = ch1_lpf_x2 + lpf_b2*ch1_lpf_in - lpf_a2*ch1_lpf_out;
	//chr current
	//chr_lpf_in = chr_delayed;
	//chr_lpf_out = lpf_b0*chr_lpf_in + chr_lpf_x1;
	//chr_lpf_x1 = chr_lpf_x1 + chr_lpf_x2 + lpf_b1*chr_lpf_in - lpf_a1*chr_lpf_out;
	//chr_lpf_x2 = chr_lpf_x2 + lpf_b2*chr_lpf_in - lpf_a2*chr_lpf_out;

	//negative dvdt ch0
	if (ch0_lpf_out>ch0_lpf_out_old){ch0_dxdt = 0;}
	else{ch0_dxdt = (ch0_lpf_out_old-ch0_lpf_out);};
	ch0_lpf_out_old = ch0_lpf_out;

	//chr buffer (2ms)
	//chr_buffer[0] = (u32)*chr_inst;
	//for (i=1;i<51;i++){chr_buffer[i] = chr_buffer_old[i-1];}
	//for (i=0;i<51;i++){chr_buffer_old[i] = chr_buffer[i];}

	//positive didt ch1 (ch1_dxdt) and negative didt(ch1_dxdt_neg)
	if(ch1_lpf_out<ch1_lpf_out_old){
		ch1_dxdt = 0;
		ch1_dxdt_neg = (ch1_lpf_out_old-ch1_lpf_out);
	}else{
		ch1_dxdt = (ch1_lpf_out-ch1_lpf_out_old);
		ch1_dxdt_neg = 0;
	};
	ch1_lpf_out_old = ch1_lpf_out;

	//----------------------------------------------------------------------------------//
	//ALGORITHMS
	//----------------------------------------------------------------------------------//
	//pos1_alg1: dv/dt
	//pos1_alg2: dv/dt + current direction
	//pos1_alg3: TW extraction (local)
	//pos1_alg4: undervoltage
	//pos1_alg5: overcurrent
	//pos1_alg6: TW + current directional (double-ended)
	//pos1_alg7: TW + current differential (double-ended)
	//pos1_alg8: Busbar
	//pos1_alg9: backup: breaker failure
	//pos1_alg10:backup (local)

	//dv/dt - negative dxdt ch0
	if(pos1_alg1_enable>0 && (u32)*enable_trip>0){
		if (ch0_dxdt>pos1_alg1_threshold){
			pos1_fault_detected = pos1_fault_detected | 0x0001;
		}else{
			pos1_fault_detected = pos1_fault_detected & 0xFFFE;
		}
		//log data
		pos1_alg1_crit = ch0_dxdt;
	}

	//dv/dt + current direction
	if(pos1_alg2_enable && (u32)*enable_trip>0){
		int done = 0;
		if (ch0_dxdt>pos1_alg1_threshold){
			ch0_dvdt_alarm = 1;
		}
		//current direction change current ch1
		if(!ch0_dvdt_alarm){
			ch1_beforefault = ch1_lpf_out;
		}
		if(ch0_dvdt_alarm){
			//positive current gets more positive:
			if     ((ch1_lpf_out > (pos1_alg2_threshold*(ch1_beforefault-zero_offset)+zero_offset) ) && (ch1_beforefault >zero_offset)){
				pos1_fault_detected = pos1_fault_detected | 0x0002;
			}
			//negative current gets more positive:
			else if((ch1_lpf_out > (ch1_beforefault+((pos1_alg2_threshold-1)*(zero_offset-ch1_beforefault))))&& (ch1_beforefault < zero_offset)){
				pos1_fault_detected = pos1_fault_detected | 0x0002;
			}
			else{
				pos1_fault_detected = pos1_fault_detected & 0xFFFD;
			}
			if (ch1_lpf_out>ch1_beforefault){
				pos1_alg2_crit = ch1_lpf_out-ch1_beforefault;
			}else{
				pos1_alg2_crit = ch1_beforefault-ch1_lpf_out;
			}
			ch0_alarm_counter = ch0_alarm_counter + 1;
			if((ch0_alarm_counter & 0x00FF) >250){ //5ms, check last 16 bits
				ch0_dvdt_alarm = 0;
				ch0_alarm_counter = ch0_alarm_counter & 0xFF00; //set last 16 bits to zero
				pos1_fault_detected = pos1_fault_detected & 0xFFFD;
				pos1_alg2_crit = 0;
			}
		}
	}

	//TW extraction S1 = u'-R*i'
	if(pos1_alg3_enable && (u32)*enable_trip>0){
		//int R = 25;	//real part of characteristic impedance at 1MHz
		//divide current by 80 for current scaling
		//R = 25 for cable --> 25/80 = 0.3125
		//R = 270 for OHL  --> 270/80 = 3.3750
		if (ch1_dxdt_neg>0){
			if(ch1_dxdt_neg*3.3750 < ch0_dxdt){
				pos1_s1 = ch0_dxdt-ch1_dxdt_neg*3.3750;
			}else{
				pos1_s1 = 0;
			}
		}else{
			pos1_s1 = ch0_dxdt+(ch1_dxdt)*3.3750;
		}
		if (pos1_s1>pos1_alg3_threshold){
			pos1_fault_detected = pos1_fault_detected | 0x0004;
		}
		else{
			pos1_fault_detected = pos1_fault_detected & 0xFFFB;
		}
		//log data
		pos1_alg3_crit = pos1_s1;
	}

	//undervoltage
	//steady state = 4V from RTDS (32764+3274*0.5*4 = 39317)
	//               0V from RTDS (32764)
	//undervoltage 0.8 p.u. = 3.2V (<38006) if positive voltage
	//undervoltage                 (>27522) if negative voltage
	//trips only if undervoltage persists for 10 ms (500 samples at 50 kHz)
	if(pos1_alg4_enable && (u32)*enable_trip>0){
		if(ch0_lpf_out <38006){
			ch0_alarm_counter = ch0_alarm_counter+256; //count 1 up
		}
		else{
			ch0_alarm_counter = ch0_alarm_counter & 0x00FF; //set first 16 bits to zero
			pos1_fault_detected = pos1_fault_detected & 0xFFF7;
		}
		pos1_alg4_crit = ch0_lpf_out;
		if((ch0_alarm_counter & 0xFF00) >(250*256)+255){  //check first 16 bits, so add 255 to everything //5ms = 250 samples
			ch0_alarm_counter = ch0_alarm_counter & 0x00FF; //set first 16 bits to zero
			pos1_fault_detected = pos1_fault_detected | 0x0008;
		}
	}

	//overcurrent

	//steady state = 1V from RTDS (32764+3274*0.5*1 = 34401)
	//               0V from RTDS (32764)
	//steady state = -1Vfrom RTDS (32764-3274*0.5*1 = 31127)
	//overcurrent 1.3 p.u. = 1.3V (>34892) in positive direction
	//overcurrent                 (<30656) in negative direction
	if(pos1_alg5_enable && (u32)*enable_trip>0){
		if(ch1_lpf_out > zero_offset){
			if(ch1_lpf_out > (zero_offset + (1637*pos1_alg5_threshold))){
				pos1_fault_detected = pos1_fault_detected | 0x0010;
			}else{
				pos1_fault_detected = pos1_fault_detected & 0xFFEF;
			}
		}else{
			if(ch1_lpf_out < (zero_offset - (1637*pos1_alg5_threshold))){
				pos1_fault_detected = pos1_fault_detected | 0x0010;
			}else{
				pos1_fault_detected = pos1_fault_detected & 0xFFEF;
			}
		}
		pos1_alg5_crit = ch1_lpf_out;
	}

	//if(pos1_alg6_enable && (u32)*enable_trip>0){
	//	pos1_fault_detected = pos1_fault_detected | 0x0020;
	//}else{pos1_fault_detected = pos1_fault_detected & 0xFFDF;}

	//Current differential
	/*if(pos1_alg7_enable && (u32)*enable_trip>0){
		if(chr_lpf_out>ch1_lpf_out){
			pos1_current_diff = chr_lpf_out - ch1_lpf_out;
		}else{
			pos1_current_diff = ch1_lpf_out - chr_lpf_out;
		}

		if(pos1_current_diff > pos1_alg7_threshold){
			pos1_fault_detected = pos1_fault_detected | 0x0040;
		}	else{pos1_fault_detected = pos1_fault_detected & 0xFFBF;}
		pos1_alg7_crit = pos1_current_diff;
	}*/

	//Busbar
	if(pos1_alg8_enable && (u32)*enable_trip>0){
		u32 *busbar_current = 0x1FF00000+32;
		u32 del_i1 = 0;
		u32 del_i2 = 0;
		u32 del_i3 = 0;
		u32 del_ibus = 0;

		pos1_current_sum = zero_offset;

		if (Alex_TripAlgorithm2pos_getCurrent()>zero_offset){
			del_i1 = Alex_TripAlgorithm2pos_getCurrent()-zero_offset;
			pos1_current_sum = pos1_current_sum + del_i1;
		}else{
			del_i1 = zero_offset-Alex_TripAlgorithm2pos_getCurrent();
			pos1_current_sum = pos1_current_sum - del_i1;
		}
		if (Alex_TripAlgorithm3pos_getCurrent()>zero_offset){
			del_i2 = Alex_TripAlgorithm3pos_getCurrent()-zero_offset;
			pos1_current_sum = pos1_current_sum + del_i2;
		}else{
			del_i2 = zero_offset-Alex_TripAlgorithm3pos_getCurrent();
			pos1_current_sum = pos1_current_sum - del_i2;
		}
		if (ch1_lpf_out>zero_offset){
			del_i3 = ch1_lpf_out-zero_offset;
			pos1_current_sum = pos1_current_sum + del_i3;
		}else{
			del_i3 = zero_offset-ch1_lpf_out;
			pos1_current_sum = pos1_current_sum - del_i3;
		}
		if ((u32)*busbar_current>zero_offset){
			del_ibus = (u32)*busbar_current-zero_offset;
			pos1_current_sum = pos1_current_sum + del_ibus;
		}else{
			del_ibus = zero_offset - (u32)*busbar_current;
			pos1_current_sum = pos1_current_sum - del_ibus;
		}

		if (pos1_current_sum>zero_offset){
			pos1_current_sum = pos1_current_sum-zero_offset;
		}else{
			pos1_current_sum = zero_offset - pos1_current_sum;
		}

		if (pos1_current_sum>(pos1_alg8_threshold)){
			pos1_fault_detected = pos1_fault_detected | 0x0080;
			TRIP_ALGORITHM_AXI_mWriteReg(XPAR_TRIP_ALGORITHM_AXI_0_S00_AXI_BASEADDR,
										TRIP_ALGORITHM_AXI_S00_AXI_SLV_REG1_OFFSET, 0x0F);
		}else{
			pos1_fault_detected = pos1_fault_detected & 0xFF7F;
		}
		pos1_alg8_crit = pos1_current_sum;
	}

	//breaker failure with external breaker failure signal
	//if(pos1_alg9_enable && (u32)*enable_trip>0){
	//	u32 *breaker_failure = 0x1FF00048;
	//	if(breaker_failure>0){
	//		pos1_fault_detected = pos1_fault_detected | 0x0100;
	//	}
	//}else{pos1_fault_detected = pos1_fault_detected & 0xFEFF;}

	//if(pos1_alg10_enable&& (u32)*enable_trip>0){
	//	pos1_fault_detected = pos1_fault_detected | 0x0200;
	//}else{pos1_fault_detected = pos1_fault_detected & 0xFDFF;}

	if ((u32)*enable_trip>0) {
		if(pos1_fault_detected>0){
			pos1_trip = 1;
			Alex_DatalogStop();
			u32 current_breaker_status = TRIP_ALGORITHM_AXI_mReadReg(XPAR_TRIP_ALGORITHM_AXI_0_S00_AXI_BASEADDR,TRIP_ALGORITHM_AXI_S00_AXI_SLV_REG1_OFFSET);
			//u32 new_breaker_status;
			u32 new_breaker_status = current_breaker_status | 0x03; //DC breaker 1+2
			//if different breakers need to be tripped depending on the fault do like this example:
			//if ((pos1_fault_detected & 0x0800)){
			//	current_breaker_status = current_breaker_status | 0x0F; //DC breaker 1+2+3+4, busbar
			//}else{
			//	current_breaker_status = current_breaker_status | 0x03; //DC breaker 1+2
			//}
			//if ((pos1_fault_detected & 0x0100) >0){                 //breaker failure indication bit
			//  u32 new_breaker_status = 0xFF; 				                //all breakers
			//}
			TRIP_ALGORITHM_AXI_mWriteReg(XPAR_TRIP_ALGORITHM_AXI_0_S00_AXI_BASEADDR,TRIP_ALGORITHM_AXI_S00_AXI_SLV_REG1_OFFSET, new_breaker_status);
		}
	}

	//trip from FPGA (manual trip)
	if (((u32)*manual1pos_trip > 0)&&((u32)*enable_trip == 0)){
		Alex_DatalogStop();
	}
	else if (((u32)*manual1pos_trip == 0)&&((u32)*enable_trip == 0)){
		pos1_fault_detected = 0;
		Alex_DatalogRun();
		pos1_trip = 0;
		TRIP_ALGORITHM_AXI_mWriteReg(XPAR_TRIP_ALGORITHM_AXI_0_S00_AXI_BASEADDR,
									TRIP_ALGORITHM_AXI_S00_AXI_SLV_REG1_OFFSET, 0x00);
	}
}

/******************************************************************************
*
* Update thresholds
*
* @param	u32 threshold, int pos1_algorithm
* @return None
* @note		None.
*
******************************************************************************/

void Alex_TripAlgorithm1pos_updateThresholds(u32 threshold, int algorithm){
	//pos1_alg1: dv/dt                                    	-range 0...320 kV
	//pos1_alg2: dv/dt + current direction					-range 1...5 p.u.
	//pos1_alg3: TW extraction (local)						-range 0...320 kV
	//pos1_alg4: undervoltage								-range 0...1 p.u.
	//pos1_alg5: overcurrent								-range 1...5 p.u.
	//pos1_alg6: TW + current directional (double-ended)
	//pos1_alg7: TW + current differential (double-ended)
	//pos1_alg8: Busbar	                                    -range 0...1 kA
	//pos1_alg9: backup: breaker failure
	//pos1_alg10:backup (local)
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
	      pos1_alg1_threshold = threshold*0.02*range_1;	      break;
	   case 2 :
	   	  pos1_alg2_threshold = threshold*0.02*range_2+1;	 	  break;
	   case 3 :
	   	  pos1_alg3_threshold = threshold*0.02*range_3;	   	  break;
	   case 4 :
	   	  pos1_alg4_threshold = threshold*0.02*range_4;	   	  break;
	   case 5 :
	   	  pos1_alg5_threshold = threshold*0.02*range_5+1;	 	  break;
	   //case 6 :
	      //pos1_alg6_threshold = threshold*0.02*range_6;	      break;
	   //case 7 :
	      //pos1_alg7_threshold = threshold*0.02*range_7;	      break;
	   case 8 :
	      pos1_alg8_threshold = threshold*0.02*range_8;	      break;
	   //case 9 :
	   	  //pos1_alg9_threshold = threshold*0.02*range_9;	      break;
	   //case 10 :
	   	  //pos1_alg10_threshold= threshold*0.02*range_10;	    break;
	}
	Alex_TripAlgorithm1neg_updateThresholds(threshold, algorithm);
}


/******************************************************************************
*
* Set line length - corresponds to communication delay
*
* @param	u32 length
* @return None
* @note		None.
*
******************************************************************************/

/*void Alex_TripAlgorithm1pos_updateLength(u32 length){
		//the length value corresponds to a delay
		//length = [0 200] km in real world values
		//length = [1 49] here, so one step corresponds to 4 km
		pos1_alg7_length = length;
}*/
/******************************************************************************
*
* Enable pos1_algorithm
*
* @param	int pos1_algorithm, int enable
* @return None
* @note		None.
*
******************************************************************************/

void Alex_TripAlgorithm1pos_enableAlgorithm(int algorithm, int enable){
	switch(algorithm) {
	   case 1 :
	     pos1_alg1_enable = enable;	      break;
	   case 2 :
	  	 pos1_alg2_enable = enable;	      break;
	   case 3 :
	  	 pos1_alg3_enable = enable;	      break;
	   case 4 :
	  	 pos1_alg4_enable = enable;	      break;
	   case 5 :
	  	 pos1_alg5_enable = enable;	      break;
	   //case 6 :
	  	 //pos1_alg6_enable = enable;	      break;
	   //case 7 :
	  	 //pos1_alg7_enable = enable;	      break;
	   case 8 :
	  	 pos1_alg8_enable = enable;	      break;
	   //case 9 :
	   	 //pos1_alg9_enable = enable;	      break;
	   //case 10:
	   	 //pos1_alg10_enable = enable;	      break;
	}
}

/******************************************************************************
*
* Get Current for this link
*
* @param	None.
* @return u32 ch1_lpf_out
* @note		None.
*
******************************************************************************/

u32 Alex_TripAlgorithm1pos_getCurrent(){
	return ch1_lpf_out;
}
