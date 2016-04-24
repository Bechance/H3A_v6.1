/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:变比模块主体程序
* 创建时间:
* 所属公司:科荟
* 文件名称:app_startmod_fun.c
* 创建人    :
* 功能描述:实现变比功能
******************************************************************************************/
#include    "app_startmod_inside.h"
static      a_sem_t*	_Expsem 	= INVALID_SEM;
static      uint8_t     _task_pro	= INVALID_PRO;


static void _key_shift_Exc( uint8_t key_buf )
{
	if (( key_buf == KEY_LEFT ) && ( m_menu_mark_pos != m_menu_start_pos ) )
	{
		m_menu_mark_pos--;
	}
	else if (( key_buf == KEY_RIGHT ) && ( m_menu_mark_pos < ( m_menu_end_pos - 1 ) ) )
	{
		m_menu_mark_pos++;
	}
	else if (( key_buf == KEY_DOWN ) && ( m_menu_mark_pos <= ( m_menu_end_pos - GRAPH_ROW - 1 ) ) )
	{
		m_menu_mark_pos += 3;
	}
	else if (( key_buf == KEY_UP ) && (( m_menu_mark_pos - GRAPH_ROW ) >= m_menu_start_pos ) )
	{
		m_menu_mark_pos -= 3;
	}
	return;
}

static void _key_SWmod_Exc( uint8_t key_buf )
{
	if(key_buf==KEY_EXIT)
	{
	    app_module_assignExt("START");
	}
	return;
}

/********************************************************************
* @创建人 :
* @功能 :主模块按键任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void _save_dsp_version(void)
{
	char str_temp[40] = "";
	char str2[30] = "";
	FIL *fp = PNULL;
	UINT rw = 0;
	
	fp = fatfs_open("DSPVER", FA_CREATE_ALWAYS | FA_WRITE);
	if(fp)
	{
		global_strcpy(str_temp, "H3AV6.1   " ); 
		global_strcpy( str2, __DATE__ );
		global_strcat(str_temp,str2);
		f_write(fp,str_temp,global_strlen(str_temp), &rw );
		fatfs_close(fp);
	}
	return;
}

/********************************************************************
* @创建人 :
* @功能 :主模块按键任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void _task( void*arg )
{
	uint8_t      key_buf;
	int          oldmark, cnt = 399;
	int			 cnt2 = 19;
	dev_id_t     sht1;
	Bsize_t      offset;
	hwin_t      *pOBJ;
	float        fdata[3];
	app_startmod_window_creat();
	app_global_set_user_dl_arg( &g_user_arg );
	os_sem_creat( &_Expsem, 0 );
	sht1 = os_driver_open( "SHT11" );
	while ( 1 )
	{
		if ( ++cnt %400 == 0 )
		{
			app_Systime_obtain();
			app_startmod_time_display(g_user_arg.DL_app_user_info.time_buf);
		}
		if ( cnt % 200 == 0 && sht1 )
		{
		    os_driver_ioctl(sht1,0,&fdata);
            app_startmod_WSD_display(fdata[0],fdata[1]);
		}
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )/*判断是否有删除请求*/
		{
			GUI_window_destory( m_3Dwin );
			os_driver_close( sht1 );
			os_task_delete( SELF_PRO );
		}
		if ( os_sem_pend( _Expsem, 2 ) == OS_NO_ERR )
		{
			if ( m_apparray_pCUR->Exe[0] == ':' )
			{
				 app_module_assignExt(( &m_apparray_pCUR->Exe[1] ) );
			}
			else
			{
			   if(global_memcmp(m_apparray_pCUR->Exe,"SELF", 4))
			   {
					if(MsgPasswordBox( 200,150,"1234"))
				    {
						app_load_eEXE( "SELF" );
					}
			   	}
			    else
			   	{
					app_load_eEXE( m_apparray_pCUR->Exe );
			   	}
			}
		}
		if ( ++cnt2 % 20 == 0 )
		{
			app_Startmod_external_display();
			app_Startmod_charge_display();
		}
		if ( ++cnt2 % 55 == 0 ) app_Startmod_battery_display();
		app_Startmod_adapter_protect_display();
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 	/*读取按键驱动值*/
		{
			oldmark = m_menu_mark_pos;
			if ( key_buf == KEY_F5 )
			{
				app_next_page();
			}
			else if ( key_buf == KEY_F1 )
			{
                  app_startmod_lcdBLK();
			}
			else if ( key_buf == KEY_F2 )
			{
				if ( MsgPasswordBox( LCD_WIDE / 2, LCD_HIGH / 2, "4567" ) )
				{
					app_Extapp_updata();
					GUI_window_destory( m_3Dwin );
					app_startmod_window_creat();
				}
				cnt = 399;
			}
			else if ( key_buf == KEY_F3 )
			{
				if ( MsgPasswordBox( LCD_WIDE / 2, LCD_HIGH / 2, "4567" ) )
				{
					GUI_window_destory( m_3Dwin );
					app_startmod_DelExp_entery();
					app_startmod_window_creat();
				}
				cnt = 399;
			}
			else if ( key_buf == KEY_F4 )
			{
				app_last_page();
			}
			else if ( KEY_ENTER == key_buf )
			{
				_save_dsp_version();
				app_startmod_markExp_start();
			}
			else if ( key_buf == KEY_7 )
			{
				app_startmod_Stime_entery();
				cnt = 399;
			}
			else if ( key_buf == KEY_SETTING )
			{
				pOBJ   = GUI_Getchild( KEY_SETTING, m_3Dwin );
				offset = GUI_drop_list_unfold( pOBJ );
				g_user_arg.DL_app_user_set.dlsr = offset;
				GUI_drop_list_member_assign( pOBJ, offset );
				fatfs_writeARG( "USERDATA", &g_user_arg, sizeof( DL_app_user_all_info_t ) );
	 			app_global_set_user_dl_arg(&g_user_arg);
			}
			_key_shift_Exc( key_buf );
			_key_SWmod_Exc( key_buf );
			if ( m_menu_mark_pos != oldmark )
			{
				GRAPH_RMARK( oldmark );
				GRAPH_MARK( m_menu_mark_pos );
			}
		}
	}
}
/********************************************************************
* @创建人 :
* @功能 :启动应用程序
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_start_EXE( void )
{
	OSSemPost( _Expsem );
	return;
}
/********************************************************************
* @创建人 :
* @功能 :模块主功能初始化
* @输入 :NONE
* @输出 :NONE
********************************************************************/
void app_startmod_fun_entry()
{
    app_startmod_lcdBLK_AA();
	_task_pro = os_task_assign_stk_build( _task, 6 );
	return;
}
/********************************************************************
 * @创建人 :
 * @功能 :模块主功能释放资源
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_startmod_fun_exit()
{
	if ( _task_pro != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_pro );
		_task_pro = INVALID_PRO;
	}
	return;
}
