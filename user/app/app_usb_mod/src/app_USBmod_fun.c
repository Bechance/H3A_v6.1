/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块主体程序
* 创建时间:2010-08-02
* 所属公司 :科荟
* 文件名称:app_USBmod_fun.c
* 创建人 :揭成
* 功能描述:实现系统设置功能
******************************************************************************************/
#include"app_USBmod_inside.h"
uint8_t 		m_usb_main_task_pro = INVALID_PRO;
extern dev_id_t g_DL_dev;
/********************************************************************
 * @创建人 :周炳权
 * @功能 :修正数据备份
 * @输入 :
 * @输出 :NONE
********************************************************************/
static void app_USBmod_correct_data_backup(void)
{
	uint8_t flag;
	correct_save_data_t*psave;
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_GET_CORRECT_FILE,&psave);
	flag = app_correct_data_backup(psave,sizeof(correct_save_data_t));
	if ( flag == 1 )
	{
		if(MsgBox(220,185,350,200, "提示",
				  "备份修正数据成功",APP_HALF_SEC*10)==KEY_ENTER)
		{}
	}
	else
	{
		if(MsgBox(220,185,350,200, "提示",
				  "备份修正数据失败",APP_HALF_SEC*10)==KEY_ENTER)
		{}
	}
    return;    
}
/********************************************************************
 * @创建人 :周炳权
 * @功能 :读取修正数据
 * @输入 :
 * @输出 :NONE
********************************************************************/
static void app_USBmod_correct_data_read(void)
{
	uint8_t flag;
	correct_save_data_t*psave;
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_GET_CORRECT_FILE,&psave);
	//GLOBAL_MEMCLR(psave, sizeof(correct_save_data_t));
	flag = app_correct_data_read(psave,sizeof(correct_save_data_t));
	if ( flag == 1 )
	{
		if(MsgBox(220,185,350,200, "提示",
				  "读取修正数据成功",APP_HALF_SEC*10)==KEY_ENTER)
		{}
	}
	else
	{
		if(MsgBox(220,185,350,200, "提示",
				  "读取修正数据失败，请检查文件是否存在!",APP_HALF_SEC*10)==KEY_ENTER)
		{}
	}
    return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :USB模块主任务
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_USBmod_main_task( void*arg )
{
	uint8_t key_buf;
	hwin_t *hp;
	hp = hp;
	if ( !app_USBmod_search_window_creat() )
	{
		app_module_assignExt("START");
	}
	app_USBmod_window_creat();
	while ( 1 )
	{
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 	/*读取按键驱动值*/
		{
			if ( key_buf == KEY_EXIT||key_buf==GUI_KEY_3D_WINDOW_CLOSE)
			{
				//app_module_assignExt("START");
				asm( "  lb 0x300000" );
			}
			else if ( key_buf == KEY_0 || key_buf == KEY_1
					  || key_buf == KEY_2 || key_buf == KEY_3
					  || key_buf == KEY_5||key_buf == KEY_8
					  ||key_buf == KEY_9 ||key_buf == KEY_F5)
			{
				app_USBmod_window_destroy();
				app_hzlib_updata_entery( key_buf );
				app_USBmod_window_creat();
			}
			else if ( key_buf == KEY_4 )
			{
			}
			else if ( key_buf == KEY_6 )
			{
				if(MsgBox(220,185,350,200, "提示",
				  "确认备份修正数据?",APP_HALF_SEC*100)==KEY_ENTER)
				{
					app_USBmod_correct_data_backup();
				}
			}
			else if ( key_buf == KEY_7 )
			{
				if(MsgBox(220,185,350,200, "提示",
				  "确认读取备份修正数据?",APP_HALF_SEC*100)==KEY_ENTER)
				{
					app_USBmod_correct_data_read();
				}
			}
		}
		OS_DELETE_REQ_SELF();
		msleep( TYPE_DELAY );
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
void app_USBmod_fun_entry( void )
{
	m_usb_main_task_pro
	= os_task_assign_stk_build( app_USBmod_main_task, 6 );
	os_task_name_set( m_usb_main_task_pro, "UT" );
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
void app_USBmod_fun_exit( void )
{
	if ( m_usb_main_task_pro != INVALID_PRO )
	{
		os_task_delete( m_usb_main_task_pro );
		m_usb_main_task_pro = INVALID_PRO;
	}
	return;
}
