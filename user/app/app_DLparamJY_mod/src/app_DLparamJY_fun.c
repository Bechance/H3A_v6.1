/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:电参数校验主体程序
* 创建时间:2013年1月7日
* 所属公司:科荟
* 文件名称:app_DLparamJYmod_fun.c
* 作者    :周炳权
* 功能描述:实现电参数校验功能
******************************************************************************************/
#include 		"app_DLparamJY_inside.h"

static  	uint8_t   _task_id			=	INVALID_PRO;
static  	uint8_t   _task2_id			=	INVALID_PRO;

uint8_t		g_DLparamJY_07_or_97 		= 1;
uint8_t		g_DLparamJY_meter_addr_flag = 0;//电表地址缓存标志
uint8_t     g_DLparamJY_start			= 0;//校验开始标志
uint8_t     g_DLparamJY_UIOUT_start		= 0;//源开始标志
BOOL 		g_DLparamJY_UIOUT_state 	= FAIL;//源稳定标志
DLparam_JY_t g_DLparam_buf = { 0, };

#define     MAX_ERROR		5

/********************************************************************
* @作者 :周炳权
* @功能 :电表电参数BCD码转换成浮点数
* @输入 :m_buf:数据缓存;m_size:缓存大小;m_pt_size:小数点位数;
* @输出 :m_temp:转换后的浮点数
********************************************************************/
float app_DLparamJYmod_meter_BCD_to_float( uint8_t *m_buf, uint8_t m_size, uint8_t m_pt_size)
{
	float m_temp = 0.0;
	uint8_t m_mod = 0;
	uint8_t m_sec = 0;
	uint8_t m_decimals = 0;
	uint8_t m_decimals2 = 0;
	uint8_t m_unit = 0;
	uint8_t m_decade = 0;
	uint8_t m_hundred = 0;
	uint16_t m_int = 0;

	if ( m_pt_size == 0 )
	{
		m_unit 		= *m_buf % 16;
		m_decade	= *m_buf / 16;
		if ( m_size >= 2 )
			m_hundred = *(m_buf+1) % 16;
	}
	if ( m_pt_size == 1 )
	{
		m_decimals 	= *m_buf % 16;
		m_unit 		= *m_buf / 16;
		if ( m_size >= 2 )
		{
			m_decade 	= *(m_buf+1) % 16;
			m_hundred 	= *(m_buf+1) / 16;
		}
	}
	else if ( m_pt_size == 2 )
	{
		m_sec = *m_buf % 16;
		m_mod = *m_buf / 16;
		m_decimals = m_mod * 10 + m_sec;
		if ( m_size >= 2 )
		{
			m_unit 		= *(m_buf+1) % 16;
			m_decade 	= *(m_buf+1) / 16;
		}
		if ( m_size >= 3 )
		{
			m_hundred 	= *(m_buf+2) % 16;
		}
	}
	else if ( m_pt_size == 3 )
	{
		m_sec 		= *m_buf / 16;
		m_decimals2 = *m_buf % 16;
		m_mod		= *(m_buf+1) % 16;
		m_decimals  = m_mod * 10 + m_sec;
		m_unit		= *(m_buf+1) / 16;
		if ( m_size >= 3 )
		{
			m_decade 	= *(m_buf+2) % 16;
			m_hundred 	= *(m_buf+2) / 16;
		}
	}
	else if ( m_pt_size == 4 )
	{
		m_sec = *m_buf % 16;
		m_mod = *m_buf / 16;
		m_decimals2 = m_mod * 10 + m_sec;
		m_sec = *(m_buf+1) % 16;
		m_mod = *(m_buf+1) / 16;
		m_decimals = m_mod * 10 + m_sec;
		if ( m_size >= 3 )
		{
			m_unit		= *(m_buf+2) % 16;
			m_decade	= *(m_buf+2) / 16;
		}
	}
	else if ( m_pt_size > 4 )
	return 0.0;
	m_temp = (float)(m_decimals2 / 10000.0) + (float)(m_decimals / 100.0);
	m_int = (uint16_t)(m_hundred * 100) + m_decade * 10 + m_unit;
	m_temp += m_int;
	return m_temp;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :电能表电参数误差计算(百分数)
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_err1_calc( void )
{
	float m_temp = 0.0;
	m_temp = g_DLparam_buf.meter_U[0];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Uerr[0] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_a.f_U))/GLOBAL_ABS(g_DL_param.phase_a.f_U)) * 100.0;
	m_temp = g_DLparam_buf.meter_U[1];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Uerr[1] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_b.f_U))/GLOBAL_ABS(g_DL_param.phase_b.f_U)) * 100.0;
	m_temp = g_DLparam_buf.meter_U[2];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Uerr[2] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_c.f_U))/GLOBAL_ABS(g_DL_param.phase_c.f_U)) * 100.0;
		
	return;
}

