/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块界面函数集合
* 创建时间:2010-11-19
* 所属公司 :科荟
* 文件名称:app_USBmod_interface.c
* 创建人:揭成
* 功能描述:提供系统设置界面画屏函数API
******************************************************************************************/
#include"app_USBmod_inside.h"
hwin_t* m_USB_win;
const char*m_usb_title[2]    = {"U盘管理",  "U Disk Manger"};
const char*m_usb_option[][2] =
{
	{"0.从U盘升级码表",       		""},
	{"1.从U盘升级字库",       		""},
	{"2.从U盘导入计划",       		""},
	{"3.升级帮助文件 ",       		""},
	{"4.导入数据到U盘",       		""},
	{"5.从U盘升级程序",       		""},
	{"6.备份修正数据",       		""},
	{"7.读取U盘修正数据",       	""},
	{"8.升级BMP文件",				""},
	{"9.增加应用程序",				""},
};
/********************************************************************
 * @创建人 :揭成
 * @功能 :USB主窗口
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
void app_USBmod_window_creat( void )
{
	Bsize_t hoffset = 0;
	Bsize_t voffset = 0;
	hwin_t*hp;
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Bgcolor( USBmod_BG_COLOR );
	GUI_clean_screen();
	GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( USBmod_FG_COLOR );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	m_USB_win = Gui_3D_window_creat((LCD_WIDE-540)/2,(LCD_HIGH-450)/2,540,450,( char* )m_usb_title[g_language], WIN_3D | WIN_FILL, GET_GUIWIN );

	GUI_window_hwin_req( m_USB_win );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	while ( voffset < ( SIZE_OF_ARRAY(m_usb_option) / 2 + 1 ) )
	{
		if ( hoffset < SIZE_OF_ARRAY(m_usb_option))
		{
			hp = Gui_button_creat(80, 20 + (voffset)*(85),150,35, app_global_Ntokey( hoffset ) | BUTTON_PAINT, m_USB_win);
			GUI_button_title_display( hp, (char* )m_usb_option[hoffset][g_language] );
			hoffset++;
		}
		if ( hoffset < SIZE_OF_ARRAY(m_usb_option))
		{
			hp = Gui_button_creat(310,20 + (voffset)*(85),150,35,app_global_Ntokey( hoffset ) | BUTTON_PAINT, m_USB_win);
			GUI_button_title_display( hp, ( char* )m_usb_option[hoffset][g_language] );
			hoffset++;
		}
		voffset++;
	}
	GUI_window_hwin_release( m_USB_win );
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}
void app_USBmod_window_destroy( void )
{
	GUI_window_destory( m_USB_win );
	return;
}
BOOL  app_USBmod_search_window_creat( void )
{
	hwin_t*hp;
	hwin_t*progress_bar;
	Bsize_t cnt=0;
	uint8_t key_buf;
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( USBmod_FG_COLOR );
	hp = Gui_3D_window_creat((LCD_WIDE-300)/2,(LCD_HIGH-200)/2,300,200,(char*)"提示",WIN_3D|WIN_FILL,GET_GUIWIN);
	progress_bar = Gui_progress_bar_creat(20,70,260,30,10,hp);
	GUI_window_hwin_release(GET_GUIWIN);

	GUI_window_hwin_req( hp );
	GUI_set_Bgcolor( USBmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( USBmod_FG_COLOR );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(20,40, "正在搜索USB设备..." );
	while ( ++cnt <110 )
	{
		if ( *pUSBstate == IS_USB_DISK_DEV_CONNECT&&(cnt>10))
		{
			break;
		}
		if ( cnt % 10 == 0 )
		{
			Gui_progress_bar_advance(progress_bar,1);
		}
		msleep( APP_100_MS );
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )
		{
			continue;
		}
		if ( key_buf == KEY_EXIT||key_buf==GUI_KEY_3D_WINDOW_CLOSE)
		{
			GUI_window_hwin_release( hp );
			GUI_window_destory( hp );	
			return FAIL;
		}
	}
	if ( cnt >109)
	{
		GUI_string_display_At(20,40, "搜索超时,请检查USB设备." );
		while ( *pUSBstate == USB_DEV_IS_IDLE)
		{
			msleep( 1 );
			if ( app_global_key_obtain( &key_buf, 1 ) != 1 )
			{
				continue;
			}
			if ( key_buf == KEY_EXIT||key_buf==GUI_KEY_3D_WINDOW_CLOSE)
			{
				GUI_window_hwin_release( hp );
				GUI_window_destory( hp );
				return FAIL;
			}
		}
	}
	GUI_window_hwin_release( hp );
	GUI_window_destory( hp );
	return TURE;
}
