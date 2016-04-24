#include 		"key_input method.h"

/*输入框大小设置*/
#define             	INPUT_WINW          	360
#define             	INPUT_WINH           	30
#define              	INPUT_HZW        		270
#define              	INPUT_HZH         		27
#define    	    		INPUT_PYW            	87
#define    	    		INPUT_PYH             	27
/*颜色配置*/
#define                 PY_WIN_BG_COLOR         C_GUI_WHITE-1
#define                 PY_WIN_FRAME_COLOR    	C_GUI_RED
#define                 PY_WIN_FG_COLOR   		C_GUI_BlACK
#define                 HZ_WIN_BG_COLOR         C_GUI_WHITE-1
#define                 HZ_WIN_FRAME_COLOR    	C_GUI_RED
#define                 HZ_WIN_FG_COLOR   		C_GUI_BlACK


#define   				CH_WIDE               	12
#define   				CH_HIGH               	24
#define                 INPUT_WAIT_TIME    	   (APP_100_MS*12)
#define           	    MAX_PAGE_HZ_NUM  	    7


/*定义拼音控制结构*/
typedef  struct
{
	uint8_t code[7];
	uint8_t offset;
	uint16_t csrx;
	uint16_t csry;
	GUI_csr_t*pcsr;
}key_input_py_ctl;

typedef  struct
{
	Bsize_t max_page;
	Bsize_t page_offset;
	Bsize_t end_offset;
	uint8_t *pbuf;
}key_input_hz_ctl;

