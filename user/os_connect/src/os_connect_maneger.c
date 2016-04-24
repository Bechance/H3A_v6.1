/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统连接层函数
* 创建时间:2010-08-01
* 所属公司 :科荟
* 文件名称:os_connect.c
* 创建人 :		揭成
* 功能描述:操作系统接口函数封装,根据操作
*			系统的不同需要有所修改,做此接口
*			是为了应用层的通用性,当前是应用
*			在UCOS的接口。
*           该模块为应用中任务的创建进行堆栈
*           管理，应用中不用考虑堆栈重复利用
*           问题，避免系统堆栈内存的错乱。
*           堆栈的管理特别是在内存较小的系统是
*           非常必要。
*----------------------------------------------------------------
修改记录:增加task_assign_build
			  目的:可以指定任务优先级

修改日期:7月8日

修改人:   揭成

*********************************************************************************************************
修改记录:测试性能说明


修改日期:9月13日
                      经过测试OSTaskCreateExt 创建任务因为增加了
                      别的参数选项，所以系统占用了更多时间
                      增加%5左右，建议在系统测试中使用
                      OSTaskCreateExt ，在产品中使用 OSTaskCreate
                      由 OS_CFG.h 中的 OS_TASK_CREATE_EXT_EN来决定。
修改人:   揭成
*********************************************************************************************************
修改记录:增加os_task_assign_stk_build，使任务可以进行堆栈
                      控制，当用户在ucshell下检测到堆栈报警后
                      应该合适对堆栈进行调整，来避免堆栈溢出
                      或造成堆栈浪费。
修改日期:9月15日

修改人:   揭成
*********************************************************************************************************
*/
#include 	"../../../kernel\includes\ucos_ii.h"
#include    "../os_connect.h"
#pragma  	DATA_SECTION (m_stk_array,   "MY_MEM")			/*把m_stk_small_array定义在MY_MEM*/
#pragma  	DATA_SECTION (OSTaskIdleStk, "MY_MEM")			/*把OSTaskIdleStk定义在MY_MEM*/
OS_STK   	m_stk_array[MY_TASK_MAX_NUM][ADVISE_STK_SIZE]={0, } ; /*用户应用任务堆栈组*/
OS_STK   	OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE] = {0, };  	/*定义空闲任务堆栈*/
uint8_t		m_task_pro_array[MY_TASK_MAX_NUM+1] ; 	 		/*任务随机分配时，标识当前任务索引*/
Bsize_t     m_event_stat_cnt = 0;							/*事件统计计数器*/
a_lock_t*   m_task_lock = INVALID_LOCK;						/*系统连接层，任务分配锁指针*/
/*********************************************************************************************************
 * @创建人 :揭成
 * @功能: 应用层资源锁，4个API函数
 *
 * @输入:NONE
 *
 * @输出:NONE
 * 	资源锁是在多任务为了防止某部分资源被
 *	同时使用，而定义的一种保护机制，并不是
 *	一种简单的标志区别，它还包含了系统的
 *	任务调度。
 *	希望使用人员能从本质上理解该定义，我们
 *	此处的的资源锁是从UCOS的SEM转化而来。
***********************************************************************************************************/
void  os_lock( a_lock_t*sem )
{
	uint8_t err;
	OSSemPend(( OS_EVENT* )sem, 0, &err );
	return;
}
void os_unlock( a_lock_t*sem )
{
	OSSemPost(( OS_EVENT* )sem );
	return;
}
void os_lock_revert( a_lock_t*sem )
{
	os_sem_revert( sem, 1 );
	return;
}
uint8_t os_lock_creat( a_lock_t**sem )
{
	if (( *sem ) == INVALID_SEM )
	{
		*sem = ( void* )OSSemCreate( 1 );
		if ( *sem == INVALID_SEM )
		{
			//OS_DBGERR( "creat lock fail \n", DLOG_NP, DLOG_NP );
			return LOCK_IS_INVAILD;
		}
		m_event_stat_cnt++;
		return LOCK_IS_CREATE;
	}
	os_lock_revert( *sem );
	return LOCK_IS_REVERT;
}
/********************************************************************
* @创建人 :揭成
* @功能: 自封装信号量创建3个函数，os_sem_post在宏定义中
*
* @输入:NONE
*
*@输出:NONE
********************************************************************/
uint8_t os_sem_creat( a_sem_t**sem, uint16_t cnt )
{
	uint8_t err = 0;
	if (( *sem ) == INVALID_SEM )
	{
		*sem = ( void* )OSSemCreate( cnt );
		if ( *sem == INVALID_SEM )
		{
			//OS_DBGERR( "creat sem fail \n", DLOG_NP, DLOG_NP );
			return SEM_IS_INVAILD;
		}
		m_event_stat_cnt++;
		return SEM_IS_CREATE;
	}
	err = os_sem_revert( *sem, cnt );
	if ( err != OS_ERR_NONE )
	{
		return SEM_IS_INVAILD;
	}
	return SEM_IS_REVERT;
}

