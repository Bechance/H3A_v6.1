/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块主体程序
* 创建时间:2010-08-02
* 所属公司 :科荟
* 文件名称:app_XBmod_fun.c
* 创建人 :揭成
* 功能描述:实现系统设置功能
******************************************************************************************/
#include	"app_XBmod_inside.h"
#include	<stdlib.h> 
static      uint8_t 	_task_id = INVALID_PRO;
/********************************************************************
 * @创建人 :揭成
 * @功能 :WAVE模块主任务
 * @输入 :NONE
 *@输出  :NONE
********************************************************************/
static void _task( void*arg )
{
	uint8_t key_buf;
	uint8_t GRtask  = INVALID_PRO;
	uint8_t NUMtask = INVALID_PRO;
	GRtask = os_task_assign_stk_build( app_XBmod_GRtask,3);
	while(1)
	{
		msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			if ( GRtask != INVALID_PRO )
			{
				OS_DELETE_REQ( GRtask );
			}
			if ( NUMtask != INVALID_PRO )
			{
				OS_DELETE_REQ( NUMtask );
			}
			os_task_delete( SELF_PRO );   			
		}
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 
		{
			if(key_buf==KEY_EXIT)
			{
				//app_module_assignExt("START");
				app_module_switch_last_mod();
			}
			else if(key_buf == KEY_F5)
			{
				if ( NUMtask != INVALID_PRO )
				{
					OS_DELETE_REQ( NUMtask );
					NUMtask = INVALID_PRO;
					GRtask  = os_task_assign_stk_build( app_XBmod_GRtask,3);
				}
				else if ( GRtask != INVALID_PRO )
				{
					OS_DELETE_REQ( GRtask );
					GRtask  = INVALID_PRO;
					NUMtask = os_task_assign_stk_build( app_XBmod_Numtask,3);
				}
			}	
			else 
			{
				GUI_keyMSG_post(m_XBmod_win_Obj,key_buf);
			}
		}
	}
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :主模块主功能初始化
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_XBmod_fun_entry( void )
{
	_task_id = os_task_assign_stk_build( _task,3);
	os_task_name_set( _task_id, "XBmod--Main task" );
	app_global_DL_measure_entery();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :主模块主功能释放资源
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_XBmod_fun_exit( void )
{
	if ( _task_id != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_id );
		_task_id = INVALID_PRO;
	}
	app_global_DL_measure_exit();
	return;
}
