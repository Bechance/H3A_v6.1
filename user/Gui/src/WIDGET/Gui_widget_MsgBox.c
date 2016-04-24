#include "../../Gui.h"
#if   GUI_WIDGET_MSGBOX_EN >0
static uint16_t  m_MSGBOX_txtcor    =  C_GUI_RED;
static uint16_t  m_MSGBOX_BGcor     =  C_GUI_WHITE - 1;
static int 		 Msgbox_timeout 	=  0;
static char*     Msgbox_pcontent 	=  0;
const  GUI_FONT* m_MSGBOX_Font      =  &GUI_HZK24_EXTASCII;
static char* 	 m_box_password     =  0;
static uint8_t   m_GUI_language     =  0;
const  char*	 m_GUI_MSG_01[2]    =  {"确认", ""};
const  char*	 m_GUI_MSG_02[2]	=  {"退出", ""};
const  char*	 m_GUI_MSG_03[2]	=  {"请输入密码:", ""};
const  char*	 m_GUI_MSG_04[2]	=  {"密码错误,请重新输入!", ""};
const  char*	 m_GUI_MSG_05[2]	=  {"提示", ""};

void GUI_MsgBOX_Setcolor( int txtcolor, int bgcolr )
{
	m_MSGBOX_txtcor = txtcolor;
	m_MSGBOX_BGcor  = bgcolr;
	return;
}
void GUI_MsgBOX_SetFont( GUI_FONT*pFont )
{
	m_MSGBOX_Font = ( const GUI_FONT* )pFont;
	return;
}

void GUI_MsgBOX_Setlanguage(uint8_t language)
{
   m_GUI_language = language;
   return;
}

int GUI_MsgBox_creat(
	uint16_t x ,
	uint16_t y,
	Bsize_t wide,
	Bsize_t high,
	char *title,
	Bsize_t( *callback )( hwin_t*hp )
)
{
	hwin_t*pOBJ;
	int    key;
	BLKmem_t pmem;
	pmem = GUI_read_block_Extp( x - 2, y - 2, x + wide + 5, y + high + 5 );
	GUI_LOCK();
	GUI_set_Bgcolor( m_MSGBOX_BGcor );
	GUI_set_Fgcolor( m_MSGBOX_txtcor );
	pOBJ = Gui_3D_window_creat( x, y, wide, high, title, WIN_3D | WIN_FILL, GET_GUIWIN );
	GUI_UNLOCK();
	GUI_TopWIN_Set( pOBJ );
	GUI_keyExt_Lock();
	key = callback( pOBJ );
	GUI_keyExt_unLock();
	GUI_TopWIN_cancel( pOBJ );
	GUI_window_destory( pOBJ );
	GUI_write_block_Extp( x - 2, y - 2, x + wide + 5, y + high + 5, pmem );
	return key;
}


static void Msgbox_display( hwin_t*pOBJ )
{
#if GUI_TOUCH_EN >0
	int    x0, y0, Bwide, Bhigh, Bgap;
	hwin_t* pB;
#endif
	GUI_window_hwin_req( pOBJ );
	GUI_set_Fgcolor( m_MSGBOX_txtcor );
	GUI_set_Bgcolor( m_MSGBOX_BGcor );
	GUI_SetFront( m_MSGBOX_Font );
	GUI_string_display_At( 2, 2, Msgbox_pcontent );
#if GUI_TOUCH_EN >0
	y0 = pOBJ->win_y + pOBJ->win_high;
	y0 = y0 - pOBJ->csr_y_pos;
	Bwide = ( m_MSGBOX_Font->CHwide * 4 ) + 20;
	Bhigh =  m_MSGBOX_Font->high + 10;
	Bgap  = ( pOBJ->win_wide - Bwide * 2 ) / 3;
	if ( Bgap > 4 && y0 > ( Bhigh + 5 ) )
	{
		GUI_MsgFIFO_create( pOBJ, 10 );
		x0 =  Bgap;
		y0 =  pOBJ->win_high - ( Bhigh + 5 );
		GUI_SetFront( &GUI_HZK16_EXTASCII );
		pB =  Gui_button_creat( x0, y0, Bwide, Bhigh, BUTTON_PAINT | GUIE_KEY_ENTER, pOBJ );
		GUI_button_title_display( pB, (char*)m_GUI_MSG_01[m_GUI_language]);
		pB =  Gui_button_creat( x0 + Bwide + Bgap, y0, Bwide, Bhigh, BUTTON_PAINT | GUIE_KEY_EXIT, pOBJ );
		GUI_button_title_display( pB, (char*)m_GUI_MSG_02[m_GUI_language]);
	}
#endif
	GUI_window_hwin_release( pOBJ );
	return;
}