void app_DLparamJYmod_err2_calc( void )
{
	float m_temp = 0.0;
	
	m_temp = g_DLparam_buf.meter_I[0];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Ierr[0] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_a.f_I))/GLOBAL_ABS(g_DL_param.phase_a.f_I)) * 100.0;
	m_temp = g_DLparam_buf.meter_I[1];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Ierr[1] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_b.f_I))/GLOBAL_ABS(g_DL_param.phase_b.f_I)) * 100.0;
	m_temp = g_DLparam_buf.meter_I[2];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Ierr[2] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_c.f_I))/GLOBAL_ABS(g_DL_param.phase_c.f_I)) * 100.0;
	
	return;
}

void app_DLparamJYmod_err3_calc( void )
{
	float m_temp = 0.0;

	m_temp = g_DLparam_buf.meter_P[0];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Perr[0] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_a.f_P))/GLOBAL_ABS(g_DL_param.phase_a.f_P)) * 100.0;
	m_temp = g_DLparam_buf.meter_P[1];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Perr[1] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_b.f_P))/GLOBAL_ABS(g_DL_param.phase_b.f_P)) * 100.0;
	m_temp = g_DLparam_buf.meter_P[2];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Perr[2] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_c.f_P))/GLOBAL_ABS(g_DL_param.phase_c.f_P)) * 100.0;
	
	return;
}

