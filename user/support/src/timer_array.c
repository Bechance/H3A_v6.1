/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:定时队列管理器
* 完成时间2010-07-22
* 所属公司 :科荟
* 文件名称:timer_array.c
* 创建人 :揭成
* 功能描述:创建定时列表，处理定时相关事件
*                       新构架 区别于原构架，应用双向链表
*                       来分配控制块，要求C编译器提供内存
*                       管理函数，malloc ()    free()
*-------------------------------------------------------------------
修改记录: 重新构架定时管理模块，具有更优越的
                       结构，没有定时成员个数的限制。

修改日期:2010-7-22

修改人:揭成
*********************************************************************************************************
请不要在中断中调用该模块API接口
*/
#include "../app_support.h"
#if TIMER_ARRAY_EN>0
/*定时队列锁函数*/
#if (COMPILER_MALLOC_SUPPORT >0) &&(LINK_LIST_MANEGER_EN >0)
uint32_t  volatile 	m_timer_array_jiffies = 0;
link_list_tcb_t*	m_tmr_link_tcb;
/********************************************************************
 * @创建人 :揭成
 * @功能:创建一个新成员
 * @输入: need_jiffies 定时间隔
 *		  type       成员类型
 *		  caller     回调函数
 *        p_arg      回调函数参数
 * @输出:成员位置
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
 * @创建人:揭成
 * @功能:重置定时节拍
 *
 * @输入:溢出时间
 *
 * @输出:NONE
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
 * @创建人:揭成
 * @功能:定时成员，定时节拍更改
 *
 * @输入:identity  over_time
 *
 * @输出:NONE
********************************************************************/
void timer_member_mod_expires( tmr_member_t*tmr,
							   uint32_t over_time )
{
	tmr->expires = m_timer_array_jiffies + over_time;
	if ( tmr->expires < m_timer_array_jiffies ) /*判断节拍是不是需要重置*/
	{
		timer_member_revert_expires( over_time );
		tmr->expires -= over_time;
	}
	return;
}

