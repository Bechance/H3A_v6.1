/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:查询模块主体程序
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_Searchmod_fun.c
* 作者 :
* 功能描述:实现用户模块功能
******************************************************************************************/
#include 		"app_Searchmod_inside.h"
#include 		"../../../includes.h"
static          uint8_t   _task_pro = INVALID_PRO;

#define     		_SERCH_FG       Searchmod_FG_COLOR
#define     		_SERCH_WIN_BG   Searchmod_WIN_BG_COLOR
#define     		_SERCH_BG       Searchmod_BG_COLOR


static hwin_t*		_Searchmod_win  = PNULL;
static hwin_t*		_Searchmod_pc   = PNULL;
static hwin_t*		_Searchmod_scrb = PNULL;
static hwin_t*		_Searchmod_seek = PNULL;
static hwin_t*		_Searchmod_USER = PNULL;


extern const search_fun_t  m_XCERR_fun;
extern const search_fun_t  m_TIME_fun;
extern const search_fun_t  m_RM_fun;
extern const search_fun_t  m_WIR_fun;

const search_fun_t*xTal[4]=
{
   &m_XCERR_fun,
   &m_WIR_fun,
   &m_TIME_fun,
   &m_RM_fun,
};
const char* xTal_msg[4]=
{
 	"校表记录",
 	"查线记录",
 	"时钟误差",
 	"读表结果"
};
static char xTpath[50]="";


typedef struct
{
	int Filepos_S ;
	int Filepos_E ;
	int Markpos   ;
	int Maxsize   ;
	int CURpage   ;
	int Pagesize  ;
	int nPage;
	int CURsize;
	uint8_t ofg;
	char FILname[40];
}page_ctl_t;

enum
{
	GUI_KEY_NEXT_PAGE = 100,
	GUI_KEY_LAST_PAGE,
	GUI_KEY_NPAGE,
	GUI_KEY_NR_0,
	GUI_KEY_NR_1,
	GUI_KEY_NR_2,
	GUI_KEY_NR_3,
	GUI_KEY_NR_4,
	GUI_KEY_NR_5,
	GUI_KEY_NR_6,
	GUI_KEY_NR_7,
	GUI_KEY_NR_8,
	GUI_KEY_NR_9,
	GUI_KEY_NR_10,
	GUI_KEY_NR_11,
	GUI_KEY_NR_12,
	GUI_KEY_NR_13,
	GUI_KEY_NR_14,
	GUI_KEY_NR_15,
	GUI_KEY_SEARCH,
	GUI_KEY_SEARCH_EDIT,
	GUI_KEY_SEARCH_EXIT,
	GUI_KEY_DELETEALL,
	GUI_KEY_DELETEONE,
	GUI_KEY_RETURN
};


static      BOOL 				  _lawF[10];
#define  	LINE_HIGH        	  25
#define    	MSG_WIN              _Searchmod_pc
#define    	SCRB_WIN             _Searchmod_scrb

#define 	GET_YLINE(LINE)   (10 + 22*LINE)

BOOL     fatfs_delete_DIR(char*path);



static void _serachmod_UserWIN_create(hwin_t*p)
{
	_Searchmod_USER = GUI_edit_creat(350,28,285,280,WIN_PAINT, p );
	GUI_window_hwin_req( _Searchmod_USER );
	GUI_set_Bgcolor(_SERCH_WIN_BG);
	GUI_set_Fgcolor(_SERCH_FG);
	GUI_SetFront(&GUI_HZK16_EXTASCII); 
    GUI_string_display_At(5, GET_YLINE( 0 ),      "工单号 :");
    GUI_string_display_At(5, GET_YLINE( 1 ),      "用户号 :");
    GUI_string_display_At(5, GET_YLINE( 2 ),      "用户名 :");	
    GUI_string_display_At(5, 10+GET_YLINE( 4 ),   "电表号1:");
    GUI_string_display_At(5, 20+GET_YLINE( 6 ),   "电表号2:");
    GUI_string_display_At(5, 30+GET_YLINE( 8 ),   "校验员 :");
    GUI_string_display_At(5, 30+GET_YLINE( 9 ),   "  时间 :");
	GUI_window_hwin_release( _Searchmod_USER );
    return;
}

