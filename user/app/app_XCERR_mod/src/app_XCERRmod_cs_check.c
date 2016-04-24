/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:变比模块主体程序
* 创建时间:
* 所属公司:科荟
* 文件名称:app_XCERRmod_cs_check.c
* 创建人  :
* 功能描述:常数校核
******************************************************************************************/
#include    "app_XCERRmod_inside.h"

#define  YPOS  		 5
#define  CH_WIDE     12
#define  MPOS  		(YPOS+CH_WIDE*4)
#define  DPOS  		(MPOS+CH_WIDE*4)
#define  HPOS  		(DPOS+CH_WIDE*4+CH_WIDE*2)
#define  MINPOS  	(HPOS+CH_WIDE*4)
#define  SPOS  		(MINPOS+CH_WIDE*4)
#define  POSY       (50-HZ_HIGH)/2
#define  LPOSY      (POSY+HZ_HIGH+2)




/********************************************************************
 * @作者 :周炳权
 * @功能 :常数校核函数
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void _get_checked_cs( hwin_t *hp)
{
	uint64_t m_CS = 1;
	uint64_t m_CS2 = 1;
	float m_defualt_err = 0.05;
    char  ch_1[10]="";
    char  ch2_2[10]="";
	uint8_t key;
	uint8_t m_qS  = global_atoi(g_user_arg.DL_app_user_set.qs);
	uint8_t m_qs2 = global_atoi(g_user_arg.DL_app_user_set.qs2);
	float m_real_value = 0.0;
	float m_count_value[3] = {0,};
	
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_GET_DDSN, m_count_value);
	if(g_DL_param.f_Pabc>1&&m_count_value[0]!=0)
	{
		m_defualt_err = (m_defualt_err * 0.5);
		m_real_value = m_defualt_err /100.0 * m_count_value[0] + m_count_value[0];
		if(m_real_value != 0 )
		{
			m_CS = (uint64_t)(360000000 / m_real_value * m_qS);
			m_CS = GLOBAL_ABS(m_CS);
		}
		if(g_user_arg.DL_app_user_set.meter_1OR3 == 1) //单表
		{
			m_defualt_err = (m_defualt_err * 0.5);
			m_real_value = m_defualt_err /100.0 * m_count_value[1] + m_count_value[1];
			if(m_real_value != 0 )
			{
				m_CS2 = (uint64_t)(360000000 / m_real_value * m_qs2);
				m_CS2 = GLOBAL_ABS(m_CS2);
			}
		}
	}
	if(m_CS == 0) m_CS = 1;
	global_ltoa((long)m_CS, ch_1);
	if(m_CS2== 0) m_CS2= 1;
	global_ltoa((long)m_CS2, ch2_2);
	
	GUI_window_hwin_req( hp );
	GUI_set_Bgcolor( 0xce79 );
	GUI_set_Fgcolor( 0 );
	GUI_edit_string_display_At(ch_1, EDIT_CS_CHECK1);
	if(g_user_arg.DL_app_user_set.meter_1OR3 == 1)
		GUI_edit_string_display_At(ch2_2, EDIT_CS_CHECK2);
	GUI_window_hwin_release( hp );
	
	app_global_key_control_power_req();
    while(1)
    {
		msleep( TYPE_DELAY );
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		if(key==KEY_ENTER)
		{
			global_ltoa(m_CS, g_user_arg.DL_app_user_set.cs);
			if(g_user_arg.DL_app_user_set.meter_1OR3 == 1)
				global_ltoa(m_CS2, g_user_arg.DL_app_user_set.cs2);
			app_global_set_user_dl_arg(&g_user_arg);
		    fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
			break;
		}
		else if(key==KEY_EXIT)
		{
			break;
		}
	}
	app_global_key_control_power_release();
	return;
}

static void _cs_check_callback( hwin_t *pwin)
{
	hwin_t*hp, *button;
	GUI_window_hwin_req( pwin );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	button = Gui_button_creat( 80, 90, 90, 35, KEY_ENTER | BUTTON_PAINT, pwin );
	GUI_button_title_display( button, (char*)m_XCERRmod_MSG_54[g_language]);
	button = Gui_button_creat( 215, 90, 90, 35, KEY_EXIT | BUTTON_PAINT, pwin );
	GUI_button_title_display( button, (char*)m_XCERRmod_MSG_55[g_language] );
	GUI_window_hwin_release( pwin );
	hp = GUI_edit_creat( 60, 10, pwin->win_wide - 30, 46, WIN_TOUCH_EN, pwin );
	GUI_window_hwin_req( hp );
	GUI_SetFront( &GUI_HZK24_EXTASCII );
	GUI_set_Bgcolor( 0xce79 );
	GUI_set_Fgcolor( 0 );
	GUI_edit_memeber_add( 5, POSY-10, 10, (char*)m_XCERRmod_MSG_51[g_language], EDIT_CS_CHECK1  );
	GUI_edit_memeber_add( 5, POSY+25, 10, (char*)m_XCERRmod_MSG_52[g_language], EDIT_CS_CHECK2 );
	GUI_window_hwin_release( hp );
	_get_checked_cs(hp);
	return;
	
}


/********************************************************************
 * @作者 :阳永锋
 * @功能 :进入常数校核
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/

void app_XCERRmod_check_entry(void)
{
	hwin_t*hp;
	uint16_t sec;
	int x1, y1, x2, y2;
	x1 = 130;
	y1 = 140;
	x2 = x1 + 400;
	y2 = y1 + 200;
	
	sec = GUI_read_block_Extp( x1, y1, x2, y2 );
	GUI_window_hwin_req( GET_GUIWIN );
	Gui_button_set_color( C_GUI_LOW_BLUE, 0xce79 );
	Gui_3D_window_set_color( C_GUI_WHITE, C_GUI_LOW_BLUE, C_GUI_RED );
	GUI_set_Bgcolor( 0xce79 );
	GUI_set_Fgcolor( 0 );
	hp = Gui_3D_window_creat( x1 + 10, y1 + 10, 380, 180, (char*)m_XCERRmod_MSG_53[g_language], WIN_3D | WIN_FILL, GET_GUIWIN );
	GUI_window_hwin_release( GET_GUIWIN );
	GUI_TopWIN_Set( hp );
	_cs_check_callback( hp );
	GUI_window_destory( hp );
	GUI_write_block_Extp( x1, y1, x2, y2, sec );
}


