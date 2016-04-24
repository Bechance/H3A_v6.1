#include "../../Gui.h"
#include "../GUI_lcd_driver.h"

#if GUI_CHECK_BOX_EN>0
typedef struct
{
	BOOL     checkbox_state;
	uint16_t BG;
	uint16_t FG;
	uint16_t title_len;
	GUI_FONT*pFont;
}widget_checkbox_t;

static widget_checkbox_t*GUI_checkbox_data_obtain( hwin_t *hp )
{
	return ( widget_checkbox_t* )hp->pwidget;
}
BOOL GUI_check_box_ontouch( hwin_t*hp, app_touch_t*data );
/********************************************************
 * @创建人 :揭成
 * @功能 :复选框重画
 * @输入 :
 * @输出 :
********************************************************/
void _checkBox_paint( hwin_t*hp )
{
	uint16_t  len;
	int  x, y, x1, x2, y1, y2;
	int  wide, high;
	widget_checkbox_t*pWidget;
	GUI_window_hwin_req( hp );
	pWidget = GUI_checkbox_data_obtain( hp );
	len = pWidget->title_len;
	x    =  hp->win_x + len + 5;
	y    =  hp->win_y + 2;
	wide = ( pWidget->pFont->CHwide * 2 + 2 );
	high =  pWidget->pFont->high;
	if ( pWidget->checkbox_state == TURE )
	{
		GUI_set_Bgcolor( pWidget->BG );
		GUI_fill_Rect( hp->win_x + len + 5, hp->win_y + 2, ( pWidget->pFont->CHwide*2 + 2 ), pWidget->pFont->high );
		GUI_widget_sink_3D_paint( hp->win_x + len + 5, hp->win_y + 2, ( pWidget->pFont->CHwide*2 + 2 ), pWidget->pFont->high );
		GUI_set_Fgcolor( pWidget->FG );
		x1 = x + wide * 1 / 8;
		y1 = y + high / 2;
		x2 = x + wide / 3;
		y2 = y + high * 7 / 8;
		GUI_draw_line( x1, y1, x2, y2 );
		x1 = x + wide / 3;
		y1 = y + high * 7 / 8;
		x2 = x + 7 * wide / 8;
		y2 = y;
		GUI_draw_line( x1, y1, x2, y2 );
	}
	else
	{
		GUI_set_Bgcolor( pWidget->BG );
		GUI_fill_Rect( hp->win_x + len + 5, hp->win_y + 2, ( pWidget->pFont->CHwide*2 + 2 ), pWidget->pFont->high );
		GUI_widget_sink_3D_paint( hp->win_x + len + 5, hp->win_y + 2, ( pWidget->pFont->CHwide*2 + 2 ), pWidget->pFont->high );
	}
	hp->WIN_Paint = 0;
	GUI_window_hwin_release( hp );
	return;
}

static void _checkbox_ready_do( hwin_t*hp )
{
	widget_checkbox_t*pWidget;
	pWidget = GUI_checkbox_data_obtain( hp );
	pWidget->checkbox_state = ( pWidget->checkbox_state == TURE ) ? ( FAIL ) : ( TURE );
	hp->_paint( hp );
	GUI_key_dummy_press( hp, ( hp->flag&0xff ) );
	return;
}

static void _checkbox_ready_display( int xs, int ys, int xe, int ye, int cor )
{
	int   npos ;
	GUI_set_Fgcolor( cor );
	for ( npos = xs;npos < xe;npos += 2 )
		GUI_draw_dot( npos, ys );
	for ( npos = xs;npos < xe;npos += 2 )
		GUI_draw_dot( npos, ye );
	for ( npos = ys;npos < ye;npos += 2 )
		GUI_draw_dot( xs, npos );
	for ( npos = ys;npos < ye + 2;npos += 2 )
		GUI_draw_dot( xe, npos );
	return;
}

