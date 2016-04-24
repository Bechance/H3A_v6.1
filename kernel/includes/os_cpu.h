/*
***************************************************************************************************
* define the type of data
***************************************************************************************************
*/
#ifndef os_cpu_h
#define os_cpu_h

typedef unsigned char   BOOLEAN;
typedef unsigned char   INT8U;
typedef unsigned int    INT16U;
typedef unsigned long   INT32U;
typedef signed char     INT8S;
typedef signed int      INT16S;
typedef signed long     INT32S;
typedef float           FLOAT32;
typedef long double     FLOAT64;
typedef unsigned int    OS_STK;

#define OS_CRITICAL_METHOD      1

#if OS_CRITICAL_METHOD==1
#define OS_ENTER_CRITICAL()     	asm(" setc INTM")
#define OS_EXIT_CRITICAL()      	asm(" clrc INTM")
#endif

#if OS_CRITICAL_METHOD==2
#define OS_ENTER_CRITICAL()
#define OS_EXIT_CRITICAL()
#endif

#if OS_CRITICAL_METHOD==3
#define OS_ENTER_CRITICAL()
#define OS_EXIT_CRITICAL()
#endif

#define OS_STK_GROWTH           0

//#define OS_TASK_SW()          asm("TRAP #31") //{OSCtxSw();}//asm("LB _OSCtxSw")  //  asm("TRAP #31")
#define OS_TASK_SW()            asm("     TRAP #31")
/**********************************************************************************************************
* 系统用自定义的变量与函数声明
**********************************************************************************************************/
extern INT16U      SysStk;
extern INT16U      OS_Critical_Counter;
extern INT8U       OS_ERROR;
void   	OSCtxSw( void );
void   	OSIntCtxSw( void );
void 	OSStartHighRdy( void );
void 	os_enter_critical( void );
void 	os_exit_critical( void );
#endif
/*
*********************************************************************************************************
* No more
*********************************************************************************************************
*/