static void  _Userdata_obtain(Slist_t*p,DL_app_user_info_t*pD)
{
    FIL*fp;
	int index;
    char name[40] = "";
	char nCH[10]  = "";
	UINT w;
	Fileview_path_obtain(name);
	global_strcat( name, "/");
	global_ltoa(p->n, nCH);
	global_strcat( name, nCH);
	global_strcat( name, "/");
	global_strcat( name, "info");
	fp = fatfs_open(name, FA_OPEN_EXISTING | FA_READ );
	if(!fp) {GLOBAL_MEMCLR(pD,sizeof(DL_app_user_info_t));return ;}
	f_read(fp,pD,sizeof(DL_app_user_info_t),&w);
	fatfs_close(fp);
    GLOBAL_MEMCLR(xTpath,40);
	Fileview_path_obtain(xTpath);
	global_strcat( xTpath, "/");
	global_ltoa(p->n, nCH);
	global_strcat( xTpath, nCH);
	global_strcat( xTpath, "/");

    GUI_window_hwin_req( _Searchmod_win);
	GUI_set_Bgcolor(_SERCH_WIN_BG);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
    for(index=0;index<SIZE_OF_ARRAY(xTal);index++)
    {
		global_memcpy(name, xTpath, 40);
		global_strcat( name, xTal[index]->name);
		GUI_set_Fgcolor(_SERCH_FG);
		GUI_int_display_At(360,320+index*30,index);
		GUI_string_display(".");
		GUI_string_display((char*)xTal_msg[index]);
	    if(!fatfs_file_ISexist(name))
	    {
	        _lawF[index] = 0;
			GUI_set_Fgcolor(C_GUI_RED);
			GUI_string_display((char*)"  记录不存在.");
		}
		else
		{
		    _lawF[index] = 1;
			GUI_set_Fgcolor(C_GUI_GREEN);
			GUI_string_display((char*)"    记录正常.");
		}
    }
	GUI_window_hwin_release( _Searchmod_win);
	return;
}

static void  _Searchmod_Userdata_view(Slist_t*pList)
{
    int high;
	DL_app_user_info_t*p = app_malloc(sizeof(DL_app_user_info_t));
	_Userdata_obtain(pList,p);
	GUI_window_hwin_req( _Searchmod_USER );
	GUI_set_Bgcolor(_SERCH_WIN_BG);
	GUI_set_Fgcolor(_SERCH_FG);
    high  =30+GET_YLINE(9)+25+5;
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
	GUI_WINDIS_HLINE(67,277,30+GET_YLINE(9)+17);
	
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_SetLineHigh(6);
	GUI_string_display_At(69,GET_YLINE( 0 ),p->wrok_list);
	GUI_string_display_At(69,GET_YLINE( 1 ),p->user_id);
	GUI_string_display_At(69,GET_YLINE( 2 ),p->user_name);
	GUI_string_display_At(69,10+GET_YLINE( 4 ),p->meter_id1);
	GUI_string_display_At(69,20+GET_YLINE( 6 ),p->meter_id2);
	GUI_string_display_At(69,30+GET_YLINE( 8 ),p->worker);
	GUI_string_display_At(69,30+GET_YLINE( 9 ),p->time_buf);
	GUI_window_hwin_release( _Searchmod_USER ); 
	app_free(p);
	return;
}

