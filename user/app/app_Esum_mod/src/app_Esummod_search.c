/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:查询模块主体程序
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_Searchmod_view.c
* 作者 :
* 功能描述:实现查询数据显示
******************************************************************************************/
#include		    "app_Esummod_inside.h"

static hwin_t*		_Searchmod_win = PNULL;
static hwin_t*		_Searchmod_pc = PNULL;
static hwin_t*		_Searchmod_scrb = PNULL;
static hwin_t*		_Searchmod_seek = PNULL;
static hwin_t*		_Searchmod_USER = PNULL;


#define  	LINE_HIGH        	 25
typedef 	Esum_save_arg_t    	 XXdata_t;
#define    	MSG_WIN              _Searchmod_pc
#define    	SCRB_WIN             _Searchmod_scrb

#define 	GET_YLINE(LINE) (10 +22*LINE)
static void _serachmod_UserWIN_create(hwin_t*p)
{
	_Searchmod_USER = GUI_edit_creat(350,160,285,250,WIN_PAINT, p );
	GUI_window_hwin_req( _Searchmod_USER );
	GUI_set_Bgcolor(g_win_Bg_color);
	GUI_set_Fgcolor(g_Fg_color);
	GUI_SetFront(&GUI_HZK16_EXTASCII); 
    GUI_string_display_At(5, GET_YLINE( 0 ),      "工单号 :");
    GUI_string_display_At(5, GET_YLINE( 1 ),      "用户名 :");
    GUI_string_display_At(5, 10+GET_YLINE( 3 ),   "电表号1:");
    GUI_string_display_At(5, 20+GET_YLINE( 5 ),   "电表号2:");
    GUI_string_display_At(5, 30+GET_YLINE( 7 ),   "校验员 :");
    GUI_string_display_At(5, 30+GET_YLINE( 8 ),   "  时间 :");
	GUI_window_hwin_release( _Searchmod_USER );
    return;
}
static void  _Searchmod_Userdata_view(DL_app_user_info_t*p)
{
    int high;
	GUI_window_hwin_req( _Searchmod_USER );
	GUI_set_Bgcolor(g_win_Bg_color);
	GUI_set_Fgcolor(g_Fg_color);
    high  =30+GET_YLINE(8)+25+5;
    GUI_fill_window_area(67,GET_YLINE( 0 ),278, high);
	GUI_WINDIS_HLINE(67,277,GET_YLINE( 0 )+17);
	GUI_WINDIS_HLINE(67,277,GET_YLINE( 1 )+17);
	GUI_WINDIS_HLINE(67,277,GET_YLINE( 2 )+17);
	GUI_WINDIS_HLINE(67,277,10+GET_YLINE( 3 )+17);
	GUI_WINDIS_HLINE(67,277,10+GET_YLINE( 4 )+17);
	GUI_WINDIS_HLINE(67,277,20+GET_YLINE( 5 )+17);
	GUI_WINDIS_HLINE(67,277,20+GET_YLINE( 6 )+17);
	GUI_WINDIS_HLINE(67,277,30+GET_YLINE(7)+17);
	GUI_WINDIS_HLINE(67,277,30+GET_YLINE(8)+17);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_SetLineHigh(6);
	GUI_string_display_At(69,GET_YLINE( 0 ),p->wrok_list);
	GUI_string_display_At(69,GET_YLINE( 1 ),p->user_name);
	GUI_string_display_At(69,10+GET_YLINE( 3 ),p->meter_id1);
	GUI_string_display_At(69,20+GET_YLINE( 5 ),p->meter_id2);
	GUI_string_display_At(69,30+GET_YLINE( 7 ),p->worker);
	GUI_string_display_At(69,30+GET_YLINE( 8 ),p->time_buf);
	GUI_window_hwin_release( _Searchmod_USER ); 
	return;
}

