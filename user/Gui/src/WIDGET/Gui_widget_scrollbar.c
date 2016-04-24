#include "../../Gui.h"
#include "../GUI_lcd_driver.h"
#if	GUI_SCROLLBAR_EN  >0
#define   DEU_BGCOR                 C_GUI_WHITE
#define   DEU_FGCOR                 1151
typedef struct
{
	uint16_t   CURpos;
	uint16_t   totalpos;
	uint16_t   FG;
	uint16_t   BG;
	uint16_t   bwide;
	uint16_t   bhigh;
	uint16_t   scrb_high;
	uint16_t   scrb_x;
	uint16_t   scrb_y;
	int        state;
	int        createF;
	uint8_t    keyup;
	uint8_t    keydown;
	uint8_t    keyshif;
}widget_scrb_t;
BOOL GUI_scrb_ontouch( hwin_t*hp, app_touch_t*data );
static uint16_t  _BG = DEU_BGCOR;
static uint16_t  _FG = DEU_FGCOR;

void GUI_scrb_SetDeuCor(uint16_t Bg,uint16_t Fg)
{
   _BG = Bg;
   _FG = Fg;
   return;
}
/********************************************************
 * @创建人 :揭成
 * @功能 :滑动条重画
 * @输入 :
 * @输出 :
********************************************************/
static void _scrb_paint( hwin_t*hp )
{
	int len, x0, y0, x, y, bwide, bhigh;
	widget_scrb_t*pWidget;
	GUI_window_hwin_req( hp );
	pWidget = ( widget_scrb_t* )hp->pwidget;
	bwide = pWidget->bwide;
	bhigh = pWidget->bhigh;
	if ( pWidget->state == 0 )
	{
		GUI_set_Bgcolor( pWidget->BG );
		GUI_fill_Rect( hp->win_x, hp->win_y, hp->win_wide, hp->win_high );
		GUI_set_Fgcolor( LCD_Color2Index_565( 0x555555) );
		GUI_draw_Rect( hp->win_x, hp->win_y, hp->win_wide, hp->win_high );
	}
	if ( pWidget->state == 1 )
		GUI_set_Bgcolor( C_GUI_HIGH_BLUE );
	else
		GUI_set_Bgcolor( pWidget->FG );
	x0 = hp->win_x + 1;
	y0 = hp->win_y + 1;
	GUI_fill_Rect( x0, y0, bwide, bhigh );
	GUI_widget_3D_paint( x0, y0, bwide, bhigh );
	y = bhigh * 2 / 3;
	len = bwide * 1 / 2;
	x = ( bwide - len + 1 ) / 2;
	GUI_set_Fgcolor( C_GUI_BlACK );
	while ( len > 0 )
	{
		GUI_WINDIS_HLINE( x, x + len, y );
		y--;
		x++;
		len -= 2;
	}
	x0 = hp->win_x + 1;
	y0 = hp->win_y + hp->win_high - bhigh - 1;
	if ( pWidget->state == 2 )
		GUI_set_Bgcolor( C_GUI_HIGH_BLUE );
	else
		GUI_set_Bgcolor( pWidget->FG );
	GUI_fill_Rect( x0, y0, bwide, bhigh );
	GUI_widget_3D_paint( x0, y0, bwide, bhigh );
	y = hp->win_high - bhigh - 2 + bhigh * 1 / 2;
	len = bwide * 1 / 2;
	x = ( bwide - len + 1 ) / 2;
	GUI_set_Fgcolor( C_GUI_BlACK );
	while ( len > 0 )
	{
		GUI_WINDIS_HLINE( x, x + len, y );
		y++;
		x++;
		len -= 2;
	}
	if ( pWidget->totalpos > 1 && pWidget->state == 0 )
	{
		x = pWidget->scrb_x;
		y = pWidget->scrb_y;
		if ( pWidget->CURpos > pWidget->totalpos )
			pWidget->CURpos = pWidget->totalpos;
		bhigh = pWidget->scrb_high / pWidget->totalpos;
		x0 = x;
		y0 = y + pWidget->CURpos * bhigh;
		if ( pWidget->CURpos == ( pWidget->totalpos - 1 ) )
		{
			y0 = pWidget->scrb_y + pWidget->scrb_high - bhigh;
		}
		GUI_set_Bgcolor( pWidget->FG );
		GUI_fill_Rect( x0, y0, bwide, bhigh );
		GUI_widget_3D_paint( x0, y0, bwide, bhigh );
	}
	/*
	if(pWidget->createF==0)
	{
       pWidget->createF = 1;
	}
	else if(pWidget->totalpos ==1)
	{
		x0 = pWidget->scrb_x;
		y0 = pWidget->scrb_y ;
		bhigh = pWidget->scrb_high-10;
		GUI_set_Bgcolor( pWidget->FG );
		GUI_fill_Rect( x0, y0, bwide, bhigh );
		GUI_widget_3D_paint( x0, y0, bwide, bhigh );
	}*/
	hp->WIN_Paint = 0;
	GUI_window_hwin_release( hp );
	return;
}


