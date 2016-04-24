/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:删除外部应用程序
* 创建时间:2013-04-18
* 所属公司 :科荟
* 文件名称:app_startmod_DelExt.c
* 创建人 :揭成
* 功能描述:删除外部应用程序
******************************************************************************************/
#include"app_startmod_inside.h"
#include <stdlib.h>
static hwin_t*   m_DelCheckbox[30];

void app_Extapp_uninstall( char*name )
{
	Extapp_name_t Ename;
	FIL* fp;
	UINT w ;
	uint32_t lseek, lseekT;
	char sname[20] = "";
	fp = fatfs_open( "EXPLIST", FA_OPEN_ALWAYS | FA_READ | FA_WRITE );
	if ( !fp ) return;
	while ( 1 )
	{
		f_read( fp, &Ename, sizeof( Extapp_name_t ), &w );
		if ( w == 0 )break;
		if ( w == sizeof( Extapp_name_t ) )
		{
			if ( global_strlen( name ) != global_strlen( Ename.Exe ) )
				continue;
			if ( !global_memcmp( Ename.Exe, name, global_strlen( name ) ) )
				continue;
			global_strcpy( sname, "EXP/" );
			global_strcat( sname, Ename.Exe );
			f_unlink( sname );
			if ( fp->fsize == sizeof( Extapp_name_t ) )
			{
				fatfs_close( fp );
				f_unlink( "EXPLIST" );
				return;
			}
			lseekT = fp->fptr - sizeof( Extapp_name_t );
			lseek  = fp->fsize - sizeof( Extapp_name_t );
			f_lseek( fp, lseek );
			f_read( fp, &Ename, sizeof( Extapp_name_t ), &w );
			f_lseek( fp, lseek );
			f_truncate( fp );
			lseek = lseekT;
			if ( lseek < fp->fsize )
			{
				f_lseek( fp, lseek );
				f_write( fp, &Ename, sizeof( Extapp_name_t ), &w );
			}
			break;
		}
	}
	fatfs_close( fp );
	return;
}


void app_Extapp_uninstall_all( void )
{
	FRESULT RET;
	Extapp_name_t Ename;
	FIL* fp;
	UINT w ;
	char name[20] = "";
	RET = RET;
	fp = fatfs_open( "EXPLIST", FA_OPEN_ALWAYS | FA_READ );
	if ( !fp ) return;
	while ( 1 )
	{
		f_read( fp, &Ename, sizeof( Extapp_name_t ), &w );
		if ( w == 0 )break;
		if ( w == sizeof( Extapp_name_t ) )
		{
			GLOBAL_MEMCLR( name, 20 );
			global_strcpy( name, "EXP/" );
			global_strcat( name, Ename.Exe );
			RET = f_unlink( name );
		}
	}
	fatfs_close( fp );
	RET = f_unlink( "EXPLIST" );
	return;
}

static void _1Appgraph_create( hwin_t*pOBJ, int startpos, int endpos )
{
	int pos;
	int wide, high, x, x0, y0;
	Extapp_name_t*pCUR;
	int   xgap, ygap, nline, nrow ;
	int   ypos = 0;
	wide  =  GRAPH_WIDE;
	high  =  GRAPH_HIGH;
	nline =  GRAPH_LINE;
	nrow  =  GRAPH_ROW;
	xgap  = ( pOBJ->win_wide - wide * nrow ) / ( nrow + 1 );
	ygap  = ( pOBJ->win_high - ( high + 16 ) * nline ) / ( nline + 1 );
	GUI_window_hwin_req( pOBJ );
	GUI_set_Bgcolor( C_GUI_BlACK );
	GUI_set_Fgcolor( C_GUI_WHITE );
	GUI_clean_window();
	GUI_window_hwin_release( pOBJ );
	for ( pos = startpos;pos < endpos;pos++ )
	{
		GUI_window_hwin_req( pOBJ );
		GUI_set_Bgcolor( C_GUI_BlACK );
		GUI_set_Fgcolor( C_GUI_WHITE );
		GUI_SetFront( &GUI_HZK16_EXTASCII );
		x0   = xgap + ( ypos % nrow ) * ( wide + xgap );
		y0   = ygap + ( ypos / nrow ) * (( high + 16 ) + ygap );
		pCUR = &m_apparray[pos];
		x = ( wide - global_strlen( pCUR->name ) * 8 ) / 2 + x0;
		app_startmod_BMPfile_view( pOBJ->win_x + x0, pOBJ->win_y + y0, pCUR->UNpressBMP );
		m_DelCheckbox[pos] = GUI_checkBox_create( pCUR->name, C_GUI_WHITE, x, y0 + high + 5, WIN_TOUCH_EN | ( 100 + pos ), pOBJ );
		ypos++;
		GUI_window_hwin_release( pOBJ );
		m_menu_mark_pos = startpos;
		GRAPH_MARK( m_menu_mark_pos );
	}
	return;
}

static void _1txtWIN_paint( void )
{
	m_pTXT = Gui_txtbox_creat( 10, 0, m_3Dwin->win_wide - WIDE_B, m_3Dwin->win_high - 35, WIN_3D, m_3Dwin );

}

static void _1last_button_callblack( void*arg )
{
	GUI_window_destory( m_pTXT );
	_1txtWIN_paint();
	m_menu_start_pos -= ( GRAPH_ROW * GRAPH_LINE );
	m_menu_end_pos = m_menu_start_pos + ( GRAPH_ROW * GRAPH_LINE );
	_1Appgraph_create( m_pTXT, m_menu_start_pos, m_menu_end_pos );;
	return;
}