static Bsize_t MsgBox_callback( hwin_t*pwin )
{
	uint8_t  key;
	uint32_t timeS = 0;
	timeS = GUI_os_jiffies_obtain();
	Msgbox_display( pwin );
	GUI_GetEXTkey( &key );
	while ( 1 )
	{
		GUI_msleep( 3 );
		if ( GUI_MsgFIFO_read( pwin, &key, 1 ) == 1 )
		{
			if ( key == GUIE_KEY_ENTER ) break;
			if ( key == GUIE_KEY_EXIT ) break;
		}
		if ( GUI_GetEXTkey( &key ) )
		{
			if ( key == GUIE_KEY_ENTER ) break;
			if ( key == GUIE_KEY_EXIT ) break;
		}
		if ( Msgbox_timeout == 0 ) continue;
		if (( GUI_os_jiffies_obtain() - timeS ) >= Msgbox_timeout )
		{
			key = GUIE_KEY_EXIT;
			break;
		}
	}
	return key;
}

int  MsgBox(
	int x, int y,
	Bsize_t wide, Bsize_t high,
	char*   title, char*pcontent,
	Bsize_t timeout
)
{
	Msgbox_pcontent = pcontent;
	Msgbox_timeout  = timeout;
	return GUI_MsgBox_creat(
			   x, y, wide, high, title,
			   MsgBox_callback );
}

const    char*m_NOkey_tab[] =
{
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "OK", "DEL"
};