static void _checkbox_ready( hwin_t*hp, int state )
{
	uint16_t x, y, wide, high;
	uint16_t BK, len;
	widget_checkbox_t*pWidget;
	pWidget = GUI_checkbox_data_obtain( hp );
	len = pWidget->title_len;
	x    =  hp->win_x + len + 5;
	y    =  hp->win_y + 2;
	wide = ( pWidget->pFont->CHwide * 2 + 2 );
	high =  pWidget->pFont->high;
	GUI_conText.LCD_Read_dot( x - 2, y - 2, &BK );
	if ( !state )
	{
		_checkbox_ready_display( x - 3, y - 3, x + wide + 3, y + high + 3, BK );
	}
	else
		_checkbox_ready_display( x - 3, y - 3, x + wide + 3, y + high + 3, ~BK );
	return;
}
/********************************************************
 * @创建人 :揭成
 * @功能 :创建复选框
 * @输入 :
 * @输出 :
********************************************************/
hwin_t*GUI_checkBox_create(
	const char*title,
	uint16_t title_cor,
	uint16_t xcoord,
	uint16_t ycoord,
	uint16_t flag,
	hwin_t*fatherhp
)
{
	hwin_t*hp;
	widget_checkbox_t*pWidget;
	GUI_FONT*pFont;
	uint16_t len;
	pFont = ( GUI_FONT* )GUI_GetFront();
	len = global_strlen(( char* )title ) * pFont->CHwide;
	GUI_window_hwin_req( fatherhp );
	hp = GUI_window_child_creat(
			 xcoord,
			 ycoord, len + pFont->CHwide * 3, ( pFont->high + 5 ), 0,
			 fatherhp );
	GUI_window_hwin_req( hp );
	hp->pwidget = GUI_malloc( sizeof( widget_checkbox_t ) );
	pWidget = GUI_checkbox_data_obtain( hp );
	pWidget->title_len	  = len;
	pWidget->BG = C_GUI_WHITE;
	pWidget->FG = C_GUI_BlACK;
	pWidget->pFont = pFont;
	pWidget->checkbox_state = FAIL;
	hp->type = WIDGET_CHECK_BOX;
	hp->flag = flag;
	GUI_set_Fgcolor( title_cor );
	GUI_string_display_At( 0, 1, ( char* )title );
#if GUI_TOUCH_EN>0
	hp->_ontouch = GUI_check_box_ontouch;
#endif
	hp->_paint  = _checkBox_paint;
	hp->WIN_Paint = 1;
	hp->_WINdo     = _checkbox_ready_do;
	hp->_WINselect = _checkbox_ready;
	GUI_window_hwin_release( hp );
	GUI_window_hwin_release( fatherhp );
	return hp;
}
/********************************************************
 * @创建人 :揭成
 * @功能 :获取复选框状态
 * @输入 :
 * @输出 :
********************************************************/
BOOL  GUI_checkbox_state_obtain( hwin_t*hp )
{
	widget_checkbox_t*pWidget;
	pWidget = GUI_checkbox_data_obtain( hp );
	return ( pWidget->checkbox_state );
}
/********************************************************
 * @创建人 :揭成
 * @功能 :设置复选框状态
 * @输入 :
 * @输出 :
********************************************************/
void GUI_checkBox_select( hwin_t*hp, BOOL state )
{
	widget_checkbox_t*pWidget;
	GUI_window_hwin_req( hp );
	pWidget = GUI_checkbox_data_obtain( hp );
	if ( state == TURE )
	{
		pWidget->checkbox_state = TURE;
	}
	else
	{
		pWidget->checkbox_state = FAIL;
	}
	hp->WIN_Paint = 1;
	GUI_window_hwin_release( hp );
	return;
}

void GUI_checkbox_state_modify( hwin_t*hp )
{
    BOOL state;
	widget_checkbox_t*pWidget;
	GUI_window_hwin_req( hp );
	pWidget = GUI_checkbox_data_obtain( hp );
    if(pWidget->checkbox_state==1)state=0;
	else  state = 1;
	pWidget->checkbox_state = state;
	hp->WIN_Paint = 1;
	GUI_window_hwin_release( hp );
	return ;
}
#if GUI_TOUCH_EN >0
/********************************************************
 * @创建人 :揭成
 * @功能 :复选框触摸坐标匹配
 *
 * @输入 :NONE
 *@输出  :NONE
***********************************************************/
static BOOL GUI_check_box_match( hwin_t*hp, app_touch_t*data )
{
	int xmax ;
	int xmin ;
	int ymax;
	int ymin ;
	xmax = hp->win_x + hp->win_wide + 1;
	xmin = hp->win_x - 1;
	ymax = hp->win_y + hp->win_high + 5;
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
 * @功能 :复选框触摸属性
 *
 * @输入 :NONE
 *@输出  :NONE
***********************************************************/
BOOL GUI_check_box_ontouch( hwin_t*hp, app_touch_t*data )
{
	widget_checkbox_t*pWidget;
	pWidget = GUI_checkbox_data_obtain( hp );
	if (( hp->flag&WIN_TOUCH_EN ) != WIN_TOUCH_EN )
		return FAIL;
	if ( GUI_check_box_match( hp, data ) )
	{
		GUI_wait_touch_release();
		GUI_key_dummy_press( hp, ( hp->flag&0xff ) );
		pWidget->checkbox_state = ( pWidget->checkbox_state == TURE ) ? ( FAIL ) : ( TURE );
		hp->_paint( hp );
		return TURE;
	}
	return FAIL;
}
#endif
#endif
