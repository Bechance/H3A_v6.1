/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:变比模块主体程序
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_BZmod_fun.c
* 创建人 :
* 功能描述:实现变比功能
******************************************************************************************/
#include "app_BZ_inside.h"
static  uint8_t   _task_id=INVALID_PRO;

/********************************************************************
* @创建人 :揭成
* @功能   :selfCOR模块主任务
* @输入   :NONE
* @输出   :NONE
********************************************************************/
static void _task(void*arg)
{
	uint8_t     key_buf;
	app_BZmod_window_creat();
	while(1)
	{
	    msleep(TYPE_DELAY);
		if ( os_task_delete_req(SELF_PRO) == OS_TASK_DEL_REQ)
		{
			os_task_delete( SELF_PRO );   			
		}
	}
}
/********************************************************************
* @创建人 :揭成
* @功能 :selfCOR模块主功能初始化
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_BZmod_fun_entry()
{
	_task_id = os_task_assign_stk_build( _task,6);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :selfCOR模块主功能释放资源
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_BZmod_fun_exit()
{
	if ( _task_id != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_id );
		_task_id = INVALID_PRO;
	}
	return;
}