static Bsize_t MsgPasswordBox_callback( hwin_t*pwin )
{
	hwin_t *	edit;
	hwin_t *	pOBJ, *pCHID;
	int         x0, y0, Bwide, Bhigh, xgap, ygap, pos;
	uint8_t  	id, key, cnt = 0;
	char        input[12] = "";
	char        dinput[12] = "";
	int         input_pos = 0;
	GUI_csr_t*  pcsr;
	GUI_FONT*pF = ( GUI_FONT* ) & GUI_HZK24_EXTASCII;
	edit = GUI_edit_creat( 0, 2, pwin->win_wide - 2, 40, 0, pwin );
	GUI_set_Fgcolor( C_GUI_BlACK );
	GUI_set_Bgcolor( MSGBOX_BG_COLOR );
	GUI_window_hwin_req( edit );
	GUI_SetFront( &GUI_HZK24_EXTASCII );
	GUI_edit_memeber_add( 5, 5, 10, (char*)m_GUI_MSG_03[m_GUI_language], MEMBER_3D | 1 );
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_edit_string_display_At( "", 1 );
	GUI_window_hwin_release( edit );
	pCHID = GUI_window_child_creat(0,42,pwin->win_wide - 2,pwin->win_high - 42, WIN_PAINT, pwin );
	GUI_window_hwin_req( pCHID );
	Bwide =  pF->CHwide * 3 + 5;
	Bhigh =  pF->high + 10;
	xgap  = ( pCHID->win_wide - Bwide * 3 ) / 4;
	ygap  = ( pCHID->win_high - Bhigh * 4 ) / 5;
	GUI_SetFront( pF );
	for ( pos = 0;pos < SIZE_OF_ARRAY( m_NOkey_tab );pos++ )
	{
		if ( pos == 10 )
			id   = GUIE_KEY_ENTER;
		else if ( pos == 11 )
			id   = GUIE_KEY_ESC;
		else
			id   = GUIE_KEY_0 + pos;
		x0   = xgap + ( pos % 3 ) * ( Bwide + xgap );
		y0   = ygap + ( pos / 3 ) * ( Bhigh + ygap );
		pOBJ = Gui_button_creat( x0, y0, Bwide, Bhigh, BUTTON_PAINT | id, pCHID );
		GUI_button_title_display( pOBJ, ( char* )m_NOkey_tab[pos] );
	}
	pcsr = GUI_cursor_start( edit, 2, edit->subord_cur->CHhigh );
	GUI_window_hwin_release( pCHID );
	GUI_GetEXTkey( &key );
	while ( cnt < 3 )
	{
		GUI_msleep( 1 );
		if ( GUI_GetEXTkey( &key ) )
		{
			if ( key == GUIE_KEY_ENTER )
			{
				if ( GUI_memcmp(m_box_password,input,GUI_strlen(m_box_password)))break;
				else
				{
					GUI_3D_window_title_mod( pwin,(char*)m_GUI_MSG_04[m_GUI_language]);
					GUI_msleep( APP_100_MS*20 );
					cnt++;
					input_pos = 0;
					GUI_window_hwin_req( edit );
					GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
					GUI_MEMCLR( input, 12 );
					GUI_MEMCLR( dinput, 12 );
					GUI_edit_string_display_At( "", 1 );
					GUI_window_hwin_release( edit );
					GUI_3D_window_title_mod( pwin, (char*)m_GUI_MSG_05[m_GUI_language]);
				}
			}
			else if ( GUIE_KEY_0 <= key && key <= GUIE_KEY_9 )
			{
				input[input_pos] = '0' + ( key - GUIE_KEY_0 );
				dinput[input_pos] = '*';
				input_pos++;
				GUI_window_hwin_req( edit );
				GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
				GUI_set_Fgcolor( C_GUI_WHITE );
				GUI_edit_string_display_At( dinput, 1 );
				GUI_window_hwin_release( edit );
			}
			else if ( key == GUIE_KEY_ESC )
			{
				input[input_pos] = 0;
				dinput[input_pos] = 0;
				if ( input_pos > 0 )input_pos--;
				GUI_window_hwin_req( edit );
				GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
				GUI_set_Fgcolor( C_GUI_WHITE );
				GUI_edit_string_display_At( dinput, 1 );
				GUI_window_hwin_release( edit );
			}
			else if (key == GUIE_KEY_EXIT || key == GUI_KEY_3D_WINDOW_CLOSE )
			{
				break;
			}
		}
	}
	if ( cnt == 3 ) key = GUIE_KEY_EXIT;
	GUI_cursor_stop( pcsr );
	return key;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :密码提示对话框
 *
 * @输入 :(x,y)坐标
 *
 *@输出  :NONE
********************************************************************/
BOOL MsgPasswordBox( int x , int y, char*password )
{
	uint8_t key;
	m_box_password = password;
	key = GUI_MsgBox_creat( x, y, 300, 250,(char*)m_GUI_MSG_05[m_GUI_language], MsgPasswordBox_callback );
	if ( key == GUIE_KEY_ENTER ) return TURE;
	else if ( key == GUIE_KEY_EXIT ) return FAIL;
	return FAIL;
}

void Gui_MsgBox_init( void )
{
	m_MSGBOX_txtcor  =  C_GUI_RED;
	m_MSGBOX_BGcor   =  C_GUI_WHITE - 1;
	return;
}
/**********************************************************************/
//wait WIN
/**********************************************************************/
#define  	WAITWIN_WIDE   250
#define  	WAITWIN_HIGH   60
#define  	WAITWIN_BAR    100

static      uint16_t 	_waitSec;
static      hwin_t*  	_waitWIN;
static      hwin_t*     _waitscrb;
void GUI_waitWIN_create(char*str)
{
	int x1, y1, x2, y2;
	x1 = (LCD_WIDE-WAITWIN_WIDE)/2;
	y1 = (LCD_HIGH-WAITWIN_HIGH)/2;
	x2 = x1 + WAITWIN_WIDE;
	y2 = y1 + WAITWIN_HIGH;
	_waitSec = GUI_read_block_Extp( x1, y1, x2, y2 );
	GUI_window_hwin_req( GET_GUIWIN );
	GUI_set_Bgcolor( C_GUI_WHITE - 1);
	_waitWIN = GUI_window_creat( x1 + 1, y1 + 1, WAITWIN_WIDE - 2, WAITWIN_HIGH - 2, WIN_3D | WIN_FILL );
	GUI_TopWIN_Set( _waitWIN );
	GUI_window_hwin_req(_waitWIN );
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(C_GUI_WHITE - 1);
	GUI_set_Fgcolor(C_GUI_RED);
	GUI_string_display_At(2,2, str);
	GUI_window_hwin_release( _waitWIN );
	GUI_window_hwin_release( GET_GUIWIN );
    return;
}

void GUI_waitWIN_CHmod(char*str)
{
	GUI_window_hwin_req( _waitWIN );
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(C_GUI_WHITE - 1);
	GUI_set_Fgcolor(C_GUI_RED);
	GUI_clean_window();
	GUI_string_display_At(2,2, str);
	GUI_window_hwin_release( _waitWIN );
	return;
}

void GUI_waitWIN_destory(void)
{
	int x1, y1, x2, y2;
	x1 = (LCD_WIDE-WAITWIN_WIDE)/2;
	y1 = (LCD_HIGH-WAITWIN_HIGH)/2;
	x2 = x1 + WAITWIN_WIDE;
	y2 = y1 + WAITWIN_HIGH;
	GUI_window_destory(_waitWIN);
	GUI_write_block_Extp(x1,y1,x2, y2,_waitSec);
	return;
}
#endif