static void _WINpaint( void )
{
	hwin_t*p, *pc, *scb, *pb;
	GUI_window_hwin_req( GET_GUIWIN );
	GUI_set_Bgcolor( g_Bg_color );
	GUI_clean_screen();
	GUI_set_Bgcolor( g_win_Bg_color );
	GUI_set_Fgcolor( g_Fg_color );
	p = Gui_3D_window_creat( 0, 0, LCD_WIDE-2,LCD_HIGH, "", WIN_3D | WIN_FILL, 0);
	GUI_window_hwin_req( p );
	GUI_set_Bgcolor( 0 );
	pc = Gui_txtbox_creat( 5, 20,340,p->win_high - 20, WIN_3D | WIN_FILL, p );
	scb = Gui_scrb_attach( pc, 25);
	GUI_set_Fgcolor( g_Fg_color );
	GUI_set_Bgcolor( g_win_Bg_color );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	GUI_string_display_At(6,  2, "序号" );
	GUI_string_display_At(96, 2, "年-月-日/时" );
	_Searchmod_win  = p;
	_Searchmod_pc   = pc;
	_Searchmod_scrb = scb;
	GUI_MsgFIFO_create( _Searchmod_pc, 2 );
	GUI_window_hwin_req( pc );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	GUI_window_hwin_release( pc );
    _serachmod_UserWIN_create(p);
	pb = Gui_button_creat(p->win_wide-265, p->win_high - 30,90, 25, BUTTON_PAINT|KEY_F4,p);
	GUI_button_title_display( pb, "删除" );
	pb = Gui_button_creat(p->win_wide-165, p->win_high - 30,90, 25, BUTTON_PAINT|KEY_F5,p);
	GUI_button_title_display( pb, "全删(慎用)" );
	GUI_window_hwin_release( p );
	GUI_window_hwin_release( GET_GUIWIN );
	return;
}


static void _WINdestory( void )
{
	GUI_window_destory( _Searchmod_win );
	return;
}

/****************************************************
* @作者 :揭成
* @功能 :删除所有记录
* @输入 :
* @输出 :NONE
****************************************************/
static void record_delete_all( page_ctl_t*p )
{ 
	if(MsgPasswordBox( LCD_WIDE / 2, LCD_HIGH / 2, "1830" ))
	{
	    p->Maxsize = 0;
		f_unlink(p->FILname);
	}
	return;
}
/****************************************************
* @作者 :揭成
* @功能 :删除一条记录
* @输入 :
* @输出 :NONE
****************************************************/
static void record_delete_1( const TCHAR *path, uint32_t record_num )
{
	UINT w;
	uint32_t fsize;
	uint32_t lseek;
	XXdata_t pbuf;
	FIL*fp;
	fp = fatfs_open( path, FA_OPEN_EXISTING | FA_READ |FA_WRITE);
	if(!fp) return;
	if ( fp->fsize == sizeof( XXdata_t ) )
	{
		fatfs_close( fp );
		f_unlink( path );
		return;
	}
	fsize = fp->fsize;
	lseek = fsize - sizeof( XXdata_t );
	f_lseek( fp, lseek );
	f_read( fp, &pbuf, sizeof( XXdata_t ), &w );
	f_lseek( fp, lseek );
	f_truncate( fp );
	lseek = record_num * sizeof( XXdata_t );
	if ( lseek < fp->fsize )
	{
		f_lseek( fp, lseek );
		f_write( fp, &pbuf, sizeof( XXdata_t ), &w );
	}
	fatfs_close( fp );
	return;
}

static int record_read( void*pD, page_ctl_t*p )
{
	UINT w;
	FIL*fp;
	XXdata_t*pdata = ( XXdata_t* )pD;
	fp = fatfs_open( p->FILname, FA_OPEN_EXISTING | FA_READ );
	if(!fp) return 0;
	f_lseek( fp, p->Filepos_S*sizeof( XXdata_t ) );
	f_read( fp, pdata, sizeof( XXdata_t )*p->Pagesize, &w );
	fatfs_close( fp );
	return ( w / sizeof( XXdata_t ) );
}