static hwin_t*             	_GB_input_WIN 		= PNULL;        /*输入主窗口句柄*/
static hwin_t*             	_GB_pybox 			= PNULL;        /*输入拼音子窗口*/
static hwin_t*            	_GB_hzbox 			= PNULL;        /*输入汉字子窗口*/
static key_input_py_ctl 	_GBpy_ctl;                          /*拼音输入光标控制*/
static key_input_hz_ctl*	_GBhz_ctl 			= PNULL;        /*汉字页面输入控制*/
static uint16_t          	_GBwin_x 			= 30;          	/*当前汉字窗口位置*/
static uint16_t           	_GBwin_y 			= 400;
static BLKmem_t 			_GB_BLK 			= 0;
static BOOL    				_GB_state    		= 1;
static int    				_GBtmr_EN 			= 0;
static int    				_GBtmr 				= 0;
static int              	_GBkey_repeat       = 0;
static int              	_GBkey_last         = 0;
/*******************************************************************
 * @创建人 :揭成
 * @功能 :设置输入框位置
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void key_input_GB_xy_set( uint16_t x, uint16_t y )
{
	_GBwin_x = x;
	_GBwin_y = y;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 创建汉语拼音输入框
 *
 * @输入 :NONE
 *
 *@输出  : NONE
********************************************************************/
void key_input_method_hzbox_creat( uint16_t x, uint16_t y )
{
	_GB_BLK=GUI_read_block_Extp(
		x - 1,
		y - 1,
		x + INPUT_WINW + 1,
		y + INPUT_WINH + 1);
	_GB_input_WIN = GUI_window_creat(x,y,INPUT_WINW,INPUT_WINH,0);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :销毁汉语拼音输入框
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
void key_input_method_hzbox_destory( uint16_t x, uint16_t y )
{
	GUI_write_block_Extp(
		x - 1,
		y - 1,
		x + INPUT_WINW + 1,
		y + INPUT_WINH + 1, _GB_BLK );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :划分划子页面
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
static void key_input_method_hz_page_divide( void )
{
	Bsize_t page_size;
	char*ptemp;
	Bsize_t offset;
	_GB_state = 1;
	if ( _GBhz_ctl ) app_free( _GBhz_ctl );
	_GBhz_ctl = PNULL;
	ptemp = ( char* )py_ime( _GBpy_ctl.code );/*返回查询到GB字符，标准*/
	if ( global_memcmp( ptemp, "  ", 2 ) ){_GB_state=0;return;}
	page_size = global_strlen( ptemp ) + MAX_PAGE_HZ_NUM * 2 - 1;
	page_size = page_size / ( MAX_PAGE_HZ_NUM * 2 );
	_GBhz_ctl = app_malloc( page_size * sizeof( key_input_hz_ctl ) );
	for ( offset = 0;offset < page_size;offset++ )
	{
		_GBhz_ctl[offset].max_page = page_size;
		_GBhz_ctl[offset].page_offset = offset;
		_GBhz_ctl[offset].pbuf = ( uint8_t* )ptemp + ( MAX_PAGE_HZ_NUM * 2 * offset );
		if ( offset == ( page_size - 1 ) )
		{
			_GBhz_ctl[offset].end_offset
			= global_strlen( ptemp ) - offset * MAX_PAGE_HZ_NUM * 2;
		}
		else
		{
			_GBhz_ctl[offset].end_offset = MAX_PAGE_HZ_NUM * 2;
		}
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :汉字页输入
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
static void key_input_method_hz_page_display( key_input_hz_ctl * pcur_page, uint8_t num_flag )
{
	Bsize_t offset;
	Bsize_t start_offset;
	char    hz_buf[5];
	start_offset = 0;
	GUI_window_hwin_req( _GB_hzbox );
	GUI_set_Fgcolor( HZ_WIN_FG_COLOR );
	GUI_set_Bgcolor( HZ_WIN_BG_COLOR );
	if ( num_flag == 0 )GUI_clean_window();
	if ( !pcur_page )
	{
		GUI_window_hwin_release( _GB_hzbox );
		return;
	}
	for ( offset = 0;offset < ( pcur_page->end_offset ) / 2;offset++ )
	{
		if ( num_flag == 1 )
		{
			hz_buf[0] = GLOBAL_NTOC( offset );
			hz_buf[1] = ASCII_NULL;
			GUI_set_Fgcolor( C_GUI_RED );
			GUI_string_display_At( offset*38, 0, ( char* )hz_buf );
		}
		else
		{
			global_memcpy( &hz_buf[0], &pcur_page->pbuf[start_offset], 2 );
			hz_buf[2] = ASCII_NULL;
			GUI_string_display_At(12+ offset*38, 0, ( char* )hz_buf );
		}
		start_offset += 2;
	}
	GUI_window_hwin_release( _GB_hzbox );
	_GBkey_repeat = 0;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :    汉字输入窗口绘制
 *
 * @输入 :
 *
 *@输出  : NONE
********************************************************************/
static void key_input_method_hzbox_paint( void )
{
	GUI_set_Bgcolor( HZ_WIN_BG_COLOR );
	GUI_set_Fgcolor( HZ_WIN_FRAME_COLOR );
	if ( !_GB_hzbox )
		_GB_hzbox = GUI_window_child_creat(
							INPUT_PYW + 1, 0, INPUT_HZW, INPUT_HZH,
							WIN_PAINT | WIN_FILL,
							_GB_input_WIN );
	key_input_method_hz_page_divide();
	key_input_method_hz_page_display( &_GBhz_ctl[0], 0 );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :等待用户输入数字按键
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
static uint8_t key_input_method_wait_Nkey( void )
{
	uint8_t key;
	char     temp;
	while ( 1 )
	{
		if ( app_global_key_force_obtain( &key, 1 ) == 1 )
		{
			temp = app_global_decide_Nkey( key );
			if ( temp != FAIL )
			{
				temp = GLOBAL_CTON( temp );
				if ( temp < MAX_PAGE_HZ_NUM )
					break;
			}
			else if ( key == KEY_EXIT )
			{
				return 10;
			}
		}
		msleep( 1 );
	}
	return temp;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :拼音输入确认等待
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
void key_GB_wait_Exc(void)
{
	if(!_GBtmr_EN)return;
	if(++_GBtmr>=INPUT_WAIT_TIME)
	{
	    _GBtmr_EN = 0;
		_GBpy_ctl.offset++;
		GUI_window_hwin_req( _GB_pybox );
		GUI_x_shift( CH_WIDE );
		GUI_window_hwin_release( _GB_pybox );
		key_input_method_hzbox_paint();
		_GBkey_repeat = 0;
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :创建等待
 *
 * @输入 :
 *
 *@输出  :NONE
********************************************************************/
static void key_input_method_wait_set( void )
{
	_GBtmr_EN 	= 1;
	_GBtmr 		= 0;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :    拼音输入窗口绘制
 *
 * @输入 :
 *
 *@输出  : NONE
********************************************************************/
static void key_input_method_pybox_paint( void )
{
	GUI_set_Bgcolor( PY_WIN_BG_COLOR );
	GUI_set_Fgcolor( PY_WIN_FRAME_COLOR);
	_GB_pybox = GUI_window_child_creat(
		0,0,INPUT_PYW,INPUT_PYH,
		WIN_PAINT|WIN_FILL,_GB_input_WIN );
	GUI_window_hwin_req( _GB_pybox );
	GUI_set_Fgcolor( PY_WIN_FG_COLOR );
	GUI_string_display_At( 0, 0, ( char* )_GBpy_ctl.code );
	GUI_x_shift( -CH_WIDE );
	_GBpy_ctl.pcsr = GUI_cursor_start(_GB_pybox,CH_WIDE,CH_HIGH);
	GUI_window_hwin_release( _GB_pybox );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :获取按键输入法字符
 *
 * @输入 :按键值  buf 汉字BUF
 *
 *@输出  : NONE
********************************************************************/
static char  _GBchar_obtain( uint8_t key )
{
	uint8_t ctemp;
	if ( key == KEY_0 || key == KEY_1 || key == KEY_POINT )
		return ' ';
	ctemp =  app_global_keytoletter( key );
	ctemp+= _GBkey_repeat;
	if ( _GBkey_last != key )
	{
		_GBkey_last = key;
		_GBkey_repeat = 0;
	}
	else
	{
		if ( key == KEY_9 || key == KEY_7 )
		{
			(_GBkey_repeat==3)?(_GBkey_repeat = 0 ):(_GBkey_repeat++ );
		}
		else
		{
			(_GBkey_repeat==2)?(_GBkey_repeat=0):(_GBkey_repeat++ );
		}
	}
	return ctemp;
}
/********************************************************************
 * @创建人:揭成
 * @功能 : 拼音输入功能函数
 * @输入 :
 * @输出 : NONE
********************************************************************/
int key_method_GB( uint8_t code_0, char*hzbuf )
{
	uint8_t key;
	BOOL    ret = FAIL;
	uint8_t page_offset = 0;
	hwin_t*hp;
	uint16_t bg,fg;
	code_0 =  app_global_keytoletter(code_0);
	hp = GUI_CURwin_obtain();
	bg = GUI_conText.Bcolor;
	fg = GUI_conText.Fcolor;
	GLOBAL_MEMCLR( &_GBpy_ctl, sizeof( key_input_py_ctl ) );
	_GBhz_ctl = PNULL;
	_GBkey_repeat = 0;
	_GBpy_ctl.code[_GBpy_ctl.offset] = code_0;
	key_input_method_hzbox_creat( _GBwin_x, _GBwin_y );
	GUI_TopWIN_Set(_GB_input_WIN);
	GUI_window_hwin_release(hp);
	GUI_window_hwin_req( _GB_input_WIN ); 
	key_input_method_pybox_paint();
	key_input_method_wait_set();
	GUI_window_hwin_release( _GB_input_WIN ); 
	while ( 1 )
	{
		msleep(1);
		key_GB_wait_Exc();
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		GUI_window_hwin_req( _GB_input_WIN ); 
		if (( app_global_decide_PYkey( key ) ) != FAIL 
			&& ( key != KEY_DEL )
			&& ( _GBpy_ctl.offset < 6 ) )
		{
			GUI_window_hwin_req( _GB_pybox ); 			
			GUI_set_Bgcolor( PY_WIN_BG_COLOR );  			
			GUI_set_Fgcolor( PY_WIN_FG_COLOR );  			
			_GBpy_ctl.code[_GBpy_ctl.offset]= _GBchar_obtain( key ); 			
			GUI_string_display(( char* )& _GBpy_ctl.code[_GBpy_ctl.offset] );
			key_input_method_wait_set();              		
			GUI_x_shift( -CH_WIDE );
			GUI_window_hwin_release( _GB_pybox ); 		
			page_offset = 0;
		}
		else if ( key == KEY_DEL )            			
		{
			if ( _GBpy_ctl.offset > 0 )
			{
				GUI_window_hwin_req( _GB_pybox );
				GUI_set_Bgcolor( PY_WIN_BG_COLOR );
				GUI_set_Fgcolor( PY_WIN_FG_COLOR );
				GUI_clean_window();
				_GBpy_ctl.code[_GBpy_ctl.offset] = ASCII_NULL;
				_GBpy_ctl.offset--;
				GUI_string_display_At( 0, 0,(char*)_GBpy_ctl.code );
				GUI_x_shift( -CH_WIDE );
				GUI_window_hwin_release( _GB_pybox );
				_GBpy_ctl.code[_GBpy_ctl.offset] = ASCII_NULL;
				page_offset = 0;
				key_input_method_hzbox_paint();
			}
		}
		if(_GB_hzbox)
		{
			if ( key == KEY_DOWN&&_GB_state)
			{
				if ( page_offset < (_GBhz_ctl[0].max_page-1))
				{
					( page_offset++ );
					key_input_method_hz_page_display( &_GBhz_ctl[page_offset], 0 );
				}
			}
			else if ( key == KEY_UP&&_GB_state)
			{
				if ( page_offset > 0 )
				{
					( page_offset-- );
					key_input_method_hz_page_display( &_GBhz_ctl[page_offset], 0 );
				}
			}
			else if ( key == KEY_ENTER)
			{
				key_input_method_hz_page_display( &_GBhz_ctl[page_offset], 1 );
				if ( _GBhz_ctl == PNULL ) 
				{
					GUI_window_hwin_release( _GB_input_WIN );
					continue;
				}
				key = key_input_method_wait_Nkey();
				if ( key < 10 )
				{
					ret = TURE;
					global_memcpy( hzbuf, &_GBhz_ctl[page_offset].pbuf[key*2], 2 );
				}
				break;
			}
			else if ( key == KEY_EXIT)
			{
				ret = FAIL;
				break;
			}
		}
		GUI_window_hwin_release( _GB_input_WIN ); 
	}
	GUI_cursor_stop( _GBpy_ctl.pcsr );       
	key_input_method_hzbox_destory( _GBwin_x, _GBwin_y );
	GUI_window_hwin_release( _GB_input_WIN ); 
	GUI_window_destory( _GB_input_WIN );		
	if ( _GBhz_ctl ) app_free( _GBhz_ctl ); 
	_GB_input_WIN 	= PNULL;
	_GB_hzbox 	= PNULL;
	_GBkey_repeat   = 0;
	GUI_window_hwin_req(hp);
	GUI_set_Fgcolor(fg);
	GUI_set_Bgcolor(bg);
	return ret;
}
