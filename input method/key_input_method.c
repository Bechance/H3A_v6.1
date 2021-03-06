/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统输入法管理
* 创建时间:2010-12-04
* 所属公司 :科荟
* 文件名称:key_input method.c
* 创建人 :揭成
* 功能描述:系统输入法相关函数
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:
********************************************************************/
#include 				"key_input method.h"
#define        			KEY_INPUT_FONT         ((GUI_FONT*)&GUI_HZK24_EXTASCII)
#define   				CH_WIDE               	12
#define   				CH_HIGH               	24
#define                 INPUT_WAIT_TIME    	   (APP_100_MS*12)
#define           	    MAX_PAGE_HZ_NUM  	    7
/*颜色配置*/
#define                 PY_WIN_BG_COLOR         C_GUI_WHITE-1
#define                 PY_WIN_FRAME_COLOR    	C_GUI_RED
#define                 PY_WIN_FG_COLOR   		C_GUI_BlACK
#define                 HZ_WIN_BG_COLOR         C_GUI_WHITE-1
#define                 HZ_WIN_FRAME_COLOR    	C_GUI_RED
#define                 HZ_WIN_FG_COLOR   		C_GUI_BlACK
/* 输入法标识大小设置*/
#if GUI_TOUCH_EN >0
#define    	      		INPUT_ICONXS      		(LCD_WIDE-100)
#define    	      		INPUT_ICONYS      		(LCD_HIGH-60)
#define           		INPUT_ICONW     		 90
#define    	      		INPUT_ICONH      		 55
#else
#define    	      		INPUT_ICONXS      		(LCD_WIDE-70)
#define    	      		INPUT_ICONYS      		(LCD_HIGH-34)
#define           		INPUT_ICONW     		 60
#define    	      		INPUT_ICONH      		 30
#endif
/*输入框大小设置*/
#define             	INPUT_WINW          	360
#define             	INPUT_WINH           	30
#define              	INPUT_HZW        		270
#define              	INPUT_HZH         		27
#define    	    		INPUT_PYW            	87
#define    	    		INPUT_PYH             	27
/*定义拼音控制结构*/
typedef  struct
{
	uint8_t code[7];
	uint8_t offset;
	uint16_t csrx;
	uint16_t csry;
	GUI_csr_t*pcsr;
}key_input_py_ctl;

/*定义变量*/
key_input_ctl_t   		m_key_input_ctl;                    /*按键输入控制*/
hwin_t*             	m_input_icon_win 	= PNULL;      	/*输入法图标窗口句柄*/
BLKmem_t               	m_con_box_buf;                     	/*保存当前输入法显存块BUF*/
hwin_t*             	m_input_win 		= PNULL;        /*输入主窗口句柄*/
hwin_t*               	m_input_pybox 		= PNULL;        /*输入拼音子窗口*/
hwin_t*               	m_input_hzbox 		= PNULL;        /*输入汉字子窗口*/
BLKmem_t 				m_key_fsec 			= 0;
key_input_py_ctl 		m_py_ctl;                           /*拼音输入光标控制*/
key_input_hz_ctl   		*m_hz_ctl 			= PNULL;        /*汉字页面输入控制*/
uint16_t               	m_input_win_posx 	= 30;          	/*当前汉字窗口位置*/
uint16_t               	m_input_win_posy 	= 400;
uint8_t                 g_key_input_keep_secret  = FAIL;	/*输入保密，用*表示*/
Bsize_t                 m_key_input_edit_memeber = 0;
static                  BOOL  _GB_state    = 1;
static 					char _XXchMEM[50]="";

