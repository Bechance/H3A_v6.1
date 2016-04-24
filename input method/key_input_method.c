/*********************************************************************************************
*            ÷È∫£ø∆‹ˆµÁ∆˜”–œﬁπ´Àæ
*            http://www.zhkh.com
* ƒ£øÈ√˚≥∆:œµÕ≥ ‰»Î∑®π‹¿Ì
* ¥¥Ω® ±º‰:2010-12-04
* À˘ Ùπ´Àæ :ø∆‹ˆ
* Œƒº˛√˚≥∆:key_input method.c
* ¥¥Ω®»À :Ω“≥…
* π¶ƒ‹√Ë ˆ:œµÕ≥ ‰»Î∑®œ‡πÿ∫Ø ˝
*----------------------------------------------------------------
–ﬁ∏ƒº«¬º:

–ﬁ∏ƒ»’∆⁄:

–ﬁ∏ƒ»À:
********************************************************************/
#include 				"key_input method.h"
#define        			KEY_INPUT_FONT         ((GUI_FONT*)&GUI_HZK24_EXTASCII)
#define   				CH_WIDE               	12
#define   				CH_HIGH               	24
#define                 INPUT_WAIT_TIME    	   (APP_100_MS*12)
#define           	    MAX_PAGE_HZ_NUM  	    7
/*—’…´≈‰÷√*/
#define                 PY_WIN_BG_COLOR         C_GUI_WHITE-1
#define                 PY_WIN_FRAME_COLOR    	C_GUI_RED
#define                 PY_WIN_FG_COLOR   		C_GUI_BlACK
#define                 HZ_WIN_BG_COLOR         C_GUI_WHITE-1
#define                 HZ_WIN_FRAME_COLOR    	C_GUI_RED
#define                 HZ_WIN_FG_COLOR   		C_GUI_BlACK
/*  ‰»Î∑®±Í ∂¥Û–°…Ë÷√*/
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
/* ‰»ÎøÚ¥Û–°…Ë÷√*/
#define             	INPUT_WINW          	360
#define             	INPUT_WINH           	30
#define              	INPUT_HZW        		270
#define              	INPUT_HZH         		27
#define    	    		INPUT_PYW            	87
#define    	    		INPUT_PYH             	27
/*∂®“Â∆¥“Ùøÿ÷∆Ω·ππ*/
typedef  struct
{
	uint8_t code[7];
	uint8_t offset;
	uint16_t csrx;
	uint16_t csry;
	GUI_csr_t*pcsr;
}key_input_py_ctl;

/*∂®“Â±‰¡ø*/
key_input_ctl_t   		m_key_input_ctl;                    /*∞¥º¸ ‰»Îøÿ÷∆*/
hwin_t*             	m_input_icon_win 	= PNULL;      	/* ‰»Î∑®Õº±Í¥∞ø⁄æ‰±˙*/
BLKmem_t               	m_con_box_buf;                     	/*±£¥Êµ±«∞ ‰»Î∑®œ‘¥ÊøÈBUF*/
hwin_t*             	m_input_win 		= PNULL;        /* ‰»Î÷˜¥∞ø⁄æ‰±˙*/
hwin_t*               	m_input_pybox 		= PNULL;        /* ‰»Î∆¥“Ù◊”¥∞ø⁄*/
hwin_t*               	m_input_hzbox 		= PNULL;        /* ‰»Î∫∫◊÷◊”¥∞ø⁄*/
BLKmem_t 				m_key_fsec 			= 0;
key_input_py_ctl 		m_py_ctl;                           /*∆¥“Ù ‰»Îπ‚±Íøÿ÷∆*/
key_input_hz_ctl   		*m_hz_ctl 			= PNULL;        /*∫∫◊÷“≥√Ê ‰»Îøÿ÷∆*/
uint16_t               	m_input_win_posx 	= 30;          	/*µ±«∞∫∫◊÷¥∞ø⁄Œª÷√*/
uint16_t               	m_input_win_posy 	= 400;
uint8_t                 g_key_input_keep_secret  = FAIL;	/* ‰»Î±£√‹£¨”√*±Ì æ*/
Bsize_t                 m_key_input_edit_memeber = 0;
static                  BOOL  _GB_state    = 1;
static 					char _XXchMEM[50]="";

BOOL 	 GUI_keypad_is_exist(void);
Bsize_t  GUI_keypad_GB_obtain(char*hzbuf );