hwin_t* Gui_scrb_create(
	int  x,
	int  y,
	int  wide,
	int  high,
	int  flags,
	hwin_t*  fatherhp
)
{
	hwin_t *hp;
	int bwide, bhigh;
	widget_scrb_t*pWidget;
	if ( high < ( wide*3 ) )return 0;

	hp = GUI_window_child_creat(
			 x,
			 y,
			 wide,
			 high,
			 0, fatherhp );
	GUI_window_hwin_req( hp );
	hp->pwidget = GUI_malloc( sizeof( widget_scrb_t ) );
	GLOBAL_MEMCLR( hp->pwidget, sizeof( widget_scrb_t ) );
	bwide   = hp->win_wide - 2;
	bhigh   = bwide * 5 / 6;
	pWidget = ( widget_scrb_t* )hp->pwidget;
	pWidget->BG       = _BG;
	pWidget->FG       = _FG;
	pWidget->CURpos   = 0;
	pWidget->totalpos = 1;
	pWidget->bwide    = bwide;
	pWidget->bhigh    = bhigh;
	pWidget->scrb_high = ( high - bhigh * 2 - 4 );
	pWidget->scrb_x   = hp->win_x + 1;
	pWidget->scrb_y   = hp->win_y + bhigh + 2;
	pWidget->createF  = 0;
	hp->type = WIDGET_SCRB;
#if GUI_TOUCH_EN>0
	hp->_ontouch = GUI_scrb_ontouch;
#endif
	hp->_paint  = _scrb_paint;
	hp->WIN_Paint = 1;
	GUI_window_hwin_release( hp );
	return hp;
}

hwin_t*Gui_scrb_attach( hwin_t*hp, int wide )
{
	hwin_t*hfp;
	int x0, y0, high;
	if ( !hp )return 0;
	GUI_window_hwin_req( hp );
	high = hp->win_high - 2;
	wide = wide;
	x0   = hp->win_wide - wide - 1;
	y0   = 0;
	hfp = Gui_scrb_create( x0, y0, wide, high, 0, hp );
	hp->win_wide = hp->win_wide - wide;
	GUI_window_hwin_release( hp );
	return hfp;
}

void Gui_scrb_Setpos( hwin_t*hp, uint16_t pos )
{
	widget_scrb_t*pWidget;
	GUI_window_hwin_req( hp );
	pWidget = ( widget_scrb_t* )hp->pwidget;
	if ( pos >= pWidget->totalpos )
		pos = pWidget->totalpos - 1;
	pWidget->CURpos = pos;
	hp->WIN_Paint = 1;
	GUI_window_hwin_release( hp );
	return;
}

