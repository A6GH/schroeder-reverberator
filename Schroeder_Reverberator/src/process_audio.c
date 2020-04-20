/*********************************************************************************

 Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

 This software is proprietary and confidential.  By using this software you agree
 to the terms of the associated Analog Devices License Agreement.

 *********************************************************************************/

/*
 * NAME:     process_audio.c (sample-based Talkthrough)
 * PURPOSE:  Function process_audio() for AD1939/ADSP-21364 Talkthrough framework.
 * USAGE:    This file contains the function to process AD1939 samples.
 */

/*
 *--------------------------------------------------------------------------------------
 *
 * DMA Buffer relationship to C Audio Variables
 *
 * Timeslot #  SPORT DMA Buffer Timeslot Data   <->   AD1939 ADC/DAC C Variables (floats)
 * ======================================================================================
 *
 * 0           DM(_rx1a_bufa + Internal_ADC_L1) -->  Left_Channel_In1
 * 1           DM(_rx1a_bufa + Internal_ADC_R1) -->  Right_Channel_In1
 * 2           DM(_rx1a_bufa + Internal_ADC_L2) -->  Left_Channel_In2
 * 3           DM(_rx1a_bufa + Internal_ADC_R2) -->  Right_Channel_In2
 * 4           DM(_rx1a_bufa + AUX_ADC_L1)      -->  X (not used in EZ-KIT Lite)
 * 5           DM(_rx1a_bufa + AUX_ADC_R1)      -->  X (not used in EZ-KIT Lite)
 * 6           DM(_rx1a_bufa + AUX_ADC_L2)      -->  X (not used in EZ-KIT Lite)
 * 7           DM(_rx1a_bufa + AUX_ADC_R2)      -->  X (not used in EZ-KIT Lite)
 *
 * 0           DM(_tx0a_buf + Internal_DAC_L1)  <--  Left_Channel_Out1
 * 1           DM(_tx0a_buf + Internal_DAC_R1)  <--  Right_Channel_Out1
 * 2           DM(_tx0a_buf + Internal_DAC_L2)  <--  Left_Channel_Out2
 * 3           DM(_tx0a_buf + Internal_DAC_R2)  <--  Right_Channel_Out2
 * 4           DM(_tx0a_buf + Internal_DAC_L3)  <--  Left_Channel_Out3
 * 5           DM(_tx0a_buf + Internal_DAC_R3)  <--  Right_Channel_Out3
 * 6           DM(_tx0a_buf + Internal_DAC_L4)  <--  Left_Channel_Out4
 * 7           DM(_tx0a_buf + Internal_DAC_R4)  <--  Right_Channel_Out4
 */

#include "ADDS_21479_EzKit.h"
#include "cycle_count.h"

//#define USE_SIMPLE_TALKTHROUGH 	// Ukoliko želimo ugasiti reverberaciju
//#define MEASURE_CYCLES 			// Za mjerenje performansi algoritma

#define USE_FBCF_COMBINED_ARRAYS	// Algoritam je brži sa kombinovanim nizovima
#define USE_APF_COMBINED_ARRAYS


#ifndef USE_SIMPLE_TALKTHROUGH

#ifdef USE_FBCF_COMBINED_ARRAYS
float dm FBCF_Delay_L[FBCFDELAYSIZE1+FBCFDELAYSIZE2+FBCFDELAYSIZE3+FBCFDELAYSIZE4] = {0};
float dm FBCF_Delay_R[FBCFDELAYSIZE1+FBCFDELAYSIZE2+FBCFDELAYSIZE3+FBCFDELAYSIZE4] = {0};

#else
float dm FBCF_Delay_L1[FBCFDELAYSIZE1] = {0};
float dm FBCF_Delay_L2[FBCFDELAYSIZE2] = {0};
float dm FBCF_Delay_L3[FBCFDELAYSIZE3] = {0};
float dm FBCF_Delay_L4[FBCFDELAYSIZE4] = {0};
float dm FBCF_Delay_R1[FBCFDELAYSIZE1] = {0};
float dm FBCF_Delay_R2[FBCFDELAYSIZE2] = {0};
float dm FBCF_Delay_R3[FBCFDELAYSIZE3] = {0};
float dm FBCF_Delay_R4[FBCFDELAYSIZE4] = {0};
#endif /*USE_FBCF_COMBINED_ARRAYS*/

