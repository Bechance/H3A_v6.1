#include "../../Gui.h"
#include "../GUI_lcd_driver.h"
#if GUI_DROP_DOWN_LIST_EN  >0
#define   DROPLIST_BORDER_COLR    		0x8000
#define   DROPLIST_BUTTON_BG_COLR    	1151

typedef struct
{
	uint16_t  SW_Fgcor;
	uint16_t  SW_Bgcor;
	uint16_t  drop_list_high;
	Bsize_t   Dhz_high;
	uint16_t  state;
	GUI_FONT*pFont;
	uint8_t  drop_list_is_start;
	const    char**drop_memeber;
	uint8_t  drop_cur_member;
	uint8_t  drop_member_size;
}widget_droplist_t;

static widget_droplist_t*GUI_droplist_data_obtain( hwin_t *hp )
{
	return ( widget_droplist_t* )hp->pwidget;
}
BOOL 		GUI_drop_list_ontouch( hwin_t*hp, app_touch_t*data );
void 		GUI_WinSize_modify( int x, int y, int wide, int high, hwin_t*pOBJ );
static    	uint16_t m_GUI_drop_list_SW_Bgcor = DROPLIST_BUTTON_BG_COLR;
static    	uint16_t m_GUI_drop_list_SW_Fgcor = 0;
static    	uint16_t m_GUI_drop_list_TXTBgcor = C_GUI_WHITE;
static    	uint16_t m_GUI_drop_list_TXTFgcor = 0;
void Gui_drop_list_set_color( uint16_t Fgcor, uint16_t Bgcor )
{
	m_GUI_drop_list_SW_Bgcor = Bgcor;
	m_GUI_drop_list_SW_Fgcor = Fgcor;
	return;
}
void Gui_drop_list_init( void )
{
	m_GUI_drop_list_SW_Bgcor = DROPLIST_BUTTON_BG_COLR;
	m_GUI_drop_list_SW_Fgcor = C_GUI_BlACK;
	return;
}
void _droplist_paint( hwin_t*hp )
{
	int offset;
	widget_droplist_t*pWidget;
	pWidget 		  = GUI_droplist_data_obtain( hp );
	if ( pWidget->state == 1 )
	{
		GUI_window_hwin_req( hp );
		GUI_set_Bgcolor( m_GUI_drop_list_TXTBgcor );
		GUI_set_Fgcolor( m_GUI_drop_list_TXTFgcor );
		GUI_SetFront( pWidget->pFont );
		GUI_clean_window();
		GUI_draw_Rect( hp->win_x, hp->win_y, hp->win_wide, hp->win_high );
		GUI_set_Bgcolor( C_GUI_LOW_BLUE );
		GUI_set_Fgcolor( C_GUI_WHITE );
		GUI_string_display_At( 0, 0, ( char* )pWidget->drop_memeber[pWidget->drop_cur_member] );
		for ( offset = 0;offset < pWidget->drop_member_size;offset++ )
		{
			if ( offset != pWidget->drop_cur_member )
			{
				GUI_set_Bgcolor( m_GUI_drop_list_TXTBgcor );
				GUI_set_Fgcolor( m_GUI_drop_list_TXTFgcor );
			}
			else
			{
				GUI_set_Bgcolor( C_GUI_BLUE );
				GUI_set_Fgcolor( C_GUI_WHITE );
			}
			GUI_string_display_At( 0, ( offset + 1 )*pWidget->Dhz_high, ( char* )pWidget->drop_memeber[offset] );
		}
		GUI_window_hwin_release( hp );
	}
	else if ( pWidget->state == 0 )
	{
		GUI_window_hwin_req( hp );
		GUI_set_Bgcolor( m_GUI_drop_list_TXTBgcor );
		GUI_clean_window();
		GUI_set_Fgcolor( m_GUI_drop_list_TXTFgcor );
		GUI_SetFront( pWidget->pFont );
		GUI_string_display_At( 0, 0, ( char* )pWidget->drop_memeber[pWidget->drop_cur_member] );
		GUI_window_hwin_release( hp );
	}
	else if ( pWidget->state == 2 )
	{
		GUI_window_hwin_req( hp );
		GUI_set_Bgcolor( C_GUI_YELLOW);
		GUI_clean_window();
		GUI_set_Fgcolor( C_GUI_BlACK );
		GUI_SetFront( pWidget->pFont );
		GUI_string_display_At( 0, 0, ( char* )pWidget->drop_memeber[pWidget->drop_cur_member] );
		GUI_window_hwin_release( hp );
	}
	hp->WIN_Paint = 0;
	return;
}