void Gui_scrb_shift( hwin_t*hp, int dx )
{
	int  itemp;
	widget_scrb_t*pWidget;
	GUI_window_hwin_req( hp );
	pWidget = ( widget_scrb_t* )hp->pwidget;
	itemp = pWidget->CURpos;
	itemp += dx;
	if ( itemp >= 0 && itemp < pWidget->totalpos )
		pWidget->CURpos = 	itemp;
	hp->WIN_Paint = 1;
	GUI_window_hwin_release( hp );
	return;
}


void Gui_scrb_Setsize( hwin_t*hp, uint16_t size )
{
	widget_scrb_t*pWidget;
	GUI_window_hwin_req( hp );
	pWidget = ( widget_scrb_t* )hp->pwidget;
	if ( size > pWidget->scrb_high )
		size = pWidget->scrb_high;
	pWidget->totalpos = size;
	hp->WIN_Paint = 1;
	GUI_window_hwin_release( hp );
	return;
}

void Gui_scrb_SetVkey( hwin_t*hp, uint8_t keyup, uint8_t keydown, uint8_t keyshift )
{
	widget_scrb_t*pWidget;
	GUI_window_hwin_req( hp );
	pWidget = ( widget_scrb_t* )hp->pwidget;
	pWidget->keydown = keydown;
	pWidget->keyup   = keyup;
	pWidget->keyshif = keyshift;
	GUI_window_hwin_release( hp );
	return;
}

void Gui_scrb_Setcorlor( hwin_t*hp, uint16_t BG, uint16_t FG )
{
	widget_scrb_t*pWidget;
	GUI_window_hwin_req( hp );
	pWidget = ( widget_scrb_t* )hp->pwidget;
	pWidget->BG = BG;
	pWidget->FG = FG;
	hp->WIN_Paint = 1;
	GUI_window_hwin_release( hp );
	return;
}

uint16_t Gui_scrb_Getpos( hwin_t*hp )
{
	widget_scrb_t*pWidget;
	pWidget = ( widget_scrb_t* )hp->pwidget;
	return pWidget->CURpos;
}

uint16_t Gui_scrb_Getsize( hwin_t*hp )
{
	widget_scrb_t*pWidget;
	pWidget = ( widget_scrb_t* )hp->pwidget;
	return pWidget->totalpos;
}