uint8_t os_sem_revert( a_sem_t*sem, uint16_t cnt )
{
	uint8_t err = 0;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0;
#endif
	OS_ENTER_CRITICAL();
	OS_EventWaitListInit( sem );
	OS_EXIT_CRITICAL();
	OSSemSet( sem, cnt, &err );
	if ( err != OS_ERR_NONE )
	{
		//OS_DBGERR( "set sem fail err=%b\n", err, DLOG_NP );
	}
	return err;
}
uint8_t os_sem_pend( a_sem_t*sem, uint16_t timeout )
{
	uint8_t err;
	OSSemPend( sem, timeout, &err );
	return err;
}
/********************************************************************
 * @创建人 :揭成
 * @功能: 自封装信号量创建,其余的在宏定义中体现
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void  os_q_creat( a_q_t**sem, void**pbuf, uint16_t size )
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0;
#endif
	if ( *sem == INVALID_SEM )
	{
		*sem = OSQCreate( pbuf, size );
		if ( *sem == INVALID_SEM )
		{
			//OS_DBGERR( "creat queue fail \n", DLOG_NP, DLOG_NP );
		}
		else
		{
			m_event_stat_cnt++;
		}
	}
	else
	{
		OS_ENTER_CRITICAL();
		OS_EventWaitListInit( *sem );
		OS_EXIT_CRITICAL();
	}
	OSQFlush( *sem );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:申请任务堆栈
 *
 * @输入: nblk需要的堆栈块个数
 *
 *@输出:pro 任务优先级,也就是对应的索引号
   注意当任务需要多个堆栈块的时候，
   将占用另外任务的堆栈块，这个时候其任务
   优先级也将被占用。
********************************************************************/
uint8_t os_task_req_pro_array( Bsize_t nblk )
{
	char pro_str[MAX_STK_NBLK];
	char*pstk;
	if ( nblk > ( MAX_STK_NBLK - 1 ) )  /*判断NBLK的合法性*/
	{
		nblk =  MAX_STK_NBLK - 1;
		//OS_DBGERR( "nblk too large,the maxest num or nblk is %d\n", nblk, DLOG_NP );
		return INVALID_PRO;
	}
	os_memset(( uint8_t * ) pro_str, 0, sizeof( char )*MAX_STK_NBLK );
	os_memset(( uint8_t * ) pro_str, PRO_IS_IDLE, sizeof( char )*nblk );/*将要找的内存合成字符串*/
	os_lock( m_task_lock );
	pstk = os_string_locate_first(( char* )m_task_pro_array, pro_str );/*定位字符串*/
	if ( pstk != PNULL )
	{
		( *pstk ) = PRO_IS_MAIN_OCCUPY; 							/*设置优先级对应的PRO为主占*/
		os_memset(( uint8_t* )( pstk + 1 ), PRO_IS_SLAVE_OCCUPY, ( nblk - 1 ) );/*其余为附属*/
		os_unlock( m_task_lock );
		return (( uint8_t* )pstk - m_task_pro_array );/*返回任务优先级别*/
	}
	os_unlock( m_task_lock );
	return INVALID_PRO;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:释放任务优先级别索引
 *
 * @输入: pro 任务优先级
 *
 *@输出:NONE
********************************************************************/
void os_task_release_pro_array( uint8_t pro )
{
	uint8_t offset;
	os_lock( m_task_lock );
	if ( pro == SELF_PRO )
	{
		offset =  OSPrioCur; /*自身删除的任务优先级*/
	}
	else
	{
		offset =  pro;
	}
	m_task_pro_array[offset] = PRO_IS_IDLE;/*释放主占块*/
	offset++;
	while ( m_task_pro_array[offset] == PRO_IS_SLAVE_OCCUPY )
	{
		m_task_pro_array[offset] = PRO_IS_IDLE;  /*释放附属块*/
		offset++;
	}
	os_unlock( m_task_lock );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:建立优先级为PRO的任务
 *
 * @输入:task:任务函数指针,PRO 优先级
 *
 *@输出:TURE 成功 FAIL 失败
********************************************************************/
BOOL  os_task_assign_pro_build( void ( *task )( void *p_arg ), uint8_t pro, Bsize_t nblk )
{
	uint8_t ret;
	char*pstk;
	char pro_str[MAX_STK_NBLK];
	if ( nblk > MAX_STK_NBLK - 1 )  /*判断nblk是否合法*/
	{
		nblk =  MAX_STK_NBLK - 1;
		//OS_DBGERR( "nblk too large,the maxest num or nblk is %d\n", nblk, DLOG_NP );
		return FAIL;
	}
	if ( pro >= MY_TASK_MAX_NUM )  /*判断优先级是否非法*/
	{
		//OS_DBGERR( " INVALID_PRO\n", DLOG_NP, DLOG_NP );
		return FAIL;
	}
	os_memset(( uint8_t * ) pro_str, 0, sizeof( char )*MAX_STK_NBLK );/*清空PROSTK*/
	os_memset(( uint8_t * ) pro_str, PRO_IS_IDLE, nblk );/*设置要找寻的字符串*/
	os_lock( m_task_lock );
	pstk = ( char* ) & m_task_pro_array[pro];
	if ( !os_memcmp(( uint8_t* )pstk, ( uint8_t* )pro_str, nblk ) )/*比较判断堆栈是否被占用*/
	{
		//OS_DBGERR( " Stack is occupied alread..\n", DLOG_NP, DLOG_NP );
		os_unlock( m_task_lock );
		return FAIL;
	}
	( *pstk ) = PRO_IS_MAIN_OCCUPY;
	os_memset(( uint8_t* )( pstk + 1 ), PRO_IS_SLAVE_OCCUPY, ( nblk - 1 ) );
	os_unlock( m_task_lock );
#if OS_TASK_CREATE_EXT_EN >0
	if ( OSTaskCreateExt( task,
						  ( void* )0,
						  ( OS_STK * ) &( m_stk_array[pro][0] ),
						  pro,
						  pro,
						  ( OS_STK * ) &( m_stk_array[pro+nblk-1][ADVISE_STK_SIZE-1] ),
						  ( ADVISE_STK_SIZE*nblk ),
						  ( void* )0,
						  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR )
		 == OS_ERR_NONE )/*在该优先级上创建任务*/
#else
	if ( OSTaskCreate( task, ( void* )0,
					   ( OS_STK * )&m_stk_array[pro],
					   pro )
		 == OS_ERR_NONE )
#endif
	{
		ret = TURE;
	}
	else
	{
		ret = FAIL;
		//OS_DBGERR( "the pro=%b task build fail...!\n", pro, DLOG_NP );
	}
	return ret;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:随机建立堆栈指定的任务
 *
 * @输入:task:任务函数指针
 *
 *@输出:INVALID_PRO :建立失败，如果成功返回任务优先级别
********************************************************************/
uint8_t os_task_assign_stk_build( void ( *task )( void *p_arg ) , Bsize_t nblk )
{
	Bsize_t index;
	uint8_t ret;
	index = os_task_req_pro_array( nblk );/*申请空兴饕?/
	if ( index == INVALID_PRO )
	{
		//OS_DBGERR( "task req stack fail...\n", DLOG_NP, DLOG_NP );
		return INVALID_PRO;
	}
#if OS_TASK_CREATE_EXT_EN >0
	ret = OSTaskCreateExt( task,
						   ( void* )0,
						   ( OS_STK * ) & ( m_stk_array[index][0] ),
						   index,
						   index,
						   ( OS_STK * ) & ( m_stk_array[index+nblk-1][ADVISE_STK_SIZE-1] ),
						   ( ADVISE_STK_SIZE * nblk ),
						   ( void* )0,
						   OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR );/*在该优先级上创建任务*/
#else
	ret = OSTaskCreate( task,
						( void* )0,
						( OS_STK * ) & m_stk_array[index],
						index );/*在该优先级上创建任务*/
#endif
	if ( ret == OS_ERR_NONE )
	{
		return ( index );/*返回优先级*/
	}
	else
	{
		//OS_DBGERR( "build pro=%d task fail....err=%b \n", index, ret );
		return INVALID_PRO;
	}
}
/********************************************************************
 * @创建人 :揭成
 * @功能:随机建立任务,一个用户建议堆栈
 *
 * @输入:task:任务函数指针
 *
 *@输出:INVALID_PRO :建立失败，如果成功返回任务优先级别
********************************************************************/
uint8_t os_task_random_build( void ( *task )( void *p_arg ) )
{
	return os_task_assign_stk_build( task, 2 );
}
/********************************************************************
* @创建人 :揭成
* @功能:自封装任务删除
*
* @输入:task:任务函数指针 pro 任务优先级
*
* @输出:
* Returns    :
*		   OS_ERR_NONE             	if the call is successful
*              OS_ERR_TASK_DEL_IDLE   if you attempted to delete uC/OS-II's idle task
*              OS_ERR_PRIO_INVALID     if the priority you specify is higher that the maximum allowed
*                                      (i.e. >= OS_LOWEST_PRIO) or, you have not specified OS_PRIO_SELF.
*              OS_ERR_TASK_DEL         	if the task is assigned to a Mutex PIP.
*              OS_ERR_TASK_NOT_EXIST   if the task you want to delete does not exist.
*              OS_ERR_TASK_DEL_ISR     if you tried to delete a task from an ISR
********************************************************************/
uint8_t os_task_delete( uint8_t pro )
{
	uint8_t ret;
	if ( pro >= 60 && pro <= 63 )
	{
		//OS_DBGERR( "you can not delete os task..pro=%d\n", pro, DLOG_NP );
		return 0;
	}
	os_task_release_pro_array( pro );
	ret = OSTaskDel( pro );
#if DEBUG_LOG_EN >0
	if ( ret != OS_ERR_NONE )
	{
		//OS_DBGERR( "delete pro =%b fail... err=%b\n", pro, ret );
	}
#endif
	return ret;
}

/********************************************************************
 * @创建人 :揭成
 * @功能:删除所有用户任务，调用该函数的任务除外
 *
 * @输入:NONE
 *
 * @输出:NONE
********************************************************************/
void  os_task_delete_all( void )
{
	Bsize_t index;
	for ( index = 0;index < MY_TASK_MAX_NUM;index++ )
	{
		if ( m_task_pro_array[index] == PRO_IS_IDLE )
		{
			continue;
		}
		else if ( index == OSPrioCur )
		{
			continue;
		}
		else if ( m_task_pro_array[index] == PRO_IS_MAIN_OCCUPY )
		{
			os_task_delete( index );
		}
	}
	return;
}
/********************************************************************
* @创建人 :揭成
* @功能:建立优先级为PRO的任务
*
* @输入:PRO 优先级,pname  任务名称
*
* @输出:		OS_ERR_NONE                if the requested task is resumed
*               OS_ERR_TASK_NOT_EXIST      if the task has not been created or is assigned to a Mutex
*               OS_ERR_TASK_NAME_TOO_LONG  if the name you are giving to the task exceeds the
*                                                   storage capacity of a task name as specified by
*                                                   OS_TASK_NAME_SIZE.
*               OS_ERR_PNAME_NULL          You passed a NULL pointer for 'pname'
*               OS_ERR_PRIO_INVALID        if you specified an invalid priority:
*                                                   A higher value than the idle task or not OS_PRIO_SELF.
*               OS_ERR_NAME_SET_ISR        if you called this function from an ISR
*
********************************************************************/
uint8_t os_task_name_set( uint8_t pro, char*pname )
{
#if OS_TASK_NAME_SIZE > 1
	uint8_t err;
	OSTaskNameSet( pro, ( uint8_t* )pname, &err );
	if ( err != OS_ERR_NONE )
	{
		//OS_DBGERR( "the task = %b name set fail,the err id =%b \n", pro, err );
	}
	return err;
#else
	return 0;
#endif
}
/********************************************************************
 * @创建人 :揭成
 * @功能:建立优先级为PRO的任务
 *
 * @输入:PRO 优先级,pname  任务名称
 *
 *@输出: 名称字符串长度

**********************************************************************/
uint8_t os_task_name_get( uint8_t pro, char*pname )
{
#if OS_TASK_NAME_SIZE > 1
	uint8_t len;
	uint8_t err;
	len = OSTaskNameGet( pro, ( uint8_t* )pname, &err );
	if ( err != OS_ERR_NONE )
	{
		//OS_DBGERR( "the task = %b name get fail,the err id =%b \n", pro, err );
	}
	else if ( len == 0 )
	{
		//OS_DBGERR( "task name lenth is unlawful", DLOG_NP, DLOG_NP );
	}
	return len;
#else
	return 0;
#endif
}
/********************************************************************
 * @创建人 :揭成
 * @功能: 系统连接层初始化
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void app_module_maneger_init( void );
void os_connect_init( void )
{
	os_memset(( uint8_t * ) m_task_pro_array, PRO_IS_IDLE, MY_TASK_MAX_NUM );
	m_task_pro_array[MY_TASK_MAX_NUM] = ASCII_NULL;
	os_lock_creat( &m_task_lock );
	m_event_stat_cnt = 0;
	os_driver_maneger_tab_init();	/*初始化驱动管理列?/
	app_module_maneger_init(); 		/*初始化模块管理列表*/
	return;
}