BOOL 	 GUI_keypad_is_exist(void);
Bsize_t  GUI_keypad_GB_obtain(char*hzbuf );

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
	m_key_fsec=GUI_read_block_Extp(
		x - 1,
		y - 1,
		x + INPUT_WINW + 1,
		y + INPUT_WINH + 1);
	m_input_win = GUI_window_creat(x,y,INPUT_WINW,INPUT_WINH,0);
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
		y + INPUT_WINH + 1, m_key_fsec );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :输入法标志显示
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
static void key_input_method_icon_display( void )
{
	key_input_ctl_t*pkey = &m_key_input_ctl;
	GUI_window_hwin_req( m_input_icon_win );
	GUI_set_Bgcolor( 0xce79);
	GUI_set_Fgcolor( C_GUI_RED);
	GUI_SetFront(KEY_INPUT_FONT);
	if ( pkey->input_select == KEY_INPUT_PY )
	{
		GUI_string_display_At( 8, 1, "汉 " );
	}
	else if ( pkey->input_select == KEY_INPUT_123 )
	{
		GUI_string_display_At( 8, 1, "123" );
	}
	else if ( pkey->input_select == KEY_INPUT_abc )
	{
		GUI_string_display_At( 8, 1, "abc" );
	}
	else if ( pkey->input_select == KEY_INPUT_ABC )
	{
		GUI_string_display_At( 8, 1, "ABC" );
	}
	GUI_window_hwin_release( m_input_icon_win );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :判断输入法是否正在运行
 * @输入  :NONE
 * @输出  :TURE OR  FAIL
********************************************************************/
BOOL key_input_method_is_run( void )
{
	return ( m_input_icon_win == PNULL ) ? ( FAIL ) : ( TURE );
}
/********************************************************************
 * @创建人:揭成
 * @功能  :输入法按键回调
 * @输入  :NONE
 *@输出   :NONE
********************************************************************/
#if GUI_TOUCH_EN >0
/********************************************************************
 * @创建人:揭成
 * @功能  :设置手写键盘位置
 * @输入  :NONE
 * @输出  :NONE
********************************************************************/
void hand_wirte_input_box_pos_set( uint16_t x, uint16_t y )
{
    GUI_keypad_Setpos(x,y);
	return;
}
void hand_write_input_entery(void)
{
	GUI_keypad_open();
    return;
}
void hand_write_input_exit(void)
{
   GUI_keypad_close();
   return;
}
void input_button_call(void*arg)
{
    hwin_t*p = (hwin_t*)arg;
    if((start_edit_win->flag&WIN_TOUCH_EN))
    {
		if(!GUI_keypad_is_exist())
		{
			GUI_button_title_display(p,"关闭键盘");
			hand_write_input_entery();
		}
		else 
		{
			GUI_button_title_display(p,"打开键盘");
			hand_write_input_exit();
		}
    }
	return;
}

void key_input_method_icon_open( void )
{
    hwin_t*p;
	m_con_box_buf=GUI_read_block_Extp(
		INPUT_ICONXS - 2,
		INPUT_ICONYS - 2,
		INPUT_ICONXS + INPUT_ICONW + 5,
		INPUT_ICONYS + INPUT_ICONH + 5);
	GUI_set_Bgcolor( 0xce79);
	GUI_set_Fgcolor( C_GUI_RED);
	m_input_icon_win= GUI_window_creat(
		  INPUT_ICONXS,
		  INPUT_ICONYS,
		  INPUT_ICONW,
		  INPUT_ICONH,WIN_3D|WIN_FILL);
	GUI_window_hwin_req(m_input_icon_win);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Bgcolor(0xce79);
	GUI_set_Fgcolor(C_GUI_RED);
	p= Gui_button_creat(
		  3,
		  INPUT_ICONH/2,
		  INPUT_ICONW-8,
		  INPUT_ICONH/2-2,0xfe|BUTTON_PAINT,m_input_icon_win);
	Gui_button_dye(p,LCD_Color2Index_565(0xc0c0c0),C_GUI_RED);
	GUI_button_title_display(p,"打开键盘");
	GUI_button_callback_mount(p,input_button_call);
	key_input_method_icon_display();
	GUI_TopWIN_Set(m_input_icon_win);
	GUI_window_hwin_release(m_input_icon_win);
	return;
}
#else
void key_input_method_icon_open( void )
{
	m_con_box_buf=GUI_read_block_Extp(
		INPUT_ICONXS - 2,
		INPUT_ICONYS - 2,
		INPUT_ICONXS + INPUT_ICONW + 5,
		INPUT_ICONYS + INPUT_ICONH + 5);
	GUI_set_Bgcolor( 0xce79);
	GUI_set_Fgcolor( C_GUI_RED);
	m_input_icon_win= GUI_window_creat(
		  INPUT_ICONXS,
		  INPUT_ICONYS,
		  INPUT_ICONW,
		  INPUT_ICONH,WIN_3D|WIN_FILL);
	GUI_window_hwin_req(m_input_icon_win);
	key_input_method_icon_display();
	GUI_TopWIN_Set(m_input_icon_win);
	GUI_window_hwin_release(m_input_icon_win);
	return;
}
void hand_write_input_entery(void)
{
}
void hand_write_input_exit(void)
{
}
void hand_wirte_input_box_pos_set( uint16_t x, uint16_t y )
{
}
#endif
/********************************************************************
 * @创建人 :揭成
 * @功能 : 关闭输入法标志
 *
 * @输入 :NONE
 *
 *@输出  : NONE
********************************************************************/
void key_input_method_icon_close( void )
{
	GUI_TopWIN_cancel(m_input_icon_win);
	GUI_write_block_Extp(
		INPUT_ICONXS -2,
		INPUT_ICONYS -2,
		INPUT_ICONXS + INPUT_ICONW + 5,
		INPUT_ICONYS + INPUT_ICONH + 5, m_con_box_buf);
	GUI_window_destory( m_input_icon_win );
	m_input_icon_win = PNULL;
	hand_write_input_exit();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 指定单条编辑输入法种类，过滤输入法
 *
 * @输入 :
 *
 *@输出  : NONE
********************************************************************/
void key_input_method_filter( uint8_t flag )
{
	key_input_ctl_t*pkey = &m_key_input_ctl;
	pkey->input_flag = flag;
	if ( !( pkey->input_flag&pkey->input_select ) )
	key_input_method_switch();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 输入法切换
 *
 * @输入 :NONE
 *
 *@输出  : NONE
********************************************************************/
void key_input_method_switch( void )
{
    uint8_t temp;
	key_input_ctl_t*pkey = &m_key_input_ctl;
	if ( pkey->input_select == 0 )
	{
		pkey->input_select = KEY_FIRST_INPUT;
	}
	temp = pkey->input_select;
	temp = temp;
	while ( 1 )
	{
		if ( pkey->input_select == KEY_END_INPUT )
		{
			pkey->input_select = KEY_FIRST_INPUT;
		}
		else
		{
			pkey->input_select = pkey->input_select << 1;
		}
		if (( pkey->input_flag&pkey->input_select ) )break;
	}
	hand_write_input_exit();
	key_input_method_icon_display();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功� : 指定输入法
 *
 * @输入 :NONE
 *
 *@输出  : NONE
********************************************************************/
void key_input_method_assign( uint8_t flag )
{
	if ( flag <= KEY_END_INPUT)
	{
		m_key_input_ctl.input_select = flag;
	}
	key_input_method_icon_display();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 获取当前输入法
 *
 * @输入 :NONE
 *
 *@输出  : NONE
********************************************************************/
uint8_t key_input_method_obtain( void )
{
	return m_key_input_ctl.input_select;
}
tmr_member_t*m_input_tmr_id = PNULL;
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
	if ( m_hz_ctl ) app_free( m_hz_ctl );
	m_hz_ctl = PNULL;
	ptemp = ( char* )py_ime( m_py_ctl.code );/*返回查询到GB字符，标准*/
	if ( global_memcmp( ptemp, "  ", 2 ) ){_GB_state=0;return;}
	page_size = global_strlen( ptemp ) + MAX_PAGE_HZ_NUM * 2 - 1;
	page_size = page_size / ( MAX_PAGE_HZ_NUM * 2 );
	m_hz_ctl = app_malloc( page_size * sizeof( key_input_hz_ctl ) );
	for ( offset = 0;offset < page_size;offset++ )
	{
		m_hz_ctl[offset].max_page = page_size;
		m_hz_ctl[offset].page_offset = offset;
		m_hz_ctl[offset].pbuf = ( uint8_t* )ptemp + ( MAX_PAGE_HZ_NUM * 2 * offset );
		if ( offset == ( page_size - 1 ) )
		{
			m_hz_ctl[offset].end_offset
			= global_strlen( ptemp ) - offset * MAX_PAGE_HZ_NUM * 2;
		}
		else
		{
			m_hz_ctl[offset].end_offset = MAX_PAGE_HZ_NUM * 2;
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
	GUI_window_hwin_req( m_input_hzbox );
	GUI_SetFront(KEY_INPUT_FONT);
	GUI_set_Fgcolor( HZ_WIN_FG_COLOR );
	GUI_set_Bgcolor( HZ_WIN_BG_COLOR );
	if ( num_flag == 0 )GUI_clean_window();
	if ( !pcur_page )
	{
		GUI_window_hwin_release( m_input_hzbox );
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
	GUI_window_hwin_release( m_input_hzbox );
	m_key_input_ctl.key_press_repeat = 0;
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
	if ( !m_input_hzbox )
		m_input_hzbox = GUI_window_child_creat(
							INPUT_PYW + 1, 0, INPUT_HZW, INPUT_HZH,
							WIN_PAINT | WIN_FILL,
							m_input_win );
	key_input_method_hz_page_divide();
	key_input_method_hz_page_display( &m_hz_ctl[0], 0 );
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
			temp = app_global_decide_inputkey( key );
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
static void key_input_method_wait( void*arg )
{
	m_input_tmr_id = PNULL;
	if ( m_key_input_ctl.input_select != KEY_INPUT_PY )
	{
		if ( m_key_input_edit_memeber == start_edit_win->subord_cur->id )
		{
			GUI_window_hwin_req( start_edit_win );
			GUI_edit_csr_shift( CSR_RIGHT_SHIFT );
			GUI_window_hwin_release( start_edit_win );
		}
	}
	else
	{
		m_py_ctl.offset++;
		GUI_window_hwin_req( m_input_pybox );
		GUI_x_shift( CH_WIDE );
		GUI_window_hwin_release( m_input_pybox );
		key_input_method_hzbox_paint();
	}
	m_key_input_ctl.key_press_repeat = 0;
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
	if ( m_input_tmr_id != PNULL )
	{
		timer_member_state_set( m_input_tmr_id, TMR_DELETE_REQ );
		m_input_tmr_id = PNULL;
	}
	m_input_tmr_id = timer_member_create(
						 INPUT_WAIT_TIME,
						 TIMER_SETTINGS,
						 key_input_method_wait, 0 );
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
static char  key_input_method_char_obtain( uint8_t key )
{
	uint8_t ctemp;
	key_input_ctl_t*pkey = &m_key_input_ctl;
	if ( pkey->input_select == KEY_INPUT_123 )
	{
		ctemp =  app_global_decide_inputkey( key );
		return ctemp;
	}
	if ( m_input_tmr_id != PNULL )
	{
		timer_member_state_set( m_input_tmr_id, TMR_DELETE_REQ );
		m_input_tmr_id = PNULL;
	}
	if ( pkey->input_select != KEY_INPUT_PY )
	{
		m_input_tmr_id = timer_member_create(
							 INPUT_WAIT_TIME,
							 TIMER_SETTINGS,
							 key_input_method_wait, 0 );
		m_key_input_edit_memeber = start_edit_win->subord_cur->id;
	}
	if ( key == KEY_0 || key == KEY_1 || key == KEY_POINT )
		return ' ';
	if ( pkey->input_select == KEY_INPUT_ABC )
	{
		ctemp  = app_global_keytoletter( key );
		ctemp  = UPCASE( ctemp );
		ctemp += pkey->key_press_repeat;
	}
	else if ( pkey->input_select == KEY_INPUT_abc
			  || pkey->input_select == KEY_INPUT_PY )
	{
		ctemp =  app_global_keytoletter( key );
		ctemp += pkey->key_press_repeat;
	}
	if ( pkey->last_key != key )
	{
		pkey->last_key = key;
		pkey->key_press_repeat = 0;
	}
	else
	{
		if ( key == KEY_9 || key == KEY_7 )
		{
			( pkey->key_press_repeat == 3 )
			? ( pkey->key_press_repeat = 0 )
			: ( pkey->key_press_repeat++ );
		}
		else
		{
			( pkey->key_press_repeat == 2 )
			? ( pkey->key_press_repeat = 0 )
			: ( pkey->key_press_repeat++ );
		}
	}
	return ctemp;
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
	m_input_pybox = GUI_window_child_creat(
		0,0,INPUT_PYW,INPUT_PYH,
		WIN_PAINT|WIN_FILL,m_input_win );
	GUI_window_hwin_req( m_input_pybox );
	GUI_set_Fgcolor( PY_WIN_FG_COLOR );
	GUI_SetFront(KEY_INPUT_FONT);
	GUI_string_display_At( 0, 0, ( char* )m_py_ctl.code );
	GUI_x_shift( -CH_WIDE );
	m_py_ctl.pcsr = GUI_cursor_start(m_input_pybox,CH_WIDE,CH_HIGH);
	GUI_window_hwin_release( m_input_pybox );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 : 拼音输入功能函数
 * @输入 :
 * @输出 : NONE
********************************************************************/
static  BOOL key_input_method_GBcod_obtain( uint8_t code_0, char*hzbuf )
{
	uint8_t key;
	BOOL    ret = FAIL;
	uint8_t page_offset = 0;
	hwin_t*hp;
	uint16_t bg,fg;
	hp = GUI_CURwin_obtain();
	bg = GUI_conText.Bcolor;
	fg = GUI_conText.Fcolor;
	GLOBAL_MEMCLR( &m_py_ctl, sizeof( key_input_py_ctl ) );
	m_hz_ctl = PNULL;
	m_key_input_ctl.key_press_repeat = 0;
	m_py_ctl.code[m_py_ctl.offset] = code_0;
	key_input_method_hzbox_creat( m_input_win_posx, m_input_win_posy );
	GUI_TopWIN_Set(m_input_win);
	GUI_window_hwin_release(hp);
	GUI_window_hwin_req( m_input_win ); 
	key_input_method_pybox_paint();
	key_input_method_wait_set();
	GUI_window_hwin_release( m_input_win ); 
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )/*判断驱动是否收到按键响应*/
		{
			continue;
		}
		GUI_window_hwin_req( m_input_win ); 
		if (( app_global_decide_inputkey( key ) ) != FAIL  /*过滤按键值*/
			&& ( key != KEY_DEL )
			&& ( m_py_ctl.offset < 6 ) )
		{
			GUI_window_hwin_req( m_input_pybox ); 			/*申请到拼音窗口*/
			GUI_SetFront(KEY_INPUT_FONT);
			GUI_set_Bgcolor( PY_WIN_BG_COLOR );  			/*设置背景色*/
			GUI_set_Fgcolor( PY_WIN_FG_COLOR );  			/*设置前景色*/
			m_py_ctl.code[m_py_ctl.offset]
			= key_input_method_char_obtain( key ); 			/*读取当前拼音码，由按键转换*/
			GUI_string_display(( char* )& m_py_ctl.code[m_py_ctl.offset] );
			key_input_method_wait_set();              		/*确认等待*/
			GUI_x_shift( -CH_WIDE );
			GUI_window_hwin_release( m_input_pybox ); 		/*释放窗口资源*/
			page_offset = 0;
		}
		else if ( key == KEY_DEL )            				/*删除操作*/
		{
			if ( m_py_ctl.offset > 0 )
			{
				GUI_window_hwin_req( m_input_pybox );
				GUI_set_Bgcolor( PY_WIN_BG_COLOR );
				GUI_set_Fgcolor( PY_WIN_FG_COLOR );
				GUI_SetFront(KEY_INPUT_FONT);
				GUI_clean_window();
				m_py_ctl.code[m_py_ctl.offset] = ASCII_NULL;
				m_py_ctl.offset--;
				GUI_string_display_At( 0, 0,(char*)m_py_ctl.code );
				GUI_x_shift( -CH_WIDE );
				GUI_window_hwin_release( m_input_pybox );
				m_py_ctl.code[m_py_ctl.offset] = ASCII_NULL;
				page_offset = 0;
				key_input_method_hzbox_paint();
			}
		}
		if(m_input_hzbox)
		{
			if ( key == KEY_DOWN&&_GB_state)
			{
				if ( page_offset < (m_hz_ctl[0].max_page-1))
				{
					( page_offset++ );
					key_input_method_hz_page_display( &m_hz_ctl[page_offset], 0 );
				}
			}
			else if ( key == KEY_UP&&_GB_state)
			{
				if ( page_offset > 0 )
				{
					( page_offset-- );
					key_input_method_hz_page_display( &m_hz_ctl[page_offset], 0 );
				}
			}
			else if ( key == KEY_ENTER)
			{
				key_input_method_hz_page_display( &m_hz_ctl[page_offset], 1 );
				if ( m_hz_ctl == PNULL ) 
				{
					GUI_window_hwin_release( m_input_win );
					continue;
				}
				key = key_input_method_wait_Nkey();
				if ( key < 10 )
				{
					ret = TURE;
					global_memcpy( hzbuf, &m_hz_ctl[page_offset].pbuf[key*2], 2 );
				}
				break;
			}
			else if ( key == KEY_EXIT)
			{
				ret = FAIL;
				break;
			}
		}
		GUI_window_hwin_release( m_input_win ); /*释放窗口资源*/
	}
    timer_member_state_set( m_input_tmr_id, TMR_DELETE_REQ );
	GUI_cursor_stop( m_py_ctl.pcsr );        /*停止光标*/
	key_input_method_hzbox_destory( m_input_win_posx, m_input_win_posy );
	GUI_window_hwin_release( m_input_win ); /*释放窗口资源*/
	GUI_window_destory( m_input_win );		/*销毁拼音窗口*/
	if ( m_hz_ctl ) app_free( m_hz_ctl ); 	/*释放汉字页面内存*/
	m_input_win 	= PNULL;
	m_input_hzbox 	= PNULL;
	m_key_input_ctl.key_press_repeat = 0;
	GUI_window_hwin_req(hp);
	GUI_set_Fgcolor(fg);
	GUI_set_Bgcolor(bg);
	return ret;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :判断输入是否合法
 * @输入  :
 * @输出  :NONE
********************************************************************/
BOOL key_method_input_lawful(char*buf,Bsize_t size)
{
   if(size==0) return 0;
   if(size==1)
   {
     if(m_key_input_ctl.input_flag&KEY_INPUT_123)
     {
        if(( '0' <= buf[0])&&(buf[0]<= '9' ))return 1;
		if(buf[0]=='.' ) return 1;
     }
     if(m_key_input_ctl.input_flag&KEY_INPUT_ABC
	   ||m_key_input_ctl.input_flag&KEY_INPUT_abc)
     {
        if(( 'a' <= buf[0])&&(buf[0]<= 'z' ))return 1;
        if(( 'A' <= buf[0])&&(buf[0]<= 'Z' ))return 1;
        if(( '0' <= buf[0])&&(buf[0]<= '9' ))return 1;
		if(buf[0]=='.' ) return 1;
     }   
   }
   else
   {
     if(m_key_input_ctl.input_flag&KEY_INPUT_PY)
     {
         return 1;
     } 
   }
   return 0;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :按键输入法，获取输入字符
 * @输入 :
 * @输出 :NONE
********************************************************************/
Bsize_t key_method_input( uint8_t key, char*buf )
{
	char ctemp[2] = "";
	BOOL ret = 0;
	key_input_ctl_t*pkey = &m_key_input_ctl;
	if ( edit_is_start == FAIL ) return FAIL;
	if ( key == KEY_DEL )
	{
		buf[0] = ASCII_NULL;
		GUI_edit_string_display( " " );
		if ( !m_eidt_end_flag )
		{
			GUI_edit_csr_shift( CSR_LEFT_SHIFT );
		}
		GUI_edit_csr_shift( CSR_LEFT_SHIFT );
		return 1;
	}
#if GUI_TOUCH_EN>0
	if(GUI_keypad_is_exist())
	{
		ret = GUI_keypad_GB_obtain(ctemp);
        if(!key_method_input_lawful(ctemp,ret))
        {
              return 0;
		}
		global_memcpy(buf,ctemp,ret);
		if ( ret )
		{
            if(g_key_input_keep_secret&&ret==1)
            {
				GUI_edit_string_display( "*" );
			}
			else
			{
				buf[ret] = ASCII_NULL;
				GUI_edit_string_display( buf );
			}
		}
		else ret = 0;  
		return ret;
	}
#endif
	ctemp[0] = key_input_method_char_obtain( key );
	if ( pkey->input_select == KEY_INPUT_123 )
	{
		if ( !g_key_input_keep_secret )
			GUI_edit_string_display( ctemp );
		else
			GUI_edit_string_display( "*" );
		buf[0] = ctemp[0];
		buf[1] = ASCII_NULL;
		ret = 1;
	}
	else if ( pkey->input_select == KEY_INPUT_PY )
	{
		ret = key_input_method_GBcod_obtain( ctemp[0], buf );
		if ( ret )
		{
			buf[2] = ASCII_NULL;
			GUI_edit_string_display( buf );
			ret = 2;
		}
		else ret = 0;
	}
	else if(pkey->input_select == KEY_INPUT_ABC
	     || pkey->input_select == KEY_INPUT_abc )
	{
		GUI_edit_string_display( ctemp );
		buf[0] = ctemp[0];
		buf[1] = ASCII_NULL;
		if (!m_eidt_end_flag)    
			GUI_edit_csr_shift( CSR_LEFT_SHIFT );/*编辑框光标是否达到尾部*/
		ret = 1;
	}
	return ret;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :  判断是字符还是汉字
 *
 * @输入 :src, offset
 *
 *@输出  : 
********************************************************************/
static Bsize_t key_method_input_char_handle(char*src,Bsize_t offset,Bsize_t*pos)
{
	Bsize_t offs=0;
	while(1)
	{
		if (( src[offs]&0xff ) >= 0x81 )
		{
			offs+=2;		
			if (offs> offset) 
			{
				(*pos) = offs-2;
				return 2;
			}
		}
		else 
		{
			offs+=1;		
			if (offs> offset) 
			{
				(*pos) = offs-1;
				return 1;
			}
		}
	}
}			
/********************************************************************
 * @创建人 :揭成
 * @功能 :  :按键输入法，保存在缓冲pshadow
 *
 * @输入 :
 *
 *@输出  : NONE
********************************************************************/
Bsize_t key_method_input_shadow( uint8_t key, char*pshadow)
{
	Bsize_t offset;
	Bsize_t len;
	Bsize_t pos;
	Bsize_t size;
	char nch[3];
	offset = GUI_edit_Boffset_obtain();
    if(!pshadow)
    {
		pshadow = _XXchMEM;
        GUI_edit_content_obtain(pshadow,GUI_edit_id_obtain(),m_cur_hwin);
    }
	if ( key == KEY_DEL )
	{
		if(offset==0)	return 0;
		else offset = offset-1;
		size=key_method_input_char_handle(pshadow,offset,&pos);
		global_memcpy( &pshadow[pos],"  ",size);
		global_string_filter( pshadow, ' ' );
		GUI_edit_string_display_At( pshadow, GUI_edit_id_obtain() );
		GUI_edit_select( GUI_edit_id_obtain() );
		size = global_strlen(pshadow);
		(offset<size)?(offset=offset):(offset=size);
		for ( len = 0;len < offset;len++ )
			GUI_edit_csr_shift( CSR_RIGHT_SHIFT);
		return 1;
	}
	else
	{

		len = key_method_input( key, nch );
	    if((offset+len)<=m_cur_hwin->subord_cur->eidt_lenth)
	    {
			global_memcpy( &pshadow[offset], nch, len );
	    }
	}
	return len;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :设置输入框位置
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void key_input_method_xy_set( uint16_t x, uint16_t y )
{
	m_input_win_posx = x;
	m_input_win_posy = y;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :初始化汉字输入模块
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void key_input_method_init( void )
{
	app_global_key_resource_req();
	GLOBAL_MEMCLR( &m_key_input_ctl, sizeof( key_input_ctl_t ) );
	m_key_input_ctl.input_select = KEY_INPUT_123;
	key_input_method_filter(KEY_METHOD_ALL);
	return;
}