#if GUI_TOUCH_EN >0
/********************************************************
 * @创建人 :揭成
 * @功能 :复选框触摸坐标匹配
 *
 * @输入 :NONE
 *@输出  :NONE
***********************************************************/
static BOOL GUI_scrb_match1( hwin_t*hp, app_touch_t*data )
{
	widget_scrb_t*pWidget;
	int xmax ;
	int xmin ;
	int ymax;
	int ymin ;
	pWidget = ( widget_scrb_t* )hp->pwidget;
	xmax = hp->win_x + hp->win_wide + 1;
	xmin = hp->win_x - 1;
	ymax = hp->win_y + pWidget->bhigh + 5;
	ymin = hp->win_y - 5;
	( xmax > LCD_WIDE ) ? ( xmax = LCD_WIDE ) : ( xmax = xmax );
	( xmin < 0 ) ? ( xmin = 0 ) : ( xmin = xmin );
	( ymax > LCD_HIGH ) ? ( ymax = LCD_HIGH ) : ( ymax = ymax );
	( ymin < 0 ) ? ( ymin = 0 ) : ( ymin = ymin );
	if ( data->x < xmin ) return FAIL;
	if ( data->x > xmax ) return FAIL;
	if ( data->y < ymin ) return FAIL;
	if ( data->y > ymax ) return FAIL;
	return TURE;
}
/********************************************************
 * @创建人 :揭成
 * @功能 :复选框触摸坐标匹配
 *
 * @输入 :NONE
 *@输出  :NONE
***********************************************************/
static BOOL GUI_scrb_match2( hwin_t*hp, app_touch_t*data )
{
	widget_scrb_t*pWidget;
	int xmax ;
	int xmin ;
	int ymax;
	int ymin ;
	pWidget = ( widget_scrb_t* )hp->pwidget;
	xmax = hp->win_x + hp->win_wide + 1;
	xmin = hp->win_x - 1;
	ymax = pWidget->scrb_y + pWidget->scrb_high;
	ymin = pWidget->scrb_y;
	( xmax > LCD_WIDE ) ? ( xmax = LCD_WIDE ) : ( xmax = xmax );
	( xmin < 0 ) ? ( xmin = 0 ) : ( xmin = xmin );
	( ymax > LCD_HIGH ) ? ( ymax = LCD_HIGH ) : ( ymax = ymax );
	( ymin < 0 ) ? ( ymin = 0 ) : ( ymin = ymin );
	if ( data->x < xmin ) return FAIL;
	if ( data->x > xmax ) return FAIL;
	if ( data->y < ymin ) return FAIL;
	if ( data->y > ymax ) return FAIL;
	return TURE;
}
/********************************************************
 * @创建人 :揭成
 * @功能 :复选框触摸坐标匹配
 *
 * @输入 :NONE
 *@输出  :NONE
***********************************************************/
static BOOL GUI_scrb_match3( hwin_t*hp, app_touch_t*data )
{
	widget_scrb_t*pWidget;
	int xmax ;
	int xmin ;
	int ymax;
	int ymin ;
	pWidget = ( widget_scrb_t* )hp->pwidget;
	xmax = hp->win_x + hp->win_wide + 1;
	xmin = hp->win_x - 1;
	ymax = hp->win_y + hp->win_high + 5;
	ymin = ymax - pWidget->bhigh - 2;
	( xmax > LCD_WIDE ) ? ( xmax = LCD_WIDE ) : ( xmax = xmax );
	( xmin < 0 ) ? ( xmin = 0 ) : ( xmin = xmin );
	( ymax > LCD_HIGH ) ? ( ymax = LCD_HIGH ) : ( ymax = ymax );
	( ymin < 0 ) ? ( ymin = 0 ) : ( ymin = ymin );
	if ( data->x < xmin ) return FAIL;
	if ( data->x > xmax ) return FAIL;
	if ( data->y < ymin ) return FAIL;
	if ( data->y > ymax ) return FAIL;
	return TURE;
}
/********************************************************
 * @创建人 :揭成
 * @功能 :复选框触摸属性
 *
 * @输入 :NONE
 *@输出  :NONE
***********************************************************/
BOOL GUI_scrb_ontouch( hwin_t*hp, app_touch_t*data )
{
	int pos;
	widget_scrb_t*pWidget;
	pWidget = ( widget_scrb_t* )hp->pwidget;
	if ( GUI_scrb_match1( hp, data ) )
	{
		pWidget->state = 1;
		_scrb_paint( hp );
		GUI_wait_touch_release();
		Gui_scrb_shift( hp, -1 );
		pWidget->state = 0;
		_scrb_paint( hp );
		GUI_key_dummy_press( hp, pWidget->keyup );
		return TURE;
	}
	else if ( GUI_scrb_match2( hp, data ) )
	{
		GUI_wait_touch_release();
		pos = ( data->y - pWidget->scrb_y ) / ( pWidget->scrb_high / pWidget->totalpos );
		Gui_scrb_Setpos( hp, pos );
		_scrb_paint( hp );
		GUI_key_dummy_press( hp, pWidget->keyshif );
		return TURE;
	}
	else if ( GUI_scrb_match3( hp, data ) )
	{
		pWidget->state = 2;
		_scrb_paint( hp );
		GUI_wait_touch_release();
		Gui_scrb_shift( hp, 1 );
		pWidget->state = 0;
		_scrb_paint( hp );
		GUI_key_dummy_press( hp, pWidget->keydown );
		return TURE;
	}
	return FAIL;
}
#endif

#endif