static void _drop_list_ready( hwin_t*hp, int state )
{
	widget_droplist_t*pWidget;
	pWidget 		  = GUI_droplist_data_obtain( hp );
	if(state==1)
	{
	    pWidget->state    = 2;
		hp->WIN_Paint     = 1 ;
	}
	else if(state==0)
	{
	    pWidget->state    = 0;
		hp->WIN_Paint     = 1 ;
	}
	return;
}
static void _drop_list_ready_do( hwin_t*hp )
{
    GUI_UNLOCK();
	GUI_drop_list_unfold(hp);
    GUI_LOCK();
	_drop_list_ready(hp,1);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :创建下拉菜单
 * @输入 :NONE
 * @输出 :NONE
***********************************************************/
hwin_t* Gui_drop_list_creat( uint16_t xcoord,
							 uint16_t ycoord,
							 const char *pmember[],
							 uint8_t size,
							 Bsize_t flag,
							 hwin_t* fatherhp )

{
	hwin_t*hp;
	uint8_t lenth = 0;
	uint8_t offset;
	Bsize_t wide ;
	uint16_t x0, y0, x, y, len, bwide, bhigh;
	widget_droplist_t*pWidget;
	for ( offset = 0;offset < size;offset++ )
	{
		if ( lenth < GUI_strlen(( char* )pmember[offset] ) )
		{
			lenth = GUI_strlen(( char* )pmember[offset] );
		}
	}

	wide = ( lenth + 2 ) * GUI_conText.CUR_Front->CHwide + 8;
	if (( wide + xcoord ) > GUI_LCD_WIDE )
		return PNULL;
	GUI_window_hwin_req( fatherhp );
	GUI_set_Bgcolor( C_GUI_WHITE );
	GUI_set_Fgcolor( DROPLIST_BORDER_COLR );
	hp = GUI_window_child_creat(
			 xcoord,
			 ycoord,
			 wide,
			 GUI_conText.CUR_Front->high + 4,
			 WIN_3D | WIN_FILL,
			 fatherhp );
	hp->flag = flag;

	hp->type = WIDGET_DROP_LIST;
#if GUI_TOUCH_EN>0
	hp->_ontouch = GUI_drop_list_ontouch;
#endif
	hp->_paint        = _droplist_paint;
	hp->pwidget       = GUI_malloc( sizeof( widget_droplist_t ) );
	pWidget 		  = GUI_droplist_data_obtain( hp );
    GUI_MEMCLR(pWidget,sizeof(widget_droplist_t));
	pWidget->drop_memeber = pmember;
	pWidget->drop_member_size = size;
	pWidget->drop_cur_member = 0;
	pWidget->pFont 	  = ( GUI_FONT* )GUI_GetFront();
	pWidget->SW_Fgcor = m_GUI_drop_list_SW_Fgcor;
	pWidget->SW_Bgcor = m_GUI_drop_list_SW_Bgcor;
	pWidget->drop_list_high = pWidget->pFont->high + 4;
	pWidget->Dhz_high = pWidget->pFont->high;
	pWidget->state    = 0;

	GUI_set_Bgcolor( pWidget->SW_Bgcor );
	bhigh = pWidget->drop_list_high - 4;
	bwide = 2 * GUI_conText.CUR_Front->CHwide + 4;
	x0 = hp->win_x + wide - bwide - 2;
	y0 = hp->win_y + 2;
	GUI_fill_Rect( x0, y0, bwide, bhigh );
	GUI_widget_3D_paint( x0, y0, bwide, bhigh );
	GUI_window_hwin_req( hp );
	GUI_set_Fgcolor( 0 );
	GUI_set_Bgcolor( pWidget->SW_Bgcor );
	x = wide - bwide - 2 + bwide / 4;
	y = bhigh / 2;
	len = bwide * 1 / 2;
	while ( len > 1 )
	{
		GUI_WINDIS_HLINE( x, x + len, y );
		y++;
		x++;
		len -= 2;
	}
	hp->win_wide = hp->win_wide - ( bwide + 2 );
	hp->_WINdo     = _drop_list_ready_do;
	hp->_WINselect = _drop_list_ready;
	hp->WIN_Paint  = 1;
	GUI_window_hwin_release( hp );
	GUI_window_hwin_release( fatherhp );
	return hp;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :菜单展开显示
 *
 * @输入 :NONE
 *@输出  :NONE
***********************************************************/
static void GUI_drop_list_unfold_display( hwin_t*hp )
{
	uint8_t offset;
	uint8_t key;
	uint8_t pos;
	widget_droplist_t*pWidget;
	pWidget 		  = GUI_droplist_data_obtain( hp );
	offset = pWidget->drop_cur_member;
	GUI_GetEXTkey( &key );
	while ( 1 )
	{
		GUI_msleep( 1 );
		if ( GUI_GetEXTkey( &key ) )
		{
			if ( key == GUIE_KEY_DOWN )
			{
				if ( pWidget->drop_cur_member < ( pWidget->drop_member_size - 1 ) )
				{
					pWidget->drop_cur_member++;
					hp->WIN_Paint = 1;
				}
			}
			else if ( key == GUIE_KEY_UP )
			{
				if ( pWidget->drop_cur_member > 0 )
				{
					pWidget->drop_cur_member--;
					hp->WIN_Paint = 1;
				}
			}
			else if ( key == GUIE_KEY_ENTER )
			{
				break;
			}
			else if ( GUIE_KEY_1 <= key && key <= GUIE_KEY_9 )
			{
				pos = key - GUIE_KEY_1;
				if ( pos < pWidget->drop_member_size )
				{
					pWidget->drop_cur_member = pos;
					break;
				}
			}
			else if ( key == GUIE_KEY_EXIT )
			{
				pWidget->drop_cur_member = offset;
				break;
			}
		}
	}
	return;
}
/********************************************************
 * @创建人 :揭成
 * @功能 :菜单展开
 *
 * @输入 :NONE
 *@输出  :NONE
***********************************************************/
Bsize_t GUI_drop_list_unfold( hwin_t*hp )
{
	int high;
	BLKmem_t pmem;
	widget_droplist_t*pWidget;
	pWidget 		  = GUI_droplist_data_obtain( hp );
	if ( !hp ) return pWidget->drop_cur_member;
	if ( pWidget->drop_list_is_start )
	{
		return pWidget->drop_cur_member;
	}
	GUI_window_hwin_req( hp );
	pWidget->drop_list_is_start = TURE;
	if (( hp->win_y + hp->win_high + 4 + pWidget->Dhz_high*( pWidget->drop_member_size ) ) > GUI_LCD_HIGH )
	{
		pWidget->drop_list_is_start = FAIL;
		GUI_window_hwin_release( hp );
		return pWidget->drop_cur_member;
	}
	high = hp->win_high;
	GUI_window_hwin_release( hp );
	GUI_WinSize_modify( hp->win_x, hp->win_y, hp->win_wide, pWidget->Dhz_high *( pWidget->drop_member_size + 1 ) + 5, hp );
	GUI_TopWIN_Set( hp );
	pmem = GUI_read_block_Extp( hp->win_x - 1,
								hp->win_y - 1,
								hp->win_x + hp->win_wide + 1,
								hp->win_y + hp->win_high + 4 );
	if ( pmem )
	{
		pWidget->state = 1;
		hp->WIN_Paint  = 1;
		GUI_keyExt_Lock();
		GUI_drop_list_unfold_display( hp );
		GUI_keyExt_unLock();
		GUI_write_block_Extp( hp->win_x - 1,
							  hp->win_y - 1,
							  hp->win_x + hp->win_wide + 1,
							  hp->win_y + hp->win_high + 4 ,
							  pmem );

	}
	GUI_WinSize_modify( hp->win_x, hp->win_y, hp->win_wide, high, hp );
	GUI_TopWIN_cancel( hp );
	pWidget->state = 0;
	hp->WIN_Paint  = 1;
	pWidget->drop_list_is_start = FAIL;
	return pWidget->drop_cur_member;
}



void  GUI_drop_list_member_assign( hwin_t*hp, Bsize_t offset )
{
	widget_droplist_t*pWidget;
	pWidget 		  = GUI_droplist_data_obtain( hp );
	GUI_LOCK();
	pWidget->drop_cur_member = offset;
	hp->WIN_Paint  = 1;
	GUI_UNLOCK();
	return;
}

Bsize_t  GUI_drop_list_member_obtain( hwin_t*hp )
{
	widget_droplist_t*pWidget;
	pWidget 		  = GUI_droplist_data_obtain( hp );
	return pWidget->drop_cur_member;
}
#if GUI_TOUCH_EN >0
/********************************************************
 * @创建人 :揭成
 * @功能 :下拉菜单触摸坐标匹配
 *
 * @输入 :NONE
 *@输出  :NONE
***********************************************************/
static BOOL GUI_drop_list_match( hwin_t*hp, app_touch_t*data )
{
	int xmax ;
	int xmin ;
	int ymax;
	int ymin ;
	widget_droplist_t*pWidget;
	pWidget 		  = GUI_droplist_data_obtain( hp );

	xmax = hp->win_x + hp->win_wide + 2 * pWidget->pFont->CHwide + 4;
	xmin = hp->win_x - 1;
	ymax = hp->win_y + hp->win_high + 1;
	ymin = hp->win_y - 1;

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
 * @功能 :下拉菜单触摸属性
 *
 * @输入 :NONE
 *@输出  :NONE
***********************************************************/
BOOL GUI_drop_list_ontouch( hwin_t*hp, app_touch_t*data )
{
	Bsize_t member;
	widget_droplist_t*pWidget;
	pWidget 		  = GUI_droplist_data_obtain( hp );
	if ( GUI_drop_list_match( hp, data ) )
	{
		GUI_wait_touch_release();
		if ( !pWidget->drop_list_is_start )
		{
			GUI_key_dummy_press( hp, hp->flag&0xff );
		}
		else
		{
			member = ( data->y - ( hp->win_y + pWidget->Dhz_high ) ) / pWidget->Dhz_high;
			if ( member <= ( pWidget->drop_member_size - 1 ) )
			{
				pWidget->drop_cur_member = member;
				GUI_key_dummy_press( hp, GUIE_KEY_ENTER );
			}
		}
		return TURE;
	}
	return FAIL;
}
#endif
#endif
