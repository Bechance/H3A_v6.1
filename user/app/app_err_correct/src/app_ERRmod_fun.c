/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块主体程序
* 创建时间:2010-08-02
* 所属公司 :科荟
* 文件名称:app_ERRmod_fun.c
* 作者 :揭成
* 功能描述:实现系统设置功能
******************************************************************************************/
#include				"app_ERRmod_inside.h"
uint8_t  			   	m_ERRmod_task_pro = INVALID_PRO;
correct_option_t   		m_correct_option;
/********************************************************************
 * @作者 :揭成
 * @功能 :发送修正步骤
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void  app_ERRmod_correct_step_send( uint8_t main_step,uint8_t sub_step,uint8_t phase,uint8_t pos)
{
	DL_correct_step_t correct_setp;
	correct_setp.main_step 	= main_step;
	correct_setp.sub_step   = sub_step;
	correct_setp.base_U 	= global_atof( m_correct_option.c_base_U );
	correct_setp.pos 		= pos;
	correct_setp.phase 		= phase;
	os_driver_ioctl( g_DL_dev, MEASURE_CMD_CORRECT_STEP_SET, &correct_setp );/*发送修正步骤*/
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :ERR模块主任务
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_ERRmod_main_task( void*arg )
{
	uint8_t key_buf;
	GLOBAL_MEMCLR(&m_correct_option,sizeof(correct_option_t));
	global_strcpy(m_correct_option.c_base_U,"220");
	app_ERRmod_window_creat();
    if(!MsgPasswordBox( 200,150,"1234"))
    {
       MOD_SWITCH_STARTMOD();
	}
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )
		{
			continue;
		}
		if(key_buf==KEY_1)
		{
		   app_ERRmod_window_destroy();
           app_ERRmod_Ucorrect_task(0);
		   app_ERRmod_window_creat();
		}
		else if(key_buf==KEY_2)
		{
		   app_ERRmod_window_destroy();
           app_ERRmod_Icorrect_task(0);
		   app_ERRmod_window_creat();
		}
		else if(key_buf==KEY_3)
		{
		   app_ERRmod_window_destroy();
           app_ERRmod_TMRcorrect_task(0);
		   app_ERRmod_window_creat();
		}
		else if(key_buf==KEY_4||key_buf==KEY_EXIT)
		{
			app_ERRmod_window_destroy();
            MOD_SWITCH_STARTMOD();
		}
	}
}
/********************************************************************
 * @作者 :揭成
 * @功能 :主模块主功能初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_ERRmod_fun_entry( void )
{
	m_ERRmod_task_pro
	= os_task_assign_stk_build( app_ERRmod_main_task,11);
	os_task_name_set( m_ERRmod_task_pro, "ERRmod--Main task" );
    app_global_DL_measure_entery();
	os_driver_ioctl( g_DL_dev, MEASURE_CMD_IAUTO_LEVEL_STOP,0);
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :主模块主功能释放资源
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_ERRmod_fun_exit( void )
{
	if ( m_ERRmod_task_pro != INVALID_PRO )
	{
		os_task_delete( m_ERRmod_task_pro );
		m_ERRmod_task_pro = INVALID_PRO;
	}
	app_global_DL_measure_exit();
	return;
}