static void _WINpaint( void )
{
	hwin_t*p, *pc, *scb;
	GUI_window_hwin_req( GET_GUIWIN );
	GUI_set_Bgcolor( _SERCH_BG );
	GUI_clean_screen();
	GUI_set_Bgcolor( _SERCH_WIN_BG );
	GUI_set_Fgcolor( _SERCH_FG );
	p = Gui_3D_window_creat( 0, 0, LCD_WIDE-2,LCD_HIGH, "", WIN_3D | WIN_FILL, 0);
	GUI_window_hwin_req( p );
	GUI_set_Bgcolor( 0 );
	pc = Gui_txtbox_creat( 5, 20,340,p->win_high - 20, WIN_3D | WIN_FILL, p );
	scb = Gui_scrb_attach( pc, 25);
	GUI_set_Fgcolor( _SERCH_FG );
	GUI_set_Bgcolor( _SERCH_WIN_BG );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	GUI_string_display_At(6,  2, "序号" );
	GUI_string_display_At(64, 2, "表号" );
	_Searchmod_win  = p;
	_Searchmod_pc   = pc;
	_Searchmod_scrb = scb;
	GUI_MsgFIFO_create( _Searchmod_pc, 2 );
	GUI_window_hwin_req( pc );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	GUI_window_hwin_release( pc );
    _serachmod_UserWIN_create(p);
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
* @功能 :删除一条记录
* @输入 :
* @输出 :NONE
****************************************************/
static void record_delete_1( page_ctl_t*p)
{
	UINT w;
	FIL*fp;
	uint32_t lseek,fsize;
	Slist_t data;
    char name[40] = "";
	char nCH[10]  = "";
	uint32_t record_num;
	record_num = p->Markpos + p->Filepos_S;
	fp = fatfs_open(p->FILname,FA_OPEN_EXISTING | FA_READ |FA_WRITE);
	if(!fp) return;
	if ( fp->fsize == sizeof( Slist_t) )
	{
		f_read( fp, &data, sizeof( Slist_t ), &w );
		global_ltoa(data.n, nCH);
		fatfs_close( fp );
		f_unlink(p->FILname);
	}
	else
	{
		lseek = record_num * sizeof( Slist_t );
		f_lseek( fp, lseek );
		f_read( fp, &data, sizeof( Slist_t ), &w );
		global_ltoa(data.n, nCH);
		fsize = fp->fsize;
		lseek = fsize - sizeof( Slist_t );
		f_lseek( fp, lseek );
		f_read( fp, &data, sizeof( Slist_t ), &w );
		f_lseek( fp, lseek );
		f_truncate( fp );
		lseek = record_num * sizeof( Slist_t );
		if ( lseek < fp->fsize )
		{
			f_lseek( fp, lseek );
			f_write( fp, &data, sizeof( Slist_t ), &w );
		}
		fatfs_close( fp );
	}
	Fileview_path_obtain(name);
	global_strcat( name, "/");
	global_strcat( name, nCH);
	fatfs_delete_DIR(name);
	return;
}


static int record_read( void*pD, page_ctl_t*p)
{
	UINT w;
	FIL*fp;
	Slist_t*pdata = ( Slist_t* )pD;
	fp = fatfs_open( p->FILname, FA_OPEN_EXISTING | FA_READ );
	if(!fp) return 0;
	f_lseek( fp, p->Filepos_S*sizeof( Slist_t ) );
	f_read( fp, pdata, sizeof( Slist_t )*p->Pagesize, &w );
	fatfs_close( fp );
	return ( w / sizeof( Slist_t ) );
}


static void record_view( void*pD, page_ctl_t*p )
{
	int index, dx;
	char ch[4] = "";
	Slist_t*pdata = ( Slist_t* )pD;
	GUI_window_hwin_req( MSG_WIN );
	GUI_set_Bgcolor( 0 );
	GUI_set_Fgcolor( _SERCH_FG );
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
				_WINdestory();
				os_task_delete( SELF_PRO );
			}
			msleep( 1 );
	     }
    }
	dx = p->Filepos_E - p->Filepos_S;
	for ( index = 0;index < dx;index++ )
	{
		global_itoa(( p->Filepos_S + index ), ch );
		GUI_string_display_At( 6, 8 + LINE_HIGH*index, ch );
		GUI_string_display_At( 64, 8 + LINE_HIGH*index, pdata->mID);
		pdata++;
	}
	GUI_string_display_At( 10, MSG_WIN->win_high - 20, "第" );
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
	Slist_t*pdata = ( Slist_t* )pD;
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
	GUI_string_display_At( 64, 8 + LINE_HIGH*p->Markpos, pdata->mID);
	GUI_window_hwin_release( MSG_WIN );
	if(state)_Searchmod_Userdata_view(pdata);
	return;
}

