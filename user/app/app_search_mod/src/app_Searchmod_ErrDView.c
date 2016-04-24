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
#include 	"app_Searchmod_inside.h"
#define  	LINE_HIGH        25
typedef 	Errsave_t    	 XXdata_t;
#define    	MSG_WIN          m_Searchmod_pc
#define    	SCRB_WIN         m_Searchmod_scrb
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
	GUI_set_Fgcolor( Searchmod_FG_COLOR );
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
	if(state)app_Searchmod_Userdata_view(&pdata->DL_app_user_info);
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
	GUI_set_Fgcolor( Searchmod_FG_COLOR );
	GUI_SetFront( &GUI_HZK16_EXTASCII);
	GUI_string_display_At( 70, MSG_WIN->win_high -20, "共" );
	GUI_int_display( p->Maxsize);
	GUI_string_display( "条记录" );
	GUI_window_hwin_release( MSG_WIN );
	return;
}

static void recordKey_Exc( page_ctl_t*p, uint8_t keyMSG, XXdata_t*pdata )
{
    int  CURpos;
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
	else if ( keyMSG >= GUI_KEY_NR_0 && keyMSG <= GUI_KEY_NR_15 )
	{
		if (( keyMSG - GUI_KEY_NR_0 ) < p->CURsize )
		{
		    CURpos = keyMSG - GUI_KEY_NR_0;
		    if(CURpos!=p->Markpos)
		    {
				record_mark( pdata, 0, p );
				p->Markpos = keyMSG - GUI_KEY_NR_0;
				record_mark( pdata, 1, p );
		    }
		}
	}
	else if ( keyMSG == GUI_KEY_NEXT_PAGE&&(p->nPage>1))
	{
		record_mark( pdata, 0, p );
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
	     app_Searchmod_main_window_destory();
         ERRDrecord_detail((void*)(pdata+p->Markpos));
		 app_Searchmod_main_window_creat();
		 app_Searchmod_seekdata_view(&m_seek_arg);
		 record_view( pdata, p );
		 record_mark( pdata, 1, p );
		 GUI_window_hwin_req( MSG_WIN );
		 GUI_set_Bgcolor( 0 );
		 GUI_set_Fgcolor( Searchmod_FG_COLOR );
		 GUI_SetFront( &GUI_HZK16_EXTASCII);
		 GUI_string_display_At( 70, MSG_WIN->win_high -20, "共" );
		 GUI_int_display( p->Maxsize);
		 GUI_string_display( "条记录" );
		 GUI_window_hwin_release( MSG_WIN );
	}
	else if( keyMSG == KEY_RIGHT)
	{
		p->ofg = 1;
		GUI_keyMSG_post( m_Searchmod_pc,GUI_KEY_NEXT_PAGE);
	}
	else if( keyMSG == KEY_LEFT)
	{
		p->ofg = 1;
		GUI_keyMSG_post( m_Searchmod_pc,GUI_KEY_LAST_PAGE);
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
	record_mark( pD, 1, p );
	Gui_scrb_Setsize(SCRB_WIN, p->nPage );
	Gui_scrb_Setpos(SCRB_WIN,  p->CURpage);
	Gui_scrb_SetVkey(SCRB_WIN,GUI_KEY_LAST_PAGE,GUI_KEY_NEXT_PAGE,GUI_KEY_NPAGE);
	return pD;
}



static void _seekFIL_create(void)
{
    FIL *fpdes;
    FIL *fpsrc;
	UINT WR;
	int  type = 0;
	XXdata_t pdata;
	DL_app_user_info_t*p;
	seek_arg_t*pSR = &m_seek_arg;
	p = &pdata.DL_app_user_info;
	fpdes = fatfs_open("seekDATA",FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
	if(!fpdes) return;
	fpsrc = fatfs_open("errDATA",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
	if(!fpsrc) {fatfs_close(fpdes);return;}
    if(pSR->meterID[0]!=0)type|=1;
	if(pSR->name[0]!=0)type|=2;	
    if(type==1)
    {
        while(1)
        {
			f_read(fpsrc,&pdata,sizeof( XXdata_t ),&WR);
			if(WR<sizeof( XXdata_t ))break;
	        if(global_memcmp(p->meter_id1,pSR->meterID,global_strlen(pSR->meterID)))
	        {
				f_write(fpdes,&pdata,sizeof( XXdata_t ),&WR);
			}
        }
	}
	else if(type==2)
	{
        while(1)
        {
			f_read(fpsrc,&pdata,sizeof( XXdata_t ),&WR);
			if(WR<sizeof( XXdata_t ))break;
	        if(global_memcmp(p->user_name,pSR->name,global_strlen(pSR->name)))
	        {
				f_write(fpdes,&pdata,sizeof( XXdata_t ),&WR);
			}
        }
	}
	else if(type==3)
	{
        while(1)
        {
			f_read(fpsrc,&pdata,sizeof( XXdata_t ),&WR);
			if(WR<sizeof( XXdata_t ))break;
	        if(global_memcmp(p->user_name,pSR->name,global_strlen(pSR->name))
				&&global_memcmp(p->meter_id1,pSR->meterID,global_strlen(pSR->meterID)))
	        {
				f_write(fpdes,&pdata,sizeof( XXdata_t ),&WR);
			}
        }
	}
    fatfs_close(fpsrc);
    fatfs_close(fpdes);
	return;
}

void _seek_task( void *arg )
{
	page_ctl_t Page;
	page_ctl_t*p = &Page;
	uint8_t keyMSG;
	XXdata_t*pdata;
	GUI_3D_window_title_mod(m_Searchmod_win,"校表数据查询/数据搜索");
	_seekFIL_create();
	GLOBAL_MEMCLR( p, sizeof( page_ctl_t ) );
	global_strcpy( p->FILname, "seekDATA" );
	pdata=record_arg_init(p);
	while ( 1 )
	{
		if ( GUI_key_read( MSG_WIN, &keyMSG, 1 ) == 1 )
		{
			recordKey_Exc( p, keyMSG, pdata );
			if ( keyMSG == GUI_KEY_DELETEONE )
			{
			//	record_delete_1( "errDATA", 1 );
				record_delete_1( "seekDATA", p->Markpos + p->Filepos_S );
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
					GUI_keyMSG_post( m_Searchmod_pc, GUI_KEY_LAST_PAGE );
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

void _view_task( void *arg )
{
	page_ctl_t Page;
	page_ctl_t*p = &Page;
	uint8_t keyMSG;
	XXdata_t*pdata;
	GLOBAL_MEMCLR( p, sizeof( page_ctl_t ) );
	global_strcpy( p->FILname, "errDATA" );
	GUI_3D_window_title_mod(m_Searchmod_win,"校表数据查询");
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

const Stask_array_t  m_errDATA_Stask=
{
      _view_task,
	  _seek_task,
	  0
};
