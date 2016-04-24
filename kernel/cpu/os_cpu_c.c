#include "../includes\ucos_ii.h"
#include "../../user\includes.h"

#define    _2812_IER_SET     (M_INT1|M_INT2|M_INT9|M_INT3|M_INT13|M_INT14|M_INT8)

/*
*********************************************************************************************************
*  任务栈初始化函数
*********************************************************************************************************
*/
OS_STK* OSTaskStkInit( void ( *task )( void *p_arg ), void *p_arg, OS_STK *ptos, INT16U opt )
{
	OS_STK* stk = ptos;
	OS_STK temp;

	opt    = opt;                           /* 'opt' is not used, prevent warning                  */
	stk    = ( INT16U * )ptos;              /* Load stack pointer                                  */

//    *stk++ = (INT16U)(pdata);         	/* Simulate call to function with argument             */
//    *stk++ = (INT16U)(pdata);

	//当栈宽度为16位是，一个指针自增操作只会将指针加1
	*stk++ = ( OS_STK )( *(( OS_STK* )p_arg ) );

	//栈增长方式为从低到高，始终指向下一个空地址
	*stk++ = ( OS_STK )( *(( OS_STK* )p_arg ) );

	*stk++ = 0x0000;						/* ST0     = 0     								       */
	*stk++ = 0x0000;					    /* T       = 0x0000   								   */


	*stk++ = 0x0000;						/* AL      = 0										   */
	*stk++ = 0x0000;						/* AH      = 0 									       */


	*stk++ = 0x0000;						/* PL      = 0										   */
	*stk++ = 0x0000;					    /* PH      = 0										   */


	*stk++ = 0x0000;					   	/* AR0     = 0										   */
	*stk++ = 0x0000;						/* AR1     = 0										   */



	*stk++ = 0x0A0A;						    /* ST1     = 0x0a0a 								  */
	*stk++ = 0x0000;    				   	    /* DP      = 0										   */

	*stk++ = _2812_IER_SET;						// 0x2103;/* IER     = 0x2103									       */
	*stk++ = 0;									/* DBGSTAT = 0 									   */

	temp   = (( INT32U )task ) & 0x0000ffff;
	*stk++ = ( INT16U )temp;					/*  保存低16位											   */

	temp   = (( INT32U )task ) >> 16;    			/*  Save task entry										   */
	*stk++ = ( INT16U )( temp );	   			/*  保存高16位											   */

//	 PUSH   RPC
	temp   = (( INT32U )task ) & 0x0000ffff;		/* RPCL     = 0xCCCC									   */
	*stk++ = ( INT16U )temp;					/*  保存低16位											   */

	temp   = (( INT32U )task ) >> 16;    			/* RPCH     = 0xCCCC									   */
	*stk++ = ( INT16U )( temp );	   			/*  保存高16位*/

	/*
	下为手工保存的寄存器
        PUSH    AR1H:AR0H   
        PUSH    XAR2
        PUSH    XAR3
        PUSH    XAR4
        PUSH    XAR5
        PUSH    XAR6
        PUSH    XAR7

        MOV32   *SP++,STF
        MOV32   *SP++,R0H
        MOV32   *SP++,R1H
        MOV32   *SP++,R2H
        MOV32   *SP++,R3H
        MOV32   *SP++,R4H
        MOV32   *SP++,R5H       
        MOV32   *SP++,R6H  
        
      	PUSH    XT
        ASP
        PUSH    DP:ST1
	*/
	*stk++ = 0x0000;	   			/*  保存高16位  PUSH    AR1H:AR0H  */
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位 PUSH    XAR2*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位 PUSH    XAR3*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位 PUSH    XAR4*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位 PUSH    XAR5*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位 PUSH    XAR6*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位 PUSH    XAR7*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位  MOV32   *SP++,STF*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位   MOV32   *SP++,R0H*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位   MOV32   *SP++,R1H*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位   MOV32   *SP++,R2H*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位  MOV32   *SP++,R3H*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位   MOV32   *SP++,R4H*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位   MOV32   *SP++,R5H*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0000;	   			/*  保存高16位   MOV32   *SP++,R6H*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	
	

	*stk++ = 0x0000;	   			/*  保存高16位 XT*/
	*stk++ = 0x0000;	   			/*  保存低16位 */

	*stk++ = 0x0A0A;	   			/*  ST1 */
	*stk++ = 0x0000;	   			/*  DP */

	stk++;
	return (( void * )stk );

}


void OSInitHookBegin( void )
{
}

void OSInitHookEnd( void )
{
}

void OSTaskCreateHook( OS_TCB *ptcb )
{
	ptcb = ptcb;
}

void OSTaskDelHook( OS_TCB *ptcb )
{
	ptcb = ptcb;
}

void OSTaskIdleHook( void )
{
	//*  空闲任务中，让CPU进入空闲模式
    //asm("     IDLE");
	//*/
}
void OSTaskStatHook( void )
{
}

#if OS_TASK_SW_HOOK_EN > 0
void OSTaskSWHook( void )
{
}
#endif

void OSTCBInitHook( OS_TCB *ptcb )
{
	ptcb = ptcb;
}

#if OS_TIME_TICK_HOOK_EN > 0

#pragma CODE_SECTION(OSTimeTickHook,"ramfuncs")

void OSTimeTickHook( void )
{
#if TIMER_ARRAY_EN>0
	timer_jiffies_add();
#endif
	return;
}
#endif

#if OS_APP_HOOKS_EN > 0
void App_TaskCreateHook( OS_TCB *ptcb )
{
	ptcb = ptcb;
}
void App_TaskDelHook( OS_TCB *ptcb )
{
	ptcb = ptcb;
}
void App_TaskIdleHook( void )
{
}

void App_TaskStatHook( void )
{

}

#if OS_TASK_SW_HOOK_EN > 0
void App_TaskSwHook( void )
{
}
#endif

void App_TCBInitHook( OS_TCB *ptcb )
{
	ptcb = ptcb;
}

#if OS_TIME_TICK_HOOK_EN > 0
void App_TimeTickHook( void )
{
}
#endif
#endif
/*
*********************************************************************************************************
* No more
*********************************************************************************************************
*/
