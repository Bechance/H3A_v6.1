#include "FatFs.h"
#include "../os_connect\os_connect.h"
#include "../support\app_support.h"
#include "src\diskio.c"
#include "src\ff.c"
#include "src\option\syscall.c"

#define  MAX_FILOBJ_SIZE  6

static   FATFS 	m_fatfs;
static   FIL    m_file_obj[MAX_FILOBJ_SIZE];
BOOL     m_sd_state = FAIL;
static   OS_MEM*  p_file_mem;
/********************************************************************
 * @创建人:揭成
 * @功能  :获取一个空闲文件句柄
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
FIL*  fatfs_FIobj_obtain( void )
{
	INT8U err;
	FIL*pfile;
	pfile = OSMemGet( p_file_mem, &err );
	if ( err == OS_NO_ERR )
	{
		return pfile;
	}
	return PNULL;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :释放一个文件句柄
 * @输入  :文件句柄地址
 * @输出  :NONE
********************************************************************/
uint8_t fatfs_FIobj_put( FIL*pfile )
{
	INT8U err;
	err = OSMemPut( p_file_mem, pfile );
	return err;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :按字节写如文件数据
 * @输入  :
 * @输出  :NONE
	说明:增加该接口的目的是为了是文件写入按目前的字节对齐方式，
	因为28XX系列编译器一个字节是16BIT，而文件系统是8BIT，因此增加
	该接口，不然是没有必要的。
********************************************************************/
FRESULT fatfs_write_bytes(
	FIL *fp,
	const void *buff,
	UINT btw,
	UINT *bw
)
{
	FRESULT  ret;
	uint16_t itemp;
	char     pMem[2];
	Bsize_t  pos;
	UINT     w;
	*bw = 0;
	for ( pos = 0;pos < btw;pos++ )
	{
		itemp  = (( uint16_t* )buff )[pos];
		pMem[0] = ( itemp & 0xff );
		pMem[1] = (( itemp & 0xff00 ) >> 8 );
		ret = f_write( fp, pMem, 2, &w );
		*bw += w;
	}
	*bw /= 2;
	return ret;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :按字节读出文件
 * @输入  :
 * @输出  :NONE
	说明:增加该接口的目的是为了是文件写入按目前的字节对齐方式，
	因为28XX系列编译器一个字节是16BIT，而文件系统是8BIT，因此增加
	该接口，不然是没有必要的。
********************************************************************/
FRESULT fatfs_read_bytes(
	FIL *fp,
	const void *buff,
	UINT btw,
	UINT *bw
)
{
	FRESULT  ret;
	char	 pMem[2];
	Bsize_t  pos;
	uint16_t itemp;
	UINT     r;
	( *bw ) =   0;
	for ( pos = 0;pos < btw;pos++ )
	{
		ret = f_read( fp, pMem, 2, &r );
		itemp = ( *pMem ) | (( *( pMem + 1 ) ) << 8 );
		(( char* )buff )[pos] = itemp;
		*bw += r;
	}
	( *bw ) /= 2;
	return ret;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :打开一个文件
 * @输入  :
 * @输出  :文件句柄
********************************************************************/
FIL*fatfs_open(
	const TCHAR *path,	/* Pointer to the file name */
	BYTE mode	/* Access mode and file open mode flags */
)
{
	FRESULT ret;
	FIL* fp = fatfs_FIobj_obtain();
	ret = f_open( fp, path, mode );
	if ( ret != FR_OK )
	{
		fatfs_FIobj_put( fp );
		return PNULL;
	}
	return fp;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :关闭一个文件
 * @输入  :文件句柄
 * @输出  :NONE
********************************************************************/
void fatfs_close( FIL* fp )
{
	f_close( fp );
	fatfs_FIobj_put( fp );
	return;
}

BOOL fatfs_file_ISexist( const char*path )
{
	FIL*fp;
	fp = fatfs_open( path, FA_OPEN_EXISTING | FA_READ );
	if ( fp )
	{
		fatfs_close( fp );
		return TURE;
	}
	return FAIL;
}

BOOL fatfs_file_create( const char*path, void*data, int size )
{
	UINT nbyte;
	FIL*fp;
	fp = fatfs_open( path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE );
	if ( fp )
	{
		fatfs_write_bytes( fp, data, size, &nbyte );
		fatfs_close( fp );
		return TURE;
	}
	return FAIL;
}
/*******************************************************
   按 16BIT 单位读写文件系统
*******************************************************/
BOOL fatfs_readARG( const char*path, void*data, int size )
{
	UINT nbyte;
	FIL*fp;
	fp = fatfs_open( path, FA_OPEN_EXISTING | FA_READ );
	if ( fp )
	{
		fatfs_read_bytes( fp, data, size, &nbyte );
		fatfs_close( fp );
		return TURE;
	}
	return FAIL;
}

BOOL fatfs_writeARG( const char*path, void*data, int size )
{
	UINT nbyte;
	FIL*fp;
	FRESULT  ret;
	fp = fatfs_FIobj_obtain();
	ret = f_open( fp, path, FA_OPEN_EXISTING | FA_WRITE );
	if ( ret == FR_OK )
	{
		fatfs_write_bytes( fp, data, size, &nbyte );
		fatfs_close( fp );
		return TURE;
	}
	else if ( ret == FR_NO_FILE )
	{
		fatfs_file_create( path, data, size );
		return TURE;
	}
	return FAIL;
}

/********************************************************************
 * @创建人:揭成
 * @功能  :文件系统初始化
 * @输入  :
 * @输出  :NONE
********************************************************************/
void fatfs_init( void )
{
	uint8_t err;
	m_sd_state = FAIL;
	p_file_mem = OSMemCreate( m_file_obj, MAX_FILOBJ_SIZE, sizeof( FIL ), &err );
	if ( err != OS_NO_ERR ) return;
	GLOBAL_MEMCLR( &m_fatfs, sizeof( FATFS ) );
	f_mount( 0, &m_fatfs );
	return;
}
/****************************************************************/
//文件浏览器
/****************************************************************/
static   int         _fileview_lsize  = 5;
static   int         _fileview_hsize  = 4;
static   char        _CURpath[40] 	  = "";
static   char        _Getpath[40];
static   hwin_t*     _fileWIN   	  = PNULL;
static   hwin_t*     _fileScrb   	  = PNULL;
static   uint8_t     _viewtaskID 	  = INVALID_PRO;
static   BOOL        _UserPower       = 0;
static   char        _pathtype        = 0 ;
static   const       GUI_FONT*     _FviewFont = &GUI_HZK24_EXTASCII;

#define  _SERCH_FG            g_Fg_color
#define  _SERCH_WIN_BG        g_win_Bg_color
#define  _SERCH_BG        	  g_Bg_color


#define  SCRB_WIN             _fileScrb
extern   const unsigned char gImage_flop[];
extern   const unsigned char gImage_file[];
typedef struct
{
	char    type;
	char    date[15];
}DTlist_t;

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
}Fpage_ctl_t;

enum
{
	GUI_KEY_NEXT_PAGE = 100,
	GUI_KEY_LAST_PAGE
};
static void _waitBAR_view(float uage);
static void _waitWIN_paint(char*str);
static void _waitWIN_destory(void);
static void _waitWIN_CHmod(char*str);

static Fpage_ctl_t  _Page;

static void _DTlist_create( void )
{
	FRESULT ret;
	DIR dir;
	FIL*fOBJ;
	UINT w;
	FILINFO  file_info;
	DTlist_t page;
	f_opendir( &dir, ( const char* )_CURpath );
	fOBJ = fatfs_open( "DTlist", FA_CREATE_ALWAYS | FA_WRITE );
	while ( 1 )
	{
		ret = f_readdir( &dir, &file_info );
		if ( ret != FR_OK || file_info.fname[0] == 0 )
			break;
		GLOBAL_MEMCLR( &page, sizeof( DTlist_t ) );
		global_strcat( page.date, file_info.fname );
		if ( file_info.fattrib&AM_DIR )page.type = 'd';
		else page.type = 'f';
		f_write( fOBJ, &page, sizeof( DTlist_t ), &w );
	}
	fatfs_close( fOBJ );
	return;
}
static void _date_3DWIN( void )
{
    char *view;
	view=app_malloc(100);
	global_strcpy(view,"文件管理器    ");
	if(_CURpath[0]=='/')
		global_strcat(view,"I:");
	else
		global_strcat(view,"I:/");
	global_strcat(view,_CURpath);
	GUI_window_hwin_req( GET_GUIWIN );
	GUI_set_Bgcolor( _SERCH_BG );
	GUI_clean_screen();
	GUI_set_Bgcolor( _SERCH_WIN_BG );
	GUI_set_Fgcolor( _SERCH_FG );
	_fileWIN = Gui_3D_window_creat(0,0,LCD_WIDE - 2,LCD_HIGH,view,WIN_3D|WIN_FILL,0);
	SCRB_WIN = Gui_scrb_attach( _fileWIN, 25 );
	GUI_MsgFIFO_create( _fileWIN, 2 );
	GUI_window_hwin_release( GET_GUIWIN );
	app_free(view);
	return;
}

static int record_readDTlist( void*pD, Fpage_ctl_t*p )
{
	UINT w;
	FIL*fp;
	DTlist_t*pdata = ( DTlist_t* )pD;
	fp = fatfs_open( p->FILname, FA_OPEN_EXISTING | FA_READ );
	if ( !fp ) return 0;
	f_lseek( fp, p->Filepos_S*sizeof( DTlist_t ) );
	f_read( fp, pdata, sizeof( DTlist_t )*p->Pagesize, &w );
	fatfs_close( fp );
	return ( w / sizeof( DTlist_t ) );
}

static void _DTlist_page_obtain( Fpage_ctl_t*p )
{
	FIL*fp;
	fp = fatfs_open( p->FILname, FA_OPEN_EXISTING | FA_READ );
	if ( !fp )
	{
		p->Maxsize = 0;
		p->nPage = 0;
		return;
	}
	p->Maxsize  = fp->fsize / sizeof( DTlist_t );
	p->nPage    = ( p->Maxsize + p->Pagesize - 1 ) / p->Pagesize;
	fatfs_close( fp );
	return;
}


static void _DTlist_init( Fpage_ctl_t*p, DTlist_t*pD )
{
	_DTlist_page_obtain( p );
	p->CURsize    = record_readDTlist( pD, p );
	p->Filepos_E  = p->Filepos_S + p->CURsize;
	return;
}


static void _DTlist_view( Fpage_ctl_t*p, DTlist_t*pD )
{
	int pos;
	int x, y;
	GUI_window_hwin_req( _fileWIN );
	GUI_set_Bgcolor( 0 );
	GUI_clean_window();
	GUI_set_Fgcolor( _SERCH_FG );
	GUI_SetFront( _FviewFont );
	for ( pos = 0;pos < p->CURsize;pos++ )
	{
		x = 20 + ( pos % _fileview_lsize ) * (( _fileWIN->win_wide - 40 ) / _fileview_lsize );
		y = 60 + ( pos / _fileview_lsize ) * (( _fileWIN->win_high - 60 ) / _fileview_hsize );
		if ( pD[pos].type == 'd' )
			GUI_bmp_display( _fileWIN->win_x + x-1, _fileWIN->win_y + y - 40, ( void* )gImage_flop );
        else
			GUI_bmp_display( _fileWIN->win_x + x-1, _fileWIN->win_y + y - 40, ( void* )gImage_file );
		GUI_string_display_At( x, y, pD[pos].date );
	}
	GUI_set_Fgcolor( _SERCH_FG );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	GUI_string_display_At( 20, _fileWIN->win_high - 30, "<方向键>选择,<确认>访问该数据,<F4>翻页,<F5>删除." );
	GUI_window_hwin_release( _fileWIN );
	return;
}

static void _DTlist_mark( DTlist_t*pD, int state, Fpage_ctl_t*p )
{
	int pos;
	int x, y;
	int FG, BG;
	pos = p->Markpos;
	GUI_window_hwin_req( _fileWIN );
	GUI_SetFront( _FviewFont );
	if ( state )
	{
		FG = C_GUI_GREEN;
		BG = 0;
		GLOBAL_MEMCLR( _Getpath, 40 );
		global_strcpy( _Getpath, _CURpath );
		global_strcat( _Getpath, "/" );
		global_strcat( _Getpath, pD[pos].date );
		_pathtype = pD[pos].type;
		if(_pathtype=='f')
		{
		 	if(global_string_locate_first(pD[pos].date,".TXT"))
			 _pathtype = 't';
		}
	}
	else
	{
		FG = C_GUI_WHITE;
		BG = 0;
	}
	GUI_set_Bgcolor( BG );
	GUI_set_Fgcolor( FG );
	x = 20 + ( pos % _fileview_lsize ) * (( _fileWIN->win_wide - 40 ) / _fileview_lsize );
	y = 60 + ( pos / _fileview_lsize ) * (( _fileWIN->win_high - 60 ) / _fileview_hsize );
	GUI_string_display_At( x, y, pD[pos].date );
	GUI_window_hwin_release( _fileWIN );
	return;
}


/********************************************************************
 * @创建人:揭成
 * @功能  :删除文件夹及下面所有文件
 * @输入  :
 * @输出  :NONE
********************************************************************/
static void _Get_path( char*path, char*name, FILINFO*p, int dip )
{
	int pos;
	GLOBAL_MEMCLR( name, 40 );
	global_strcpy( name, path );
	for ( pos = 0;pos<dip;pos++ )
	{
		global_strcat( name, "/" );
		global_strcat( name, p[pos].fname );
	}
	return;
}


static uint32_t _Get_path_fn(char*path)
{
	DIR       fdir;
	FILINFO   finfo;
	FRESULT   ret;
	uint32_t  size =0;
    f_opendir(&fdir,path);
	while(1)
	{
		ret = f_readdir( &fdir,&finfo);
		if ( ret != FR_OK || finfo.fname[0] == 0 )
        break;
		else  size++;
	}	
	return size;
}


BOOL fatfs_delete_DIR( char*path )
{
	FRESULT   ret;
	DIR       *fdir;
	FILINFO   *finfo;
	char 	  name[40]    = "";
	int       pos = 0;
	int       flag        = 1;
	uint32_t  allsize     = 0;
	uint32_t  Csize       = 0;
	fdir  = app_malloc(10*sizeof(DIR));
	finfo = app_malloc(10*sizeof(FILINFO));
	_Get_path( path, name, finfo, pos );
	allsize = _Get_path_fn(name);
	Csize   = allsize;	
	while (flag)
	{
	    _Get_path( path, name, finfo, pos );
	    f_opendir(&fdir[pos],name);
		while(1)
		{
			if(allsize>0)
		    {
				_waitBAR_view((float)(allsize-Csize)/allsize);
		    }
			ret = f_readdir( &fdir[pos],&finfo[pos] );
			if ( ret != FR_OK || finfo[pos].fname[0] == 0 )
			{
			    if(pos==0)flag = 0;
				else 
				{
					pos--;
					if(pos==0)Csize--;
					_Get_path( path, name, finfo, pos+1);
					if(f_unlink(name)!=FR_OK)
					{app_free(fdir);app_free(finfo);return FAIL;}
				}
				break;
			}
			if ( !( finfo[pos].fattrib&AM_DIR ) )
			{
				_Get_path( path, name, finfo, pos+1);
				if(f_unlink(name)!=FR_OK)
				{app_free(fdir);app_free(finfo);return FAIL;}
				if(pos==0)Csize--;
				continue;
			}
			else {pos++;break;}
		}
	}
	if(f_unlink(path)!=FR_OK)
	{app_free(fdir);app_free(finfo);return FAIL;}
	return TURE;
}

static void File_Vtask( void*arg )
{
	uint8_t     keyMSG;
	Fpage_ctl_t*	p = &_Page;
	DTlist_t  * pdata = app_malloc( sizeof( DTlist_t ) * _fileview_lsize * _fileview_hsize );

	GLOBAL_MEMCLR(p, sizeof(Fpage_ctl_t));
	GLOBAL_MEMCLR(pdata, sizeof( DTlist_t ) * _fileview_lsize * _fileview_hsize );
		
	_UserPower  = 0;
	p->Pagesize = _fileview_lsize * _fileview_hsize;
	global_strcpy( p->FILname, "DTlist" );
	_date_3DWIN();
	_DTlist_init( p, pdata );
	_DTlist_view( p, pdata );
	_DTlist_mark( pdata, 1, p );
	Gui_scrb_Setsize( SCRB_WIN, p->nPage );
	Gui_scrb_Setpos( SCRB_WIN,  p->CURpage );
	while ( 1 )
	{
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			app_free( pdata );
			GUI_window_destory( _fileWIN );
			_fileWIN  = 0;
			os_task_delete( SELF_PRO );
		}
		if ( GUI_key_read( _fileWIN, &keyMSG, 1 ) == 1 )
		{
			if ( keyMSG == GUI_KEY_NEXT_PAGE )
			{
				if ( p->Filepos_E < p->Maxsize )
				{
					_DTlist_mark( pdata, 0, p );
					p->Filepos_S = p->Filepos_E;
					p->Markpos = 0;
					p->CURsize = record_readDTlist( pdata, p );
					p->Filepos_E = p->Filepos_S + p->CURsize;
					_DTlist_view( p, pdata );
					_DTlist_mark( pdata, 1, p );
					p->CURpage = p->Filepos_S / p->Pagesize;
					Gui_scrb_Setpos( SCRB_WIN,  p->CURpage );
				}
				else
				{
					p->CURpage    = 0;
					p->Filepos_S  = 0;
					p->CURsize 	  = record_readDTlist( pdata, p );
					p->Filepos_E  = p->Filepos_S + p->CURsize;
					p->Markpos    = 0;
					_DTlist_view( p, pdata );
					_DTlist_mark( pdata, 1, p );
					Gui_scrb_Setpos( SCRB_WIN,  p->CURpage );
				}
			}
			else if ( keyMSG == GUI_KEY_LAST_PAGE )
			{
				if ( p->Filepos_S > 0 )
				{
					p->Filepos_S = p->Filepos_S - p->Pagesize;
					p->Markpos = 0;
					p->CURsize = record_readDTlist( pdata, p );
					p->Filepos_E = p->Filepos_S + p->CURsize;
					_DTlist_view( p, pdata );
					_DTlist_mark( pdata, 1, p );
					p->CURpage = p->Filepos_S / p->Pagesize;
					Gui_scrb_Setpos( SCRB_WIN,  p->CURpage );
				}
			}
			else if ( keyMSG == KEY_RIGHT )
			{
				if (( p->Markpos % _fileview_lsize ) < ( _fileview_lsize - 1 ) && ( p->Markpos + 1 ) < p->CURsize )
				{
					_DTlist_mark( pdata, 0, p );
					p->Markpos = p->Markpos + 1;
					_DTlist_mark( pdata, 1, p );
				}
			}
			else if ( keyMSG == KEY_LEFT )
			{
				if (( p->Markpos % _fileview_lsize ) > 0 )
				{
					_DTlist_mark( pdata, 0, p );
					p->Markpos = p->Markpos - 1;
					_DTlist_mark( pdata, 1, p );
				}
			}
			else if ( keyMSG == KEY_F4 )
			{
				GUI_keyMSG_post( _fileWIN, GUI_KEY_NEXT_PAGE );
			}
			else if ( keyMSG == KEY_UP )
			{
				if (( p->Markpos / _fileview_lsize ) > 0 )
				{
					_DTlist_mark( pdata, 0, p );
					p->Markpos = p->Markpos - _fileview_lsize;
					_DTlist_mark( pdata, 1, p );
				}
				else
				{
					GUI_keyMSG_post( _fileWIN, GUI_KEY_LAST_PAGE );
				}
			}
			else if ( keyMSG == KEY_DOWN )
			{
				if (( p->Markpos / _fileview_lsize ) < ( _fileview_hsize - 1 ) && ( p->Markpos + _fileview_lsize ) < p->CURsize )
				{
					_DTlist_mark( pdata, 0, p );
					p->Markpos = p->Markpos + _fileview_lsize;
					_DTlist_mark( pdata, 1, p );
				}
				else
				{
					GUI_keyMSG_post( _fileWIN, GUI_KEY_NEXT_PAGE );
				}
			}
			else if ( keyMSG == KEY_F5 )
			{
				if ( _UserPower == 1 )
				{
					if ( MsgBox( 100, 100, 350, 100,
								 "提示", "确认删除?(<确认>/<退出>)",
								 APP_HALF_SEC*20 ) != GUIE_KEY_ENTER )continue;
				}
				else if ( !MsgPasswordBox( 200, 150, "3721" ) ) continue;

				_UserPower = 1;
				_waitWIN_paint( "正在删除请稍后..." );
				if ( !fatfs_delete_DIR( _Getpath ) )
				{
					_waitWIN_CHmod( "删除失败!" );
					msleep( APP_100_MS*10 );
				}
				_waitWIN_destory();
				_DTlist_create();
				_DTlist_init( p, pdata );
				_DTlist_view( p, pdata );
				if(p->CURsize != 0)					
				{
					if(p->Markpos >= p->CURsize)	//判断是否删除最后一个文件夹
						p->Markpos = p->CURsize-1;
					_DTlist_mark( pdata, 1, p );
				}
				Gui_scrb_Setsize( SCRB_WIN, p->nPage );
				Gui_scrb_Setpos( SCRB_WIN,  p->CURpage );
			}
		}
		msleep( 5 );
	}
}
/**********************************************************************/
//wait WIN
/**********************************************************************/
#define  	WAITWIN_WIDE   250
#define  	WAITWIN_HIGH   60
#define  	WAITWIN_BAR    100

static      uint16_t 	_waitSec;
static      hwin_t*  	_waitWIN  = 0;
static      hwin_t*     _waitscrb = 0;
static void _waitWIN_paint(char*str)
{
	int x1, y1, x2, y2;
	x1 = (LCD_WIDE-WAITWIN_WIDE)/2;
	y1 = (LCD_HIGH-WAITWIN_HIGH)/2;
	x2 = x1 + WAITWIN_WIDE;
	y2 = y1 + WAITWIN_HIGH;
	_waitSec = GUI_read_block_Extp( x1, y1, x2, y2 );
	GUI_window_hwin_req( GET_GUIWIN );
	GUI_set_Bgcolor( 0x4208);
	_waitWIN = GUI_window_creat( x1 + 1, y1 + 1, WAITWIN_WIDE - 2, WAITWIN_HIGH - 2, WIN_3D | WIN_FILL );
	_waitscrb = Gui_progress_bar_creat( 5, 20, _waitWIN->win_wide - 10,15, WAITWIN_BAR, _waitWIN );
	GUI_TopWIN_Set( _waitWIN );
	GUI_window_hwin_req(_waitWIN );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor(0x4208);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_string_display_At(2,2, str);
	GUI_window_hwin_release( _waitWIN );
	GUI_window_hwin_release( GET_GUIWIN );
    return;
}

static void _waitWIN_CHmod(char*str)
{
	GUI_window_hwin_req( _waitWIN );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor(0x4208);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_clean_window();
	GUI_string_display_At(2,2, str);
	GUI_window_hwin_release( _waitWIN );
	return;
}

static void _waitBAR_view(float uage)
{
	uint16_t vlue;
	if(_waitscrb){
	vlue = WAITWIN_BAR *uage;
	Gui_progress_bar_setValue( _waitscrb, vlue );}
	return;
}

static void _waitWIN_destory(void)
{
	int x1, y1, x2, y2;
	x1 = (LCD_WIDE-WAITWIN_WIDE)/2;
	y1 = (LCD_HIGH-WAITWIN_HIGH)/2;
	x2 = x1 + WAITWIN_WIDE;
	y2 = y1 + WAITWIN_HIGH;
	GUI_window_destory(_waitWIN);
	GUI_write_block_Extp(x1,y1,x2, y2,_waitSec);
	_waitWIN  = 0;
	_waitscrb = 0;
	return;
}


void Fileview_sendMSG( uint8_t key )
{
	GUI_keyMSG_post( _fileWIN, key );
	return;
}

void Fileview_path_obtain( char*path )
{
	global_strcpy( path, _Getpath );
	return;
}
char Fileview_type_obtain( void )
{
	return _pathtype;
}
BOOL Fileview_ISexit( void )
{
	return ( _viewtaskID == INVALID_PRO ) ? ( 0 ) : ( 1 );
}

void Fileview_setARG(int lsize,int hsize,const void*pf)
{
	_fileview_lsize = lsize;
	_fileview_hsize = hsize;
	if(lsize==0||hsize==0)
	{
		_fileview_lsize = 5;
		_fileview_hsize = 4;
	}
	_FviewFont = (const GUI_FONT*)pf;
	return;
}

void Fileview_open( char*path, int posRST )
{
	GLOBAL_MEMCLR( _CURpath, 40 );
	global_strcpy( _CURpath, path );
	Fileview_setARG(0,0,&GUI_HZK24_EXTASCII);
	if ( posRST )
	{
		GLOBAL_MEMCLR( &_Page, sizeof( Fpage_ctl_t ) );
		_DTlist_create();
	}
	_viewtaskID = os_task_assign_stk_build( File_Vtask, 3 );
	return;
}

void Fileview_close( void )
{
	OS_DELETE_REQ( _viewtaskID );
	_viewtaskID = INVALID_PRO;
	return;
}
//文本浏览器
void File_txtView(char*path,char*title)
{
#define  	TXTWIN_WIDE   400
#define  	TXTWIN_HIGH   400
    hwin_t*  pwin;
	char  *  pbuf ;
	FIL   *  fil;
	char  *  p;
	uint8_t  key_buf;
	int      y = 5;
	uint16_t sec;
	int      x1, y1, x2, y2;
    uint32_t *page;
	int       CURpage = 0;
    page      = app_malloc(100*sizeof(uint32_t));

	x1 = (LCD_WIDE-TXTWIN_WIDE)/2;
	y1 = (LCD_HIGH-TXTWIN_HIGH)/2;
	x2 = x1 + TXTWIN_WIDE;
	y2 = y1 + TXTWIN_HIGH;
	sec = GUI_read_block_Extp( x1, y1, x2, y2 );
	pbuf = app_malloc(1000);
	GUI_window_hwin_req( GET_GUIWIN );
	GUI_3D_widow_setFornt(&GUI_HZK16_EXTASCII);
	Gui_button_set_color(C_GUI_LOW_BLUE,0xce79);
	Gui_3D_window_set_color(0xce79,C_GUI_LOW_BLUE,C_GUI_RED);
	GUI_set_Bgcolor(0xce79);
	GUI_set_Fgcolor(0);
	pwin = Gui_3D_window_creat(x1 + 1, y1 + 1, TXTWIN_WIDE - 2, TXTWIN_HIGH - 2, title, WIN_3D | WIN_FILL, 0 );
    GUI_TopWIN_Set(pwin);
	GUI_window_hwin_release( GET_GUIWIN ); 
	fil=fatfs_open(path,FA_OPEN_EXISTING|FA_READ|FA_WRITE);
	page[CURpage] = fil->fptr;
	while(1)
	{
	    p = f_gets(pbuf,500,fil);
		if(!p)
		{
		    while(1)
			{
			    msleep(1);
				if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 
				{
					 if(key_buf==KEY_EXIT)
					 {
					    fatfs_close(fil);
						app_free(pbuf);
						app_free(page);
						GUI_window_destory(pwin);
						GUI_write_block_Extp( x1, y1, x2, y2,sec );
						app_module_color_select(2);
						return;
					 }
					 else if(key_buf==KEY_UP)
					 {
					 	if(CURpage>0)
						{
	                         f_lseek(fil,page[--CURpage]);
						 	 GUI_window_hwin_req( pwin );
							 GUI_set_Bgcolor( 0 );
							 GUI_set_Fgcolor( C_GUI_WHITE);
						     GUI_clean_window();
				             y = 5;
							 GUI_window_hwin_release( pwin );
							 break;
					 	}
					 }
				}
		    }
		}
		GUI_window_hwin_req( pwin );
		GUI_set_Bgcolor(0xce79);
		GUI_set_Fgcolor(0);
		GUI_SetFront(&GUI_HZK16_EXTASCII);
		GUI_string_display_At(2,y,pbuf);
		GUI_window_hwin_release( pwin );
		y+=GUI_conText.CUR_Front->high;
		if((y+GUI_conText.CUR_Front->high)>pwin->win_high)
		{
		    while(1)
		    {
		        msleep(1);
				if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 
				{
				     if(key_buf==KEY_DOWN)
				     {
				     	 page[++CURpage] = fil->fptr;
				     	 GUI_window_hwin_req( pwin );
						 GUI_set_Bgcolor( 0 );
						 GUI_set_Fgcolor( C_GUI_WHITE);
					     GUI_clean_window();
			             y = 5;
						 GUI_window_hwin_release( pwin );
						 break;
				     }
					 else if(key_buf==KEY_UP)
					 {
					     if(CURpage>0)
					     {
	                         f_lseek(fil,page[--CURpage]);
					     	 GUI_window_hwin_req( pwin );
							 GUI_set_Bgcolor( 0 );
							 GUI_set_Fgcolor( C_GUI_WHITE);
						     GUI_clean_window();
				             y = 5;
							 GUI_window_hwin_release( pwin );
							 break;
					     }
					 }
					 else if(key_buf==KEY_EXIT)
					 {
						app_free(page);
						app_free(pbuf);
						GUI_window_destory(pwin);
						GUI_write_block_Extp( x1, y1, x2, y2,sec );
						app_module_color_select(2);
						return;
					 }
				}
		    }
		}
	}
}
