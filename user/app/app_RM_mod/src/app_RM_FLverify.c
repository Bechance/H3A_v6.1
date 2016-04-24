/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:读表费率程序
* 创建时间:2013-09-30
* 所属公司 :科荟
* 文件名称:app_RM_FLverify.c
* 作者 :周炳权
* 功能描述:实现读表费率时段检查和设置功能
******************************************************************************************/
#include	"app_RM_inside.h"

hwin_t* m_RMwin_FLWIN  		= PNULL;
hwin_t* m_RMwin_FLedit 		= PNULL;
hwin_t* m_FL_edit_Wmeter 	= PNULL;
hwin_t* m_FL_edit_Rmeter 	= PNULL;
hwin_t* m_FL_droplist_Ntime = PNULL;
hwin_t* m_FL_droplist_Ntao 	= PNULL;
hwin_t* m_FL_droplist_FL 	= PNULL;

static  	uint8_t   _FLtask_id		=	INVALID_PRO;
static  	uint8_t   _FLrectask_id		=	INVALID_PRO;
/*1.费率数XX个,2.第一日时段表(第1~m时段起始时间和费率号hhmmNN),07协议为第一套数据
3.第二日时段表数据,4.第三日时段表数据,5.第四日时段表数据,6.第五日时段表数据,
7.第六日时段表数据,8.第七日时段表数据,9.第八日时段表数据,
*/

const uint8_t m_FLcmd_97_DI1[] = {0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,};
const uint8_t m_FLcmd_97_DI0[] = {0x13,0x30,0x40,0x50,0x60,0x70,0x80,0x90,0xa0,};

const uint8_t m_FLcmd_07_DI3[] = {0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,};
const uint8_t m_FLcmd_07_DI2[] = {0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,};
const uint8_t m_FLcmd_07_DI1[] = {0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};
const uint8_t m_FLcmd_07_DI0[] = {0x04,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,};

const char *m_FL_Ntime[]={"第一日时段表","第二日时段表","第三日时段表","第四日时段表",
						  "第五日时段表","第六日时段表","第七日时段表","第八日时段表"};

const char *m_FL_Ntao[] ={"第一套时区表(07规约)","第二套时区表(07规约)"};
const char *m_FL_feilv[]={"   "," 尖 "," 峰 "," 平 "," 谷 "};
	
/*写表时提示用户打开编程开关(若有编程开关),编程密码
"写表操作必须打开电能表编程开关,请检查编程开关是否打开,确定打开按<确定>写表"*/
//费率号:尖,峰,平,谷对应1,2,3,4;
//日时段数 97:m<= 10; 07:m<= 14;
uint8_t g_FL_Ntime_offset 		= 0;	//第几日时段表偏移
uint8_t g_FL_Ntao_offset 		= 0;	//第几套时区表偏移
uint8_t g_FL_feilv_offset 		= 0;	//费率选择偏移
uint8_t g_FL_rec_start_flag 	= 0;	//开始接收数据标志
uint8_t g_fl_write_offset 		= 0;	//费率写缓存偏移
uint8_t g_fl_read_offset 		= 0;	//费率读缓存偏移
uint8_t g_fl_Wnum 				= 0;	//费率数k<= 14(97)/63(07)
uint8_t g_fl_Rnum 				= 0;	//费率数k<= 14(97)/63(07)
uint8_t g_fl_send_read_offset	= 0;	//发送读取电表命令偏移(日)
uint8_t g_fl_send_read_offset2	= 0;	//发送读取电表命令偏移2(时段)
uint8_t g_fl_send_write_offset	= 0;	//发送写电表命令偏移
uint8_t g_fl_read_or_write		= 0;	//发送读或写表命令标志,0:读;1:写;
uint8_t g_fl_set_finish_97		= 0;	//97规约费率设置完成标志;

FL_buf_t	g_write_fl_buf[14] = { 0, };
FL_buf_t	g_read_fl_buf[8][14]  = { 0, };

