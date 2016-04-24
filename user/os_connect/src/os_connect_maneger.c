/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:ϵͳ���Ӳ㺯��
* ����ʱ��:2010-08-01
* ������˾ :����
* �ļ�����:os_connect.c
* ������ :		�ҳ�
* ��������:����ϵͳ�ӿں�����װ,���ݲ���
*			ϵͳ�Ĳ�ͬ��Ҫ�����޸�,���˽ӿ�
*			��Ϊ��Ӧ�ò��ͨ����,��ǰ��Ӧ��
*			��UCOS�Ľӿڡ�
*           ��ģ��ΪӦ��������Ĵ������ж�ջ
*           ����Ӧ���в��ÿ��Ƕ�ջ�ظ�����
*           ���⣬����ϵͳ��ջ�ڴ�Ĵ��ҡ�
*           ��ջ�Ĺ����ر������ڴ��С��ϵͳ��
*           �ǳ���Ҫ��
*----------------------------------------------------------------
�޸ļ�¼:����task_assign_build
			  Ŀ��:����ָ���������ȼ�

�޸�����:7��8��

�޸���:   �ҳ�

*********************************************************************************************************
�޸ļ�¼:��������˵��


�޸�����:9��13��
                      ��������OSTaskCreateExt ����������Ϊ������
                      ��Ĳ���ѡ�����ϵͳռ���˸���ʱ��
                      ����%5���ң�������ϵͳ������ʹ��
                      OSTaskCreateExt ���ڲ�Ʒ��ʹ�� OSTaskCreate
                      �� OS_CFG.h �е� OS_TASK_CREATE_EXT_EN��������
�޸���:   �ҳ�
*********************************************************************************************************
�޸ļ�¼:����os_task_assign_stk_build��ʹ������Խ��ж�ջ
                      ���ƣ����û���ucshell�¼�⵽��ջ������
                      Ӧ�ú��ʶԶ�ջ���е������������ջ���
                      ����ɶ�ջ�˷ѡ�
�޸�����:9��15��

