/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:按键应用
* 创建时间:2010-07-12
* 所属公司 :科荟
* 文件名称:buz_aap.c
* 创建人:揭成
* 功能描述:处理蜂鸣事件，扩展一个报警接口
*----------------------------------------------------------------
修改记录:增加函数buz_alarm，扩展报警功能

修改日期:7月13日

修改人:   揭成

**********************************************************************************************************/
#include 		"../includes.h"
#include 		"app_buz.h"
extern          dev_id_t    g_gpio_dev;
#define   		BUZ_ON()   	os_driver_ioctl(g_gpio_dev,GPIO_CMD_SET_BUZ_ON,0)
#define   		BUZ_OFF()	os_driver_ioctl(g_gpio_dev,GPIO_CMD_SET_BUZ_OFF,0)
/*本模块变量*/
static a_sem_t*		m_buz_sem 		= INVALID_SEM;
static buz_ctl_t 	m_buz_ctl_buf;
static a_lock_t*    m_buz_lock 		= INVALID_LOCK;
static uint8_t      m_buz_task_pro 	= INVALID_PRO;
/********************************************************************
 * @创建人:揭成
 * @功能 : 蜂鸣任务主体，等待蜂鸣实践的到来
 *
 * @输入 :p_arg 任务参数
 *
 *@输出  :NONE
********************************************************************/
static void app_global_buz_task( void *p_arg )
{
	msleep( TYPE_DELAY );
	while ( 1 )
	{
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )/*判断是否有删除请求*/
		{
			os_task_delete( SELF_PRO );   			      		/*收到删除请求，删除自身任务*/
		}
		if ( os_sem_pend( m_buz_sem, 0) == OS_NO_ERR )			/*等待信号*/
		{
			os_lock( m_buz_lock );
			while ( m_buz_ctl_buf.ctl_times > 0 )  				/*判断响应次数*/
			{
	    		BUZ_ON();
				msleep( m_buz_ctl_buf.alarm_time );				/*等待响应节拍*/
				BUZ_OFF();
				msleep( m_buz_ctl_buf.stop_time ); 				/*等待关时间节拍*/
				m_buz_ctl_buf.ctl_times--;		   				/*响应次数减*/
			}
			os_unlock( m_buz_lock );
		}
	}
}
/********************************************************************
 * @创建人:揭成
 * @功能 : 设置蜂鸣报警，向蜂鸣任务发送信号量
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_global_buz_alarm( Bsize_t stop_jiffies, Bsize_t alarm_jiffies, Bsize_t cycle_times )
{
	if ( m_buz_task_pro == INVALID_PRO )
		return ;
	os_lock( m_buz_lock );
	m_buz_ctl_buf.alarm_time = alarm_jiffies;
	m_buz_ctl_buf.ctl_times  = cycle_times;
	m_buz_ctl_buf.stop_time  = stop_jiffies;
	os_unlock( m_buz_lock );
	os_sem_post( m_buz_sem );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 : 设置蜂鸣响应，向蜂鸣任务发送信号量
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_global_buz_response( Bsize_t response_jiffies )
{
	if ( m_buz_task_pro == INVALID_PRO )
		return ;
	os_lock( m_buz_lock );
	m_buz_ctl_buf.alarm_time = response_jiffies;
	m_buz_ctl_buf.ctl_times = 1;
	m_buz_ctl_buf.stop_time = 0;
	os_unlock( m_buz_lock );
	os_sem_post( m_buz_sem );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 : 蜂鸣器模块初始化，创建蜂鸣任务
 *
 * @输入 :buz_dev_id  已经注册的 对应于蜂鸣器硬件ID
 *
 *@输出  :NONE
********************************************************************/
void app_global_buz_entery( void )
{
	os_sem_creat( &m_buz_sem, 0 );
	os_lock_creat( &m_buz_lock );
	global_memset(( uint8_t * )& m_buz_ctl_buf, 0, sizeof( buz_ctl_t ) );
	m_buz_task_pro
	= os_task_assign_stk_build( app_global_buz_task, 1 );
	os_task_name_set( m_buz_task_pro , "Global--BUZ" );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 : 蜂鸣器模块退出
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_global_buz_exit( void )
{
	os_task_delete( m_buz_task_pro );
	m_buz_task_pro = INVALID_PRO;
	global_memset(( uint8_t * )& m_buz_ctl_buf, 0, sizeof( buz_ctl_t ) );
	return;
}
