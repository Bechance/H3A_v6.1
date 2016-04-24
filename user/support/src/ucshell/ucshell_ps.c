#include	"ucshell.h"
#if UC_SHELL_EN >0
tmr_member_t*		m_ucshell_pst_tmr = PNULL;
tmr_member_t*		m_ucshell_psd_tmr = PNULL;
tmr_member_t*		m_ucshell_psk_tmr = PNULL;
tmr_member_t*		m_ucshell_eff_tmr = PNULL;

/********************************************************************
 * @创建人:揭成
 * @功能:PST 命令LOG
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_pst_log( void *arg )
{
	uint8_t index;
	char task_name[OS_TASK_NAME_SIZE];
	Bsize_t cnt;
	USER_LOG( "\n-------------task table-------------------" );
	cnt = 0;
	for ( index = 0;index < MY_TASK_MAX_NUM;index++ )
	{
		if ( m_task_pro_array[index] == PRO_IS_MAIN_OCCUPY )
		{
			os_task_name_get( index, task_name );
			if ( index <= 9 )
			{
				DBGMSG( "\npro : %b  name : %s", index, task_name[0] );
			}
			else
			{
				DBGMSG( "\npro :%b  name : %s", index, task_name[0] );
			}
			cnt++;
		}
		else if ( m_task_pro_array[index] == PRO_IS_SLAVE_OCCUPY )
		{
			cnt++;
		}
	}
#if OS_TASK_STAT_EN>0
	index = OS_TASK_STAT_PRIO;
	os_task_name_get( index, task_name );
	DBGMSG( "\npro :%b  name : %s", index, task_name[0] );
#endif
	index = OS_TASK_IDLE_PRIO;
	os_task_name_get( index, task_name );
	DBGMSG( "\npro :%b  name : %s", index, task_name[0] );
	index = MY_TASK_MAX_NUM;
	cnt = MY_TASK_MAX_NUM - cnt;
	DBGMSG( "\nsurplus\\toplimit=%d\\%d   ", cnt, index );
	index = OS_MAX_EVENTS;
	DBGMSG( "Sem\\toplimit=%d\\%d\n", m_event_stat_cnt, index );
	USER_LOG( "-------------------------------------------\n" );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:PSD命令LOG
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
extern os_dirver_maneger_t 	m_driver_manege_tab;
void ucshell_psd_log( void *arg )
{
	uint8_t index;
	uint8_t dev_id;
	Bsize_t cnt = 0;
	char*pname;
	USER_LOG( "\n-------------driver table------------------" );
	for ( index = 0;index < MAX_SURPPORT_DRIVER;index++ )
	{
		if ( m_driver_manege_tab.os_driver_member[index].id != 0 )
		{
			pname = m_driver_manege_tab.os_driver_member[index].os_driver_name;
			dev_id =  m_driver_manege_tab.os_driver_member[index].id;
			if ( dev_id <= 9 )
			{
				DBGMSG( "\ndev id : %b  name : %s", dev_id, pname[0] );
			}
			else
			{
				DBGMSG( "\ndev id :%b  name : %s", dev_id, pname[0] );
			}
			cnt++;
		}
	}
	index =  MAX_SURPPORT_DRIVER;
	DBGMSG( "\ndrivers\\toplimit=%d\\%d\n", cnt, index );
	USER_LOG( "-------------------------------------------\n" );
	return;
}

/********************************************************************
 * @创建人:揭成
 * @功能:PSK 命令LOG
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_psk_log( void *arg )
{
#if OS_TASK_CREATE_EXT_EN  >0
	uint8_t index;
	OS_STK_DATA stk_state;
	USER_LOG( "\n-------------stk state table-------------------" );
	for ( index = 0;index < MY_TASK_MAX_NUM;index++ )
	{
		if ( m_task_pro_array[index] == PRO_IS_MAIN_OCCUPY )
		{
			OSTaskStkChk( index, &stk_state );
			if ( index <= 9 )
			{
				DBGMSG( "\npro : %b  ", index, LOG_NP );
			}
			else
			{
				DBGMSG( "\npro :%b  ", index, LOG_NP );
			}
			DBGMSG( "Used :%l,Free:%l", stk_state.OSUsed, stk_state.OSFree );
			if ( stk_state.OSFree < 40 )
			{
				USER_LOG( "    Warning :The task must be increased stack" );
			}
		}
	}
	USER_LOG( "\n-------------------------------------------\n" );
#endif
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:eff 命令LOG
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_eff_log( void *arg )
{
	char icon = '%';
	if ( OSCPUUsage > m_max_cpu_usage )
	{
		m_max_cpu_usage = OSCPUUsage;
	}
	DBGMSG( "\n--the current CPU utilization =%c%b", icon, OSCPUUsage );
	DBGMSG( "  the highest value in history =%c%b", icon, m_max_cpu_usage );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:PST命令
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_cmd_pst( void*arg )
{
	ucshell_pst_log( 0 );
	if ( !m_ucshell_pst_tmr )
	{
		m_ucshell_pst_tmr = timer_member_create( APP_HALF_SEC * 10,
							TIMER_AUTO,
							ucshell_pst_log,
							0 );
	}
	return;
}
void ucshell_cmd_pst_exit( void*arg )
{
	timer_member_state_set( m_ucshell_pst_tmr,
							TMR_DELETE_REQ );
	m_ucshell_pst_tmr = PNULL;
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:PSD命令
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_cmd_psd( void*arg )
{
	ucshell_psd_log( 0 );
	if ( !m_ucshell_psd_tmr )
	{
		m_ucshell_psd_tmr = timer_member_create( APP_HALF_SEC * 10 * 2,
							TIMER_AUTO,
							ucshell_psd_log,
							0 );
	}
	return;
}
void ucshell_cmd_psd_exit( void*arg )
{
	timer_member_state_set( m_ucshell_psd_tmr,
							TMR_DELETE_REQ );
	m_ucshell_psd_tmr = PNULL;
	return;
}

/********************************************************************
 * @创建人:揭成
 * @功能:PSD命令
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_cmd_psk( void*arg )
{
	ucshell_psk_log( 0 );
	if ( !m_ucshell_psk_tmr )
	{
		m_ucshell_psk_tmr = timer_member_create( APP_HALF_SEC * 10,
							TIMER_AUTO,
							ucshell_psk_log,
							0 );
	}
	return;
}
void ucshell_cmd_psk_exit( void*arg )
{
	timer_member_state_set( m_ucshell_psk_tmr,
							TMR_DELETE_REQ );
	m_ucshell_psk_tmr = PNULL;
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:eff命令
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_cmd_eff( void*arg )
{
	ucshell_eff_log( 0 );
	if ( !m_ucshell_eff_tmr )
	{
		m_ucshell_eff_tmr = timer_member_create( APP_HALF_SEC * 2,
							TIMER_AUTO,
							ucshell_eff_log,
							0 );
	}
	return;
}
void ucshell_cmd_eff_exit( void*arg )
{
	timer_member_state_set( m_ucshell_eff_tmr,
							TMR_DELETE_REQ );
	m_ucshell_eff_tmr = PNULL;
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:PS命令
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_cmd_ps( void*arg )
{
	ucshell_psk_log( 0 );
	msleep( TYPE_DELAY + 2 );
	ucshell_pst_log( 0 );
	msleep( TYPE_DELAY + 2 );
	DBGMSG( "Pro array[%s]\n", m_task_pro_array[0], LOG_NP );
	return;
}
void ucshell_cmd_ps_exit( void*arg )
{
	ucshell_cmd_pst_exit( 0 );
	ucshell_cmd_psd_exit( 0 );
	ucshell_cmd_eff_exit( 0 );
	ucshell_cmd_psk_exit( 0 );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:初始化PS 系列命令参数
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_ps_cmd_init( void )
{
	m_ucshell_pst_tmr = PNULL;
	m_ucshell_psd_tmr = PNULL;
	m_ucshell_eff_tmr = PNULL;
	m_ucshell_psk_tmr = PNULL;
	return;
}
#endif
