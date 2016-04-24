#ifndef _2010_TASK_MONITRO_H
#define  _2010_TASK_MONITRO_H
#if TASK_MONITOR_EN >0

#include "../app_support.h"

#define MAX_MONITOR_NUM 	 (MY_TASK_MAX_NUM+2)
#define TASK_RUN_NOMAL    	 100

typedef void( *os_task_rst_t )( uint8_t pro );

typedef struct
{
	uint8_t pro;
	uint8_t occupy_flag;
	uint32_t cnt;
	OS_STK* task_stk;
	os_task_rst_t rst_inlet;
}monitor_ctl_t;

typedef struct
{
	uint32_t cnt;
	OS_STK *cur_stk_ptr;
}monitor_param_t;

void  	thread_add_self_cnt( void );
void 	thread_sleep_subsetp( Bsize_t jiffies );
void 	thread_add_monitor_list( uint8_t pro, OS_STK*task_stk, os_task_rst_t rst_inlet );
void  	thread_monitor_insmod( void );
#endif
#endif