/********************************************************************
 * @创建人:揭成
 * @功能:创建一个新成员
 * @输入: tmr       成员指针
 *		  state     要设置的状态，
                    TMR_HAND_UP_REQ 挂起
				    TMR_IS_OK       正常
					TMR_DELETE_REQ  删除
					TMR_REVERT_REQ  取消挂起
 * @输出:NONE
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
 * @创建人 :揭成
 * @功能:定时基准节拍自加
 *
 * @输入: NONE

 *@输出:NONE
********************************************************************/
void timer_jiffies_add( void )
{
	m_timer_array_jiffies++;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:定时管理器主任务
 *
 * @输入:
 *
 *@输出:NONE
********************************************************************/
void timer_array_maneger_task( void*p_arg )
{
	link_list_t *		tmr_cur_link ;
	link_list_t *		tmr_link_shadow ;
	tmr_member_t *	tmr;
	while ( 1 )
	{
		tmr_cur_link = m_tmr_link_tcb->link_list_end ; 			                   /*得到定时器链表尾*/
		while ( tmr_cur_link )
		{
			tmr = ( tmr_member_t* )tmr_cur_link->cur_member;     				 /*得到定时器成员位置*/
			if ( tmr->tmr_state == TMR_DELETE_REQ )		      					/*判断是否有删除请求   */
			{
				tmr_link_shadow = tmr_cur_link; 								/*做一次保存避免空间释放时候出错*/
				tmr_cur_link = tmr_cur_link->last_member;                          		  /*移动链表指向下一个成员*/
				link_list_memeber_destory( m_tmr_link_tcb, tmr_link_shadow ); /*删除请求成员*/
				continue;
			}
			if ( tmr->tmr_state == TMR_HAND_UP_REQ ) 						/*判断是否有挂起请求   */
			{
				tmr->expires = tmr->expires - m_timer_array_jiffies;                     	 	/*挂起时成员节拍剩余时间保存 */
				tmr->tmr_state   = TMR_HAND_UP_STATE;
				tmr_cur_link = tmr_cur_link->last_member;                        		/*移动链表指向下一个成员*/
				continue;
			}
			if ( tmr->tmr_state == TMR_REVERT_REQ ) 			     			/*判断是否有取消挂起请求   */
			{
				timer_member_mod_expires( tmr, tmr->expires );			/*挂起时成员节拍剩余装载*/
				tmr->tmr_state = TMR_IS_OK;
				tmr_cur_link = tmr_cur_link->last_member;    					/*移动链表指向下一个成员*/
				continue;
			}
			if ( tmr->expires <= m_timer_array_jiffies )                       					/*判断时间定时时间是否溢出*/
			{
				if ( tmr->type == TIMER_AUTO )                					/*判断成员类型是否是自装载节拍*/
				{
					timer_member_mod_expires
					( tmr,
					  tmr->auto_load_expires );								/*自动装载时间节拍*/
					tmr_cur_link = tmr_cur_link->last_member;                    	/*移动链表指向下一个成员*/
				}
				tmr->function( tmr->p_arg );   								/*调用回调函数*/
				if ( tmr->type == TIMER_SETTINGS )               				/*判断成员类型是否是一次性定时实件*/
				{
					tmr_link_shadow = tmr_cur_link;				           		/*做一次保存避免空间释放时候出错*/
					tmr_cur_link = tmr_cur_link->last_member;                  		/*移动链表指向下一个成员*/
					link_list_memeber_destory( m_tmr_link_tcb, tmr_link_shadow );/*删除已执行的时间成员*/
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
 * @创建人 :揭成
 * @功能:定时管理器初始化
 *
 * @输入:
 *
 *@输出:NONE
********************************************************************/
void timer_array_mod_init( void )
{
	m_tmr_link_tcb = link_list_tcb_req();
	os_task_assign_pro_build( timer_array_maneger_task, TIMER_ARRAY_TASK_PRO, 2 );
	os_task_name_set( TIMER_ARRAY_TASK_PRO, "Support--Timer array" );
	return;
}
#else   
/*不提供malloc 支持模块写法*/
/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:定时队列管理器
* 完成时间2010-07-15
* 所属公司 :科荟
* 文件名称:timer_array.c
* 创建人 :揭成
* 功能描述:创建定时列表，处理定时相关事件
*-------------------------------------------------------------------
修改记录: tmr_member_t 中增加VOID*arg,回调函数参数
目的:     是回调函数可以带参数传递

修改日期:2010-7-19

修改人:揭成
*********************************************************************************************************
*/
uint32_t  volatile 	m_timer_array_jiffies = 0;

tmr_array_t   		m_timer_array = {0, };

a_lock_t *		    m_timer_lock = INVALID_SEM;

/*定时队列资源锁接口函数，在多任务是使用，如果是单任务系统不予考虑*/
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
 * @创建人 :揭成
 * @功能:创建一个新成员
 *
 * @输入: need_jiffies 定时间隔
 *			type       成员类型
 *			caller     回调函数
 *          p_arg      回调函数参数
 *@输出:成员位置
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
 * @创建人 :揭成
 * @功能:重置定时节拍
 *
 * @输入:溢出时间
 *
 *@输出:NONE
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
 * @创建人 :揭成
 * @功能:定时成员，定时节拍更改
 *
 * @输入:identity  over_time
 *
 *@输出:NONE
********************************************************************/
void timer_member_mod_expires( tmr_member_t*tmr,
							   uint32_t over_time )
{
	tmr->expires = m_timer_array_jiffies + over_time;
	if ( tmr->expires < m_timer_array_jiffies ) /*判断节拍是不是需要重置*/
	{
		timer_member_revert_expires( over_time );
		tmr->expires -= over_time;
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:创建一个新成员
 *
 * @输入: tmr            成员指针
 *			state         要设置的状态，
                                  	TMR_HAND_UP_REQ     挂起
						TMR_IS_OK          正常
						TMR_DELETE_REQ  删除
						TMR_REVERT_REQ  取消挂起
 *@输出:NONE
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
 * @创建人 :揭成
 * @功能:定时基准节拍自加
 *
 * @输入: NONE

 *@输出:NONE
********************************************************************/
void timer_jiffies_add( void )
{
	m_timer_array_jiffies++;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:定时管理器主任务
 *
 * @输入:
 *
 *@输出:NONE
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
			if ( tmr->tmr_state == TMR_DELETE_REQ )		      /*判断是否有删除请求   */
			{
				timer_array_lock();
				global_memset(( uint8_t * ) tmr, 0, sizeof( tmr_member_t ) );
				m_timer_array.ture_size--;
				timer_array_unlock();
				continue;
			}
			if ( tmr->tmr_state == TMR_HAND_UP_REQ ) 			/*判断是否有挂起请求   */
			{
				tmr->tmr_state = TMR_HAND_UP_STATE;              /*设置成挂起状态*/
				tmr->expires =   tmr->expires - m_timer_array_jiffies;
				continue;
			}
			if ( tmr->tmr_state == TMR_REVERT_REQ ) 			     /*判断是否有取消挂起请求   */
			{
				timer_member_mod_expires( tmr, tmr->expires );		/*挂起时成员节拍剩余装载*/
				tmr->tmr_state = TMR_IS_OK;
				continue;
			}
			if (( tmr->expires <= m_timer_array_jiffies )
				&& ( tmr->tmr_state == TMR_IS_OK ) )   /*判断时间定时时间是否溢出*/
			{
				if ( tmr->type == TIMER_AUTO )             /*判断成员类型是否是自装载节拍*/
				{
					timer_member_mod_expires
					( tmr,
					  tmr->auto_load_expires );			/*自动装载时间节拍*/
				}
				tmr->function( tmr->p_arg );   			/*调用回调函数*/
				if ( tmr->type == TIMER_SETTINGS )       /*判断成员类型是否是一次性定时实件*/
				{
					timer_array_lock();
					global_memset(( uint8_t * )tmr, 0,
								  sizeof( tmr_member_t ) );/*删除已执行的时间成员*/
					m_timer_array.ture_size--;
					timer_array_unlock();
				}
			}
		}
		msleep( 2 );
	}
}
/********************************************************************
 * @创建人 :揭成
 * @功能:定时管理器初始化
 *
 * @输入:
 *
 *@输出:NONE
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
