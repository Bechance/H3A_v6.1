/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:变比模块主体程序
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_BBmod_fun.c
* 创建人 :
* 功能描述:实现变比功能
******************************************************************************************/
#include "app_BBmod_inside.h"
extern   void hand_wirte_input_box_pos_set(uint16_t x ,uint16_t y);
uint8_t   m_bianbi_main_task_pro = INVALID_PRO;
Bsize_t   m_QB_offset_1=0;
Bsize_t   m_QB_offset_2=0;
DL_app_user_all_info_t *m_bb_user_arg;
/********************************************************************
 * @创建人 :揭成
 * @功能 :开始测量
 * @输入 :缓存区arg
 * @输出 :NONE
********************************************************************/
void app_BBmod_measure_start( Bsize_t offset_1,Bsize_t offset_2)
{
	uint8_t key_buf;
	DL_flag_t dl_flag;
	measure_param_t *pmem = &g_DL_param;
	Bsize_t QB_1_offset = QB_N_100A;
	Bsize_t QB_2_offset = QB_N_5A;
	Bsize_t  cnt = 19;
	app_BBmod_state_display(1);
	app_global_DL_measure_entery();
	QB_1_offset += offset_1;
	QB_2_offset += offset_2;
	m_QB_offset_1=QB_1_offset;
	m_QB_offset_2=QB_2_offset;
	msleep( APP_100_MS*11 );
	app_global_QB_select( QB_1_offset, QB_2_offset,QB_2_offset);
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 	/*读取按键驱动值*/
		{
			if ( key_buf == KEY_EXIT )
			{
				app_global_DL_measure_exit();
				app_BBmod_window_destroy();
				app_module_switch_last_mod();
			}
			else if(key_buf ==KEY_F2 || key_buf ==KEY_F3)
			{
			    app_BBmod_state_display(0);
				app_global_DL_measure_exit();
				app_global_key_dummy_press( key_buf);
				break;
			}
			else if(key_buf==KEY_SAVE)
			{
				//app_global_DL_measure_exit();
				//app_BBmod_window_destroy();
			}
		}
		app_global_DLflag_obtain(&dl_flag);
		if ((dl_flag.DL_data_is_new)&& ( ++cnt % 20 == 0 ) )
		{
			GLOBAL_MEMCLR( pmem, sizeof( measure_param_t ) );
			os_driver_ioctl( g_DL_dev, MEASURE_CMD_DL_PARAM_GET, ( void* )pmem );/*获取电参数*/
			app_BBmod_DLarg_display( pmem );
		}
	}
	return;
}
/********************************************************************
 * @创建人 :
 * @功能 :变比模块主任务
 * @输入 :缓存区arg
 * @输出 :NONE
********************************************************************/
void app_BBmod_main_task( void *arg )
{
	uint8_t key_buf;
	Bsize_t offset;
	Bsize_t offset_1=0,offset_2=0;
	offset = offset;
	app_BBmod_window_creat();
	fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
    app_global_set_user_dl_arg(&g_user_arg);
	m_bb_user_arg=&g_user_arg;
	hand_wirte_input_box_pos_set(25,80);
	while ( 1 )
	{
		msleep(TYPE_DELAY);
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 	/*读取按键驱动值*/
		{
			if ( key_buf == KEY_EXIT )
			{
				app_BBmod_window_destroy();
				app_module_switch_last_mod();
			}
			else if ( key_buf == KEY_F2)
			{
				offset_1=GUI_drop_list_unfold(GET_DOBJ(1));
			}
			else if(key_buf == KEY_F3)
			{
				offset_2=GUI_drop_list_unfold(GET_DOBJ(2));
			}
			else if ( key_buf == KEY_ENTER )
			{
				app_BBmod_measure_start( offset_1,offset_2 );
			}
		}
	}
}
/********************************************************************
* @创建人 :
* @功能 :变比模块主功能初始化
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_BBmod_fun_entry()
{
	m_bianbi_main_task_pro
	= os_task_assign_stk_build( app_BBmod_main_task, 6 );
	os_task_name_set( m_bianbi_main_task_pro, "bianbimod--Main task" );
	return;
}
/********************************************************************
 * @创建人 :
 * @功能 :变比模块主功能释放资源
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_BBmod_fun_exit()
{
	if ( m_bianbi_main_task_pro != INVALID_PRO )
	{
		os_task_delete( m_bianbi_main_task_pro );
		m_bianbi_main_task_pro = INVALID_PRO;
	}
	if(g_DL_dev!=INVALID_DEV_ID)
	{
		app_global_DL_measure_exit();
	}
	return;
}
