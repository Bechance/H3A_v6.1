#include "../app_support.h"
/********************************************************************
* ģ������:�̼߳��Ӻ���
* ����ʱ��:2010-07-06
* ������˾:����
* �ļ�����:taskmonitor.C
* ������  :�ҳ�
* ��������:���Ӹ��������Ƿ���������������������в���������������
*----------------------------------------------------------------
�޸ļ�¼:

�޸�����:

�޸���:

********************************************************************/
#if TASK_MONITOR_EN >0
monitor_ctl_t  	 	g_monitor_ctl_tcb[MAX_MONITOR_NUM];
monitor_param_t     g_monitor_cnt_buf[MAX_MONITOR_NUM];
/********************************************************************
 * @������ :�ҳ�
 * @���� :�������������������,�ṩ��ǰ��ջ
 *
 * @���� :NONE
 *
 * @��� :NONE
********************************************************************/
void  thread_add_self_cnt( void )
{
	g_monitor_cnt_buf[OSPrioCur].cnt++;
	g_monitor_cnt_buf[OSPrioCur].cur_stk_ptr = OSTCBCur->OSTCBStkPtr;
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :��������ӵ������б�
 *
 * @����:pro  �������ȼ��� task_stk �����ջ��
 *               rst_inlet  ����λ���
 *
 * @���:NONE
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
 * @������ :�ҳ�
 * @���� :�߳��쳣����
 *
 * @����:ptr   ���ӿ���TCB
 *
 * @���:NONE
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
 * @������ :�ҳ�
 * @���� :�����ջ���
 *
 * @����:ptr   ���ӿ���TCB
 *
 * @���:NONE
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
 * @������ :�ҳ�
 * @���� :�������
 *
 * @����:ptr   ���ӿ���TCB
 *
 * @���:NONE
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
 * @������ :�ҳ�
 * @���� :�߳���������
 *
 * @����:NONE
 *
 * @���:NONE
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
 * @������ :�ҳ�
 * @���� :�̼߳�������
 *
 * @����:p_arg �������
 *
 * @���:NONE
********************************************************************/
void thread_monitor_task( void *p_arg )
{
	OSTimeDly( TYPE_DELAY*200 );   //��Ҫʹ��msleep
	while ( 1 )
	{
		thread_monitor_loop();
		OSTimeDly( TYPE_DELAY*20 );//��Ҫʹ��msleep
	}
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :����˯�߷ֲ�
 *
 * @����:delay_jiffies  ���߽���
 *
 * @���:NONE
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
 * @������ :�ҳ�
 * @���� :�̼߳���ģ���ʼ��,��ʼ�����ƿ飬������������
 *
 * @����:NONE
 *
 * @���:NONE
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