�޸���:   �ҳ�
*********************************************************************************************************
*/
#include 	"../../../kernel\includes\ucos_ii.h"
#include    "../os_connect.h"
#pragma  	DATA_SECTION (m_stk_array,   "MY_MEM")			/*��m_stk_small_array������MY_MEM*/
#pragma  	DATA_SECTION (OSTaskIdleStk, "MY_MEM")			/*��OSTaskIdleStk������MY_MEM*/
OS_STK   	m_stk_array[MY_TASK_MAX_NUM][ADVISE_STK_SIZE]={0, } ; /*�û�Ӧ�������ջ��*/
OS_STK   	OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE] = {0, };  	/*������������ջ*/
uint8_t		m_task_pro_array[MY_TASK_MAX_NUM+1] ; 	 		/*�����������ʱ����ʶ��ǰ��������*/
Bsize_t     m_event_stat_cnt = 0;							/*�¼�ͳ�Ƽ�����*/
a_lock_t*   m_task_lock = INVALID_LOCK;						/*ϵͳ���Ӳ㣬���������ָ��*/
/*********************************************************************************************************
 * @������ :�ҳ�
 * @����: Ӧ�ò���Դ����4��API����
 *
 * @����:NONE
 *
 * @���:NONE
 * 	��Դ�����ڶ�����Ϊ�˷�ֹĳ������Դ��
 *	ͬʱʹ�ã��������һ�ֱ������ƣ�������
 *	һ�ּ򵥵ı�־��������������ϵͳ��
 *	������ȡ�
 *	ϣ��ʹ����Ա�ܴӱ��������ö��壬����
 *	�˴��ĵ���Դ���Ǵ�UCOS��SEMת��������
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
* @������ :�ҳ�
* @����: �Է�װ�ź�������3��������os_sem_post�ں궨����
*
* @����:NONE
*
*@���:NONE
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
 * @������ :�ҳ�
 * @����: �Է�װ�ź�������,������ں궨��������
 *
 * @����:NONE
 *
 *@���:NONE
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
 * @������ :�ҳ�
 * @����:���������ջ
 *
 * @����: nblk��Ҫ�Ķ�ջ�����
 *
 *@���:pro �������ȼ�,Ҳ���Ƕ�Ӧ��������
   ע�⵱������Ҫ�����ջ���ʱ��
   ��ռ����������Ķ�ջ�飬���ʱ��������
   ���ȼ�Ҳ����ռ�á�
********************************************************************/
uint8_t os_task_req_pro_array( Bsize_t nblk )
{
	char pro_str[MAX_STK_NBLK];
	char*pstk;
	if ( nblk > ( MAX_STK_NBLK - 1 ) )  /*�ж�NBLK�ĺϷ���*/
	{
		nblk =  MAX_STK_NBLK - 1;
		//OS_DBGERR( "nblk too large,the maxest num or nblk is %d\n", nblk, DLOG_NP );
		return INVALID_PRO;
	}
	os_memset(( uint8_t * ) pro_str, 0, sizeof( char )*MAX_STK_NBLK );
	os_memset(( uint8_t * ) pro_str, PRO_IS_IDLE, sizeof( char )*nblk );/*��Ҫ�ҵ��ڴ�ϳ��ַ���*/
	os_lock( m_task_lock );
	pstk = os_string_locate_first(( char* )m_task_pro_array, pro_str );/*��λ�ַ���*/
	if ( pstk != PNULL )
	{
		( *pstk ) = PRO_IS_MAIN_OCCUPY; 							/*�������ȼ���Ӧ��PROΪ��ռ*/
		os_memset(( uint8_t* )( pstk + 1 ), PRO_IS_SLAVE_OCCUPY, ( nblk - 1 ) );/*����Ϊ����*/
		os_unlock( m_task_lock );
		return (( uint8_t* )pstk - m_task_pro_array );/*�����������ȼ���*/
	}
	os_unlock( m_task_lock );
	return INVALID_PRO;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:�ͷ��������ȼ�������
 *
 * @����: pro �������ȼ�
 *
 *@���:NONE
********************************************************************/
void os_task_release_pro_array( uint8_t pro )
{
	uint8_t offset;
	os_lock( m_task_lock );
	if ( pro == SELF_PRO )
	{
		offset =  OSPrioCur; /*����ɾ�����������ȼ�*/
	}
	else
	{
		offset =  pro;
	}
	m_task_pro_array[offset] = PRO_IS_IDLE;/*�ͷ���ռ��*/
	offset++;
	while ( m_task_pro_array[offset] == PRO_IS_SLAVE_OCCUPY )
	{
		m_task_pro_array[offset] = PRO_IS_IDLE;  /*�ͷŸ�����*/
		offset++;
	}
	os_unlock( m_task_lock );
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @����:�������ȼ�ΪPRO������
 *
 * @����:task:������ָ��,PRO ���ȼ�
 *
 *@���:TURE �ɹ� FAIL ʧ��
********************************************************************/
BOOL  os_task_assign_pro_build( void ( *task )( void *p_arg ), uint8_t pro, Bsize_t nblk )
{
	uint8_t ret;
	char*pstk;
	char pro_str[MAX_STK_NBLK];
	if ( nblk > MAX_STK_NBLK - 1 )  /*�ж�nblk�Ƿ�Ϸ�*/
	{
		nblk =  MAX_STK_NBLK - 1;
		//OS_DBGERR( "nblk too large,the maxest num or nblk is %d\n", nblk, DLOG_NP );
		return FAIL;
	}
	if ( pro >= MY_TASK_MAX_NUM )  /*�ж����ȼ��Ƿ�Ƿ�*/
	{
		//OS_DBGERR( " INVALID_PRO\n", DLOG_NP, DLOG_NP );
		return FAIL;
	}
	os_memset(( uint8_t * ) pro_str, 0, sizeof( char )*MAX_STK_NBLK );/*���PROSTK*/
	os_memset(( uint8_t * ) pro_str, PRO_IS_IDLE, nblk );/*����Ҫ��Ѱ���ַ���*/
	os_lock( m_task_lock );
	pstk = ( char* ) & m_task_pro_array[pro];
	if ( !os_memcmp(( uint8_t* )pstk, ( uint8_t* )pro_str, nblk ) )/*�Ƚ��ж϶�ջ�Ƿ�ռ��*/
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
		 == OS_ERR_NONE )/*�ڸ����ȼ��ϴ�������*/
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
 * @������ :�ҳ�
 * @����:���������ջָ��������
 *
 * @����:task:������ָ��
 *
 *@���:INVALID_PRO :����ʧ�ܣ�����ɹ������������ȼ���
********************************************************************/
uint8_t os_task_assign_stk_build( void ( *task )( void *p_arg ) , Bsize_t nblk )
{
	Bsize_t index;
	uint8_t ret;
	index = os_task_req_pro_array( nblk );/*���������?/
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
						   OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR );/*�ڸ����ȼ��ϴ�������*/
#else
	ret = OSTaskCreate( task,
						( void* )0,
						( OS_STK * ) & m_stk_array[index],
						index );/*�ڸ����ȼ��ϴ�������*/
#endif
	if ( ret == OS_ERR_NONE )
	{
		return ( index );/*�������ȼ�*/
	}
	else
	{
		//OS_DBGERR( "build pro=%d task fail....err=%b \n", index, ret );
		return INVALID_PRO;
	}
}
/********************************************************************
 * @������ :�ҳ�
 * @����:�����������,һ���û������ջ
 *
 * @����:task:������ָ��
 *
 *@���:INVALID_PRO :����ʧ�ܣ�����ɹ������������ȼ���
********************************************************************/
uint8_t os_task_random_build( void ( *task )( void *p_arg ) )
{
	return os_task_assign_stk_build( task, 2 );
}
/********************************************************************
* @������ :�ҳ�
* @����:�Է�װ����ɾ��
*
* @����:task:������ָ�� pro �������ȼ�
*
* @���:
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
 * @������ :�ҳ�
 * @����:ɾ�������û����񣬵��øú������������
 *
 * @����:NONE
 *
 * @���:NONE
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
* @������ :�ҳ�
* @����:�������ȼ�ΪPRO������
*
* @����:PRO ���ȼ�,pname  ��������
*
* @���:		OS_ERR_NONE                if the requested task is resumed
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
 * @������ :�ҳ�
 * @����:�������ȼ�ΪPRO������
 *
 * @����:PRO ���ȼ�,pname  ��������
 *
 *@���: �����ַ�������

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
 * @������ :�ҳ�
 * @����: ϵͳ���Ӳ��ʼ��
 *
 * @����:NONE
 *
 *@���:NONE
********************************************************************/
void app_module_maneger_init( void );
void os_connect_init( void )
{
	os_memset(( uint8_t * ) m_task_pro_array, PRO_IS_IDLE, MY_TASK_MAX_NUM );
	m_task_pro_array[MY_TASK_MAX_NUM] = ASCII_NULL;
	os_lock_creat( &m_task_lock );
	m_event_stat_cnt = 0;
	os_driver_maneger_tab_init();	/*��ʼ������������?/
	app_module_maneger_init(); 		/*��ʼ��ģ������б�*/
	return;
}