static void record_view( void*pD, page_ctl_t*p )
{
	int index, dx;
	char ch[4] = "";
	XXdata_t*pdata = ( XXdata_t* )pD;
	GUI_window_hwin_req( MSG_WIN );
	GUI_set_Bgcolor( 0 );
	GUI_set_Fgcolor( g_Fg_color );
    GUI_SetFront( &GUI_HZK16_EXTASCII );
	GUI_fill_Rect_At(
		MSG_WIN->win_x + 1,
		MSG_WIN->win_y + 2,
		MSG_WIN->win_x + MSG_WIN->win_wide - 1,
		MSG_WIN->win_y + MSG_WIN->win_high - 50 );
    if(p->Maxsize==0)
    {
		GUI_string_display_At(10, 5,"暂无记录!");
		GUI_window_hwin_release( MSG_WIN );
	     while(1)
		 {
			if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
			{
				app_free( pD );
				os_task_delete( SELF_PRO );
			}
			msleep( 1 );
	     }
    }
	dx = p->Filepos_E - p->Filepos_S;
	for ( index = 0;index < dx;index++ )
	{
		global_itoa(( p->Filepos_S + index ), ch );
		GUI_string_display_At(6, 8 + LINE_HIGH*index, ch );
		GUI_string_display_At(96,8 + LINE_HIGH*index, pdata->DL_app_user_info.time_buf );
		pdata++;
	}
	GUI_string_display_At( 10, MSG_WIN->win_high -20, "第" );
	GUI_int_display( p->Filepos_S / p->Pagesize+ 1 );
	GUI_string_display( "页  " );
	GUI_window_hwin_release( MSG_WIN );
	return;
}

static void record_mark( void*pD, int state, page_ctl_t*p )
{
	int x0, y0, x1, y1;
	uint16_t FG, BG;
	char ch[4] = "";
	XXdata_t*pdata = ( XXdata_t* )pD;
	if ( state )
	{
		FG = C_GUI_BlACK;
		BG = C_GUI_YELLOW;
	}
	else
	{
		FG = C_GUI_WHITE;
		BG = C_GUI_BlACK;
	}
	pdata += p->Markpos;
	GUI_window_hwin_req( MSG_WIN );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	GUI_set_Bgcolor( BG );
	GUI_set_Fgcolor( FG );
	x0 = 1;
	y0 = 2 + LINE_HIGH * p->Markpos - 1;
	x1 = MSG_WIN->win_wide - 5 + x0;
	y1 = LINE_HIGH + y0 + 2;
	GUI_fill_window_area( x0, y0, x1, y1 );
	global_itoa(( p->Filepos_S + p->Markpos ), ch );
	GUI_string_display_At( 6,  8 + LINE_HIGH*p->Markpos, ch );
	GUI_string_display_At( 96, 8 + LINE_HIGH*p->Markpos, pdata->DL_app_user_info.time_buf );
	GUI_window_hwin_release( MSG_WIN );
	if(state)_Searchmod_Userdata_view(&pdata->DL_app_user_info);
	return;
}

static void record_page_obtain( page_ctl_t*p )
{
	FIL*fp;
	fp = fatfs_open( p->FILname, FA_OPEN_EXISTING | FA_READ );
	if(!fp){p->Maxsize=0;p->nPage=0;return;}
	p->Maxsize  = fp->fsize / sizeof( XXdata_t );
	p->nPage    = ( p->Maxsize + p->Pagesize - 1 ) / p->Pagesize;
	fatfs_close( fp );
	GUI_window_hwin_req( MSG_WIN );
	GUI_set_Bgcolor( 0 );
	GUI_set_Fgcolor( g_Fg_color );
	GUI_SetFront( &GUI_HZK16_EXTASCII);
	GUI_string_display_At( 70, MSG_WIN->win_high -20, "共" );
	GUI_int_display( p->Maxsize);
	GUI_string_display( "条记录" );
	GUI_window_hwin_release( MSG_WIN );
	return;
}