#define  FL_Y_OFFSET	30
#define  DRAW_WIDE		317
/********************************************************************
 * @作者 :周炳权
 * @功能 :写费率初始化窗口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void write_FL_window( hwin_t *m_hp )
{
	hwin_t *hp = PNULL;
	GUI_window_hwin_req(m_hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(RM_BG_COLOR);
    GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_WINDIS_HLINE(0,DRAW_WIDE,33);
	GUI_string_display_At(73, FL_Y_OFFSET*0+5, "日时段费率设置");
	
	GUI_edit_memeber_add(5, FL_Y_OFFSET*1+5, 2,"时", MEMBER_3D|EDIT_FL_HOUR);
	GUI_edit_memeber_add(65, FL_Y_OFFSET*1+5, 2,"分", MEMBER_3D|EDIT_FL_MIN);
	GUI_string_display_At(150, FL_Y_OFFSET*1+5, "费率");

	m_FL_droplist_FL = 
		Gui_drop_list_creat(200,FL_Y_OFFSET*1+5,m_FL_feilv,SIZE_OF_ARRAY(m_FL_feilv),GUI_KEY_FL_FL,m_hp);
	
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*3+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*4+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*5+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*6+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*7+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*8+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*9+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*10+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*11+15);
	GUI_WINDIS_VLINE(43, FL_Y_OFFSET*3+15, FL_Y_OFFSET*11+15);
	GUI_WINDIS_VLINE(113, FL_Y_OFFSET*3+15, FL_Y_OFFSET*11+15);
	GUI_WINDIS_VLINE(156, FL_Y_OFFSET*3+15, FL_Y_OFFSET*11+15);
	GUI_WINDIS_VLINE(199, FL_Y_OFFSET*3+15, FL_Y_OFFSET*11+15);
	GUI_WINDIS_VLINE(269, FL_Y_OFFSET*3+15, FL_Y_OFFSET*11+15);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(5,  FL_Y_OFFSET*3+20, "序号");
	GUI_string_display_At(60, FL_Y_OFFSET*3+20, "时段");
	GUI_string_display_At(117, FL_Y_OFFSET*3+20,"费率");
	GUI_string_display_At(160, FL_Y_OFFSET*3+20,"序号");
	GUI_string_display_At(215, FL_Y_OFFSET*3+20,"时段");
	GUI_string_display_At(278, FL_Y_OFFSET*3+20,"费率");

	hp = Gui_button_creat( 20,FL_Y_OFFSET*2+10,75,28,BUTTON_PAINT|GUI_KEY_FLSET_SET|KEY_F1,m_hp );
	GUI_button_title_display( hp, "F1_设置" );
	hp = Gui_button_creat( 115,FL_Y_OFFSET*2+10,75,28,BUTTON_PAINT|GUI_KEY_FLSET_ADD|KEY_F2,m_hp );
	GUI_button_title_display( hp, "F2_添加" );
	hp = Gui_button_creat( 210,FL_Y_OFFSET*2+10,75,28,BUTTON_PAINT|GUI_KEY_FLSET_DELETE|KEY_F3,m_hp );
	GUI_button_title_display( hp, "F3_删除" );

	//hp = Gui_button_creat( 120,FL_Y_OFFSET*12-10,150,33,BUTTON_PAINT|GUI_KEY_FL_WRITE,m_hp );
	//GUI_button_title_display( hp, "写表时段" );
	
	GUI_window_hwin_release(m_hp);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :读费率初始化窗口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void read_FL_window( hwin_t *m_hp )
{
//	hwin_t *hp = PNULL;
	GUI_window_hwin_req(m_hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(RM_BG_COLOR);
    GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_WINDIS_HLINE(0,DRAW_WIDE,33);
	GUI_string_display_At(73, FL_Y_OFFSET*0+5, "日时段费率信息");
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(5, FL_Y_OFFSET*1+10, "提示: 序号:第几时段;时段:起始时间;F4:写表;F5:读表;");
	//GUI_string_display_At(1, FL_Y_OFFSET*2+10, "费率尖,峰,平,谷对应费率号1,2,3,4");
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*3+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*4+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*5+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*6+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*7+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*8+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*9+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*10+15);
	GUI_WINDIS_HLINE(0,DRAW_WIDE-2,FL_Y_OFFSET*11+15);
	GUI_WINDIS_VLINE(43, FL_Y_OFFSET*3+15, FL_Y_OFFSET*11+15);
	GUI_WINDIS_VLINE(113, FL_Y_OFFSET*3+15, FL_Y_OFFSET*11+15);
	GUI_WINDIS_VLINE(156, FL_Y_OFFSET*3+15, FL_Y_OFFSET*11+15);
	GUI_WINDIS_VLINE(199, FL_Y_OFFSET*3+15, FL_Y_OFFSET*11+15);
	GUI_WINDIS_VLINE(269, FL_Y_OFFSET*3+15, FL_Y_OFFSET*11+15);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(5,  FL_Y_OFFSET*3+20, "序号");
	GUI_string_display_At(60, FL_Y_OFFSET*3+20, "时段");
	GUI_string_display_At(117, FL_Y_OFFSET*3+20,"费率");
	GUI_string_display_At(160, FL_Y_OFFSET*3+20,"序号");
	GUI_string_display_At(215, FL_Y_OFFSET*3+20,"时段");
	GUI_string_display_At(278, FL_Y_OFFSET*3+20,"费率");
	//hp = Gui_button_creat( 100,FL_Y_OFFSET*12-10,150,33,BUTTON_PAINT|GUI_KEY_FL_READ,m_hp );
	//GUI_button_title_display( hp, "读表时段" );
	
	GUI_window_hwin_release(m_hp);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :创建窗口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_FL_window_create(void) 
{ 
	GUI_window_hwin_req(GET_GUIWIN);
  	GUI_set_Bgcolor(RM_BG_COLOR);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
    GUI_clean_screen();
	m_RMwin_FLWIN=Gui_3D_window_creat(2,2,636,475,"费率时段读取与设置",WIN_3D | WIN_FILL,GET_GUIWIN);
    GUI_window_hwin_req(m_RMwin_FLWIN);
	m_RMwin_FLedit
		= GUI_edit_creat(2,2,635,450,WIN_TOUCH_EN,m_RMwin_FLWIN);
	GUI_window_hwin_req( m_RMwin_FLedit );
	GUI_set_Bgcolor(RM_BG_COLOR);
	GUI_set_Fgcolor(RM_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_edit_memeber_add(10, FL_Y_OFFSET*0+5, 8,"写表权限密码:", MEMBER_3D|EDIT_FL_CODE);
	GUI_edit_memeber_add(295, FL_Y_OFFSET*0+5, 4,"费率个数:", MEMBER_3D|EDIT_FL_NUM|EDIT_DIS);
	GUI_string_display_At(10,FL_Y_OFFSET*1+14,"当前选取:");

	m_FL_droplist_Ntime =
		Gui_drop_list_creat(130,FL_Y_OFFSET*1+14,m_FL_Ntime,SIZE_OF_ARRAY(m_FL_Ntime),GUI_KEY_FL_NTSELECT,m_RMwin_FLedit);	
	m_FL_droplist_Ntao	=
		Gui_drop_list_creat(325,FL_Y_OFFSET*1+14,m_FL_Ntao,SIZE_OF_ARRAY(m_FL_Ntao),GUI_KEY_FL_SQSELECT,m_RMwin_FLedit);
	
	//hp = Gui_button_creat( 500,FL_Y_OFFSET*0+5,95,33,BUTTON_PAINT|GUI_KEY_FLSETTING|KEY_SETTING,m_RMwin_FLWIN );
	//GUI_button_title_display( hp, "设 置" );

	m_FL_edit_Wmeter = GUI_edit_creat(0,86,DRAW_WIDE,m_RMwin_FLWIN->win_high-86,WIN_3D|WIN_TOUCH_EN,m_RMwin_FLWIN);
	write_FL_window(m_FL_edit_Wmeter);
	m_FL_edit_Rmeter = GUI_edit_creat(DRAW_WIDE,86,DRAW_WIDE,m_RMwin_FLWIN->win_high-86,WIN_3D|WIN_TOUCH_EN,m_RMwin_FLWIN);
	read_FL_window(m_FL_edit_Rmeter);

	GUI_window_hwin_release(m_RMwin_FLedit);
	GUI_window_hwin_release(m_RMwin_FLWIN);		
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :费率时段显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void FL_SD_display( hwin_t *m_hp, FL_buf_t *m_buf )
{
	uint8_t m_offset = 0;
	GUI_window_hwin_req( m_hp );
	GUI_set_Bgcolor(RM_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	for ( m_offset = 0;m_offset < 14;m_offset++ )
	{
		if ( m_buf[m_offset].m_valid_flag == 1 )
		{
			if ( m_offset <= 6 )
			{
				GUI_int_display_At(7, FL_Y_OFFSET*(4+m_offset)+20, m_buf[m_offset].m_offset);
				GUI_string_display_At(55-8, FL_Y_OFFSET*(4+m_offset)+20, m_buf[m_offset].m_fl_hour);
				GUI_string_display_At(55+16, FL_Y_OFFSET*(4+m_offset)+20, ":");
				GUI_string_display_At(55+28, FL_Y_OFFSET*(4+m_offset)+20, m_buf[m_offset].m_fl_min);
				if ( m_buf[m_offset].m_fl[0] == 0 )
					 GUI_string_display_At(122, FL_Y_OFFSET*(4+m_offset)+20, "  ");
				else GUI_string_display_At(122, FL_Y_OFFSET*(4+m_offset)+20, m_buf[m_offset].m_fl);
			}
			else
			{
				GUI_int_display_At(162, FL_Y_OFFSET*(4+(m_offset-7))+20, m_buf[m_offset].m_offset);
				GUI_string_display_At(212-8, FL_Y_OFFSET*(4+(m_offset-7))+20, m_buf[m_offset].m_fl_hour);
				GUI_string_display_At(212+16, FL_Y_OFFSET*(4+(m_offset-7))+20, ":");
				GUI_string_display_At(212+28, FL_Y_OFFSET*(4+(m_offset-7))+20, m_buf[m_offset].m_fl_min);
				if ( m_buf[m_offset].m_fl[0] == 0 )
					 GUI_string_display_At(280, FL_Y_OFFSET*(4+(m_offset-7))+20, "  ");
				else GUI_string_display_At(280, FL_Y_OFFSET*(4+(m_offset-7))+20, m_buf[m_offset].m_fl);
			}
		}
		else
		{
			if ( m_offset <= 6 )
			{
				GUI_string_display_At(7, FL_Y_OFFSET*(4+m_offset)+20, "  ");
				GUI_string_display_At(55-8, FL_Y_OFFSET*(4+m_offset)+20, "     ");
				GUI_string_display_At(122, FL_Y_OFFSET*(4+m_offset)+20, "  ");
			}
			else
			{
				GUI_string_display_At(162, FL_Y_OFFSET*(4+(m_offset-7))+20, "  ");
				GUI_string_display_At(212-8, FL_Y_OFFSET*(4+(m_offset-7))+20, "     ");
				GUI_string_display_At(280, FL_Y_OFFSET*(4+(m_offset-7))+20, "  ");
			}
		}
	}
	GUI_window_hwin_release( m_hp );
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :密码和费率数显示
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void FLnum_code_display( uint8_t m_int )
{
	GUI_window_hwin_req( m_RMwin_FLedit );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At(m_meter_Password,EDIT_FL_CODE);
	GUI_edit_int_display_At(m_int,EDIT_FL_NUM);
	GUI_window_hwin_release( m_RMwin_FLedit );
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :界面销毁
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RMmod_FL_window_destory(void)
{
	GUI_window_destory(m_FL_edit_Wmeter);
	GUI_window_destory(m_FL_edit_Rmeter);
	GUI_window_destory(m_RMwin_FLedit);
    GUI_window_destory(m_RMwin_FLWIN);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :(编辑框设置回调函数)
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
uint8_t app_RMmod_FLcode_setting( hwin_t*hp )
{
	uint8_t key;
	Bsize_t member;
	char 	m_temp[4] = { 0, };
	
	GUI_window_hwin_req( hp );
	hand_wirte_input_box_pos_set(20,315);
	key_input_method_filter( KEY_INPUT_123 );
	GUI_window_hwin_release( hp );
	app_global_key_control_power_req();

	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )/*判断驱动是否收到按键响应*/
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( RM_FG_COLOR );
		if (( app_global_decide_inputkey( key ) ) != FAIL )/*判断是否是输入法相关按键按下*/
		{
			member = GUI_edit_id_obtain();     /*获取当前编辑成员ID*/
			if ( member == EDIT_FL_CODE )
			{
				key_method_input_shadow( key, m_meter_Password);
			}
			else if ( member == EDIT_FL_NUM )
			{
				key_method_input_shadow( key, m_temp);
			}
		}
		else if(key == KEY_DOWN)GUI_edit_shift( EDIT_NEXT);	
		else if(key == KEY_UP)	GUI_edit_shift( EDIT_LAST );
		GUI_window_hwin_release( hp );
		if ( key == KEY_ENTER || key == GUI_KEY_ENTER )
		{
			m_meter_Password[8] = ASCII_NULL;
			g_fl_Wnum = global_atoi(m_temp);
			break;
		}
		else if ( key == KEY_EXIT)
		{
			m_meter_Password[8] = ASCII_NULL;
			break;
		}
	}
	app_global_key_control_power_release();
	return key;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :(编辑框设置回调函数)
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
uint8_t app_RMmod_FLwrite_setting( hwin_t*hp )
{
	uint8_t key;
	Bsize_t member;

	GUI_window_hwin_req( hp );
	hand_wirte_input_box_pos_set(20,315);
	key_input_method_filter( KEY_INPUT_123 );
	GUI_window_hwin_release( hp );
	app_global_key_control_power_req();

	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )/*判断驱动是否收到按键响应*/
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( RM_FG_COLOR );
		if (( app_global_decide_inputkey( key ) ) != FAIL )/*判断是否是输入法相关按键按下*/
		{
			member = GUI_edit_id_obtain();     /*获取当前编辑成员ID*/
			if ( member == EDIT_FL_HOUR )
			{
				key_method_input_shadow( key, g_write_fl_buf[g_fl_write_offset].m_fl_hour);
			}
			else if ( member == EDIT_FL_MIN )
			{
				key_method_input_shadow( key, g_write_fl_buf[g_fl_write_offset].m_fl_min);
			}
			
		}
		else if(key == KEY_DOWN)GUI_edit_shift( EDIT_NEXT);	
		else if(key == KEY_UP)	GUI_edit_shift( EDIT_LAST );
		GUI_window_hwin_release( hp );
		if ( key == KEY_ENTER || key == GUI_KEY_ENTER )
		{
			break;
		}
		else if ( key == KEY_EXIT)
		{
			break;
		}
	}
	
	app_global_key_control_power_release();
	return key;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :费率时段设置规范处理
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void FL_SD_set_handle(void)
{
	if ( g_write_fl_buf[g_fl_write_offset].m_fl_hour[0] == 0 &&
		 g_write_fl_buf[g_fl_write_offset].m_fl_hour[1] == 0 )
	{
		g_write_fl_buf[g_fl_write_offset].m_fl_hour[0] = '0';
		g_write_fl_buf[g_fl_write_offset].m_fl_hour[1] = '0';
		g_write_fl_buf[g_fl_write_offset].m_fl_hour[2] = '\0';
	}
	else if ( g_write_fl_buf[g_fl_write_offset].m_fl_hour[0] == 0 )
	{
		g_write_fl_buf[g_fl_write_offset].m_fl_hour[0] = '0';
		g_write_fl_buf[g_fl_write_offset].m_fl_hour[2] = '\0';
	}
	else if ( g_write_fl_buf[g_fl_write_offset].m_fl_hour[1] == 0 )
	{
		g_write_fl_buf[g_fl_write_offset].m_fl_hour[1] = g_write_fl_buf[g_fl_write_offset].m_fl_hour[0];
		g_write_fl_buf[g_fl_write_offset].m_fl_hour[0] = '0';
		g_write_fl_buf[g_fl_write_offset].m_fl_hour[2] = '\0';
	}

	if ( g_write_fl_buf[g_fl_write_offset].m_fl_min[0] == 0 &&
		 g_write_fl_buf[g_fl_write_offset].m_fl_min[1] == 0 )
	{
		g_write_fl_buf[g_fl_write_offset].m_fl_min[0] = '0';
		g_write_fl_buf[g_fl_write_offset].m_fl_min[1] = '0';
		g_write_fl_buf[g_fl_write_offset].m_fl_min[2] = '\0';
	}
	else if ( g_write_fl_buf[g_fl_write_offset].m_fl_min[0] == 0 )
	{
		g_write_fl_buf[g_fl_write_offset].m_fl_min[0] = '0';
		g_write_fl_buf[g_fl_write_offset].m_fl_min[2] = '\0';
	}
	else if ( g_write_fl_buf[g_fl_write_offset].m_fl_min[1] == 0 )
	{
		g_write_fl_buf[g_fl_write_offset].m_fl_min[1] = g_write_fl_buf[g_fl_write_offset].m_fl_min[0];
		g_write_fl_buf[g_fl_write_offset].m_fl_min[0] = '0';
		g_write_fl_buf[g_fl_write_offset].m_fl_min[2] = '\0';
	}
	
	GUI_window_hwin_req( m_FL_edit_Wmeter );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At(g_write_fl_buf[g_fl_write_offset].m_fl_min, EDIT_FL_MIN);
	GUI_edit_string_display_At(g_write_fl_buf[g_fl_write_offset].m_fl_hour, EDIT_FL_HOUR);
	GUI_window_hwin_release( m_FL_edit_Wmeter );
	
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :费率时段设置合法性判断
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
uint8_t app_RMmod_FLSD_set_law_decide( void )
{
	uint8_t m_ret = 0;
	uint8_t m_hour = global_atoi(g_write_fl_buf[g_fl_write_offset].m_fl_hour);
	uint8_t m_min = global_atoi(g_write_fl_buf[g_fl_write_offset].m_fl_min);
	
	if ( ( m_hour >= 24) || ( m_min >= 60) ||
		 (g_FL_feilv_offset == 0) )
	{
		m_ret = 1;
	}
		 
	return m_ret;
}

/********************************************************************
* @作者 :周炳权
* @功能 :费率时段数据分析获取
* @输入 :*data
* @输出 :NONE
********************************************************************/
static void FL_SD_data_obtain( uint8_t m_offset,uint8_t *m_data )
{
	uint8_t m_data_buf = 0;
	
	g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_offset = m_offset+1;
	g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_offset = m_data[0];
	
	bcd_convert_hex(m_data[2],&m_data_buf);
	global_itoa(m_data_buf, g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour);
	bcd_convert_hex(m_data[1],&m_data_buf);
	global_itoa(m_data_buf, g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min);
	
	if ( g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_offset == 1 ) 
		global_memcpy(g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl, "尖", 2);
	else if ( g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_offset == 2 ) 
		global_memcpy(g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl, "峰", 2);
	else if ( g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_offset == 3 ) 
		global_memcpy(g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl, "平", 2);
	else if ( g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_offset == 4 ) 
		global_memcpy(g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl, "谷", 2);

	if ( g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[0] == 0 &&
		 g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[1] == 0 )
	{
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[0] = '0';
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[1] = '0';
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[2] = '\0';
	}
	else if ( g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[0] == 0 )
	{
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[0] = '0';
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[2] = '\0';
	}
	else if ( g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[1] == 0 )
	{
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[1] = 
			g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[0];
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[0] = '0';
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_hour[2] = '\0';
	}

	if ( g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[0] == 0 &&
		 g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[1] == 0 )
	{
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[0] = '0';
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[1] = '0';
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[2] = '\0';
	}
	else if ( g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[0] == 0 )
	{
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[0] = '0';
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[2] = '\0';
	}
	else if ( g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[1] == 0 )
	{
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[1] = 
			g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[0];
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[0] = '0';
		g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_fl_min[2] = '\0';
	}
	
	g_read_fl_buf[g_FL_Ntime_offset][m_offset].m_valid_flag = 1;
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :97规约写费率时段
* @输入 :m_offset:发送时段偏移
* @输出 :NONE
********************************************************************/
static void write_FLSD_to_meter_97( uint8_t m_offset )
{
	uint8_t m_cmd[2] = { 0,};
	uint8_t m_data_buf[4] = { 0,};
	uint8_t m_temp = 0;
	
	if ( g_write_fl_buf[m_offset].m_valid_flag == 1 )
	{
		m_cmd[1] = m_FLcmd_97_DI1[g_FL_Ntime_offset+1];
		m_cmd[0] = m_FLcmd_97_DI0[g_FL_Ntime_offset+1]+
				   g_write_fl_buf[m_offset].m_offset;
		m_data_buf[0] = g_write_fl_buf[m_offset].m_fl_offset;
		m_temp = global_atoi(g_write_fl_buf[m_offset].m_fl_hour);
		hex_convert_bcd(m_temp, &m_data_buf[2]);
		m_temp = global_atoi(g_write_fl_buf[m_offset].m_fl_min);
		hex_convert_bcd(m_temp, &m_data_buf[1]);
		app_RMmod_97_set_meter(m_cmd,(void *)m_data_buf,3);
	}
	else
	{
		g_FL_rec_start_flag = 0;
		g_fl_set_finish_97 = 1;
	}
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :07规约写费率时段
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void write_FLSD_to_meter_07( void )
{
	uint8_t m_cmd[4] = { 0,};
	uint8_t m_data_buf[14][3] = { 0,};
	uint8_t m_temp = 0;
	uint8_t m_offset = 0;
	
	m_cmd[3] = m_FLcmd_07_DI3[g_FL_Ntime_offset+1];
	m_cmd[2] = m_FLcmd_07_DI2[g_FL_Ntime_offset+1]+g_FL_Ntao_offset;
	m_cmd[1] = m_FLcmd_07_DI1[g_FL_Ntime_offset+1];
	m_cmd[0] = m_FLcmd_07_DI0[g_FL_Ntime_offset+1];

	for ( m_offset = 0;m_offset < 14;m_offset++ )
	{
		if (g_write_fl_buf[m_offset].m_valid_flag == 1)
		{
			m_data_buf[m_offset][0] = g_write_fl_buf[m_offset].m_fl_offset;
			m_temp = global_atoi(g_write_fl_buf[m_offset].m_fl_hour);
			hex_convert_bcd(m_temp, &m_data_buf[m_offset][2]);
			m_temp = global_atoi(g_write_fl_buf[m_offset].m_fl_min);
			hex_convert_bcd(m_temp, &m_data_buf[m_offset][1]);
		}
	}
	app_RMmod_07_set_meter(m_cmd,(void *)m_data_buf,14*3);
	
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :发送读表指令
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void FL_send_write_meter_cmd(void)
{
	if(m_meter_data.guiyue==0)
	{
		write_FLSD_to_meter_97(g_fl_send_write_offset);
		g_fl_send_write_offset++;
		if ( (g_fl_send_write_offset > 10) || (g_FL_rec_start_flag == 0))
		{
			g_fl_send_write_offset = 0;
			g_FL_rec_start_flag = 0;
			GUI_3D_window_title_mod(m_RMwin_FLWIN,"费率时段设置完成!");
		}
	}
	else if(m_meter_data.guiyue==1)
	{
		write_FLSD_to_meter_07();
		g_FL_rec_start_flag = 0;
	}
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :97规约数据分析
* @输入 :*data
* @输出 :NONE
********************************************************************/
static void FLhandle_guiyue_97(uint8_t*data)
{
	uint8_t offset1=0;
	uint8_t biaoshi1=0;
	uint8_t biaoshi2=0;
	uint8_t s_data = 0;
	uint8_t m_temp[3] = {0,};
	biaoshi1 = (data[11]-0x33) & 0xff;
	biaoshi2 = (data[10]-0x33) & 0xff;	

	if ( (biaoshi1 == 0xc3) && (biaoshi2 == 0x13) )
	{
		s_data = data[12] - 0x33;
		if ( (s_data & 0xff) == 0xff )	s_data = 0;
		bcd_convert_hex(s_data, &g_fl_Rnum);
		FLnum_code_display(g_fl_Rnum);
	}
	else if ( (biaoshi1 == 0xc3) && (((biaoshi2 & 0xf0) == 0x30)  || 
		((biaoshi2 & 0xf0) == 0x40)|| ((biaoshi2 & 0xf0) == 0x50) || 
		((biaoshi2 & 0xf0) == 0x60)|| ((biaoshi2 & 0xf0) == 0x70) || 
		((biaoshi2 & 0xf0) == 0x80)|| ((biaoshi2 & 0xf0) == 0x90) || 
		((biaoshi2 & 0xf0) == 0xa0)) )
	{
		for ( offset1= 0;offset1 <3;offset1++ )
	 	{
	 		if(	data[12+offset1] < 0x33 ) s_data = 0;
			else
			{
				s_data = data[12+offset1] - 0x33;
				if ( (s_data & 0xff) == 0xff )	s_data = 0;
			}
			m_temp[offset1] = s_data;
		}
		
		FL_SD_data_obtain(g_fl_read_offset,m_temp);
		FL_SD_display(m_FL_edit_Rmeter,g_read_fl_buf[g_FL_Ntime_offset]);
	}
		
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :07规约数据分析
* @输入 :*data
* @输出 :NONE
********************************************************************/
static void FLhandle_guiyue_07(uint8_t *data)
{
	uint8_t s_data = 0;
	uint8_t m_temp[3] = {0,};
	uint8_t offset1=0,offset2=0;
	uint8_t biaoshi1=0,biaoshi2=0,biaoshi3=0,biaoshi4=0;
	biaoshi1=(data[13]-0x33) & 0xff;
	biaoshi2=(data[12]-0x33) & 0xff;
	biaoshi3=(data[11]-0x33) & 0xff;	
	biaoshi4=(data[10]-0x33) & 0xff;

	if ( (biaoshi1 == m_FLcmd_07_DI3[0]) && (biaoshi2 == m_FLcmd_07_DI2[0]) &&
		 (biaoshi3 == m_FLcmd_07_DI1[0]) && (biaoshi4 == m_FLcmd_07_DI0[0]))
	{
		s_data = data[14] - 0x33;
		if ( (s_data & 0xff) == 0xff )	s_data = 0;
		bcd_convert_hex(s_data, &g_fl_Rnum);
		FLnum_code_display(g_fl_Rnum);
	}
	else if ( (biaoshi1 == 0x04) && (biaoshi3 == 0x00))
	{
		if ( (biaoshi2 == 0x01) || (biaoshi2 == 0x02))//第一套或第二套时区表
		{
			if ( biaoshi4 == 0x01 )
			{
				g_FL_Ntime_offset = 0;
			}
			else if ( biaoshi4 == 0x02 )
			{
				g_FL_Ntime_offset = 1;
			}
			else if ( biaoshi4 == 0x03 )
			{
				g_FL_Ntime_offset = 2;
			}
			else if ( biaoshi4 == 0x04 )
			{
				g_FL_Ntime_offset = 3;
			}
			else if ( biaoshi4 == 0x05 )
			{
				g_FL_Ntime_offset = 4;
			}
			else if ( biaoshi4 == 0x06 )
			{
				g_FL_Ntime_offset = 5;
			}
			else if ( biaoshi4 == 0x07 )
			{
				g_FL_Ntime_offset = 6;
			}
			else if ( biaoshi4 == 0x08 )
			{
				g_FL_Ntime_offset = 7;
			}
			for ( offset1 = 0;offset1 < 14;offset1++ )
			{
				for (offset2 = 0;offset2 < 3;offset2++ )
				{
					if(	data[12+offset1] < 0x33 ) s_data = 0;
					else
					{
						s_data = data[14+offset1*3+offset2] - 0x33;
						if ( (s_data & 0xff) == 0xff )	s_data = 0;
					}
					m_temp[offset2] = s_data;
				}
				g_fl_read_offset = offset1;
				FL_SD_data_obtain(g_fl_read_offset,m_temp);
			}
			FL_SD_display(m_FL_edit_Rmeter,g_read_fl_buf[g_FL_Ntime_offset]);
		}
	}
	
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :费率个数合法分析
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void FL_num_analysis( void )
{
	if ( (g_fl_Wnum > 14) && (m_offset_guiyue == 0))
	{
		g_fl_Wnum = g_fl_Rnum;
		MsgBox(200,200,200,200,
		"提示!","费率数设置不正确,DL/T 645-97规约规定费率数k<=14.",APP_HALF_SEC*10);
	}
	else if ( (g_fl_Wnum > 63) && (m_offset_guiyue == 1))
	{
		g_fl_Wnum = g_fl_Rnum;
		MsgBox(200,200,200,200,
		"提示!","费率数设置不正确,DL/T 645-07规约规定费率数k<=63.",APP_HALF_SEC*10);
	}
	else if ( g_fl_Wnum == 0 )
		g_fl_Wnum = g_fl_Rnum;
	FLnum_code_display(g_fl_Wnum);
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :初始化显示费率时段设置
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void FLSD_init_display( void )
{
	g_FL_feilv_offset = 0;
	GUI_window_hwin_req( m_FL_edit_Wmeter );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_select(EDIT_FL_HOUR);
	GUI_edit_clean();
	GUI_edit_select(EDIT_FL_MIN);
	GUI_edit_clean();
	GUI_drop_list_member_assign(m_FL_droplist_FL,g_FL_feilv_offset);
	GUI_window_hwin_release( m_FL_edit_Wmeter );
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :添加费率设置处理
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void FL_set_add_handle( void )
{
	g_write_fl_buf[g_fl_write_offset].m_offset = g_fl_write_offset+1;
	g_write_fl_buf[g_fl_write_offset].m_fl_offset = g_FL_feilv_offset;
	
	if ( g_FL_feilv_offset == 1 ) 
		global_memcpy(g_write_fl_buf[g_fl_write_offset].m_fl, "尖", 2);
	else if ( g_FL_feilv_offset == 2 ) 
		global_memcpy(g_write_fl_buf[g_fl_write_offset].m_fl, "峰", 2);
	else if ( g_FL_feilv_offset == 3 ) 
		global_memcpy(g_write_fl_buf[g_fl_write_offset].m_fl, "平", 2);
	else if ( g_FL_feilv_offset == 4 ) 
		global_memcpy(g_write_fl_buf[g_fl_write_offset].m_fl, "谷", 2);
	g_write_fl_buf[g_fl_write_offset].m_valid_flag = 1;
	g_fl_write_offset++;
	if ( g_fl_write_offset >= 14 ) g_fl_write_offset = 13;
	
	FL_SD_display(m_FL_edit_Wmeter,g_write_fl_buf);
	FLSD_init_display();
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :添加费率设置处理
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void FL_set_delete_handle( void )
{	
	GLOBAL_MEMCLR(&g_write_fl_buf[g_fl_write_offset], sizeof(FL_buf_t));
	if ( g_fl_write_offset == 0 ) g_fl_write_offset = 0;
	else g_fl_write_offset--;
	FL_SD_display(m_FL_edit_Wmeter,g_write_fl_buf);
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :添加费率设置处理
* @输入 :NONE
* @输出 :NONE
********************************************************************/
/*static uint8_t FL_SD_number_obtain( void )
{
	uint8_t m_ret = 0;
	uint8_t m_offset = 0;
	
	for ( m_offset = 0; m_offset < 14; m_offset++)
	{
		if ( g_write_fl_buf[m_offset].m_valid_flag == 1 )
			
	}
	
	return m_ret;
}*/

/********************************************************************
* @作者 :周炳权
* @功能 :模块发送读97协议数据命令
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void FL_send_read_meter_cmd_97(uint8_t offset)
{
	send_data_97_t send_cmd_97={0,};
	GLOBAL_MEMCLR(&send_cmd_97, sizeof(send_data_97_t));
	send_cmd_97.head[0]=0xFE;
	send_cmd_97.head[1]=0xFE;
	send_cmd_97.head[2]=0xFE;
	send_cmd_97.head1=0x68;	
	send_cmd_97.addr[0] = (( CharToHex(&m_meter_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[11]) & 0x0F );//0x99;
	send_cmd_97.addr[1] = (( CharToHex(&m_meter_addr[8]) & 0x0F ) << 4 ) + (CharToHex(& m_meter_addr[9]) & 0x0F );//0x99;
	send_cmd_97.addr[2] = (( CharToHex(&m_meter_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[7]) & 0x0F );//0x99;
	send_cmd_97.addr[3] = (( CharToHex(&m_meter_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[5]) & 0x0F );//0x99;
	send_cmd_97.addr[4] = (( CharToHex(&m_meter_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[3]) & 0x0F );//0x99;
	send_cmd_97.addr[5] = (( CharToHex(&m_meter_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter_addr[1]) & 0x0F );//0x99;
	send_cmd_97.head2=0x68;
	send_cmd_97.cmd=0x01;
	send_cmd_97.len=0x02;	
	send_cmd_97.biaoshi_L=m_FLcmd_97_DI0[offset]+0x33+g_fl_send_read_offset2;
	send_cmd_97.biaoshi_H=m_FLcmd_97_DI1[offset]+0x33;
	send_cmd_97.crc=global_sum_array((char*)&send_cmd_97.head1,sizeof(send_data_97_t)-5,IS_CHAR);
	send_cmd_97.end=0x16;
	app_RMmod_Usart3_send((char*)&send_cmd_97,sizeof(send_data_97_t));
	
	return;
}
/********************************************************************
* @作者 :周炳权
* @功能 :模块发送读07协议数据命令
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void FL_send_read_meter_cmd_07(uint8_t offset)
{
	send_data_07_t send_cmd_07={0,};
	GLOBAL_MEMCLR(&send_cmd_07, sizeof(send_data_07_t));
	send_cmd_07.head[0]=0xFE;
	send_cmd_07.head[1]=0xFE;
	send_cmd_07.head[2]=0xFE;
	send_cmd_07.head1=0x68;	
	send_cmd_07.addr[0] = (( CharToHex(&m_meter07_addr[10]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[11]) & 0x0F );//0x99;
	send_cmd_07.addr[1] = (( CharToHex(&m_meter07_addr[8]) & 0x0F ) << 4 ) + (CharToHex(& m_meter07_addr[9]) & 0x0F );//0x99;
	send_cmd_07.addr[2] = (( CharToHex(&m_meter07_addr[6]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[7]) & 0x0F );//0x99;
	send_cmd_07.addr[3] = (( CharToHex(&m_meter07_addr[4]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[5]) & 0x0F );//0x99;
	send_cmd_07.addr[4] = (( CharToHex(&m_meter07_addr[2]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[3]) & 0x0F );//0x99;
	send_cmd_07.addr[5] = (( CharToHex(&m_meter07_addr[0]) & 0x0F ) << 4 ) + ( CharToHex(&m_meter07_addr[1]) & 0x0F );//0x99;
	send_cmd_07.head2=0x68;
	send_cmd_07.cmd=0x11;
	send_cmd_07.len=0x04;	
	send_cmd_07.biaoshi[0]=m_FLcmd_07_DI0[offset]+0x33;
	send_cmd_07.biaoshi[1]=m_FLcmd_07_DI1[offset]+0x33;
	send_cmd_07.biaoshi[2]=m_FLcmd_07_DI2[offset]+0x33+g_FL_Ntao_offset;
	send_cmd_07.biaoshi[3]=m_FLcmd_07_DI3[offset]+0x33;
	send_cmd_07.crc=global_sum_array((char*)&send_cmd_07.head1,sizeof(send_data_07_t)-5,IS_CHAR);
	send_cmd_07.end=0x16;
	app_RMmod_Usart3_send((char*)&send_cmd_07,sizeof(send_data_07_t));
	
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :发送读表指令
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
static void FL_send_read_meter_cmd(void)
{
	if(m_meter_data.guiyue==0)
	{
		FL_send_read_meter_cmd_97(g_fl_send_read_offset);
	}
	else if(m_meter_data.guiyue==1)
	{
		FL_send_read_meter_cmd_07(g_fl_send_read_offset);
	}
	return;
}

/********************************************************************
* @作者 :周炳权
* @功能 :接收通讯数据分析任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void _FL_rectask(void*arg)
{
	Bsize_t size;
	Bsize_t Fsize = 5;
	uint8_t m_error = 0;
	//uint8_t m_cnt = 0;
	uint8_t rev_data[100];
	GLOBAL_MEMCLR(rev_data, 100);
	
	while(1)
	{
		msleep( TYPE_DELAY );
		
		if ( g_FL_rec_start_flag == 1 )
		{
			os_driver_ioctl(g_usart3_dev_id, SET_EMPTY_FIFO, 0);
			if ( g_fl_read_or_write == 0 )
				FL_send_read_meter_cmd();
			else if ( g_fl_read_or_write == 1 )
				FL_send_write_meter_cmd();
			if ( g_fl_set_finish_97 == 1 )
				continue;
			msleep(10);
			while ( Fsize )
			{
				size = os_driver_read( g_usart3_dev_id, &rev_data[0], 1, 0 );
				Fsize--;
				if( (size == 1) && (rev_data[0] == 0x68) )
				{
					Fsize = 5;
					break;
				}
				else msleep(5);
			}
			if ( (Fsize == 0) && ((rev_data[0] != 0x68) || (size != 1)))
			{
				Fsize = 5;
				GUI_3D_window_title_mod(m_RMwin_FLWIN,"与电能表通讯不良!");
				//m_cnt++;
				//if ( m_cnt > 5 )
				{
					g_FL_rec_start_flag = 0;
					//m_cnt = 0;
				}
				continue;
			}
			
			if ( m_offset_baud == 0 )	  msleep(80);	//自适应
			else if ( m_offset_baud == 1 )msleep(90);	//300
			else if ( m_offset_baud == 2 )msleep(55);	//600
			else if ( m_offset_baud == 3 )msleep(40);	//1200
			else if ( m_offset_baud == 4 )msleep(20);	//2400
			else if ( m_offset_baud == 5 )msleep(15);	//4800
			else if ( m_offset_baud == 6 )msleep(10);	//9600
			else if ( m_offset_baud == 7 )msleep(6);	//19200
			size = os_driver_read( g_usart3_dev_id, &rev_data[1], 7, 0 );
			if ( size != 7 || ( rev_data[7] != 0x68 ) )  //收第二个68H；
			{
				continue;
			}
			size = os_driver_read( g_usart3_dev_id, &rev_data[8], 2, 0 );
			if ( size != 2 )	  						//收控制码和长度,8为控制码,9长度;
			{
				continue;
			}
			size = os_driver_read( g_usart3_dev_id, &rev_data[10], rev_data[9], 0 );		
			size = os_driver_read( g_usart3_dev_id, &rev_data[10+rev_data[9]], 2, 0 );		
			if(size==2&&rev_data[10+rev_data[9]+1]==0x16)
			{
				if ( rev_data[9] == 0 )//从站正常应答
				{
					GUI_3D_window_title_mod(m_RMwin_FLWIN,"正在设置费率时段!");
					if(m_meter_data.guiyue==1)//07
						GUI_3D_window_title_mod(m_RMwin_FLWIN,"费率时段设置完成!");
					continue;
				}
				else if ( rev_data[9] == 1 )//从站异常应答
				{
					m_error = rev_data[10] - 0x33;
					if ( rev_data[8] == 0xD4 || rev_data[8] == 0xC4)//写表从站异常应答
					{
						if ( (m_error & 0x04) == 0x04 )
							 GUI_3D_window_title_mod(m_RMwin_FLWIN,"密码错或未授权,费率时段设置失败!");
						else 
							 GUI_3D_window_title_mod(m_RMwin_FLWIN,"异常应答其他错误,费率时段设置失败!");
						g_FL_rec_start_flag = 0;
						continue;
					}
					else if ( rev_data[8] == 0xD1 || rev_data[8] == 0xC1)//读表从站异常应答
					{
						if(m_meter_data.guiyue==0) //97
						{
							if ( g_fl_send_read_offset == (g_FL_Ntime_offset+1) )
							{
								g_fl_send_read_offset2++;
								g_fl_read_offset++;
								if ( g_fl_send_read_offset2 > 10 )
								{
									g_fl_send_read_offset2 = 0;
									g_FL_rec_start_flag = 0;
									GUI_3D_window_title_mod(m_RMwin_FLWIN,"费率时段读取完成,无部分请求数据!");
								}
							}
							else
							{
								g_fl_send_read_offset = g_FL_Ntime_offset+1;
								g_fl_send_read_offset2 = 1;
							}
						}
						else if(m_meter_data.guiyue==1)//07
						{
							g_FL_rec_start_flag = 0;
							GUI_3D_window_title_mod(m_RMwin_FLWIN,"异常应答,费率时段读取失败!");
						}
						continue;
					}
				}
			}
			if(m_meter_data.guiyue==0) //97
			{
				FLhandle_guiyue_97(rev_data);
				if ( g_fl_send_read_offset == (g_FL_Ntime_offset+1) )
				{
					g_fl_send_read_offset2++;
					g_fl_read_offset++;
					if ( g_fl_send_read_offset2 > 10 )
					{
						g_fl_send_read_offset2 = 0;
						g_FL_rec_start_flag = 0;
						GUI_3D_window_title_mod(m_RMwin_FLWIN,"费率时段读取完成!");
					}
				}
				else
				{
					g_fl_send_read_offset = g_FL_Ntime_offset+1;
					g_fl_send_read_offset2 = 1;
				}
			}
			else if(m_meter_data.guiyue==1)//07
			{
				FLhandle_guiyue_07(rev_data);
				if ( g_fl_send_read_offset == (g_FL_Ntime_offset+1) )
				{
					g_FL_rec_start_flag = 0;
					GUI_3D_window_title_mod(m_RMwin_FLWIN,"费率时段读取完成!");
				}
				else 
					g_fl_send_read_offset = g_FL_Ntime_offset+1;
			}			
		}
	}
}

/********************************************************************
* @作者 :周炳权
* @功能 :费率时段检查与设置任务
* @输入 :NONE
* @输出 :NONE
********************************************************************/
static void _FLtask(void*arg)
{
	uint8_t* key_buf = 0;
	uint8_t  err;

	while(1)
	{
		msleep( TYPE_DELAY );
		
		key_buf = (uint8_t*)os_q_pend(
					  m_RMmod_keymsg_qevent,
					  APP_100_MS, &err );          
		if ( err != OS_NO_ERR )
		continue;

		/*else if ( *key_buf == GUI_KEY_EDIT_START )
		{
	   	 	if ( Gui_start_editOBJ_obtain() == m_RMwin_FLedit )
	   	 	{
				GUI_edit_start( m_RMwin_FLedit,app_RMmod_FLcode_setting );
				FL_num_analysis();
			}
			else if ( Gui_start_editOBJ_obtain() == m_FL_edit_Wmeter )
			{
				GUI_edit_start( m_FL_edit_Wmeter,app_RMmod_FLwrite_setting );
				FL_SD_set_handle();
			}
			GUI_3D_window_title_mod(m_RMwin_FLWIN,"费率时段读取与设置");
		}*/
		if ( *key_buf == GUI_KEY_FLSETTING || *key_buf == KEY_SETTING)
		{
			GUI_edit_start( m_RMwin_FLedit,app_RMmod_FLcode_setting );
			FL_num_analysis();
			GUI_3D_window_title_mod(m_RMwin_FLWIN,"费率时段读取与设置");
		}
		else if ( *key_buf == GUI_KEY_FLSET_SET || *key_buf == KEY_F1)
		{
			GUI_edit_start( m_FL_edit_Wmeter,app_RMmod_FLwrite_setting );
			FL_SD_set_handle();
			GUI_3D_window_title_mod(m_RMwin_FLWIN,"费率时段读取与设置");
		}
		else if ( *key_buf == GUI_KEY_FL_FL )
		{
			g_FL_feilv_offset = GUI_drop_list_unfold(m_FL_droplist_FL);
		}
		else if ( *key_buf == GUI_KEY_FL_NTSELECT )
		{
			g_FL_Ntime_offset = GUI_drop_list_unfold(m_FL_droplist_Ntime);
			FL_SD_display(m_FL_edit_Rmeter,g_read_fl_buf[g_FL_Ntime_offset]);
		}
		else if ( *key_buf == GUI_KEY_FL_SQSELECT )
		{
			g_FL_Ntao_offset = GUI_drop_list_unfold(m_FL_droplist_Ntao);
		}
		else if ( *key_buf == GUI_KEY_FLSET_ADD || *key_buf == KEY_F2)
		{
			if ( app_RMmod_FLSD_set_law_decide() )
			{
				MsgBox(200,200,200,200,"提示!","费率时段设置不正确,请检查修改!",APP_HALF_SEC*10);
			}
			else 
				FL_set_add_handle();
		}
		else if ( *key_buf == GUI_KEY_FLSET_DELETE || *key_buf == KEY_F3)
		{
			FL_set_delete_handle();
		}
		else if ( *key_buf == GUI_KEY_FL_WRITE || *key_buf == KEY_F4)
		{
			if (MsgBox(200,200,200,200,"提示!","确保通讯正常后,若有编程开关,请打开电能表编程开关,打开后按<确定>写表",APP_HALF_SEC*10)
				== GUIE_KEY_ENTER )
			{
				if ( g_write_fl_buf[0].m_valid_flag == 1 )
				{
					g_fl_send_write_offset = 0;
					g_fl_read_or_write = 1;
					g_FL_rec_start_flag = 1;
					g_fl_set_finish_97 = 0;
					GUI_3D_window_title_mod(m_RMwin_FLWIN,"正在设置电能表费率时段信息");
				}
				else
					GUI_3D_window_title_mod(m_RMwin_FLWIN,"没有设置费率时段,请检查");
			}
		}
		else if ( *key_buf == GUI_KEY_FL_READ || *key_buf == KEY_F5)
		{
			if (MsgBox(200,200,200,200,"提示!","请确保通过通讯检测后再使用本功能,否则无法与电能表通讯,按<确定>读表",APP_HALF_SEC*10)
				== GUIE_KEY_ENTER )
			{
				g_fl_read_or_write = 0;
				g_FL_rec_start_flag = 1;
				g_fl_send_read_offset = 0;
				g_fl_send_read_offset2 = 0;
				g_fl_read_offset = 0;
				g_fl_set_finish_97 = 0;
				GUI_3D_window_title_mod(m_RMwin_FLWIN,"正在读取电能表费率时段信息");
			}
		}
		else if ( *key_buf == KEY_UP )
		{
			GUI_WINready(m_FL_droplist_Ntime,g_FL_Ntime_offset);
		}
		else if ( *key_buf == KEY_DOWN )
		{
			GUI_WINready(m_FL_droplist_FL,g_FL_feilv_offset);
		}
		else if ( *key_buf == KEY_RIGHT )
		{
			if(m_CURwidget != m_FL_droplist_Ntao
				|| m_pMember != m_CURwidget->subord_end )
		  	GUI_WINshift(0);
		}
		else if ( *key_buf == KEY_LEFT )
		{
			if(m_CURwidget!= m_FL_droplist_Ntime
					||m_pMember!=m_CURwidget->subord_head)
		  	GUI_WINshift(1);
		}			 
		else if ( *key_buf == KEY_ENTER )
		{
			GUI_WINdo();
			g_FL_feilv_offset = GUI_drop_list_member_obtain(m_FL_droplist_FL);
			g_FL_Ntime_offset = GUI_drop_list_member_obtain(m_FL_droplist_Ntime);
			if ( m_CURwidget == m_FL_droplist_Ntime )
			FL_SD_display(m_FL_edit_Rmeter,g_read_fl_buf[g_FL_Ntime_offset]);
			g_FL_Ntao_offset = GUI_drop_list_member_obtain(m_FL_droplist_Ntao);
		}
	}
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :读表校验费率时段检查与设置
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_RM_verify_funtion2( void )
{
	uint8_t key_buf;
	
	_FLtask_id 
		= os_task_assign_stk_build( _FLtask,3);
	_FLrectask_id 
		= os_task_assign_stk_build( _FL_rectask,5);
	
	app_RMmod_FL_window_create();
	GLOBAL_MEMCLR(g_write_fl_buf, sizeof(FL_buf_t)*14);
	GLOBAL_MEMCLR(g_read_fl_buf, sizeof(FL_buf_t)*14*8);
	FLnum_code_display(g_fl_Wnum);
	GUI_WINready(m_FL_droplist_Ntime,g_FL_Ntime_offset);
	while (1)
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )  //从驱动读取按键值
		{
			continue;
		}
		if ( key_buf == KEY_EXIT || key_buf == GUI_KEY_EXIT)
		{
			app_RMmod_FL_window_destory();
			if ( _FLtask_id != INVALID_PRO )
			{
				os_task_delete( _FLtask_id );
				_FLtask_id = INVALID_PRO;
			}
			if ( _FLrectask_id != INVALID_PRO )
			{
				os_task_delete( _FLrectask_id );
				_FLrectask_id = INVALID_PRO;
			}
			break;
		}
		else 
		{
			os_q_post(m_RMmod_keymsg_qevent,(void *)&key_buf);
		}
	}
	return;
}