/********************************************************************
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ : ¥¥Ω®∫∫”Ô∆¥“Ù ‰»ÎøÚ
 *
 * @ ‰»Î :NONE
 *
 *@ ‰≥ˆ  : NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹   :œ˙ªŸ∫∫”Ô∆¥“Ù ‰»ÎøÚ
 * @ ‰»Î   :NONE
 * @ ‰≥ˆ   :NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹   : ‰»Î∑®±Í÷æœ‘ æ
 * @ ‰»Î   :NONE
 * @ ‰≥ˆ   :NONE
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
		GUI_string_display_At( 8, 1, "∫∫ " );
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
 * @¥¥Ω®»À:Ω“≥…
 * @π¶ƒ‹  :≈–∂œ ‰»Î∑® «∑Ò’˝‘⁄‘À––
 * @ ‰»Î  :NONE
 * @ ‰≥ˆ  :TURE OR  FAIL
********************************************************************/
BOOL key_input_method_is_run( void )
{
	return ( m_input_icon_win == PNULL ) ? ( FAIL ) : ( TURE );
}
/********************************************************************
 * @¥¥Ω®»À:Ω“≥…
 * @π¶ƒ‹  : ‰»Î∑®∞¥º¸ªÿµ˜
 * @ ‰»Î  :NONE
 *@ ‰≥ˆ   :NONE
********************************************************************/
#if GUI_TOUCH_EN >0
/********************************************************************
 * @¥¥Ω®»À:Ω“≥…
 * @π¶ƒ‹  :…Ë÷√ ÷–¥º¸≈ÃŒª÷√
 * @ ‰»Î  :NONE
 * @ ‰≥ˆ  :NONE
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
			GUI_button_title_display(p,"πÿ±’º¸≈Ã");
			hand_write_input_entery();
		}
		else 
		{
			GUI_button_title_display(p,"¥Úø™º¸≈Ã");
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
	GUI_button_title_display(p,"¥Úø™º¸≈Ã");
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ : πÿ±’ ‰»Î∑®±Í÷æ
 *
 * @ ‰»Î :NONE
 *
 *@ ‰≥ˆ  : NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ : ÷∏∂®µ•Ãı±‡º≠ ‰»Î∑®÷÷¿‡£¨π˝¬À ‰»Î∑®
 *
 * @ ‰»Î :
 *
 *@ ‰≥ˆ  : NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :  ‰»Î∑®«–ªª
 *
 * @ ‰»Î :NONE
 *
 *@ ‰≥ˆ  : NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶‹ : ÷∏∂® ‰»Î∑®
 *
 * @ ‰»Î :NONE
 *
 *@ ‰≥ˆ  : NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ : ªÒ»°µ±«∞ ‰»Î∑®
 *
 * @ ‰»Î :NONE
 *
 *@ ‰≥ˆ  : NONE
********************************************************************/
uint8_t key_input_method_obtain( void )
{
	return m_key_input_ctl.input_select;
}
tmr_member_t*m_input_tmr_id = PNULL;
/********************************************************************
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :ªÆ∑÷ªÆ◊”“≥√Ê
 *
 * @ ‰»Î :
 *
 *@ ‰≥ˆ  :NONE
********************************************************************/
static void key_input_method_hz_page_divide( void )
{
	Bsize_t page_size;
	char*ptemp;
	Bsize_t offset;
	_GB_state = 1;
	if ( m_hz_ctl ) app_free( m_hz_ctl );
	m_hz_ctl = PNULL;
	ptemp = ( char* )py_ime( m_py_ctl.code );/*∑µªÿ≤È—ØµΩGB◊÷∑˚£¨±Í◊º*/
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :∫∫◊÷“≥ ‰»Î
 *
 * @ ‰»Î :
 *
 *@ ‰≥ˆ  :NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :    ∫∫◊÷ ‰»Î¥∞ø⁄ªÊ÷∆
 *
 * @ ‰»Î :
 *
 *@ ‰≥ˆ  : NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :µ»¥˝”√ªß ‰»Î ˝◊÷∞¥º¸
 *
 * @ ‰»Î :
 *
 *@ ‰≥ˆ  :NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :∆¥“Ù ‰»Î»∑»œµ»¥˝
 *
 * @ ‰»Î :
 *
 *@ ‰≥ˆ  :NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :¥¥Ω®µ»¥˝
 *
 * @ ‰»Î :
 *
 *@ ‰≥ˆ  :NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :ªÒ»°∞¥º¸ ‰»Î∑®◊÷∑˚
 *
 * @ ‰»Î :∞¥º¸÷µ  buf ∫∫◊÷BUF
 *
 *@ ‰≥ˆ  : NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :    ∆¥“Ù ‰»Î¥∞ø⁄ªÊ÷∆
 *
 * @ ‰»Î :
 *
 *@ ‰≥ˆ  : NONE
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
 * @¥¥Ω®»À:Ω“≥…
 * @π¶ƒ‹ : ∆¥“Ù ‰»Îπ¶ƒ‹∫Ø ˝
 * @ ‰»Î :
 * @ ‰≥ˆ : NONE
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
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )/*≈–∂œ«˝∂Ø «∑Ò ’µΩ∞¥º¸œÏ”¶*/
		{
			continue;
		}
		GUI_window_hwin_req( m_input_win ); 
		if (( app_global_decide_inputkey( key ) ) != FAIL  /*π˝¬À∞¥º¸÷µ*/
			&& ( key != KEY_DEL )
			&& ( m_py_ctl.offset < 6 ) )
		{
			GUI_window_hwin_req( m_input_pybox ); 			/*…Í«ÎµΩ∆¥“Ù¥∞ø⁄*/
			GUI_SetFront(KEY_INPUT_FONT);
			GUI_set_Bgcolor( PY_WIN_BG_COLOR );  			/*…Ë÷√±≥æ∞…´*/
			GUI_set_Fgcolor( PY_WIN_FG_COLOR );  			/*…Ë÷√«∞æ∞…´*/
			m_py_ctl.code[m_py_ctl.offset]
			= key_input_method_char_obtain( key ); 			/*∂¡»°µ±«∞∆¥“Ù¬Î£¨”…∞¥º¸◊™ªª*/
			GUI_string_display(( char* )& m_py_ctl.code[m_py_ctl.offset] );
			key_input_method_wait_set();              		/*»∑»œµ»¥˝*/
			GUI_x_shift( -CH_WIDE );
			GUI_window_hwin_release( m_input_pybox ); 		/* Õ∑≈¥∞ø⁄◊ ‘¥*/
			page_offset = 0;
		}
		else if ( key == KEY_DEL )            				/*…æ≥˝≤Ÿ◊˜*/
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
		GUI_window_hwin_release( m_input_win ); /* Õ∑≈¥∞ø⁄◊ ‘¥*/
	}
    timer_member_state_set( m_input_tmr_id, TMR_DELETE_REQ );
	GUI_cursor_stop( m_py_ctl.pcsr );        /*Õ£÷ππ‚±Í*/
	key_input_method_hzbox_destory( m_input_win_posx, m_input_win_posy );
	GUI_window_hwin_release( m_input_win ); /* Õ∑≈¥∞ø⁄◊ ‘¥*/
	GUI_window_destory( m_input_win );		/*œ˙ªŸ∆¥“Ù¥∞ø⁄*/
	if ( m_hz_ctl ) app_free( m_hz_ctl ); 	/* Õ∑≈∫∫◊÷“≥√Êƒ⁄¥Ê*/
	m_input_win 	= PNULL;
	m_input_hzbox 	= PNULL;
	m_key_input_ctl.key_press_repeat = 0;
	GUI_window_hwin_req(hp);
	GUI_set_Fgcolor(fg);
	GUI_set_Bgcolor(bg);
	return ret;
}
/********************************************************************
 * @¥¥Ω®»À:Ω“≥…
 * @π¶ƒ‹  :≈–∂œ ‰»Î «∑Ò∫œ∑®
 * @ ‰»Î  :
 * @ ‰≥ˆ  :NONE
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
 * @¥¥Ω®»À:Ω“≥…
 * @π¶ƒ‹ :∞¥º¸ ‰»Î∑®£¨ªÒ»° ‰»Î◊÷∑˚
 * @ ‰»Î :
 * @ ‰≥ˆ :NONE
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
			GUI_edit_csr_shift( CSR_LEFT_SHIFT );/*±‡º≠øÚπ‚±Í «∑Ò¥ÔµΩŒ≤≤ø*/
		ret = 1;
	}
	return ret;
}
/********************************************************************
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :  ≈–∂œ «◊÷∑˚ªπ «∫∫◊÷
 *
 * @ ‰»Î :src, offset
 *
 *@ ‰≥ˆ  : 
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :  :∞¥º¸ ‰»Î∑®£¨±£¥Ê‘⁄ª∫≥Âpshadow
 *
 * @ ‰»Î :
 *
 *@ ‰≥ˆ  : NONE
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
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :…Ë÷√ ‰»ÎøÚŒª÷√
 *
 * @ ‰»Î :NONE
 *
 *@ ‰≥ˆ  :NONE
********************************************************************/
void key_input_method_xy_set( uint16_t x, uint16_t y )
{
	m_input_win_posx = x;
	m_input_win_posy = y;
	return;
}
/********************************************************************
 * @¥¥Ω®»À :Ω“≥…
 * @π¶ƒ‹ :≥ı ºªØ∫∫◊÷ ‰»Îƒ£øÈ
 *
 * @ ‰»Î :NONE
 *
 *@ ‰≥ˆ  :NONE
********************************************************************/
void key_input_method_init( void )
{
	app_global_key_resource_req();
	GLOBAL_MEMCLR( &m_key_input_ctl, sizeof( key_input_ctl_t ) );
	m_key_input_ctl.input_select = KEY_INPUT_123;
	key_input_method_filter(KEY_METHOD_ALL);
	return;
}
