/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:	系统模块管理单元
* 创建时间:	2010-08-02
* 所属公司:	科荟
* 文件名称:	app_mod_maneger.c
* 创建人 :		揭成
* 功能描述:管理系统中创建的模块
*----------------------------------------------------------------
修改记录: 在模块切换中增加ID检测

修改目的: 为了避免在切换时进入不存在的模块

修改日期: 2010-08-21

修改人  : 揭成

*********************************************************************************************************
修改记录:在模块切换中增加模块名称

修改目的:增加名称切换  

修改日期:2013-05-21

修改人  :揭成
*********************************************************************************************************/
#include 		"../../../kernel\includes\ucos_ii.h"
#include        "../os_connect.h"

char         				m_mod_pos_name[20] = "";
app_mod_maneger_t 			m_mod_maneger_array ;               /*模块管理列表*/
app_mod_handle_msg_t 		m_mod_hmsg_box[MAX_HMSG_SIZE];  	/*消息队列指针缓冲*/
void *	      				m_mod_hqmsg_buf[MAX_HMSG_SIZE];		/*消息队列内容缓冲*/
a_q_t *  					m_mod_hmsg_qevent = INVALID_SEM;    /*定义消息对立事件标志*/
a_sem_t *  					m_mod_hmsg_sem 	= INVALID_SEM;      /*定义获取消息空间资源锁*/
Bsize_t 					m_last_mod_id 	= INVALID_MOD_ID;	/*上一次模块ID*/
Bsize_t 					m_cur_mod_id 	= INVALID_MOD_ID;	/*当前模块ID*/
Bsize_t                     m_reboot_flag 	= APP_CMD_REBOOT;   /*模块管理退出标志--复位标志*/


#define  GET_MOD_POS(val)  ( &(m_mod_maneger_array.mod_inlet_list[(val)-1]))

