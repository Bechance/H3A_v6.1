#include "../../Gui.h"
#include "../GUI_lcd_driver.h"
#if	GUI_3D_WINDOW_EN  >0
uint16_t m_GUI_3D_window_Bg_cor  	= C_GUI_LOW_BLUE;
uint16_t m_GUI_3D_window_Fg_cor  	= C_GUI_WHITE;
uint16_t m_GUI_3D_window_txt_cor 	= C_GUI_WHITE;
uint16_t m_GUI_3D_window_titleHigh 	= 20;
const GUI_FONT*m_GUI_3D_pFONT;


void  GUI_3D_widow_setFornt(const GUI_FONT*pFONT )
{
	m_GUI_3D_pFONT = pFONT;
	m_GUI_3D_window_titleHigh = m_GUI_3D_pFONT->high + 10;
	return;
}

void  Gui_3D_window_set_color( uint16_t Fgcor, uint16_t Bgcor, uint16_t txtcor )
{
	m_GUI_3D_window_Bg_cor  =  Bgcor;
	m_GUI_3D_window_Fg_cor  =  Fgcor;
	m_GUI_3D_window_txt_cor =  txtcor;
	return;
}



void Gui_3D_window_title_nomal( hwin_t *hp )
{
	uint16_t wide, high;
	uint16_t x, y, x1, x2, y1, y2;
	uint16_t flag;

	flag = hp->flag;
	flag &= WIN_CLOSE_BUTTON;
	if ( !flag )return;
	GUI_set_Bgcolor( C_GUI_RED );
	GUI_set_Fgcolor( C_GUI_WHITE );
	wide = m_GUI_3D_window_titleHigh - 2;
	high = m_GUI_3D_window_titleHigh - 2;
	x   = hp->win_x + hp->win_wide - wide;
	y   = hp->win_y + ( m_GUI_3D_window_titleHigh - high ) / 2 - ( m_GUI_3D_window_titleHigh + 2 );
	GUI_fill_Rect( x, y, wide, high );

	x1 = x + 2;
	y1 = y + 4;
	x2 = x + wide - 4;
	y2 = y + high - 4;
	GUI_draw_line( x1, y1, x2, y2 );

	x1 = x + wide - 4;
	y1 = y + 4;
	x2 = x + 2;
	y2 = y + high - 4;
	GUI_draw_line( x1, y1, x2, y2 );
	return;
}

void Gui_3D_window_title_press( hwin_t *hp )
{
	uint16_t wide, high;
	uint16_t x, y, x1, x2, y1, y2;
	GUI_set_Bgcolor( C_GUI_WHITE );
	GUI_set_Fgcolor( C_GUI_RED );
	wide = m_GUI_3D_window_titleHigh - 2;
	high = m_GUI_3D_window_titleHigh - 2;
	x   = hp->win_x + hp->win_wide - wide;
	y   = hp->win_y + ( m_GUI_3D_window_titleHigh - high ) / 2 - ( m_GUI_3D_window_titleHigh + 2 );
	GUI_fill_Rect( x, y, wide, high );
	x1 = x + 2;
	y1 = y + 4;
	x2 = x + wide - 4;
	y2 = y + high - 4;
	GUI_draw_line( x1, y1, x2, y2 );
	x1 = x + wide - 4;
	y1 = y + 4;
	x2 = x + 2;
	y2 = y + high - 4;
	GUI_draw_line( x1, y1, x2, y2 );
	return;
}