static void recordKey_Exc( page_ctl_t*p, uint8_t keyMSG, XXdata_t*pdata )
{
	if ( KEY_DOWN == keyMSG )
	{
		if ( p->Markpos < ( p->CURsize - 1 ) )
		{
			record_mark( pdata, 0, p );
			p->Markpos++;
			record_mark( pdata, 1, p );
		}
		else
		{
			p->ofg = 1;
			GUI_keyMSG_post( MSG_WIN, GUI_KEY_NEXT_PAGE );
		}
	}
	else if ( KEY_UP == keyMSG )
	{
		if ( p->Markpos > 0 )
		{
			record_mark( pdata, 0, p );
			p->Markpos--;
			record_mark( pdata, 1, p );
		}
		else
		{
			p->ofg = 1;
			GUI_keyMSG_post( MSG_WIN, GUI_KEY_LAST_PAGE );
		}
	}
	else if ( keyMSG == GUI_KEY_NEXT_PAGE&&(p->nPage>1))
	{
		if ( p->Filepos_E < p->Maxsize )
		{
			p->Filepos_S = p->Filepos_E;
		}
        else
        {
			p->Filepos_S = 0;
		}
		p->Markpos = 0;
		p->CURsize = record_read( pdata, p );
		p->Filepos_E = p->Filepos_S + p->CURsize;
		record_view( pdata, p );
		record_mark( pdata, 1, p );
		p->CURpage = p->Filepos_S / p->Pagesize;
		if ( p->ofg == 1 )
		{
			Gui_scrb_Setpos( SCRB_WIN, p->CURpage );
			p->ofg = 0;
		}
	}
	else if ( keyMSG == GUI_KEY_LAST_PAGE&&(p->nPage>1))
	{
		if ( p->Filepos_S > 0 )
		{
		    p->Filepos_S = p->Filepos_S - p->Pagesize;
		}
		else
		{
		    p->Filepos_S = (p->nPage-1)*p->Pagesize;
		}
		p->Markpos = 0;
		p->CURsize = record_read( pdata, p );
		p->Filepos_E = p->Filepos_S + p->CURsize;
		record_view( pdata, p );
		record_mark( pdata, 1, p );
		p->CURpage = p->Filepos_S / p->Pagesize;
		if ( p->ofg == 1 )
		{
			Gui_scrb_Setpos( SCRB_WIN, p->CURpage );
			p->ofg = 0;
		}
	}
	else if ( keyMSG == GUI_KEY_NPAGE )
	{
		p->Markpos = 0;
		p->CURpage = Gui_scrb_Getpos( SCRB_WIN );
		p->Filepos_S = p->CURpage * p->Pagesize;
		p->CURsize = record_read( pdata, p );
		p->Filepos_E = p->Filepos_S + p->CURsize;
		record_view( pdata, p );
		record_mark( pdata, 1, p );
	}
	else if(keyMSG == KEY_ENTER)
	{
	     _WINdestory();
         app_Esummod_record_display((void*)(pdata+p->Markpos));
		 _WINpaint();
		 record_view( pdata, p );
		 record_mark( pdata, 1, p );
		 GUI_window_hwin_req( MSG_WIN );
		 GUI_set_Bgcolor( 0 );
		 GUI_set_Fgcolor( g_Fg_color );
		 GUI_SetFront( &GUI_HZK16_EXTASCII);
		 GUI_string_display_At( 70, MSG_WIN->win_high -20, "共" );
		 GUI_int_display( p->Maxsize);
		 GUI_string_display( "条记录" );
		 GUI_window_hwin_release( MSG_WIN );
	}
	else if( keyMSG == KEY_RIGHT)
	{
		p->ofg = 1;
		GUI_keyMSG_post( _Searchmod_pc,GUI_KEY_NEXT_PAGE);
	}
	else if( keyMSG == KEY_LEFT)
	{
		p->ofg = 1;
		GUI_keyMSG_post( _Searchmod_pc,GUI_KEY_LAST_PAGE);
	}
    return;
}


