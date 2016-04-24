/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:��ʱ���й�����
* ���ʱ��2010-07-22
* ������˾ :����
* �ļ�����:timer_array.c
* ������ :�ҳ�
* ��������:������ʱ�б�����ʱ����¼�
*                       �¹��� ������ԭ���ܣ�Ӧ��˫������
*                       ��������ƿ飬Ҫ��C�������ṩ�ڴ�
*                       ��������malloc ()    free()
*-------------------------------------------------------------------
�޸ļ�¼: ���¹��ܶ�ʱ����ģ�飬���и���Խ��
                       �ṹ��û�ж�ʱ��Ա���������ơ�

�޸�����:2010-7-22

�޸���:�ҳ�
*********************************************************************************************************
�벻Ҫ���ж��е��ø�ģ��API�ӿ�
*/
#include "../app_support.h"
#if TIMER_ARRAY_EN>0
/*��ʱ����������*/
#if (COMPILER_MALLOC_SUPPORT >0) &&(LINK_LIST_MANEGER_EN >0)
uint32_t  volatile 	m_timer_array_jiffies = 0;
link_list_tcb_t*	m_tmr_link_tcb;
/********************************************************************
 * @������ :�ҳ�
 * @����:����һ���³�Ա
 * @����: need_jiffies ��ʱ���
 *		  type       ��Ա����
 *		  caller     �ص�����
 *        p_arg      �ص���������
 * @���:��Աλ��
********************************************************************/
tmr_member_t*timer_member_create( uint32_t need_jiffies,
								  Bsize_t type,
								  tmr_callblack caller,
								  void*p_arg )
{

	link_list_t *      cur_link_list;
	tmr_member_t *tmr;
	cur_link_list = link_list_memeber_create
					( m_tmr_link_tcb,
					  LINK_ADD_END,
					  sizeof( tmr_member_t ) );
	if ( cur_link_list )
	{
		tmr = ( tmr_member_t* )cur_link_list->cur_member;
		tmr ->auto_load_expires	= need_jiffies;
		timer_member_mod_expires( tmr, need_jiffies );
		tmr ->function = caller;
		tmr ->tmr_state = TMR_IS_OK;
		tmr->type = type;
		tmr ->p_arg =  p_arg;
		return  tmr;
	}
	return PNULL;
}
/********************************************************************
 * @������:�ҳ�
 * @����:���ö�ʱ����
 *
 * @����:���ʱ��
 *
 * @���:NONE
********************************************************************/
void timer_member_revert_expires( uint32_t over_jiffies )
{
	link_list_t *		tmr_cur_link ;
	tmr_member_t*		tmr;
	tmr_cur_link = m_tmr_link_tcb->link_list_end ;
	while ( tmr_cur_link )
	{
		tmr = ( tmr_member_t* )tmr_cur_link->cur_member;
		tmr->expires += over_jiffies;
		tmr_cur_link = tmr_cur_link->last_member;
	}
	m_timer_array_jiffies += over_jiffies;
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:��ʱ��Ա����ʱ���ĸ���
 *
 * @����:identity  over_time
 *
 * @���:NONE
********************************************************************/
void timer_member_mod_expires( tmr_member_t*tmr,
							   uint32_t over_time )
{
	tmr->expires = m_timer_array_jiffies + over_time;
	if ( tmr->expires < m_timer_array_jiffies ) /*�жϽ����ǲ�����Ҫ����*/
	{
		timer_member_revert_expires( over_time );
		tmr->expires -= over_time;
	}
	return;
}

/********************************************************************
 * @������:�ҳ�
 * @����:����һ���³�Ա
 * @����: tmr       ��Աָ��
 *		  state     Ҫ���õ�״̬��
                    TMR_HAND_UP_REQ ����
				    TMR_IS_OK       ����
					TMR_DELETE_REQ  ɾ��
					TMR_REVERT_REQ  ȡ������
 * @���:NONE
********************************************************************/
void timer_member_state_set( tmr_member_t*tmr, Bsize_t state )
{
	if ( state == TMR_REVERT_REQ )
	{
		if ( tmr->tmr_state == TMR_HAND_UP_STATE )
		{
			tmr->tmr_state = state;
		}
	}
	else
	{
		tmr->tmr_state = state;
	}
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:��ʱ��׼�����Լ�
 *
 * @����: NONE

 *@���:NONE
********************************************************************/
void timer_jiffies_add( void )
{
	m_timer_array_jiffies++;
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:��ʱ������������
 *
 * @����:
 *
 *@���:NONE
********************************************************************/
void timer_array_maneger_task( void*p_arg )
{
	link_list_t *		tmr_cur_link ;
	link_list_t *		tmr_link_shadow ;
	tmr_member_t *	tmr;
	while ( 1 )
	{
		tmr_cur_link = m_tmr_link_tcb->link_list_end ; 			                   /*�õ���ʱ������β*/
		while ( tmr_cur_link )
		{
			tmr = ( tmr_member_t* )tmr_cur_link->cur_member;     				 /*�õ���ʱ����Աλ��*/
			if ( tmr->tmr_state == TMR_DELETE_REQ )		      					/*�ж��Ƿ���ɾ������   */
			{
				tmr_link_shadow = tmr_cur_link; 								/*��һ�α������ռ��ͷ�ʱ�����*/
				tmr_cur_link = tmr_cur_link->last_member;                          		  /*�ƶ�����ָ����һ����Ա*/
				link_list_memeber_destory( m_tmr_link_tcb, tmr_link_shadow ); /*ɾ�������Ա*/
				continue;
			}
			if ( tmr->tmr_state == TMR_HAND_UP_REQ ) 						/*�ж��Ƿ��й�������   */
			{
				tmr->expires = tmr->expires - m_timer_array_jiffies;                     	 	/*����ʱ��Ա����ʣ��ʱ�䱣�� */
				tmr->tmr_state   = TMR_HAND_UP_STATE;
				tmr_cur_link = tmr_cur_link->last_member;                        		/*�ƶ�����ָ����һ����Ա*/
				continue;
			}
			if ( tmr->tmr_state == TMR_REVERT_REQ ) 			     			/*�ж��Ƿ���ȡ����������   */
			{
				timer_member_mod_expires( tmr, tmr->expires );			/*����ʱ��Ա����ʣ��װ��*/
				tmr->tmr_state = TMR_IS_OK;
				tmr_cur_link = tmr_cur_link->last_member;    					/*�ƶ�����ָ����һ����Ա*/
				continue;
			}
			if ( tmr->expires <= m_timer_array_jiffies )                       					/*�ж�ʱ�䶨ʱʱ���Ƿ����*/
			{
				if ( tmr->type == TIMER_AUTO )                					/*�жϳ�Ա�����Ƿ�����װ�ؽ���*/
				{
					timer_member_mod_expires
					( tmr,
					  tmr->auto_load_expires );								/*�Զ�װ��ʱ�����*/
					tmr_cur_link = tmr_cur_link->last_member;                    	/*�ƶ�����ָ����һ����Ա*/
				}
				tmr->function( tmr->p_arg );   								/*���ûص�����*/
				if ( tmr->type == TIMER_SETTINGS )               				/*�жϳ�Ա�����Ƿ���һ���Զ�ʱʵ��*/
				{
					tmr_link_shadow = tmr_cur_link;				           		/*��һ�α������ռ��ͷ�ʱ�����*/
					tmr_cur_link = tmr_cur_link->last_member;                  		/*�ƶ�����ָ����һ����Ա*/
					link_list_memeber_destory( m_tmr_link_tcb, tmr_link_shadow );/*ɾ����ִ�е�ʱ���Ա*/
				}
				continue;
			}
			tmr_cur_link = tmr_cur_link->last_member;
		}
		msleep( TYPE_DELAY );
	}
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:��ʱ��������ʼ��
 *
 * @����:
 *
 *@���:NONE
********************************************************************/
void timer_array_mod_init( void )
{
	m_tmr_link_tcb = link_list_tcb_req();
	os_task_assign_pro_build( timer_array_maneger_task, TIMER_ARRAY_TASK_PRO, 2 );
	os_task_name_set( TIMER_ARRAY_TASK_PRO, "Support--Timer array" );
	return;
}
#else   
/*���ṩmalloc ֧��ģ��д��*/
/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:��ʱ���й�����
* ���ʱ��2010-07-15
* ������˾ :����
* �ļ�����:timer_array.c
* ������ :�ҳ�
* ��������:������ʱ�б�����ʱ����¼�
*-------------------------------------------------------------------
�޸ļ�¼: tmr_member_t ������VOID*arg,�ص���������
Ŀ��:     �ǻص��������Դ���������

�޸�����:2010-7-19

�޸���:�ҳ�
*********************************************************************************************************
*/
uint32_t  volatile 	m_timer_array_jiffies = 0;

tmr_array_t   		m_timer_array = {0, };

a_lock_t *		    m_timer_lock = INVALID_SEM;

/*��ʱ������Դ���ӿں������ڶ�������ʹ�ã�����ǵ�����ϵͳ���迼��*/
void timer_array_lock( void )
{
	os_lock( m_timer_lock );
	return;
}
void timer_array_unlock( void )
{
	os_unlock( m_timer_lock );
	return;
}
void timer_array_lock_creat( void )
{
	os_lock_creat( &m_timer_lock );
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:����һ���³�Ա
 *
 * @����: need_jiffies ��ʱ���
 *			type       ��Ա����
 *			caller     �ص�����
 *          p_arg      �ص���������
 *@���:��Աλ��
********************************************************************/
tmr_member_t*timer_member_create( uint32_t need_jiffies,
								  Bsize_t type,
								  tmr_callblack caller,
								  void*p_arg )
{
	Bsize_t index = 0;
	tmr_member_t*tmr_pos;
	timer_array_lock();
	for ( index = 0;index < MAX_TMR_NUM;index++ )
	{
		if ( m_timer_array.tmr_array[index].tmr_state == TMR_IDLE )
		{
			m_timer_array.ture_size++;
			tmr_pos = &m_timer_array.tmr_array[index];
			tmr_pos->auto_load_expires = need_jiffies;
			tmr_pos->function = caller;
			tmr_pos->p_arg = p_arg;
			tmr_pos->type = type;
			tmr_pos->tmr_state = TMR_IS_OK;
			timer_member_mod_expires( tmr_pos, need_jiffies );
			timer_array_unlock();
			return  tmr_pos;
		}
	}
	timer_array_unlock();
	USER_LOG( "\nThe timer number has reached the maximum, please MAX_TMR_NUM modification.\n" );
	return PNULL;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:���ö�ʱ����
 *
 * @����:���ʱ��
 *
 *@���:NONE
********************************************************************/
void timer_member_revert_expires( uint32_t over_jiffies )
{
	Bsize_t index = 0;
	tmr_member_t *	tmr;
	for (index=0;index < MAX_TMR_NUM;index++)
	{
		if ( m_timer_array.tmr_array[index].tmr_state != TMR_IDLE )
		{
			tmr->expires += over_jiffies;
		}
	}
	m_timer_array_jiffies += over_jiffies;
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:��ʱ��Ա����ʱ���ĸ���
 *
 * @����:identity  over_time
 *
 *@���:NONE
********************************************************************/
void timer_member_mod_expires( tmr_member_t*tmr,
							   uint32_t over_time )
{
	tmr->expires = m_timer_array_jiffies + over_time;
	if ( tmr->expires < m_timer_array_jiffies ) /*�жϽ����ǲ�����Ҫ����*/
	{
		timer_member_revert_expires( over_time );
		tmr->expires -= over_time;
	}
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:����һ���³�Ա
 *
 * @����: tmr            ��Աָ��
 *			state         Ҫ���õ�״̬��
                                  	TMR_HAND_UP_REQ     ����
						TMR_IS_OK          ����
						TMR_DELETE_REQ  ɾ��
						TMR_REVERT_REQ  ȡ������
 *@���:NONE
********************************************************************/
void timer_member_state_set( tmr_member_t*tmr, Bsize_t state )
{
	if ( state == TMR_REVERT_REQ )
	{
		if ( tmr->tmr_state == TMR_HAND_UP_STATE )
		{
			tmr->tmr_state = state;
		}
	}
	else
	{
		tmr->tmr_state = state;
	}
	return;
}

/********************************************************************
 * @������ :�ҳ�
 * @����:��ʱ��׼�����Լ�
 *
 * @����: NONE

 *@���:NONE
********************************************************************/
void timer_jiffies_add( void )
{
	m_timer_array_jiffies++;
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:��ʱ������������
 *
 * @����:
 *
 *@���:NONE
********************************************************************/
void timer_array_maneger_task( void*p_arg )
{
	tmr_member_t *	tmr;
	Bsize_t index = 0;
	while ( 1 )
	{
		for ( index = 0; index < MAX_TMR_NUM;index++ )
		{
			tmr = &m_timer_array.tmr_array[index];
			if ( tmr->tmr_state == TMR_IDLE )
				continue;
			if ( tmr->tmr_state == TMR_DELETE_REQ )		      /*�ж��Ƿ���ɾ������   */
			{
				timer_array_lock();
				global_memset(( uint8_t * ) tmr, 0, sizeof( tmr_member_t ) );
				m_timer_array.ture_size--;
				timer_array_unlock();
				continue;
			}
			if ( tmr->tmr_state == TMR_HAND_UP_REQ ) 			/*�ж��Ƿ��й�������   */
			{
				tmr->tmr_state = TMR_HAND_UP_STATE;              /*���óɹ���״̬*/
				tmr->expires =   tmr->expires - m_timer_array_jiffies;
				continue;
			}
			if ( tmr->tmr_state == TMR_REVERT_REQ ) 			     /*�ж��Ƿ���ȡ����������   */
			{
				timer_member_mod_expires( tmr, tmr->expires );		/*����ʱ��Ա����ʣ��װ��*/
				tmr->tmr_state = TMR_IS_OK;
				continue;
			}
			if (( tmr->expires <= m_timer_array_jiffies )
				&& ( tmr->tmr_state == TMR_IS_OK ) )   /*�ж�ʱ�䶨ʱʱ���Ƿ����*/
			{
				if ( tmr->type == TIMER_AUTO )             /*�жϳ�Ա�����Ƿ�����װ�ؽ���*/
				{
					timer_member_mod_expires
					( tmr,
					  tmr->auto_load_expires );			/*�Զ�װ��ʱ�����*/
				}
				tmr->function( tmr->p_arg );   			/*���ûص�����*/
				if ( tmr->type == TIMER_SETTINGS )       /*�жϳ�Ա�����Ƿ���һ���Զ�ʱʵ��*/
				{
					timer_array_lock();
					global_memset(( uint8_t * )tmr, 0,
								  sizeof( tmr_member_t ) );/*ɾ����ִ�е�ʱ���Ա*/
					m_timer_array.ture_size--;
					timer_array_unlock();
				}
			}
		}
		msleep( 2 );
	}
}
/********************************************************************
 * @������ :�ҳ�
 * @����:��ʱ��������ʼ��
 *
 * @����:
 *
 *@���:NONE
********************************************************************/
void timer_array_mod_init( void )
{
	global_memset(( uint8_t * )&m_timer_array, 0, sizeof( tmr_array_t ) );
	timer_array_lock_creat();
	os_task_assign_pro_build( timer_array_maneger_task, TIMER_ARRAY_TASK_PRO, 2 );
	os_task_name_set( TIMER_ARRAY_TASK_PRO, "Support--Timer array" );
	return;
}
#endif
#endif