void app_DLparamJYmod_err4_calc( void )
{
	float m_temp = 0.0;

	m_temp = g_DLparam_buf.meter_Q[0];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Qerr[0] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_a.f_Q))/GLOBAL_ABS(g_DL_param.phase_a.f_Q)) * 100.0;
	m_temp = g_DLparam_buf.meter_Q[1];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Qerr[1] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_b.f_Q))/GLOBAL_ABS(g_DL_param.phase_b.f_Q)) * 100.0;
	m_temp = g_DLparam_buf.meter_Q[2];
	m_temp = GLOBAL_ABS(m_temp);
	if ( m_temp != 0 )
	g_DLparam_buf.meter_Qerr[2] = ((m_temp - GLOBAL_ABS(g_DL_param.phase_c.f_Q))/GLOBAL_ABS(g_DL_param.phase_c.f_Q)) * 100.0;

	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :源稳定提示函数
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_XNFZ_steady_handle( void )
{
	const char *m_title_buf[2] 	= {"虚拟负载已经稳定,等电表复位完成后就可以开始测量!",""};
	GUI_3D_window_title_mod(m_DLparamJYmod_win,(char *)m_title_buf[g_language]);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :走字开始源稳定处理函数
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_XNFZ_NOTsteady_handle( void )
{
	const char *m_title_buf[2] 	= {"虚拟负载还没稳定,等稳定后再开始校验",""};
	GUI_3D_window_title_mod(m_DLparamJYmod_win,(char *)m_title_buf[g_language]);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :主任务按键处理
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_key_handle(void)
{
	uint8_t key_buf;
	const char *m_tip[2] 		= {"提示!",""};
	const char *m_begin[2] 		= {"请检查电能表是否处于正常工作状态,是则按<确定>开始测量,否则按<退出>不测量",""};
//	const char *m_XNFZ_start[2] = {"请检查电表电流量程与实际设置的量程是否一致!按<确定>启动源,按<退出>不启动",""};
	const char *m_title_buf[2] 	= {"电能表测量参数校验结束",""};
	const char *m_title1_buf[2] = {"电能表测量参数校验开始",""};
//	const char *m_title2_buf[2] = {"智能电能表参数校验",""};
	const char *m_title3_buf[2] = {"回路当中无电压,请检查电能表是否工作正常!",""};
	if (app_global_key_obtain(&key_buf,1))
	{
		if( key_buf == KEY_EXIT || key_buf == GUI_KEY_EXIT)
		{
			//app_module_assignExt("START");
			asm( "  lb 0x300000" );
		}
		/*else if ( key_buf == GUI_KEY_BEGIN && g_DLparamJY_UIOUT_state == FAIL && g_DLparamJY_UIOUT_start == 1 )
		{
			g_DLparamJY_start = 0;
			app_DLparamJYmod_XNFZ_NOTsteady_handle();
		}*/
		else if ( key_buf == KEY_F1 || key_buf == GUI_KEY_BEGIN )
		{
			if ( g_DL_param.phase_a.f_U < 30 )
			{
				GUI_3D_window_title_mod(m_DLparamJYmod_win,(char *)m_title3_buf[g_language]);
			}
			else
			{
				if (MsgBox(200,170,250,200,(char *)m_tip[g_language],(char *)m_begin[g_language],APP_HALF_SEC*30) == GUIE_KEY_ENTER )
				{
					g_DLparamJY_start = 1;
					GUI_3D_window_title_mod(m_DLparamJYmod_win,(char *)m_title1_buf[g_language]);
				}
			}
		}
		else if ( key_buf == KEY_F2 ||  key_buf == GUI_KEY_END )
		{
			if ( _task_cmt_id != INVALID_PRO )
			{
				os_task_delete( _task_cmt_id );
				_task_cmt_id = INVALID_PRO;
			}
			g_DLparamJY_start = 0;
			g_DLparamJY_UIOUT_start = 0;
			GUI_3D_window_title_mod(m_DLparamJYmod_win,(char *)m_title_buf[g_language]);
		}
		else if ( key_buf == GUI_KEY_TERATY_SELECT )
		{
			g_DLparamJY_07_or_97 = GUI_radio_curmember_obtain(GET_DOBJ(DLPARAMJYMOD_TREATYSELECT_WAY));
		}
		else if ( key_buf==KEY_RIGHT )
		{
			if( m_pMember != m_CURwidget->subord_end )
		  	GUI_WINshift(0);
		}
		else if ( key_buf==KEY_LEFT )
		{
			if( m_pMember!=m_CURwidget->subord_head )
		  	GUI_WINshift(1);
		}			 
		else if ( key_buf==KEY_ENTER )
		{
			GUI_WINdo();
		}
		/*else if ( key_buf == GUI_KEY_SELFUP_UI )
		{
			if ( task_DLparam_XNFZ_id == INVALID_PRO )
			{
				app_DLparamJYmod_xnfz_window_create();
				task_DLparam_XNFZ_id = 
				os_task_assign_stk_build( app_DLparam_XNFZ_task,2);
			}
		}
		else if( key_buf == GUI_KEY_XNFZ_START)
		{
			if (MsgBox(200,395,200,200,(char *)m_tip[g_language],(char *)m_XNFZ_start[g_language],APP_HALF_SEC*30) == GUIE_KEY_ENTER )
			{			
				OS_DELETE_REQ( task_DLparam_XNFZ_id );
				task_DLparam_XNFZ_id = INVALID_PRO;
				os_driver_ioctl(g_UIout_dev,UIOUT_OPEN,0);
				g_DLparamJY_UIOUT_start = 1;
				GUI_3D_window_title_mod(m_DLparamJYmod_win,(char *)m_title2_buf[g_language]);
			}
			else
			{
				OS_DELETE_REQ( task_DLparam_XNFZ_id );
				task_DLparam_XNFZ_id = INVALID_PRO;
				os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
				g_DLparamJY_UIOUT_start = 0;
				GUI_3D_window_title_mod(m_DLparamJYmod_win,(char *)m_title2_buf[g_language]);
			}
		}
		else if( key_buf == GUI_KEY_XNFZ_CLOSE)
		{
			OS_DELETE_REQ( task_DLparam_XNFZ_id );
			task_DLparam_XNFZ_id = INVALID_PRO;
			os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
			g_DLparamJY_UIOUT_start = 0;
			GUI_3D_window_title_mod(m_DLparamJYmod_win,(char *)m_title2_buf[g_language]);
		}*/
	}
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :主模块测量任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void _task(void*arg)
{
	DL_flag_t dl_flag;
	int  cnt 	= 59;
	msleep(100);
	while (1)
	{
		msleep( TYPE_DELAY );
		app_global_DLflag_obtain(&dl_flag);
		if ( dl_flag.DL_data_is_new && (++cnt%30==0) )
		{
			app_global_DLarg_obtain(&g_DL_param);
			app_DLparamJYmod_DLarg_display(&g_DL_param);
		}
	}
}
/********************************************************************
* @作者 :周炳权
* @功能 :主模块任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void _task2(void*arg)
{
	//uint8_t m_tip_flag = 0;
	fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
    g_user_arg.DL_app_user_set.meter_1OR3 = 0;
    g_user_arg.DL_app_user_set.PorQ = 0;
    g_user_arg.DL_app_user_set.handOR_GD = 0;
    app_global_set_user_dl_arg(&g_user_arg);
	app_DLparamJYmod_window_main_creat();
    fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
    app_global_set_user_dl_arg(&g_user_arg);
	GUI_WINready(m_DLparamJYmod_radio_TREATYSELECT,g_DLparamJY_07_or_97);
	while(1)
	{   
		msleep( TYPE_DELAY );
		if ( g_DLparamJY_UIOUT_start == 1 )
		{
			//app_DLparamJYmod_XNFZ_unlawful_close();
			//g_DLparamJY_UIOUT_state = app_DLparamJYmod_XNFZ_state_get();
			//测试
			if ( g_DLparamJY_UIOUT_state != 1 )
			g_DLparamJY_UIOUT_state = 1;
		}
		if ( g_DLparamJY_start == 1 )
		{
			g_DLparamJY_start = 0;
			/*if ( g_DLparamJY_UIOUT_start == 1 && g_DLparamJY_UIOUT_state == FAIL )
			{
				//虚拟负载还没稳定
				//app_DLparamJYmod_XNFZ_NOTsteady_handle();
			}
			else */
			{
				GLOBAL_MEMCLR(&(g_DLparam_buf.meter_U[0]), (sizeof(float)*24));
				app_DLparamJYmod_param_and_err_display(&g_DLparam_buf);
				ReadDL_param_handle();
			}
		}
		/*if ( g_DLparamJY_UIOUT_state == 1 && g_DLparamJY_UIOUT_start == 1 )
		{
			if ( m_tip_flag == 0 )
			{
				m_tip_flag = 1;
				app_DLparamJYmod_XNFZ_steady_handle();
			}
		}*/
		if ( g_DLparam_data_finish == 1 )
		{
			g_DLparam_data_finish = 0;
			app_global_key_dummy_press( GUI_KEY_END );
		}
		app_DLparamJYmod_key_handle();
	}		
}
/********************************************************************
* @作者 :
* @功能 :模块主功能初始化
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_DLparamJYmod_fun_entry( void )
{
	_task_id =
		os_task_assign_stk_build( _task,3);
	_task2_id = 
		os_task_assign_stk_build( _task2,6);
	app_global_DL_measure_entery();
	app_DLparamJY_Rmeter_init();
	GLOBAL_MEMCLR(&g_DLparam_buf, sizeof(DLparam_JY_t));
	g_DLparamJY_UIOUT_start = 0;
	return;
}
/********************************************************************
 * @作者 :
 * @功能 :模块主功能释放资源
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_fun_exit( void )
{
	app_DLparamJYmod_window_main_destory();
	if ( _task2_id != INVALID_PRO )
	{
		os_task_delete( _task2_id );
		_task2_id = INVALID_PRO;
	}
	if ( _task_id != INVALID_PRO )
	{
		os_task_delete( _task_id );
		_task_id = INVALID_PRO;
	}
	app_DLparamJY_Rmeter_uninstall();
	os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
	app_global_DL_measure_exit();
	return;
}