#ifdef USE_APF_COMBINED_ARRAYS
float dm APF_Delay_L[APFDELAYSIZE1+APFDELAYSIZE2+APFDELAYSIZE1+APFDELAYSIZE2] = {0};
float dm APF_Delay_R[APFDELAYSIZE1+APFDELAYSIZE2+APFDELAYSIZE1+APFDELAYSIZE2] = {0};

#else
float dm APF_Input_Delay_L1[APFDELAYSIZE1] = {0};
float dm APF_Input_Delay_L2[APFDELAYSIZE2] = {0};
float dm APF_Output_Delay_L1[APFDELAYSIZE1] = {0};
float dm APF_Output_Delay_L2[APFDELAYSIZE2] = {0};
float dm APF_Input_Delay_R1[APFDELAYSIZE1] = {0};
float dm APF_Input_Delay_R2[APFDELAYSIZE2] = {0};
float dm APF_Output_Delay_R1[APFDELAYSIZE1] = {0};
float dm APF_Output_Delay_R2[APFDELAYSIZE2] = {0};
#endif /*USE_APF_COMBINED_ARRAYS*/

int pm FBCF_Index1  = 0;
int pm FBCF_Index2  = 0;
int pm FBCF_Index3  = 0;
int pm FBCF_Index4  = 0;
int pm APF_Input_Index1   = 0;
int pm APF_Output_Index1   = 0;
int pm APF_Input_Index2   = 0;
int pm APF_Output_Index2   = 0;

#endif /*USE_SIMPLE_TALKTHROUGH*/

#ifdef MEASURE_CYCLES
cycle_t start_count;
cycle_t final_count;
#endif /*MEASURE_CYCLES*/

