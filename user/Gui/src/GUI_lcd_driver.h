#ifndef  _PEC_LCD_DRIVER_H_2010
#define  _PEC_LCD_DRIVER_H_2010
#include 		"../Gui.h"
BLKmem_t  		BLKmem_malloc( uint32_t size );
void  			BLKmem_free( BLKmem_t offset );
void 			LCD_L0_init( void );
void 			GUI_L0_DEVReg( void );
///#define DELAY_4_NOP() {asm( "	nop" );asm( "	nop" );asm( "	nop" );asm( "	nop" );asm( "	nop" );}
#endif
