#include "../../Gui.h"
#include "../GUI_lcd_driver.h"

#if   GUI_BUTTON_EN >0
uint16_t LCD_Color2Index_565( uint32_t Color );


void GUI_widget_3D_DrawDownRect( uint16_t x, uint16_t y, Bsize_t wide, Bsize_t high );
BOOL GUI_button_ontouch( hwin_t*hp, app_touch_t*data );
void GUI_bmp_display( int x0, int y0, void*pDATA );
void GUI_bmp_displayExt( int x0, int y0, void*pDATA, int sel );
void 	_button_select( hwin_t*hp, int state );
void 	_button_do( hwin_t*hp );
void 	_button_invalid( hwin_t*hp );
void  	_button_invalid_cancle( hwin_t*hp );

static uint16_t m_GUI_button_Bgcor = BUTTON_BG_COLOR;
static uint16_t m_GUI_button_Fgcor = BUTTON_TXT_COLOR;
void     GUI_draw_dot( uint16_t X, uint16_t Y );

static void GUI_button_fill_Rect( uint16_t x1, uint16_t y1, Bsize_t wide, Bsize_t high, uint16_t corlor )
{
	GUI_set_Bgcolor( corlor );
	GUI_fill_Rect( x1, y1, wide, high );
	return;
}

static void GUI_button_string_display(
	uint16_t x, uint16_t y,
	uint16_t xspace,
	uint16_t yspace,
	uint16_t Bcolor,
	uint16_t Fcolor,
	GUI_FONT*pFont,
	void *buf
)
{
	GUI_set_Bgcolor( Bcolor );
	GUI_set_Fgcolor( Fcolor );
	GUI_SetFront(( const GUI_FONT* )pFont );
	GUI_print_string( x, y, xspace, yspace, buf );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :获取按键控件数据
 * @输入  :
 * @输出  :
********************************************************************/
static widget_button_t*GUI_button_data_obtain( hwin_t *hp )
{
	return ( widget_button_t* )hp->pwidget;
}

/********************************************************************
 * @创建人:揭成
 * @功能  :按键选定状态显示
 * @输入  :
 * @输出  :
********************************************************************/
static void GUI_button_select_state_display( hwin_t *hp, uint16_t corlor )
{
	int x, y, n, x0, y0;
	if ( !hp )return;
	x0 = hp->win_x + 1;
	y0 = hp->win_y + 1;
	GUI_set_Fgcolor( corlor );
	for ( x = x0 + 2, n = 0;x < ( x0 + hp->win_wide - 4 );x++ )
	{
		if ( n++ % 2 == 0 )GUI_draw_dot( x, 2 + y0 );
	}
	for ( x = x0 + 2, n = 0;x < ( x0 + hp->win_wide - 4 );x++ )
	{
		if ( n++ % 2 == 0 )GUI_draw_dot( x, hp->win_high - 4 + y0 );
	}
	for ( y = 2 + y0, n = 0;y < ( hp->win_high - 4 + y0 );y++ )
	{
		if ( n++ % 2 == 0 )GUI_draw_dot( 2 + x0, y );
	}
	for ( y = 2 + y0, n = 0;y < ( hp->win_high - 4 + y0 );y++ )
	{
		if ( n++ % 2 == 0 )GUI_draw_dot( hp->win_wide - 4 + x0, y );
	}
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :按键重画
 * @输入  :
 * @输出  :
********************************************************************/
static void  _BUTTON_paint( hwin_t *hp )
{
	uint16_t flag = hp->flag;
	int x;
	int y = hp->win_high;
	widget_button_t *pbutton;
	pbutton = GUI_button_data_obtain( hp );
	if ( pbutton->Button_is_inv )
	{
		GUI_window_hwin_req( hp );
		( y > pbutton->pFONT->high ) ? ( y = ( hp->win_high - pbutton->pFONT->high ) / 2 ) : ( y = 0 );
		x = MIDDLE_COORD_GET( hp->win_wide, pbutton->press_title , pbutton->pFONT->CHwide );
		pbutton->Is_press = 0;
		GUI_button_fill_Rect( hp->win_x, hp->win_y,
							  hp->win_wide + 1, hp->win_high, LCD_Color2Index_565(0x555555) );
		GUI_widget_3D_DrawDownRect( hp->win_x + 1, hp->win_y,
									hp->win_wide - 2, hp->win_high );
		GUI_button_string_display( hp->win_x + x, hp->win_y + y,
								   hp->win_x + hp->win_wide, hp->win_y + hp->win_high,
								   LCD_Color2Index_565(0x555555),0X2145, pbutton->pFONT,
								   pbutton->press_title );
		GUI_window_hwin_release( hp );
		hp->WIN_Paint = 0;
		return;
	}
	if (( flag&BUTTON_ICON ) == BUTTON_ICON )
	{
		GUI_window_hwin_req( hp );
		if ( pbutton->Is_press )
		{
			if ( pbutton->press_icon )
				GUI_bmp_display( hp->win_x, hp->win_y, pbutton->press_icon );
			else
				GUI_bmp_displayExt( hp->win_x, hp->win_y, pbutton->unpress_icon, 1 );
		}
		else
		{
			if ( pbutton->unpress_icon )
				GUI_bmp_display( hp->win_x, hp->win_y, pbutton->unpress_icon );
		}
		GUI_window_hwin_release( hp );
		hp->WIN_Paint = 0;
		return;
	}
	if (( flag&BUTTON_PAINT ) != BUTTON_PAINT )
		return;
	GUI_window_hwin_req( hp );
	flag = flag;
	( y > pbutton->pFONT->high ) ? ( y = ( hp->win_high - pbutton->pFONT->high ) / 2 ) : ( y = 0 );
	if ( pbutton->Is_press )
	{
		x = MIDDLE_COORD_GET( hp->win_wide, pbutton->press_title , pbutton->pFONT->CHwide );
		pbutton->Is_press = 1;
		GUI_button_fill_Rect( hp->win_x, hp->win_y,
							  hp->win_wide + 1, hp->win_high, pbutton->press_BG_corlor );
		GUI_widget_3D_DrawDownRect( hp->win_x + 1, hp->win_y,
									hp->win_wide - 2, hp->win_high );
		GUI_button_string_display( hp->win_x + x, hp->win_y + y,
								   hp->win_x + hp->win_wide, hp->win_y + hp->win_high,
								   pbutton->press_BG_corlor, pbutton->press_FG_corlor, pbutton->pFONT,
								   pbutton->press_title );
	}
	else
	{
		x = MIDDLE_COORD_GET( hp->win_wide, pbutton->UNpress_title , pbutton->pFONT->CHwide );
		GUI_button_fill_Rect( hp->win_x, hp->win_y,
							  hp->win_wide + 1, hp->win_high, pbutton->unpress_BG_corlor );
		GUI_widget_3D_paint( hp->win_x, hp->win_y,
							 hp->win_wide, hp->win_high );
		GUI_button_string_display( hp->win_x + x, hp->win_y + y,
								   hp->win_x + hp->win_wide, hp->win_y + hp->win_high,
								   pbutton->unpress_BG_corlor, pbutton->unpress_FG_corlor, pbutton->pFONT,
								   pbutton->UNpress_title );
	}
	if ( pbutton->button_state == 1 )
	{
		GUI_button_select_state_display(hp, 0);
	}
	hp->WIN_Paint = 0;
	GUI_window_hwin_release(hp);
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :创建一个按键
 * @输入  :
 * @输出  :
********************************************************************/
hwin_t* Gui_button_creat(
	uint16_t xcoord,
	uint16_t ycoord,
	Bsize_t wide,
	Bsize_t high,
	Bsize_t flags,
	hwin_t* fatherhp
)
{
	hwin_t *hp;
	widget_button_t *pbutton;
	hp = GUI_window_child_creat(
			 xcoord,
			 ycoord,
			 wide,
			 high,
			 0,
			 fatherhp );
	GUI_LOCK();
	hp->type = WIDGET_BUTTON;
	hp->flag = flags;
	hp->pwidget = GUI_malloc( sizeof( widget_button_t ) );
	GUI_memset( hp->pwidget, 0, sizeof( widget_button_t ) );
	pbutton = GUI_button_data_obtain( hp );
	pbutton->buttoncallback   = 0;
	pbutton->unpress_BG_corlor  = m_GUI_button_Bgcor;
	pbutton->unpress_FG_corlor  = m_GUI_button_Fgcor;
	pbutton->press_BG_corlor    = m_GUI_button_Bgcor;
	pbutton->press_FG_corlor    = C_GUI_RED;

	pbutton->pFONT = ( GUI_FONT* )GUI_GetFront();
	if ( flags&BUTTON_2STATE )
	{
		pbutton->Is_2state = 1;
	}
#if GUI_TOUCH_EN>0
	hp->_ontouch = GUI_button_ontouch;
#endif
	hp->_paint      	   = _BUTTON_paint;
	if (( flags&BUTTON_PAINT ) == BUTTON_PAINT )
	{
		hp->_WINdo      	   = _button_do;
		hp->_WINinvalid 	   = _button_invalid;
		hp->_WINinvalid_cancle = _button_invalid_cancle;
		hp->_WINselect         = _button_select;
	}
	hp->WIN_Paint = 1;
	GUI_UNLOCK();
	return hp;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:把按键设置为选定状态
 * @输入:
 * @输出:
********************************************************************/
void _button_select( hwin_t*hp, int state )
{
	widget_button_t *pbutton;
	if ( !hp )return;
	if (( hp->flag&BUTTON_PAINT ) != BUTTON_PAINT )
		return;
	pbutton = GUI_button_data_obtain( hp );
	pbutton->button_state = state;
	hp->WIN_Paint = 1;
	return;
}

void _button_do( hwin_t*hp )
{
	widget_button_t *pbutton;
	if ( !hp ) return;
	GUI_LOCK();
	pbutton = GUI_button_data_obtain( hp );
	pbutton->Is_press = 1;
	hp->_paint( hp );
	GUI_msleep( APP_100_MS );
	pbutton->Is_press = 0;
	hp->_paint( hp );
	GUI_key_dummy_press( hp, ( hp->flag )&0xff );
	if ( pbutton->buttoncallback )
	{
		pbutton->buttoncallback( 0 );
	}
	GUI_UNLOCK();
	return;
}

void _button_invalid( hwin_t*hp )
{
	widget_button_t *pbutton;
	GUI_LOCK();
	pbutton = GUI_button_data_obtain( hp );
	pbutton->Button_is_inv = TURE;
	hp->WIN_Paint = 1;
	GUI_UNLOCK();
	return ;
}


void  _button_invalid_cancle( hwin_t*hp )
{
	widget_button_t *pbutton;
	GUI_LOCK();
	pbutton = GUI_button_data_obtain( hp );
	pbutton->Button_is_inv = FAIL;
	hp->WIN_Paint = 1;
	GUI_UNLOCK();
	return ;
}

/********************************************************************
 * @创建人: 揭?
 * @功能:给BUTTON 换底色
 * @输入:
 * @输出:
********************************************************************/
void Gui_button_set_color( uint16_t Fgcor, uint16_t Bgcor )
{
	m_GUI_button_Bgcor = Bgcor;
	m_GUI_button_Fgcor = Fgcor;
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:给BUTTON 换底色
 *
 * @输入:

 * @输出:
********************************************************************/
void Gui_button_insert_icon( hwin_t*hp, const char*picon, uint16_t Fgcor, uint16_t Bgcor )
{
	return;
}

void GUI_button_SetFont( hwin_t*hp, const GUI_FONT*pFONT )
{
	widget_button_t *pbutton;
	pbutton = GUI_button_data_obtain( hp );
	pbutton->pFONT = ( GUI_FONT* )pFONT;
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:给BUTTON 换底色
 *
 * @输入:

 * @输出:
********************************************************************/
void Gui_button_dye( hwin_t*hp, uint16_t bcolor, uint16_t fcolor )
{
	widget_button_t *pbutton;

	pbutton = GUI_button_data_obtain( hp );
	if ( hp->type != WIDGET_BUTTON )
		return;
	GUI_LOCK();
	pbutton->unpress_BG_corlor  = bcolor;
	pbutton->unpress_FG_corlor = fcolor;
	hp->WIN_Paint = 1;
	GUI_UNLOCK();
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:给BUTTON 换底色
 *
 * @输入:

 * @输出:
********************************************************************/
void Gui_2state_button_dye( hwin_t*hp, uint16_t bcolor, uint16_t fcolor )
{
	widget_button_t *pbutton;
	pbutton = GUI_button_data_obtain( hp );
	if ( hp->type != WIDGET_BUTTON )
		return;
	GUI_LOCK();
	pbutton->press_BG_corlor  = bcolor;
	pbutton->press_FG_corlor = fcolor;
	hp->WIN_Paint = 1;
	GUI_UNLOCK();
	return;
}

BOOL GUI_button_title_display( hwin_t*hp, char*title )
{
	widget_button_t *pbutton;
	GUI_LOCK();
	pbutton = GUI_button_data_obtain( hp );
	GUI_STRCPY( pbutton->UNpress_title, title );
	GUI_STRCPY( pbutton->press_title, title );
	hp->WIN_Paint = 1;
	GUI_UNLOCK();
	return 1;
}

//state = 0 未按
//state = 1 按下
BOOL GUI_button_Set_icon( hwin_t*hp, void*pDATA, int state )
{
	widget_button_t *pbutton;
	GUI_LOCK();
	pbutton = GUI_button_data_obtain( hp );
	if ( state == 0 )
		pbutton->unpress_icon = pDATA;
	else
		pbutton->press_icon   = pDATA;
	hp->WIN_Paint = 1;
	GUI_UNLOCK();
	return 1;
}

BOOL GUI_2state_button_press_titlecopy( hwin_t*hp, char*title )
{
	widget_button_t *pbutton;
	GUI_LOCK();
	pbutton = GUI_button_data_obtain( hp );
	GUI_STRCPY( pbutton->press_title, title );
	hp->WIN_Paint = 1;
	GUI_UNLOCK();
	return 1;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:设置按键回掉函数
 *
 * @输入:

 * @输出:
********************************************************************/
void GUI_button_callback_mount( hwin_t*hp, void( *callback )( void*arg ) )
{
	widget_button_t *pbutton;
	pbutton = GUI_button_data_obtain( hp );
	pbutton->buttoncallback = callback;
	return;
}
//2 态按键状态设置  state = 1 按下  = 0  抬起
void GUI_2state_button_assign( hwin_t*hp, BOOL state )
{

	widget_button_t *pbutton;
	GUI_window_hwin_req( hp );
	pbutton = GUI_button_data_obtain( hp );
	pbutton->Is_press = state;
	pbutton->button_state = state;
	hp->WIN_Paint = 1;
	GUI_window_hwin_release( hp );
	return;
}

BOOL GUI_2state_button_obtain( hwin_t*hp )
{
	widget_button_t *pbutton;
	pbutton = GUI_button_data_obtain( hp );
	return pbutton->Is_press;
}



BOOL  GUI_button_id_mod( hwin_t*hp, Bsize_t id )
{
	if (( hp->type == WIDGET_BUTTON )
		&& ( id < 255 ) )
	{
		hp->flag = ( hp->flag ) & ( 0xff00 );
		hp->flag |= id;
		return TURE;
	}
	return FAIL;
}

#if GUI_TOUCH_EN >0
void GUI_wait_touch_release( void )
{
	app_touch_t touch_data;
	Bsize_t cnt = 0;
	Bsize_t size;
	while (( cnt++ ) < 10 )
	{
		size = GUI_touch_read(
				   ( void* ) & touch_data,
				   sizeof( app_touch_t ) );
		if ( size == sizeof( app_touch_t ) && touch_data.state == 0 )
		{
			if ( cnt < 5 )GUI_msleep( 2 );
			return;
		}
		GUI_msleep( 1 );
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :按键状态改变，(按下或释放)
 *
 * @输入 :hp  BUTTON 句柄，data 获取到的触摸数据
 *
 *@输出  :NONE
***********************************************************/
static void Gui_button_ontouch_display( hwin_t*hp, app_touch_t*data )
{
	uint16_t flag = hp->flag;
	widget_button_t *pbutton;
	pbutton = GUI_button_data_obtain( hp );
	flag = flag;
	if ( data->state == 1 )
	{
		if ( !pbutton->Is_2state )
		{
			pbutton->Is_press = 1;
			hp->_paint( hp );
			GUI_wait_touch_release();
			pbutton->Is_press = 0;
			hp->_paint( hp );
		}
		else
		{
			if ( !pbutton->Is_press )
			{
				pbutton->Is_press = 1;
				hp->_paint( hp );
				GUI_wait_touch_release();
			}
			else if ( pbutton->Repress_EN )
			{
				pbutton->Is_press = 0;
				pbutton->button_state = 1;
				hp->_paint( hp );
				GUI_wait_touch_release();
			}
		}
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :触摸按键匹配
 *
 * @输入 :hp  BUTTON 句柄，data 获取到的触摸数据
 *
 *@输出  :NONE
***********************************************************/
static BOOL Gui_button_match( hwin_t*hp, app_touch_t*data )
{
	widget_button_t *pbutton;
	uint16_t flag = hp->flag;
	int xmax = hp->win_x + hp->win_wide + 2;
	int xmin  = hp->win_x - 2;
	int ymax = hp->win_y + hp->win_high + 2;
	int ymin  = hp->win_y - 2;
	pbutton = GUI_button_data_obtain( hp );
	( xmax > LCD_WIDE ) ? ( xmax = LCD_WIDE ) : ( xmax = xmax );
	( xmin < 0 ) ? ( xmin = 0 ) : ( xmin = xmin );
	( ymax > LCD_HIGH ) ? ( ymax = LCD_HIGH ) : ( ymax = ymax );
	( ymin < 0 ) ? ( ymin = 0 ) : ( ymin = ymin );
	if ( data->x < xmin ) return FAIL;
	if ( data->x > xmax ) return FAIL;
	if ( data->y < ymin ) return FAIL;
	if ( data->y > ymax ) return FAIL;
	if (( flag&BUTTON_PAINT ) == BUTTON_PAINT
		|| ( flag&BUTTON_ICON ) == BUTTON_ICON )
	{
		Gui_button_ontouch_display( hp, data );
	}
	else
	{
		if ( !pbutton->buttoncallback )
		{
			GUI_wait_touch_release();
		}
	}
	return TURE;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :按键触摸处理
 *
 * @输入 :hp  BUTTON 句柄，data 获取到的触摸数据
 *
 *@输出  :NONE
***********************************************************/
BOOL GUI_button_ontouch( hwin_t*hp, app_touch_t*data )
{
	widget_button_t *pbutton;
	pbutton = GUI_button_data_obtain( hp );
	if ( pbutton->Button_is_inv )return FAIL;
	if ( Gui_button_match( hp, data ) )
	{
		if ( pbutton->buttoncallback )
		{
			pbutton->buttoncallback( hp );
		}
		GUI_key_dummy_press( hp, ( hp->flag )&0xff );
		return TURE;
	}
	return FAIL;
}
#else
void GUI_wait_touch_release( void )
{

}
#endif
/********************************************************************
 * @创建人 :揭成
 * @功能 :按键触摸处理
 *
 * @输入 :控件初始化
 *@输出  :NONE
***********************************************************/
void GUI_button_init( void )
{
	m_GUI_button_Bgcor = BUTTON_BG_COLOR;
	m_GUI_button_Fgcor = BUTTON_TXT_COLOR;
	return;
}
#endif