void  process_AD1939_samples(  uint32_t iid, void *handlerarg)
{
	/* Perform AD1939 Audio Processing Here Using 32-bit Floating-point Math
	 *  (Note: The ADC/DAC variables below are used to "double-buffer" the audio
	 *  samples with temporary variables, allowing active DMA completion of
	 *  rx1a_buf[] and tx0a_buf[])
	 */

	Receive_ADC_Samples();

#ifndef USE_SIMPLE_TALKTHROUGH

#ifdef MEASURE_CYCLES
	START_CYCLE_COUNT(start_count);
#endif /*MEASURE_CYCLES*/

	static float temp;

	// FBCF_LEFT
#ifdef USE_FBCF_COMBINED_ARRAYS
	FBCF_Delay_L[FBCF_Index1] = Left_Channel_In1 + g1 * FBCF_Delay_L[FBCF_Index1];
	FBCF_Delay_L[FBCF_Index2+FBCB_SP1] = Left_Channel_In1 + g1 * FBCF_Delay_L[FBCF_Index2+FBCB_SP1];
	FBCF_Delay_L[FBCF_Index3+FBCB_SP2] = Left_Channel_In1 + g1 * FBCF_Delay_L[FBCF_Index3+FBCB_SP2];
	FBCF_Delay_L[FBCF_Index4+FBCB_SP3] = Left_Channel_In1 + g1 * FBCF_Delay_L[FBCF_Index4+FBCB_SP3];

	Left_Channel_Out1 = FBCF_Delay_L[FBCF_Index1] +
						FBCF_Delay_L[FBCF_Index2+FBCB_SP1] +
						FBCF_Delay_L[FBCF_Index3+FBCB_SP2] +
						FBCF_Delay_L[FBCF_Index4+FBCB_SP3];

#else
	FBCF_Delay_L1[FBCF_Index1] = Left_Channel_In1 + g1 * FBCF_Delay_L1[FBCF_Index1];
	FBCF_Delay_L2[FBCF_Index2] = Left_Channel_In1 + g2 * FBCF_Delay_L2[FBCF_Index2];
	FBCF_Delay_L3[FBCF_Index3] = Left_Channel_In1 + g3 * FBCF_Delay_L3[FBCF_Index3];
	FBCF_Delay_L4[FBCF_Index4] = Left_Channel_In1 + g4 * FBCF_Delay_L4[FBCF_Index4];

	Left_Channel_Out1 = FBCF_Delay_L1[FBCF_Index1] +
						FBCF_Delay_L2[FBCF_Index2] +
						FBCF_Delay_L3[FBCF_Index3] +
						FBCF_Delay_L4[FBCF_Index4];
#endif /*USE_FBCF_COMBINED_ARRAYS*/

	// FBCF_RIGHT
#ifdef USE_FBCF_COMBINED_ARRAYS
	FBCF_Delay_R[FBCF_Index1] = Right_Channel_In1 + g1 * FBCF_Delay_R[FBCF_Index1];
	FBCF_Delay_R[FBCF_Index2+FBCB_SP1] = Right_Channel_In1 + g1 * FBCF_Delay_R[FBCF_Index2+FBCB_SP1];
	FBCF_Delay_R[FBCF_Index3+FBCB_SP2] = Right_Channel_In1 + g1 * FBCF_Delay_R[FBCF_Index3+FBCB_SP2];
	FBCF_Delay_R[FBCF_Index4+FBCB_SP3] = Right_Channel_In1 + g1 * FBCF_Delay_R[FBCF_Index4+FBCB_SP3];

	Right_Channel_Out1 = FBCF_Delay_R[FBCF_Index1] +
						 FBCF_Delay_R[FBCF_Index2+FBCB_SP1] +
						 FBCF_Delay_R[FBCF_Index3+FBCB_SP2] +
						 FBCF_Delay_R[FBCF_Index4+FBCB_SP3];

#else
	FBCF_Delay_R1[FBCF_Index1] = Right_Channel_In1 + g1 * FBCF_Delay_R1[FBCF_Index1];
	FBCF_Delay_R2[FBCF_Index2] = Right_Channel_In1 + g2 * FBCF_Delay_R2[FBCF_Index2];
	FBCF_Delay_R3[FBCF_Index3] = Right_Channel_In1 + g3 * FBCF_Delay_R3[FBCF_Index3];
	FBCF_Delay_R4[FBCF_Index4] = Right_Channel_In1 + g4 * FBCF_Delay_R4[FBCF_Index4];

	Right_Channel_Out1 = FBCF_Delay_R1[FBCF_Index1++] +
						 FBCF_Delay_R2[FBCF_Index2++] +
						 FBCF_Delay_R3[FBCF_Index3++] +
						 FBCF_Delay_R4[FBCF_Index4++];
#endif /*USE_FBCF_COMBINED_ARRAYS*/

	if (++FBCF_Index1 == FBCFDELAYSIZE1) FBCF_Index1 = 0;
	if (++FBCF_Index2 == FBCFDELAYSIZE2) FBCF_Index2 = 0;
	if (++FBCF_Index3 == FBCFDELAYSIZE3) FBCF_Index3 = 0;
	if (++FBCF_Index4 == FBCFDELAYSIZE4) FBCF_Index4 = 0;

	// APF1_LEFT
	temp = Left_Channel_Out1;

#ifdef USE_APF_COMBINED_ARRAYS
	Left_Channel_Out1 = -g5*Left_Channel_Out1 + APF_Delay_L[APF_Input_Index1];
	APF_Delay_L[APF_Input_Index1] = temp;
	Left_Channel_Out1 += g5*APF_Delay_L[APF_Output_Index1+APF_SP2];
	APF_Delay_L[APF_Output_Index1+APF_SP2] = Left_Channel_Out1;

#else
	Left_Channel_Out1 = -g5*Left_Channel_Out1 + APF_Input_Delay_L1[APF_Input_Index1];
	APF_Input_Delay_L1[APF_Input_Index1] = temp;
	Left_Channel_Out1 += g5*APF_Output_Delay_L1[APF_Output_Index1];
	APF_Output_Delay_L1[APF_Output_Index1] = Left_Channel_Out1;
#endif /*USE_APF_COMBINED_ARRAYS*/

	// APF2_LEFT
	temp = Left_Channel_Out1;

#ifdef USE_APF_COMBINED_ARRAYS
	Left_Channel_Out1 = -g6*Left_Channel_Out1 + APF_Delay_L[APF_Input_Index2+APF_SP1];
	APF_Delay_L[APF_Input_Index2+APF_SP1] = temp;
	Left_Channel_Out1 += g6*APF_Delay_L[APF_Input_Index2+APF_SP3];
	APF_Delay_L[APF_Input_Index2+APF_SP3] = Left_Channel_Out1;

#else
	Left_Channel_Out1 = -g6*Left_Channel_Out1 + APF_Input_Delay_L2[APF_Input_Index2];
	APF_Input_Delay_L2[APF_Input_Index2] = temp;
	Left_Channel_Out1 += g6*APF_Output_Delay_L2[APF_Output_Index2];
	APF_Output_Delay_L2[APF_Output_Index2] = Left_Channel_Out1;
#endif /*USE_APF_COMBINED_ARRAYS*/

	// APF1_RIGHT
	temp = Right_Channel_Out1;

#ifdef USE_APF_COMBINED_ARRAYS
	Right_Channel_Out1 = -g5*Right_Channel_Out1 + APF_Delay_R[APF_Input_Index1];
	APF_Delay_R[APF_Input_Index1] = temp;
	Right_Channel_Out1 += g5*APF_Delay_R[APF_Output_Index1+APF_SP2];
	APF_Delay_R[APF_Output_Index1+APF_SP2] = Right_Channel_Out1;

#else
	Right_Channel_Out1 = -g5*Right_Channel_Out1 + APF_Input_Delay_R1[APF_Input_Index1];
	APF_Input_Delay_R1[APF_Input_Index1++] = temp;
	Right_Channel_Out1 += g5*APF_Output_Delay_R1[APF_Output_Index1];
	APF_Output_Delay_R1[APF_Output_Index1++] = Right_Channel_Out1;
#endif /*USE_APF_COMBINED_ARRAYS*/

	// APF2_RIGHT
	temp = Right_Channel_Out1;

#ifdef USE_APF_COMBINED_ARRAYS
	Right_Channel_Out1 = -g6*Right_Channel_Out1 + APF_Delay_R[APF_Input_Index2+APF_SP1];
	APF_Delay_R[APF_Input_Index2+APF_SP1] = temp;
	Right_Channel_Out1 += g6*APF_Delay_R[APF_Output_Index2+APF_SP3];
	APF_Delay_R[APF_Output_Index2+APF_SP3] = Right_Channel_Out1;

#else
	Right_Channel_Out1 = -g6*Right_Channel_Out1 + APF_Input_Delay_R2[APF_Input_Index2];
	APF_Input_Delay_R2[APF_Input_Index2++] = temp;
	Right_Channel_Out1 += g6*APF_Output_Delay_R2[APF_Output_Index2];
	APF_Output_Delay_R2[APF_Output_Index2++] = Right_Channel_Out1;
#endif /*USE_APF_COMBINED_ARRAYS*/

	if (++APF_Input_Index1 == APFDELAYSIZE1) APF_Input_Index1 = 0;
	if (++APF_Output_Index1 == APFDELAYSIZE1) APF_Output_Index1 = 0;
	if (++APF_Input_Index2 == APFDELAYSIZE2) APF_Input_Index2 = 0;
	if (++APF_Output_Index2 == APFDELAYSIZE2) APF_Output_Index2 = 0;


	Left_Channel_Out1 = Left_Channel_In1 + g7*Left_Channel_Out1;
	Right_Channel_Out1 = Right_Channel_In1 + g7*Right_Channel_Out1;


#ifdef MEASURE_CYCLES
	STOP_CYCLE_COUNT(final_count, start_count);
	PRINT_CYCLES("Broj ciklusa: ", final_count);
#endif /*MEASURE_CYCLES*/

#else
	Left_Channel_Out1 = 3*Left_Channel_In1;
	Right_Channel_Out1 = 3*Right_Channel_In1;
#endif /*USE_SIMPLE_TALKTHROUGH*/

	Transmit_DAC_Samples();
}

