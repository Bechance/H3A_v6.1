/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:����Ӧ��
* ����ʱ��:2010-07-12
* ������˾ :����
* �ļ�����:buz_aap.c
* ������:�ҳ�
* ��������:��������¼�����չһ�������ӿ�
*----------------------------------------------------------------
�޸ļ�¼:���Ӻ���buz_alarm����չ��������

�޸�����:7��13��

�޸���:   �ҳ�

**********************************************************************************************************/
#include 		"../includes.h"
#include 		"app_buz.h"
extern          dev_id_t    g_gpio_dev;
#define   		BUZ_ON()   	os_driver_ioctl(g_gpio_dev,GPIO_CMD_SET_BUZ_ON,0)
#define   		BUZ_OFF()	os_driver_ioctl(g_gpio_dev,GPIO_CMD_SET_BUZ_OFF,0)
/*��ģ�����*/
static a_sem_t*		m_buz_sem 		= INVALID_SEM;
static buz_ctl_t 	m_buz_ctl_buf;
static a_lock_t*    m_buz_lock 		= INVALID_LOCK;
static uint8_t      m_buz_task_pro 	= INVALID_PRO;
/********************************************************************
 * @������:�ҳ�
 * @���� : �����������壬�ȴ�����ʵ���ĵ���
 *
 * @���� :p_arg �������
 *
 *@���  :NONE
********************************************************************/
static void app_global_buz_task( void *p_arg )
{
	msleep( TYPE_DELAY );
	while ( 1 )
	{
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )/*�ж��Ƿ���ɾ������*/
		{
			os_task_delete( SELF_PRO );   			      		/*�յ�ɾ������ɾ����������*/
		}
		if ( os_sem_pend( m_buz_sem, 0) == OS_NO_ERR )			/*�ȴ��ź�*/
		{
			os_lock( m_buz_lock );
			while ( m_buz_ctl_buf.ctl_times > 0 )  				/*�ж���Ӧ����*/
			{
	    		BUZ_ON();
				msleep( m_buz_ctl_buf.alarm_time );				/*�ȴ���Ӧ����*/
				BUZ_OFF();
				msleep( m_buz_ctl_buf.stop_time ); 				/*�ȴ���ʱ�����*/
				m_buz_ctl_buf.ctl_times--;		   				/*��Ӧ������*/
			}
			os_unlock( m_buz_lock );
		}
	}
}
/********************************************************************
 * @������:�ҳ�
 * @���� : ���÷���������������������ź���
 *
 * @���� :NONE
 *
 *@���  :NONE
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
 * @������:�ҳ�
 * @���� : ���÷�����Ӧ��������������ź���
 *
 * @���� :NONE
 *
 *@���  :NONE
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
 * @������:�ҳ�
 * @���� : ������ģ���ʼ����������������
 *
 * @���� :buz_dev_id  �Ѿ�ע��� ��Ӧ�ڷ�����Ӳ��ID
 *
 *@���  :NONE
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
 * @������:�ҳ�
 * @���� : ������ģ���˳�
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
void app_global_buz_exit( void )
{
	os_task_delete( m_buz_task_pro );
	m_buz_task_pro = INVALID_PRO;
	global_memset(( uint8_t * )& m_buz_ctl_buf, 0, sizeof( buz_ctl_t ) );
	return;
}