static XXdata_t* record_arg_init( page_ctl_t*p)
{
	XXdata_t*pD;
	p->Pagesize = 15;
	pD = app_malloc( sizeof( XXdata_t ) * p->Pagesize );
	record_page_obtain(p);
	p->CURpage   = p->nPage-1;
	p->Filepos_S = p->CURpage * p->Pagesize;
    p->CURsize   = record_read(pD,p);
	p->Filepos_E = p->Filepos_S + p->CURsize;
	p->Markpos   = p->CURsize-1;
	record_view( pD, p );
	if(p->CURsize != 0)
		record_mark( pD, 1, p );
	Gui_scrb_Setsize(SCRB_WIN, p->nPage );
	Gui_scrb_Setpos(SCRB_WIN,  p->CURpage);
	Gui_scrb_SetVkey(SCRB_WIN,GUI_KEY_LAST_PAGE,GUI_KEY_NEXT_PAGE,GUI_KEY_NPAGE);
	return pD;
}


static void _Viewtask( void *arg )
{
	page_ctl_t Page;
	page_ctl_t*p = &Page;
	uint8_t keyMSG;
	XXdata_t*pdata;
	GLOBAL_MEMCLR( p, sizeof( page_ctl_t ) );
	global_strcpy( p->FILname, "ESUM" );
	_WINpaint();
	GUI_3D_window_title_mod(_Searchmod_win,"走字数据查询");
	pdata=record_arg_init(p);
	while ( 1 )
	{
		if ( GUI_key_read( MSG_WIN, &keyMSG, 1 ) == 1 )
		{
			recordKey_Exc( p, keyMSG, pdata );
			if ( keyMSG == GUI_KEY_DELETEONE )
			{
				record_delete_1( p->FILname, p->Markpos + p->Filepos_S );
				p->Markpos = 0;
				record_page_obtain(p);
				Gui_scrb_Setsize(SCRB_WIN, p->nPage );			
				p->CURsize = record_read( pdata, p );
				if (p->Maxsize==0)
				{
					record_view( pdata, p );
				}
				else if ( p->CURsize == 0 && p->Filepos_S != 0 )
				{
					GUI_keyMSG_post( MSG_WIN, GUI_KEY_LAST_PAGE );
				}
				else
				{
					p->Filepos_E = p->Filepos_S + p->CURsize;
					record_view( pdata, p );
					record_mark( pdata, 1, p );
					p->CURpage = p->Filepos_S / p->Pagesize;
				}
			}
			else if(keyMSG == GUI_KEY_DELETEALL)
			{
                record_delete_all(p);
				record_view( pdata, p );
			}
		}
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			app_free( pdata );
			os_task_delete( SELF_PRO );
		}
		msleep( 1 );
	}
}

void app_Esummod_search( void *arg )
{
	uint8_t  key;
	uint8_t  viewtask;
    viewtask =os_task_assign_stk_build(_Viewtask,3);
	while ( 1 )
	{
		if ( app_global_key_obtain( &key, 1 ) == 1 )
		{
			if ( key == KEY_EXIT)
			{
				OS_DELETE_REQ( viewtask );
				break;
			}
			else if(key == KEY_F4)
			{
				GUI_keyMSG_post( _Searchmod_pc,GUI_KEY_DELETEONE);
			}
			else if(key == KEY_F5)
			{
				GUI_keyMSG_post( _Searchmod_pc,GUI_KEY_DELETEALL);
			}
			else 
			{
				GUI_keyMSG_post( _Searchmod_pc,key);
			}
		}
		msleep(5);
	}
}