void app_module_start_hook( void*arg )
{
	return;
}
void app_module_stop_hook( void*arg )
{
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 启动ID为 app_mod_id的模块
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_module_start( Bsize_t app_mod_id )
{
	app_mod_inlet_t*p_temp = GET_MOD_POS( app_mod_id );
	if ( p_temp->mod_id == app_mod_id )
	{
		m_cur_mod_id  = app_mod_id;
		if ( p_temp->insmod_fun )
		{
			p_temp->insmod_fun();
		}
		app_module_start_hook( 0 );
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :退出ID为 app_mod_id的模块
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
void app_module_exit( Bsize_t app_mod_id )
{
	app_mod_inlet_t*p_temp = GET_MOD_POS( app_mod_id );
	if ( p_temp->mod_id == app_mod_id )
	{
		if ( p_temp->exitmod_fun )
		{
			p_temp->exitmod_fun();
		}
		app_module_stop_hook( 0 );
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :模块切换
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_module_switch( Bsize_t new_mod_id, Bsize_t old_mod_id )
{
	app_mod_inlet_t*p_temp = GET_MOD_POS( new_mod_id );
    if(new_mod_id==0||old_mod_id==0) return;
	if ( p_temp->mod_id != new_mod_id )
	{
		return;
	}
	p_temp = GET_MOD_POS( old_mod_id );
	if ( p_temp->mod_id != old_mod_id )
	{
		return;
	}
	app_module_exit( old_mod_id );
	app_module_start( new_mod_id );
	m_last_mod_id = old_mod_id;
	m_cur_mod_id  = new_mod_id;
	return;
}


/********************************************************************
 * @创建人 :揭成
 * @功能 :将模块添加到管理列表 并返回ID
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
Bsize_t app_module_add( app_mod_inlet_t*tp_ptr )
{
	Bsize_t index;
	app_mod_inlet_t*temp;
	Bsize_t max;
	max = max;
	for ( index = 0;index < MAX_MOD_NUM;index++ )
	{
		if ( m_mod_maneger_array.mod_inlet_list[index].mod_id == IDLE )
		{
			m_mod_maneger_array.real_mod_num ++;
			temp = &( m_mod_maneger_array.mod_inlet_list[index] );
			temp->mod_id      = ( index + 1 );
			temp->insmod_fun  = tp_ptr->insmod_fun;
			temp->exitmod_fun = tp_ptr->exitmod_fun;
			OS_MEMCLR(temp->name, MAX_MOD_NAME);
			os_strcpy(temp->name, tp_ptr->name);
			return ( index + 1 );
		}
	}
	max = MAX_MOD_NUM;
	return 0;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :在列表中将ID为mod_id 的模块删除
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
void app_module_delete( Bsize_t mod_id )
{
	app_mod_inlet_t*p_temp = GET_MOD_POS( mod_id );
	if ( p_temp->mod_id == mod_id )
	{
		os_memset(( uint8_t * )p_temp,0,sizeof( app_mod_inlet_t ) );
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :得到装消息的空间
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
app_mod_handle_msg_t*app_module_get_qmsg_box( void )
{
	uint16_t index;
	for ( index = 0;index < MAX_HMSG_SIZE;index++ )
	{
		if ( m_mod_hmsg_box[index].occupy_flag == IDLE )
		{
			m_mod_hmsg_box[index].occupy_flag = BUSY;
			return &( m_mod_hmsg_box[index] );
		}
	}
	return 0;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :发送模块状态处理消息
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
void app_module_send_hmsg(Bsize_t msg_type,Bsize_t start_mod_id,Bsize_t stop_mod_id )
{
	app_mod_handle_msg_t*msgptr = 0;
	while ( 1 )
	{
		if ( os_sem_pend( m_mod_hmsg_sem, TYPE_TIME_OUT )
			 == OS_NO_ERR )
		{
			msgptr =  app_module_get_qmsg_box();
			os_sem_post( m_mod_hmsg_sem );
			if ( msgptr )
			{
				msgptr->msg_type = msg_type;
				msgptr->start_mod_id = start_mod_id;
				msgptr->stop_mod_id = stop_mod_id;
				os_q_post( m_mod_hmsg_qevent, ( void* )msgptr );
				break;
			}
			msleep( 1 );
		}
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 切换到上一个模块
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
void app_module_switch_last_mod( void )
{
	if ( m_last_mod_id == m_cur_mod_id )
		return;	
	if ( m_last_mod_id == 0 )
		return;
	app_module_send_hmsg( APP_MOD_SWTICH,m_last_mod_id,m_cur_mod_id );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 切换到指定模块
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
void app_module_assign_mod( Bsize_t start_mod_id )
{
	app_module_send_hmsg( APP_MOD_SWTICH,start_mod_id, m_cur_mod_id );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :获取模块ID
 * @输入 :
 * @输出 :NONE
********************************************************************/
Bsize_t app_module_ID_obtain(const char*name)
{
	uint8_t index;
	app_mod_inlet_t*temp;
	for ( index = 0;index < MAX_MOD_NUM;index++ )
	{
		if ( m_mod_maneger_array.mod_inlet_list[index].mod_id != IDLE )
		{
			temp = &( m_mod_maneger_array.mod_inlet_list[index]);
            if(os_strlen((char*)name)!=os_strlen(temp->name))
                continue;
		    if(!os_memcmp(temp->name,(char*)name,os_strlen((char*)name)))
				continue;
			return (index+1);
		}
	}
	return 0 ;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :设置模块名称
 * @输入 :
 * @输出 :NONE
********************************************************************/
void app_module_set_name(Bsize_t id,const char*name)
{
	app_mod_inlet_t*p_temp = GET_MOD_POS( id );
    OS_MEMCLR(p_temp->name, MAX_MOD_NAME);
	os_strcpy(p_temp->name,(char*)name);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :模块切换
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_module_assignExt(const char*name)
{
	Bsize_t ID;
	ID = app_module_ID_obtain(name);
	if(ID!=0)app_module_assign_mod(ID);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 系统软复位
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
void app_module_maneger_exit( Bsize_t flag )
{
	m_reboot_flag = flag;
	app_module_send_hmsg( APP_MOD_MANEGER_EXIT, 0, 0 );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :发送模块状态处理消息
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
void app_module_state_handle( void*arg )
{
	app_mod_handle_msg_t *msg_ptr;
	uint8_t err;
	while ( 1 )
	{
		msg_ptr = ( app_mod_handle_msg_t* )os_q_pend(
					  m_mod_hmsg_qevent,
					  APP_100_MS, &err );                                           /*等待各个模块发送模块处理消息*/
		if ( err == OS_NO_ERR )
		{
			if ( msg_ptr->msg_type == APP_MOD_START )
			{
				app_module_start( msg_ptr->start_mod_id ); /*收到模块启动命令*/
			}
			else if ( msg_ptr->msg_type == APP_MOD_EXIT )
			{
				app_module_exit( msg_ptr->stop_mod_id ); /*收到模块退出命令*/
			}
			else if ( msg_ptr->msg_type == APP_MOD_SWTICH )
			{
				app_module_switch( msg_ptr->start_mod_id,
								   msg_ptr->stop_mod_id );  /*收到模块切换命令*/
			}
			else if ( msg_ptr->msg_type == APP_MOD_MANEGER_EXIT ) /*收到模块退出命令*/
			{
				break;
			}
			msg_ptr->occupy_flag = IDLE;
		}
	}
}

/********************************************************************
 * @创建人 :揭成
 * @功能   :获取当前模块名称
 * @输入   :
 * @输出   :NONE
********************************************************************/
void app_module_CURmod_OB( char*name )
{
	uint8_t index;
	app_mod_inlet_t*temp;
	for ( index = 0;index < MAX_MOD_NUM;index++ )
	{
		if ( m_mod_maneger_array.mod_inlet_list[index].mod_id != IDLE )
		{
			temp = &( m_mod_maneger_array.mod_inlet_list[index]);
            if(temp->mod_id==m_cur_mod_id)
            {
            	os_strcpy( name,temp->name);
                break;
            }
		}
	}
	return ;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 模块管理任务初始化
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_module_maneger_init( void )
{
	os_memset(( uint8_t * ) &m_mod_maneger_array,
			  0,
			  sizeof( app_mod_maneger_t ) );

	os_memset(( uint8_t * )& m_mod_hmsg_box,
			  0,
			  sizeof( app_mod_handle_msg_t )*MAX_HMSG_SIZE );

	os_q_creat( &m_mod_hmsg_qevent,
				&m_mod_hqmsg_buf[0],
				MAX_HMSG_SIZE );

	os_sem_creat( &m_mod_hmsg_sem, 1 );
	return;
}
