// TI File $Revision: 1.9 $
// Checkin $Date: 2014/10/23 02:21:03 $
//###########################################################################
//
// FILE:	DSP2833x_Gpio.c
//
// TITLE:	DSP2833x General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//---------------------------------------------------------------------------
// InitGpio: 
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known (default) state.
//
// For more details on configuring GPIO's as peripheral functions,
// refer to the individual peripheral examples and/or GPIO setup example. 
void InitGpio(void)
{
   EALLOW;
   
   // Each GPIO pin can be: 
   // a) a GPIO input/output
   // b) peripheral function 1
   // c) peripheral function 2
   // d) peripheral function 3
   // By default, all are GPIO Inputs 
   // GPIO2 控制风扇
    GpioCtrlRegs.GPAMUX1.bit.GPIO2=0;
	GpioCtrlRegs.GPADIR.bit.GPIO2=1;

	// GPIO60 控制显示器复位
	GpioCtrlRegs.GPBMUX2.bit.GPIO60=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO60=1;

		// GPIO60 控制蜂鸣器
	GpioCtrlRegs.GPBMUX2.bit.GPIO53=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO53=1;
		// GPIO52 控制DSP工作指示灯
	GpioCtrlRegs.GPBMUX2.bit.GPIO52=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO52=1;

    	// GPIO59  SEL1
	GpioCtrlRegs.GPBMUX2.bit.GPIO59=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO59=0;

    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;     // Enable pull-up on GPIO13 (CAP1)
	GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;


    GpioCtrlRegs.GPAMUX1.bit.GPIO13=0;  //used for IO_EXT_CLK IN CTScale.c f28335 pin 24
	GpioCtrlRegs.GPAMUX1.bit.GPIO14=0;  //used for IO_EXT_DATA	  f28335 pin 25
	GpioCtrlRegs.GPAMUX1.bit.GPIO15=0;  //used for IO_EXT_EN	 f28335 pin 26


	GpioCtrlRegs.GPADIR.bit.GPIO13=1;  //USE FOR OutPut
	GpioCtrlRegs.GPADIR.bit.GPIO14=1;
	GpioCtrlRegs.GPADIR.bit.GPIO15=1;

	//setup for fm25c164 select chip 
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO12=0;
	GpioCtrlRegs.GPADIR.bit.GPIO12=1;

   	GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO24=0;
	GpioCtrlRegs.GPADIR.bit.GPIO24=1;

	GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO23=0;
	GpioCtrlRegs.GPADIR.bit.GPIO23=1;


    //setup for SPI
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.  
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.




    GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   // Enable pull-up on GPIO54 (SPISIMOA)
    GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;   // Enable pull-up on GPIO55 (SPISOMIA)
    GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
    GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;   // Enable pull-up on GPIO57 (SPISTEA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.



    GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; // Asynch input GPIO54 (SPISIMOA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 3; // Asynch input GPIO55 (SPISOMIA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch input GPIO56 (SPICLKA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3; // Asynch input GPIO57 (SPISTEA)

    
/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.


    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1; // Configure GPIO54 as SPISIMOA
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1; // Configure GPIO55 as SPISOMIA
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1; // Configure GPIO56 as SPICLKA
    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0; // Configure GPIO57 as IO

 

    //SETUP for sci
//	GpioCtrlRegs.GPAMUX2.bit.GPIO18=1;  //used for scia-txd
//	GpioCtrlRegs.GPAMUX2.bit.GPIO19=1;  //used for scia_rxd


	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	   // Enable pull-up for GPIO18 (SCITXDB)
  	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	   // Enable pull-up for GPIO19 (SCIRXDB)
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for SCITXDB operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for SCIRXDB operation
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (SCIRXDB) 


	GpioCtrlRegs.GPBMUX2.bit.GPIO62=1;  //used for sciB-txd
	GpioCtrlRegs.GPBMUX2.bit.GPIO63=1;  //used for sciB_rxd
	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO62 (SCIRXDC)

    //setup for pwm
     GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;     // Enable pull-up on GPIO8 
     GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;    // setup GPIO8 as ePWM5A  

	 GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;     // Enable pull-up on GPIO10 
     GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;    // setup GPIO10 as ePWM6A  
   // Each input can have different qualification
   // a) input synchronized to SYSCLKOUT
   // b) input qualified by a sampling window
   // c) input sent asynchronously (valid for peripheral inputs only)
   GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT 
   GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
   GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO39 Synch to SYSCLKOUT 
   GpioCtrlRegs.GPBQSEL2.all = 0x0000;    // GPIO48-GPIO63 Synch to SYSCLKOUT 

   // Pull-ups can be enabled or disabled. 
   GpioCtrlRegs.GPAPUD.all = 0x0000;      // Pullup's enabled GPIO0-GPIO31
   GpioCtrlRegs.GPBPUD.all = 0x0000;      // Pullup's enabled GPIO32-GPIO63
   GpioCtrlRegs.GPCPUD.all = 0x0000;      // Pullup's enabled GPIO64-GPIO79   ?????????stop

   //GpioCtrlRegs.GPAPUD.all = 0xFFFF;    // Pullup's disabled GPIO0-GPIO31
   //GpioCtrlRegs.GPBPUD.all = 0xFFFF;    // Pullup's disabled GPIO32-GPIO34
   //GpioCtrlRegs.GPCPUD.all = 0xFFFF     // Pullup's disabled GPIO64-GPIO79
   //===================================
   //anjylau
   GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;	//为普通IO口   临时取缔
   GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;	//设为输出

	//setup for HT1380 select chip 
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO22=0;
	GpioCtrlRegs.GPADIR.bit.GPIO22=1;

   	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO32=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO32=1;

	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO33=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO33=1;


   EDIS;

}	
	
//===========================================================================
// End of file.
//===========================================================================