static void record_page_obtain( page_ctl_t*p )
{
	FIL*fp;
	fp = fatfs_open( p->FILname, FA_OPEN_EXISTING | FA_READ );
	if(!fp){p->Maxsize=0;p->nPage=0;return;}
	p->Maxsize  = fp->fsize / sizeof( Slist_t );
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

static void recordKey_Exc( page_ctl_t*p, uint8_t keyMSG, Slist_t*pdata )
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
	else if ( keyMSG == GUI_KEY_NEXT_PAGE )
	{
		if ( p->Filepos_E < p->Maxsize )
		{
			record_mark( pdata, 0, p );
			p->Filepos_S = p->Filepos_E;
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
	}
	else if ( keyMSG == GUI_KEY_LAST_PAGE )
	{
		if ( p->Filepos_S > 0 )
		{
			p->Filepos_S = p->Filepos_S - p->Pagesize;
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
	else if(keyMSG == KEY_F5)
	{
        record_delete_1(p);
		record_page_obtain(p);
		p->CURpage   = p->nPage-1;
		p->Filepos_S = p->CURpage * p->Pagesize;
	    p->CURsize   = record_read(pdata,p);
		p->Filepos_E = p->Filepos_S + p->CURsize;
		p->Markpos   = p->CURsize-1;
		record_view( pdata, p);
		record_mark( pdata, 1, p );
		Gui_scrb_Setsize(SCRB_WIN, p->nPage );
		Gui_scrb_Setpos(SCRB_WIN,  p->CURpage);
	}
    return;
}


static Slist_t* record_arg_init( page_ctl_t*p)
{
	Slist_t*pD;
	p->Pagesize = 15;
	pD = app_malloc( sizeof( Slist_t ) * p->Pagesize );
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
	char index;
	Slist_t*pdata;
	GLOBAL_MEMCLR( p, sizeof( page_ctl_t ) );
	Fileview_path_obtain(p->FILname);
	global_strcat( p->FILname, "/");
	global_strcat( p->FILname, "list" );
	_WINpaint();
	GUI_3D_window_title_mod(_Searchmod_win,"数据查询");
	pdata=record_arg_init(p);
	while ( 1 )
	{
		if ( GUI_key_read( MSG_WIN, &keyMSG, 1 ) == 1 )
		{
			if(keyMSG == KEY_0
				||keyMSG == KEY_1
				||keyMSG == KEY_2
				||keyMSG == KEY_3)
			{
			    index =app_global_decide_inputkey(keyMSG)-'0';
				if(_lawF[index])
				{
				    _WINdestory();
					xTal[index]->_record_detail(pdata+p->Markpos);
					app_free(pdata);
					_WINpaint();
					GUI_3D_window_title_mod(_Searchmod_win,"数据查询");
					pdata=record_arg_init(p);
				}
			}
			else  recordKey_Exc( p, keyMSG, pdata );			
		}
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			app_free( pdata );
			_WINdestory();
			os_task_delete( SELF_PRO );
		}
		msleep( 1 );
	}
}

static void _task( void *arg )
{
	uint8_t     key;
	uint8_t     viewtask 	 = INVALID_PRO;
    Fileview_open("PECD",1);
	while ( 1 )
	{
		if ( app_global_key_obtain( &key, 1 ) == 1 )
		{
			if(key == KEY_ENTER)
			{
			    if(Fileview_ISexit())
			    {
					Fileview_close();
    				viewtask =os_task_assign_stk_build(_Viewtask,4);
				}
                else	GUI_keyMSG_post(_Searchmod_pc,key);
			}
			else if(key == KEY_EXIT)
			{
			    if(viewtask!=INVALID_PRO)
			    {
					OS_DELETE_REQ( viewtask );
					viewtask = INVALID_PRO;
    				Fileview_open("PECD",0);
				}
				else 
				{
					Fileview_close();
					break;
				}
			}
			else if(!Fileview_ISexit())
				GUI_keyMSG_post(_Searchmod_pc,key);
			else
				Fileview_sendMSG(key);

		}
		msleep(5);
	}
	f_unlink("DTlist");
	app_module_switch_last_mod();
	while(1)	
	{
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			os_task_delete( SELF_PRO );
		}
	}
}
/********************************************************************
 * @作者 :揭成
 * @功能 :主模块主功能初始化
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_Searchmod_fun_entry( void )
{
	_task_pro = os_task_assign_stk_build( _task, 3 );
	os_task_name_set( _task_pro, "Searchmod--Main task" );
	return;
}
/********************************************************************
 * @作者 :
 * @功能 :主模块主功能释放资源
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_Searchmod_fun_exit( void )
{
	if ( _task_pro != INVALID_PRO )
	{
		OS_DELETE_REQ( _task_pro );
		_task_pro = INVALID_PRO;
	}
	return;
}