static void _1next_button_callblack( void*arg )
{
	int dx;
	dx = m_array_pos - m_menu_end_pos;
	GUI_window_destory( m_pTXT );
	_1txtWIN_paint();
	if ( dx > ( GRAPH_ROW*GRAPH_LINE ) )
	{
		m_menu_start_pos  = m_menu_end_pos;
		m_menu_end_pos    = m_menu_start_pos + ( GRAPH_ROW * GRAPH_LINE );
		_1Appgraph_create( m_pTXT, m_menu_start_pos, m_menu_end_pos );
	}
	else
	{
		m_menu_start_pos  = m_menu_end_pos;
		m_menu_end_pos    = m_menu_start_pos + dx;
		_1Appgraph_create( m_pTXT, m_menu_start_pos, m_menu_end_pos );
	}
	return;
}


static hwin_t*app_startmod_DelExp_WINcreate( void )
{
	hwin_t*hp;
	hwin_t*pobj;

	int pos;
	pos = pos;
	GUI_window_hwin_req( GET_GUIWIN );
	GUI_set_Bgcolor( 0 );
	GUI_clean_screen();
	GUI_set_Bgcolor( Startmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( Startmod_FG_COLOR );
	hp = Gui_3D_window_creat( 5, 5, 790, 590, "删除应用程序", WIN_3D | WIN_FILL | WIN_CLOSE_ICON_DIS, GET_GUIWIN );
	GUI_window_hwin_req( hp );
	GLOBAL_MEMCLR( m_apparray, sizeof( Extapp_name_t )*30 );
	m_array_pos 	 = 0;
	m_menu_start_pos = 0;
	m_menu_end_pos   = 0;
	_1txtWIN_paint();
	app_startmod_Expapp_obtain();
	if ( m_array_pos > ( GRAPH_ROW*GRAPH_LINE ) )
	{
		_1Appgraph_create( m_pTXT, 0, GRAPH_ROW*GRAPH_LINE );
		m_menu_end_pos += ( GRAPH_ROW * GRAPH_LINE );
		GUI_SetFront( &GUI_HZK16_EXTASCII );
		m_button_last = Gui_button_creat( m_3Dwin->win_wide - 200, m_3Dwin->win_high - 40, 80, 30, 150 | BUTTON_PAINT, m_3Dwin );
		GUI_button_title_display( m_button_last, "上一页" );
		GUI_button_callback_mount( m_button_last, _1last_button_callblack );
		m_button_next = Gui_button_creat( m_3Dwin->win_wide - 100, m_3Dwin->win_high - 40, 80, 30, 150 | BUTTON_PAINT, m_3Dwin );
		GUI_button_title_display( m_button_next, "下一页" );
		GUI_button_callback_mount( m_button_next, _1next_button_callblack );
	}
	else
	{
		m_menu_end_pos = m_array_pos;
		_1Appgraph_create( m_pTXT, 0, m_array_pos );
		if ( m_array_pos == 0 )
		{
			GUI_3D_window_title_mod( hp, "暂不存在可以卸载的应用." );
		}
	}
	GUI_set_Bgcolor( Startmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_RED );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	GUI_string_display_At( 20, m_3Dwin->win_high - 25, "请选择需要删除的应用程序,按确认键选择,按F5删除,过程不可逆." );
	GUI_set_Fgcolor( Startmod_FG_COLOR );
	pobj = Gui_button_creat( m_3Dwin->win_wide - WIDE_B + 35, 250, 100, 40, BUTTON_PAINT | KEY_DEL, hp );
	GUI_button_title_display( pobj, "删除" );
	pobj = Gui_button_creat( m_3Dwin->win_wide - WIDE_B + 35, 320, 100, 40, BUTTON_PAINT | KEY_EXIT, hp );
	GUI_button_title_display( pobj, "退出" );
	GUI_window_hwin_release( hp );
	GUI_window_hwin_release( GET_GUIWIN );
	return hp;
}

void  app_startmod_DelExp_entery( void )
{
	uint8_t     key_buf;
	int         pos, oldmark;
	hwin_t*		hp;
	hwin_t*		pOBJ;
	BOOL   		state;
	hp = app_startmod_DelExp_WINcreate();
	while ( 1 )
	{
		msleep( 1 );
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 )
		{
			oldmark = m_menu_mark_pos;
			if ( key_buf == KEY_DEL )
			{
				for ( pos = 0;pos < m_array_pos;pos++ )
				{
					if ( GUI_checkbox_state_obtain( m_DelCheckbox[pos] ) )
					{
						app_Extapp_uninstall( m_apparray[pos].Exe );
					}
				}
				GUI_window_destory( hp );
				hp = app_startmod_DelExp_WINcreate();
			}
			else if ( key_buf == KEY_EXIT )
			{
				GUI_window_destory( hp );
				break;
			}
			else if (( key_buf == KEY_LEFT ) && ( m_menu_mark_pos != m_menu_start_pos ) )
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
			else if (( key_buf == KEY_UP ) && ( m_menu_mark_pos - GRAPH_ROW ) >= m_menu_start_pos )
			{
				m_menu_mark_pos -= 3;
			}
			else if ( KEY_ENTER == key_buf )
			{
				if ( m_menu_mark_pos >= m_menu_end_pos )
					continue;
				if ( m_menu_mark_pos < m_menu_start_pos )
					continue;
				pOBJ = m_DelCheckbox[m_menu_mark_pos];
				state = GUI_checkbox_state_obtain( pOBJ );
				if ( state )
				{
					GUI_checkBox_select( pOBJ, 0 );
				}
				else
				{
					GUI_checkBox_select( pOBJ, 1 );
				}
			}
			if ( m_menu_mark_pos != oldmark )
			{
				GRAPH_RMARK( oldmark );
				GRAPH_MARK( m_menu_mark_pos );
			}
		}
	}
}
