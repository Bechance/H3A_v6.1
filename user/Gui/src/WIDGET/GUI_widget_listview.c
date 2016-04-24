#include "../../Gui.h"
#include "../GUI_lcd_driver.h"
subord_mem_t*				GUI_edit_mem_req( void );
#if GUI_LISTVIEW_EN>0
static uint16_t m_GUI_listview_Bgcor = 0x841F;
static uint16_t m_GUI_listview_Fgcor = C_GUI_BlACK;
typedef struct
{
	Bsize_t id;
	char    title[20];
}listview_member_t;
#define  MAX_LIST_SIZE            20
typedef struct
{
	uint16_t    	   listview_bg_color;
	uint16_t    	   listview_fg_color;
	uint16_t    	   listview_winBg_color;
	Bsize_t            listview_member_pos;
	uint16_t           sub_high;
	uint16_t           member_x;
	uint16_t           member_y;
	listview_member_t  listview_member[MAX_LIST_SIZE];
}widget_listview_t;

BOOL GUI_listview_ontouch( hwin_t*hp, app_touch_t*data );
static widget_listview_t*GUI_listview_data_obtain( hwin_t *hp )
{
	return ( widget_listview_t* )hp->pwidget;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:给list换底色
 * @输入:
 * @输出:
********************************************************************/
void Gui_listview_set_color( uint16_t Fgcor, uint16_t Bgcor )
{
	m_GUI_listview_Bgcor = Bgcor;
	m_GUI_listview_Fgcor = Fgcor;
	return;
}

hwin_t* GUI_listview_create(
	uint16_t xcoord,
	uint16_t ycoord,
	Bsize_t wide,
	Bsize_t high,
	Bsize_t flags,
	hwin_t* fatherhp
)
{
	hwin_t *hp;
	widget_listview_t *plistview;
	int  sub_high;
	sub_high = m_GUI_CHhigh + 5;
	high = high - sub_high;
	hp = GUI_window_child_creat(
			 xcoord,
			 ycoord + sub_high,
			 wide,
			 high,
			 flags,
			 fatherhp );
	if ( !hp ) return PNULL;
	hp->type = WIDGET_LISTVIEW;
	hp->flag = flags;
#if GUI_TOUCH_EN>0
	hp->_ontouch = GUI_listview_ontouch;
#endif
	hp->pwidget = GUI_malloc( sizeof( widget_listview_t ) );
	GUI_MEMCLR( hp->pwidget, sizeof( widget_listview_t ) );
	plistview = GUI_listview_data_obtain( hp );
	plistview->listview_bg_color    = m_GUI_listview_Bgcor;
	plistview->listview_fg_color    = m_GUI_listview_Fgcor;
	plistview->listview_winBg_color = GUI_conText.Bcolor;
	plistview->listview_member_pos = 0;
	plistview->sub_high = sub_high;
	plistview->member_y = fatherhp->win_y + ycoord;
	plistview->member_x = fatherhp->win_x + xcoord + 1;
	return hp;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:增加并选框成员
 *
 * @输入:

 * @输出:
********************************************************************/
static void GUI_listview_memeber_display( subord_mem_t*pedit, char*title )
{
	int wide, high, x0, y0;
	x0   = pedit->x;
	y0   = pedit->y - 2;
	wide = pedit->eidt_lenth * pedit->CHwide + 15;
	high = pedit->CHhigh + 8;
	GUI_set_Bgcolor( m_GUI_listview_Bgcor );
	GUI_fill_Rect_At( x0, y0, x0 + wide, y0 + high );
	GUI_widget_3D_paint( x0, y0, wide, high );

	GUI_set_Bgcolor( m_GUI_listview_Bgcor );
	GUI_set_Fgcolor( m_GUI_listview_Fgcor );
	GUI_SetFront(( GUI_FONT * )pedit->pfront );
	GUI_print_string( x0 + 8, y0 + 4, 800, 600, title );
	return;
}

static void GUI_listview_select_state_display( int x0, int y0, uint16_t wide, uint16_t high, uint16_t corlor )
{
	int x, y, n;
	n = 0;
	x0 = x0 + 2;
	y0 = y0 + 1;
	wide = wide - 3;
	GUI_set_Fgcolor( corlor );
	for ( x = x0 + 2;x < x0 + ( wide - 4 );x++ )
	{
		if ( n % 2 == 0 )GUI_draw_dot( x, 2 + y0 );
		n++;
	}
	n = 0;
	for ( x = x0 + 2;x < ( x0 + wide - 4 );x++ )
	{
		if ( n % 2 == 0 )GUI_draw_dot( x, y0 + high - 4 );
		n++;
	}
	n = 0;
	for ( y = y0 + 2;y < ( y0 + high - 4 );y++ )
	{
		if ( n % 2 == 0 )GUI_draw_dot( 2 + x0, y );
		n++;
	}
	n = 0;
	for ( y = y0 + 2;y <= ( y0 + high - 4 );y++ )
	{
		if ( n % 2 == 0 )GUI_draw_dot( wide - 4 + x0, y );
		n++;
	}
	return;
}
static void GUI_listview_select_memeber_display( subord_mem_t*pedit, char*title )
{
	int wide, high, x0, y0;
	x0   = pedit->x;
	y0   = pedit->y - 2;
	wide = pedit->eidt_lenth * pedit->CHwide + 15;
	high = pedit->CHhigh + 8;
	GUI_set_Bgcolor( m_GUI_listview_Bgcor );
	GUI_fill_Rect_At( x0, y0, x0 + wide, y0 + high );
	GUI_widget_3D_DrawDownRect( x0 + 1, y0, wide - 2, high );


	GUI_set_Bgcolor( m_GUI_listview_Bgcor );
	GUI_set_Fgcolor( C_GUI_RED );
	GUI_SetFront(( GUI_FONT * )pedit->pfront );
	GUI_print_string( x0 + 8, y0 + 4, 800, 600, title );
	GUI_listview_select_state_display( x0, y0, wide, high, C_GUI_BlACK );
	return;
}

void GUI_listview_memeber_add( char*title, Bsize_t id )
{
	subord_mem_t*pedit;
	widget_listview_t *plistview;
	uint16_t x, y;
	uint16_t len;
	Bsize_t pos;
	plistview = GUI_listview_data_obtain( m_cur_hwin );
	x = plistview->member_x;
	y = plistview->member_y + ( plistview->sub_high - m_GUI_CHhigh ) - 5;
	if ( m_cur_hwin->type != WIDGET_LISTVIEW ) return;
	pedit = GUI_edit_mem_req();
	if ( !pedit ) return;
	pedit->CHhigh = m_GUI_CHhigh;
	pedit->CHwide = m_GUI_CHwide;
	pedit->pfront = GUI_GetFront();
	len = GUI_strlen(( char* )title ) * m_GUI_CHwide;
	pedit->id = id;
	pedit->y  = y;
	pedit->x  = x;
	pedit->eidt_lenth = GUI_strlen(( char* )title );
	pedit->flag = 0;
	pedit->next_edit = PNULL;
	if ( !( m_cur_hwin->subord_end ) )
	{
		m_cur_hwin->subord_head = pedit;
		m_cur_hwin->subord_end  = pedit;
		pedit->last_edit        = PNULL;
	}
	else
	{
		( m_cur_hwin->subord_end )->next_edit = pedit;
		pedit->last_edit = m_cur_hwin->subord_end;
		m_cur_hwin->subord_end = pedit;
	}
	GUI_listview_memeber_display( pedit, title );

	pos = plistview->listview_member_pos;
	if ( pos < MAX_LIST_SIZE )
	{
		plistview->listview_member_pos++;
		plistview->listview_member[pos].id = id;
		GUI_strcpy( plistview->listview_member[pos].title, title );
		plistview->member_x = plistview->member_x + len + 15;
	}
	return;
}
/********************************************************
 * @创建人 :揭成
 * @功能 :获取并选框状态
 * @输入 :
 * @输出 :
********************************************************/
Bsize_t GUI_listview_curmember_obtain( hwin_t*hp )
{
	return (( hp->subord_cur )->id );
}

char* GUI_listview_title_obtain( hwin_t*hp, Bsize_t id )
{
	int offset;
	widget_listview_t *plistview;
	plistview = GUI_listview_data_obtain( hp );
	for ( offset = 0;offset < plistview->listview_member_pos;offset++ )
	{
		if ( plistview->listview_member[offset].id == id )
			return ( plistview->listview_member[offset].title );
	}
	return 0;
}
/********************************************************
 * @创建人 :揭成
 * @功能 :设置并选框状态
 * @输入 :
 * @输出 :
********************************************************/
void GUI_listview_select( hwin_t*hp, Bsize_t id )
{
	subord_mem_t*plist;
	subord_mem_t*pedit;
	plist = hp->subord_head;
	GUI_window_hwin_req( hp );
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
		pedit  = hp->subord_cur;
		GUI_listview_memeber_display( pedit, GUI_listview_title_obtain( hp, pedit->id ) );
	}
	hp->subord_cur = plist;
	pedit  = hp->subord_cur;
	GUI_listview_select_memeber_display( pedit, GUI_listview_title_obtain( hp, id ) );
	GUI_window_hwin_release( hp );
	return;
}
#if GUI_TOUCH_EN >0
/********************************************************************
 * @创建人: 揭成
 * @功能:编辑框坐标匹配
 *
 * @输入:

 * @输出:
********************************************************************/
static BOOL Gui_listview_match( subord_mem_t*pedit, app_touch_t*data, hwin_t*hp )
{
	int xmax, xmin, ymax, ymin;//,offset;
	xmax = pedit->x + pedit->eidt_lenth * pedit->CHwide;
	xmin = pedit->x ;
	ymax = pedit->y + HZ_HIGH + 3;
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
/********************************************************************
 * @创建人: 揭成
 * @功能:
 * @输入:
 * @输出:
********************************************************************/
BOOL GUI_listview_ontouch( hwin_t*hp, app_touch_t*data )
{
	subord_mem_t*plist;
	plist = hp->subord_head;
	while ( 1 )
	{
		if ( plist == NULL )
		{
			return FAIL;
		}
		if ( Gui_listview_match( plist, data, hp ) )
		{
			if ( plist->id != GUI_listview_curmember_obtain( hp ) )
			{
				GUI_listview_select( hp, plist->id );
				GUI_key_dummy_press( hp, ( hp->flag )&0xff );
				return TURE;
			}
		}
		plist = plist->next_edit;
	}
}
#endif
/********************************************************************
 * @创建人 :揭成
 * @功能 :按键触摸处理
 *
 * @输入 :控件初始化
 *@输出  :NONE
***********************************************************/
void GUI_listview_init( void )
{
	m_GUI_listview_Bgcor = 0x841F;
	m_GUI_listview_Fgcor = C_GUI_BlACK;
	return;
}
#endif