hwin_t* Gui_3D_window_creat(
	uint16_t xcoord,
	uint16_t ycoord,
	Bsize_t wide,
	Bsize_t high,
	char*title,
	Bsize_t flags,
	hwin_t* fatherhp
)
{
	hwin_t *hp;
	uint16_t flag;
	if ( fatherhp )
	{
		hp = GUI_window_child_creat(
				 xcoord,
				 ycoord,
				 wide,
				 high,
				 flags,
				 fatherhp );
	}
	else
	{
		hp = GUI_window_creat( xcoord, ycoord, wide, high, flags );
	}
	hp->type = WIDGET_COLOR_WINDOW;
	GUI_window_hwin_req( hp );
	GUI_SetFront( m_GUI_3D_pFONT );
	GUI_set_Bgcolor( m_GUI_3D_window_Bg_cor );
	GUI_set_Fgcolor( m_GUI_3D_window_Fg_cor );
	GUI_fill_Rect( hp->win_x + 1, hp->win_y + 1,
				   hp->win_wide - 2, m_GUI_3D_window_titleHigh - 2 );

	GUI_DRAW_HLINE( hp->win_x + 1, hp->win_x + hp->win_wide,
					hp->win_y + m_GUI_3D_window_titleHigh - 1 );
	GUI_DRAW_HLINE( hp->win_x + 1, hp->win_x + hp->win_wide, hp->win_y );
	GUI_DRAW_VLINE( hp->win_x + 1, hp->win_y, hp->win_y + m_GUI_3D_window_titleHigh );
	GUI_set_Fgcolor( m_GUI_3D_window_txt_cor );
//	GUI_DISPLAY_IN_MID(0,title,m_GUI_3D_pFONT->CHwide);

	hp->win_y    = hp->win_y + m_GUI_3D_window_titleHigh + 2;
	hp->win_high = hp->win_high - m_GUI_3D_window_titleHigh - 2;
	flag = hp->flag;
	flag &= WIN_CLOSE_BUTTON;
	if (flag )
	{
		Gui_3D_window_title_nomal( hp );
	}
#if GUI_TOUCH_EN >0
	hp->_ontouch = GUI_3D_window_ontouch;
#endif
	GUI_window_hwin_release( hp );
	GUI_3D_window_title_mod( hp, title );
	return hp;
}

void GUI_3D_window_title_mod( hwin_t*hp, char*title )
{
	uint16_t y;
	GUI_window_hwin_req( hp );
	GUI_SetFront( m_GUI_3D_pFONT );
	y = hp->win_y - 1 - m_GUI_3D_window_titleHigh;
	GUI_set_Bgcolor( m_GUI_3D_window_Bg_cor );
	GUI_set_Fgcolor( m_GUI_3D_window_txt_cor );
	GUI_fill_Rect( hp->win_x + 2, y,
				   hp->win_wide - 18, m_GUI_3D_window_titleHigh - 2 );
	Gui_3D_window_title_nomal( hp );
	GUI_set_Bgcolor( m_GUI_3D_window_Bg_cor );
	GUI_set_Fgcolor( m_GUI_3D_window_txt_cor );


	GUI_print_string(4+ hp->win_x,y+3, hp->win_x + hp->win_wide - 1,
					  hp->win_y + m_GUI_3D_window_titleHigh, ( char* )title );
	/*
	GUI_print_string(2+hp->win_x
		, y, hp->win_x+hp->win_wide - 1,
		hp->win_y+m_GUI_3D_window_titleHigh, ( char* )title );*/
	GUI_window_hwin_release( hp );
	return;
}

#if GUI_TOUCH_EN>0
static BOOL GUI_3D_window_match( hwin_t*hp, app_touch_t*data )
{
	int xmax, xmin, ymax, ymin;
	xmax = hp->win_x + hp->win_wide + 40 + TOUCH_ERR_DATA;
	xmin = hp->win_x + hp->win_wide - 40 - TOUCH_ERR_DATA;
	ymax = hp->win_y - 1 - m_GUI_3D_window_titleHigh + 10 + TOUCH_ERR_DATA;
	ymin = hp->win_y - 1 - m_GUI_3D_window_titleHigh - TOUCH_ERR_DATA;
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
BOOL GUI_3D_window_ontouch( hwin_t*hp, app_touch_t*data )
{
	if ( !(hp->flag&WIN_CLOSE_BUTTON) ) return FAIL;
	if ( GUI_3D_window_match( hp, data ) )
	{
		Gui_3D_window_title_press( hp );
		GUI_wait_touch_release();
		Gui_3D_window_title_nomal( hp );
		GUI_key_dummy_press( hp, GUI_KEY_3D_WINDOW_CLOSE );
		return TURE;
	}
	return FAIL;
}
#endif
void GUI_3D_window_init( void )
{
	m_GUI_3D_window_Bg_cor  = C_GUI_LOW_BLUE;
	m_GUI_3D_window_Fg_cor  = C_GUI_WHITE;
	m_GUI_3D_window_txt_cor = C_GUI_WHITE;
	m_GUI_3D_pFONT = ( GUI_FONT* )GUI_GET_DEFGRONT();
	GUI_3D_widow_setFornt(m_GUI_3D_pFONT);
	return;
}
#endif


