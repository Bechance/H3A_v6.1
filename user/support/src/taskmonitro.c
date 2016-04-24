#include "../app_support.h"
/********************************************************************
* 模块名称:线程监视函数
* 创建时间:2010-07-06
* 所属公司:科荟
* 文件名称:taskmonitor.C
* 创建人  :揭成
* 功能描述:监视各个任务是否运行正常，如果发现运行不正常将做出处理
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:

********************************************************************/
#if TASK_MONITOR_EN >0
monitor_ctl_t  	 	g_monitor_ctl_tcb[MAX_MONITOR_NUM];
monitor_param_t     g_monitor_cnt_buf[MAX_MONITOR_NUM];
/********************************************************************
 * @创建人 :揭成
 * @功能 :任务增加自身的心跳数,提供当前堆栈
 *
 * @输入 :NONE
 *
 * @输出 :NONE
********************************************************************/
void  thread_add_self_cnt( void )
{
	g_monitor_cnt_buf[OSPrioCur].cnt++;
	g_monitor_cnt_buf[OSPrioCur].cur_stk_ptr = OSTCBCur->OSTCBStkPtr;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :把任务添加到监视列表
 *
 * @输入:pro  任务优先级别 task_stk 任务堆栈顶
 *               rst_inlet  任务复位入口
 *
 * @输出:NONE
********************************************************************/
void thread_add_monitor_list( uint8_t pro, OS_STK*task_stk, os_task_rst_t rst_inlet )
{
	uint8_t index;
	for ( index = 0;index < MAX_MONITOR_NUM;index++ )
	{
		if ( !g_monitor_ctl_tcb[index].occupy_flag )
		{
			g_monitor_ctl_tcb[index].pro 			= pro;
			g_monitor_ctl_tcb[index].rst_inlet 		= rst_inlet;
			g_monitor_ctl_tcb[index].task_stk 		= task_stk;
			g_monitor_ctl_tcb[index].occupy_flag 	= 1;
			return ;
		}
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :线程异常处理
 *
 * @输入:ptr   监视控制TCB
 *
 * @输出:NONE
********************************************************************/
void thread_exception_handling( monitor_ctl_t*ptr )
{
	if ( ptr->occupy_flag )
	{
		ptr->rst_inlet( ptr->pro );
		g_monitor_cnt_buf[ptr->pro].cnt++;
		DBGMSG( "the task reset of pro is %b\n", ptr->pro, LOG_NP );
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :任务堆栈检测
 *
 * @输入:ptr   监视控制TCB
 *
 * @输出:NONE
********************************************************************/
BOOL  thread_stk_check( monitor_ctl_t*ptr )
{
	OS_STK*stk;
	stk = g_monitor_cnt_buf[ptr->pro].cur_stk_ptr;

	if ( stk < ( ptr->task_stk ) )
	{
		return FAIL;
	}
	if ( stk > ((( OS_STK* )ptr->task_stk ) + ADVISE_STK_SIZE ) )
	{
		return FAIL;
	}

	return TURE;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :心跳检测
 *
 * @输入:ptr   监视控制TCB
 *
 * @输出:NONE
********************************************************************/

BOOL  thread_cnt_check( monitor_ctl_t*ptr )
{
	uint8_t cnt;
	cnt = g_monitor_cnt_buf[ptr->pro].cnt;
	if ( cnt != ptr->cnt )
	{
		return TURE;
	}
	else
	{
		return FAIL;
	}
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :线程心跳监视
 *
 * @输入:NONE
 *
 * @输出:NONE
********************************************************************/
void  thread_monitor_loop( void )
{
	uint8_t index;
	uint8_t pro;
	monitor_ctl_t*temp_ptr;
	for ( index = 0;index < MAX_MONITOR_NUM;index++ )
	{
		if ( g_monitor_ctl_tcb[index].occupy_flag )
		{
			pro = g_monitor_ctl_tcb[index].pro;
			temp_ptr = & g_monitor_ctl_tcb[index] ;
			if ( thread_cnt_check( temp_ptr ) )
			{
				if ( thread_stk_check( temp_ptr ) )
				{
					g_monitor_ctl_tcb[index].cnt
					= g_monitor_cnt_buf[pro].cnt;
					continue;
				}
			}
			thread_exception_handling( temp_ptr );
		}
	}
	return ;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :线程监视任务
 *
 * @输入:p_arg 任务参数
 *
 * @输出:NONE
********************************************************************/
void thread_monitor_task( void *p_arg )
{
	OSTimeDly( TYPE_DELAY*200 );   //不要使用msleep
	while ( 1 )
	{
		thread_monitor_loop();
		OSTimeDly( TYPE_DELAY*20 );//不要使用msleep
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :任务睡眠分步
 *
 * @输入:delay_jiffies  休眠节拍
 *
 * @输出:NONE
********************************************************************/
void thread_sleep_subsetp( Bsize_t delay_jiffies )
{
	Bsize_t index = delay_jiffies;

	if ( delay_jiffies <= TYPE_DELAY*5 )
	{
		TASK_ADD_SELF_CNT();
		OSTimeDly( delay_jiffies );
	}
	else
	{
		while ( index > TYPE_DELAY*5 )
		{
			index -= TYPE_DELAY * 5;
			TASK_ADD_SELF_CNT();
			OSTimeDly( TYPE_DELAY*5 );
		}
		if ( index > 0 )
		{
			TASK_ADD_SELF_CNT();
			OSTimeDly( index );
		}
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :线程监视模块初始化,初始化控制块，建立监视任务
 *
 * @输入:NONE
 *
 * @输出:NONE
********************************************************************/
void thread_monitor_insmod( void )
{
	GLOBAL_MEMCLR(g_monitor_ctl_tcb,MAX_MONITOR_NUM*sizeof(monitor_ctl_t));
	GLOBAL_MEMCLR(g_monitor_cnt_buf,MAX_MONITOR_NUM*sizeof(monitor_param_t));
	os_task_assign_pro_build( thread_monitor_task, MONITOR_TASK_PRO, 1 );
	os_task_name_set( MONITOR_TASK_PRO, "Support--Task monitor" );
	DBGMSG( "task monitor mod init finish !\n", LOG_NP, LOG_NP );
	return;
}
#endif
