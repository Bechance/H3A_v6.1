#include "../../Gui.h"
#include "../GUI_lcd_driver.h"
subord_mem_t*				GUI_edit_mem_req( void );
#if GUI_RADIO_EN>0
void 	GUI_radio_select( hwin_t*hp, Bsize_t id );
BOOL 	GUI_radio_ontouch( hwin_t*hp, app_touch_t*data );
extern  subord_mem_t* m_pMember;

#define    CIRCLE_BIGFILL         	   C_GUI_WHITE
#define    CIRCLE_SMALLFILL            C_GUI_BlACK

static void _ready_display( int xs, int ys, int xe, int ye, int cor )
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

static void _radio_ready( hwin_t*hp, int state )
{
	uint16_t BK;
	subord_mem_t*plist;
	uint16_t x, y, r,xtitle;
	plist = m_pMember;
	if ( !plist ) return;
	r = ( uint16_t )( 5.0 / 16.0 * plist->CHhigh );
	x = plist->x + r + 4;
	y = plist->y + plist->CHhigh / 2 + 1;
	GUI_window_hwin_req( hp );
	GUI_conText.LCD_Read_dot( x - r - 2, y - r - 2, &BK );
	xtitle = GUI_strlen((char*)plist->title)*plist->pfront->CHwide;
	if ( !state )
	{
		_ready_display( x - r - 8 -xtitle, y - r - 5, x + r +8, y + r + 5, BK );
	}
	else
	{
		_ready_display( x - r - 8 -xtitle, y - r - 5, x + r + 8, y + r + 5,~BK);
	}
	GUI_window_hwin_release( hp );
	return;
}


static void _radio_ready_do( hwin_t*hp )
{
	GUI_radio_select( hp, m_pMember->id );
	GUI_key_dummy_press( hp, ( hp->flag )&0xff );
	return;
}



