#include	 "INCLUDE\DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include 	 "INCLUDE\DSP2833x_Examples.h"   // DSP2833x Examples Include File


#include 	 "SRC\DSP2833x_CpuTimers.c"   
#include 	 "SRC\DSP2833x_DefaultIsr.c"   
#include 	 "SRC\DSP2833x_GlobalVariableDefs.c"   
#include 	 "SRC\DSP2833x_PieCtrl.c"   
#include 	 "SRC\DSP2833x_PieVect.c"   
#include 	 "SRC\DSP2833x_Spi.c"   
#include 	 "SRC\DSP2833x_SysCtrl.c"   
#include 	 "SRC\DSP2833x_Xintf.c"   
#include     "SRC\DSP2833x_MemCopy.c"



void			DSP28x_usDelay( Uint32 Count );
void 			WDog_disable( void );

void  	BSP_Dly( unsigned long	us )
{
	DSP28x_usDelay(us);
	return;
}


void  BSP_Dly_ms( int  ms )
{
	int  dly;
	dly = ms * 800;
	while ( dly-- )
	{
		asm( "	nop" );

	}
	return;
}

void global_memset( void*pur_add, unsigned char pad_data, unsigned short lenth );
void xintf_zone6and7_timing_quick(void);

/* 平台引导 将平台带入合适的配置*/
void bsp_init( void )
{
	DINT;
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	WDog_disable();
	InitFlash();
	xintf_zone6and7_timing();
	InitSysCtrl();
	InitXintf16Gpio();	//zq
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
//	PieCtrlRegs.PIEIER1.bit.INTx4= 1;
//	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	IER |= M_INT1;	
	return;
}

