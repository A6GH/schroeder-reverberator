/*********************************************************************************

 Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

 This software is proprietary and confidential.  By using this software you agree
 to the terms of the associated Analog Devices License Agreement.

 *********************************************************************************/
/*This project is a talk-through example using the onboard AD1939 to acquire
 * and output an audio stream. The digital audio data is available for processing
 * in the file SPORT1_isr.c. The block size is 256 samples per audio channel.
 * The output on OUT2 is delayed and the user can expect to hear an echo effect.
 */

#include "ADDS_21479_EzKit.h"
#include "math.h"

float g1, g2, g3, g4; // Pojačanja češljastih filtara u povratnoj vezi

float g5 = 0.7, g6 = 0.7; // Pojačanja svepropusnih filtara
float g7 = 1; // Faktor reverberacije - (0 - isključena/dry, 1 - potpuno prisutna/wet)
float t60 = 4; // Trajanje reverberacije u sekundama
int fs = 48000; // Frekvencija odmjeravanja

void main()
{
	int count=0;

	/* Initialize managed drivers and/or services at the start of main(). */
	adi_initComponents();

	/* Initialize SHARC PLL*/
	initPLL();

	/* Initialize DDR2 SDRAM controller to access memory */
	initExternalMemory();

	/* Initialize DAI because the SPORT and SPI signals need to be routed */
    InitDAI();

    /* This function will configure the AD1939 codec on the 21469 EZ-KIT */
    init1939viaSPI();

    /* Turn on SPORT0 TX and SPORT1 RX for Multichannel Operation */
	enable_SPORT01_MCM_mode();

	/* Install and enable a handler for the SPORT1 Receiver interrupt.*/
	adi_int_InstallHandler(ADI_CID_P3I,process_AD1939_samples,0,true);

	/* Enable multichannel operation (SPORT mode and DMA in standby and ready) */
	*pSPMCTL0 |= MCEA;
	*pSPMCTL1 |= MCEA;

	SinTableInit();

	/* Set up Schroeder reverberation parameters */
	g1 = pow(0.001, FBCFDELAYSIZE1/(fs*t60));
	g2 = pow(0.001, FBCFDELAYSIZE2/(fs*t60));
	g3 = pow(0.001, FBCFDELAYSIZE3/(fs*t60));
	g4 = pow(0.001, FBCFDELAYSIZE4/(fs*t60));
//	printf("g1 = %f\ng2 = %f\ng3 = %f\ng4 = %f\n", g1, g2, g3, g4);

	for (;;)
	  {
		if(count==10)
			puts("Talkthrough is running successfully");
		count++;
    	asm("idle;");
      }
}

