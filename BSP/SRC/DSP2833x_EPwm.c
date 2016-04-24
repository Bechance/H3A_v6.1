// TI File $Revision: 1.9 $
// Checkin $Date: 2014/10/23 02:21:03 $
//###########################################################################
//
// FILE:   DSP2833x_EPwm.c
//
// TITLE:  DSP2833x ePWM Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File




void HRPWM5_Config(period)
{
// ePWM1 register configuration with HRPWM
// ePWM1A toggle low/high with MEP control on Rising edge
	EPwm5Regs.TBCTL.bit.FREE_SOFT=TB_FREEZE;
	EPwm5Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;	        // set Immediate load
	EPwm5Regs.TBPRD = period-1;		                    // PWM frequency = 1 / period
	EPwm5Regs.CMPA.half.CMPA = period / 2;	            // set duty 50% initially
    EPwm5Regs.CMPA.half.CMPAHR = 0;//disble the HRPWM  (1 << 8);              // initialize HRPWM extension
	EPwm5Regs.CMPB = period / 2;	                    // set duty 50% initially
	EPwm5Regs.TBPHS.all = 0;
	EPwm5Regs.TBCTR = 0;

	EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
	EPwm5Regs.TBCTL.bit.PHSEN = TB_DISABLE;		       // EPWM1 is the Master
	EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
	EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm5Regs.AQCTLA.bit.ZRO = AQ_CLEAR;               // PWM toggle low/high
	EPwm5Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm5Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm5Regs.AQCTLB.bit.CBU = AQ_SET;
  
   
	EALLOW;
	EPwm5Regs.HRCNFG.all = 0x0;
	EPwm5Regs.HRCNFG.bit.EDGMODE = HR_REP;				//MEP control on Rising edge
	EPwm5Regs.HRCNFG.bit.CTLMODE = HR_CMP;
	EPwm5Regs.HRCNFG.bit.HRLOAD  = HR_CTR_ZERO;
	EDIS;
}


void HRPWM6_Config(period)
{
// ePWM1 register configuration with HRPWM
// ePWM1A toggle low/high with MEP control on Rising edge
    EPwm6Regs.TBCTL.bit.FREE_SOFT=TB_FREEZE;
	EPwm6Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;	        // set Immediate load
	EPwm6Regs.TBPRD = period-1;		                    // PWM frequency = 1 / period
	EPwm6Regs.CMPA.half.CMPA = period / 2;	            // set duty 50% initially
    EPwm6Regs.CMPA.half.CMPAHR = 0;//disble the HRPWM  (1 << 8);              // initialize HRPWM extension
	EPwm6Regs.CMPB = period / 2;	                    // set duty 50% initially
	EPwm6Regs.TBPHS.all = 0;
	EPwm6Regs.TBCTR = 0;

	EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
	EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;		       // EPWM1 is the Master
	EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
	EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
	EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm6Regs.AQCTLA.bit.ZRO = AQ_CLEAR;               // PWM toggle low/high
	EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm6Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm6Regs.AQCTLB.bit.CBU = AQ_SET;

	EALLOW;
	EPwm6Regs.HRCNFG.all = 0x0;
	EPwm6Regs.HRCNFG.bit.EDGMODE = HR_REP;				//MEP control on Rising edge
	EPwm6Regs.HRCNFG.bit.CTLMODE = HR_CMP;
	EPwm6Regs.HRCNFG.bit.HRLOAD  = HR_CTR_ZERO;
	EDIS;
}

void InitEPwmGpio(void)
{
   EALLOW;
	
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;    // Enable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;    // Enable pull-up on GPIO3 (EPWM3B)

/* Configure ePWM-2 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM2 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO3 as EPWM2B
   
    EDIS;
}


//---------------------------------------------------------------------------
// InitEPwm: 
//---------------------------------------------------------------------------
// This function initializes the ePWM(s) to a known state.
//
void InitH3A_PWM(void)
{
   // Initialize ePWM1/2/3/4/5/6

   //tbd...	 
InitEPwmGpio();
HRPWM6_Config(1500);
HRPWM5_Config(1500);
 
}




//---------------------------------------------------------------------------
// Example: InitTzGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as Trip Zone (TZ) pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 

void InitTzGpio(void)
{
   EALLOW;
   
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.
   GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // Enable pull-up on GPIO12 (TZ1)
   GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;    // Enable pull-up on GPIO13 (TZ2)
   GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // Enable pull-up on GPIO14 (TZ3)
   GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up on GPIO15 (TZ4)

   GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;    // Enable pull-up on GPIO16 (TZ5)
// GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up on GPIO28 (TZ5)

   GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;    // Enable pull-up on GPIO17 (TZ6) 
// GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // Enable pull-up on GPIO29 (TZ6)  
   
/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

   GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;  // Asynch input GPIO12 (TZ1)
   GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;  // Asynch input GPIO13 (TZ2)
   GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;  // Asynch input GPIO14 (TZ3)
   GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (TZ4)

   GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;  // Asynch input GPIO16 (TZ5)
// GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (TZ5)

   GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;  // Asynch input GPIO17 (TZ6) 
// GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;  // Asynch input GPIO29 (TZ6)  

   
/* Configure TZ pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be TZ functional pins.
// Comment out other unwanted lines.   
   GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;  // Configure GPIO12 as TZ1
   GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;  // Configure GPIO13 as TZ2
   GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;  // Configure GPIO14 as TZ3
   GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;  // Configure GPIO15 as TZ4

   GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 3;  // Configure GPIO16 as TZ5
// GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;  // Configure GPIO28 as TZ5

   GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 3;  // Configure GPIO17 as TZ6               
// GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 3;  // Configure GPIO29 as TZ6  

   EDIS;
}



//===========================================================================
// End of file.
//===========================================================================