hwin_t* GUI_radio_create(uint16_t xcoord,
	uint16_t ycoord,Bsize_t wide,
	Bsize_t high,Bsize_t flags,
	hwin_t* fatherhp
)
{
	hwin_t *hp;
	GUI_window_hwin_req( fatherhp );
	hp = GUI_window_child_creat(
			 xcoord,
			 ycoord,
			 wide,
			 high,
			 flags,
			 fatherhp );
	if ( !hp ) return PNULL;
	hp->type = WIDGET_RADIO;
#if GUI_TOUCH_EN>0
    hp->flag |= WIN_TOUCH;
	hp->_ontouch = GUI_radio_ontouch;
#endif
	hp->_WINdo     = _radio_ready_do;
	hp->_WINselect = _radio_ready;
	GUI_window_hwin_release( fatherhp );
	return hp;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:增加并选框成员
 *
 * @输入:

 * @输出:
********************************************************************/
void GUI_radio_memeber_add( uint16_t x, uint16_t y, char*title, Bsize_t id )
{
	uint16_t circleX, circleY, circleR;
	subord_mem_t*pedit;
	uint16_t len;
	if ( m_cur_hwin->type != WIDGET_RADIO ) return;
	pedit = GUI_edit_mem_req();
	if ( !pedit ) return;
	pedit->CHhigh = m_GUI_CHhigh;
	pedit->CHwide = m_GUI_CHwide;
	pedit->pfront = GUI_GetFront();
	len = GUI_strlen((char*)title)*m_GUI_CHwide;
	pedit->id = id;
	pedit->y  = m_cur_hwin->win_y + y;
	pedit->x  = m_cur_hwin->win_x + x + len;
	pedit->eidt_lenth = len;
	pedit->flag       = 0;
	pedit->title      = title;
	pedit->next_edit  = PNULL;
	if ( !( m_cur_hwin->subord_end ) )
	{
		m_cur_hwin->subord_head = pedit;
		m_cur_hwin->subord_end  = pedit;
		pedit->last_edit        = PNULL;
	}
	else
	{
		(m_cur_hwin->subord_end)->next_edit = pedit;
		pedit->last_edit = m_cur_hwin->subord_end;
		m_cur_hwin->subord_end = pedit;
	}
	GUI_string_display_At( x, y, ( char* )title );
	circleR = ( uint16_t )( 5.0 / 16.0 * pedit->CHhigh );
	circleX = pedit->x + circleR + 4;
	circleY = pedit->y + pedit->CHhigh / 2 + 1;
	GUI_drawcircle( circleX, circleY, circleR, CIRCLE_SMALLFILL );
	GUI_fillcircle( circleX, circleY, circleR, CIRCLE_BIGFILL );
	return;
}
/********************************************************
 * @创建人 :揭成
 * @功能 :获取并选框状态
 * @输入 :
 * @输出 :
********************************************************/
Bsize_t GUI_radio_curmember_obtain( hwin_t*hp )
{
	return (hp->subord_cur->id);
}
/********************************************************
 * @创建人 :揭成
 * @功能 :获取并选框状态
 * @输入 :
 * @输出 :
********************************************************/
void* GUI_radio_title_obtain( hwin_t*hp )
{
	return (void*)(hp->subord_cur->title);
}
/********************************************************
 * @创建人 :揭成
 * @功能 :设置并选框状态
 * @输入 :
 * @输出 :
********************************************************/
void GUI_radio_select( hwin_t*hp, Bsize_t id )
{
	uint16_t circleX, circleY, circleR;
	subord_mem_t*plist;
	GUI_window_hwin_req( hp );
	plist = hp->subord_head;
	while ( 1 )
	{
		if ( plist->id == id ) break;
		plist = plist->next_edit;
		if ( plist == NULL )
		{
			GUI_window_hwin_release( hp );
			return ;
		}
	}
	if ( hp->subord_cur )
	{
		circleR = ( uint16_t )( 5.0 / 16.0 * hp->subord_cur->CHhigh );
		circleX = hp->subord_cur->x + circleR + 4;
		circleY = hp->subord_cur->y + hp->subord_cur->CHhigh / 2 + 1;
		GUI_drawcircle( circleX, circleY, circleR, CIRCLE_SMALLFILL );
		GUI_fillcircle( circleX, circleY, circleR, CIRCLE_BIGFILL );
	}
	hp->subord_cur = plist;
	circleR = ( uint16_t )( 5.0 / 16.0 * hp->subord_cur->CHhigh );
	circleX = hp->subord_cur->x + circleR + 4;
	circleY = hp->subord_cur->y + hp->subord_cur->CHhigh / 2 + 1;
	GUI_fillcircle( circleX, circleY, circleR / 2, CIRCLE_SMALLFILL );
	GUI_window_hwin_release( hp );
	return;
}
#if GUI_TOUCH_EN>0
/********************************************************
 * @创建人 :揭成
 * @功能 :并选框触摸坐标匹配
 *
 * @输入 :NONE
 *@输出  :NONE
***********************************************************/
static BOOL GUI_radio_match( subord_mem_t*pedit, app_touch_t*data, hwin_t*hp )
{
	int xmax, xmin, ymax, ymin, offset;
	offset = offset;
	xmax = pedit->x + 20;
	xmin = pedit->x - 20;
	ymax = pedit->y + pedit->CHhigh + 3;
	ymin = pedit->y - 3;
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
 * @功能   :并选框触摸属性
 * @输入   :NONE
 * @输出   :NONE
********************************************************/
BOOL GUI_radio_ontouch( hwin_t*hp, app_touch_t*data )
{
	subord_mem_t*plist;
	plist = hp->subord_head;
	if (( hp->flag&WIN_TOUCH_EN)!= WIN_TOUCH_EN )
		return FAIL;
	while ( 1 )
	{
		if ( plist == NULL )
		{
			return FAIL;
		}
		if ( GUI_radio_match( plist, data, hp ) )
		{
			GUI_radio_select( hp, plist->id );
			GUI_wait_touch_release();
			GUI_key_dummy_press( hp, ( hp->flag )&0xff );
			return TURE;
		}
		plist = plist->next_edit;
	}
}
#endif
#endif
