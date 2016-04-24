/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GUI核心
* 创建时间:2010-10-11
* 所属公司 :科荟
* 文件名称:Gui_core.c
* 创建人 :揭成
* 功能描述:简单的图形管理模块,主要是为了模块
* 化界面程序，它以窗口管理形式
*----------------------------------------------------------------
修改记录:
修改日期:
修改人:
//
在core.c文件中的函数坐标均以窗口为基础的相对坐标
********************************************************************/
#include "../Gui.h"
/********************************************************************/
//本地变量
/********************************************************************/
hwin_t*                             m_hwin_list_end = PNULL;
hwin_t*                             m_hwin_list_head = PNULL;
hwin_t*         					m_cur_hwin   = 0;
hwin_t * 							m_Gui_hwin   = 0;
hwin_t* 							m_Gui_locked_win[GUI_LOOP_SIZE] = {0, };
int									m_win_depth = 0;
GUI_const_t                         m_GUItext_bak[GUI_LOOP_SIZE];
GUI_top_t*							m_pTop_list_head = 0;
GUI_top_t*							m_pTop_list_end  = 0;
hwin_t*								m_CURwidget  = 0;
subord_mem_t* 						m_pMember = 0;

static BOOL GUI_window_is_son( hwin_t*pCUR, hwin_t*farther );
void GUI_handPen_check( app_touch_t*pdata );


/*获取GUI 主窗口，初始化时创建*/

hwin_t*GUI_hwin_obtain( void )
{
	return m_Gui_hwin;
}

hwin_t*GUI_CURwin_obtain( void )
{
	return m_cur_hwin;
}

static void GUI_TopWIN_add_to_link( GUI_top_t* pCUR )
{
	pCUR->pNext = 0;
	if ( !m_pTop_list_end )
	{
		m_pTop_list_end =  pCUR;
		m_pTop_list_head = pCUR;
	}
	else
	{
		m_pTop_list_end->pNext = pCUR;
		pCUR->pLast     = m_pTop_list_end;
		m_pTop_list_end = pCUR ;
	}
	return;
}
static void GUI_TopWIN_delete_from_link( GUI_top_t* pCUR )
{
	if ( pCUR->pNext )
		pCUR->pNext->pLast = pCUR->pLast;
	else
		m_pTop_list_end = pCUR->pLast;
	pCUR->pLast->pNext = pCUR->pNext;
	GUI_free( pCUR );
	return;
}

void  GUI_TopWIN_Set( hwin_t*pOBJ )
{
	GUI_top_t*pM;
	GUI_LOCK();
	pM = GUI_malloc( sizeof( GUI_top_t ) );
	if ( pM )
	{
		pM->pWIN = pOBJ;
		GUI_TopWIN_add_to_link( pM );
	}
	GUI_UNLOCK();
	return;
}

void  GUI_TopWIN_cancel( hwin_t*pOBJ )
{
	GUI_top_t*pM = m_pTop_list_head;
	GUI_LOCK();
	while ( pM )
	{
		if ( pM->pWIN == pOBJ )
		{
			if ( pM != m_pTop_list_head )
			{
				GUI_TopWIN_delete_from_link( pM );
				m_pTop_list_end->pNext = 0;
			}
			else if ( m_pTop_list_end != pM )
			{
				m_pTop_list_head = m_pTop_list_head->pNext;
				m_pTop_list_head->pLast = 0;
				GUI_free( pM );
			}
			else
			{
				m_pTop_list_head = 0;
				m_pTop_list_end  = 0;
				GUI_free( pM );
			}
			break;
		}
		pM = pM->pNext;
	}
	GUI_UNLOCK();
	return;
}

BOOL  GUI_foucus_dot_divide( int x, int y )
{
	int xmax, xmin, ymax, ymin;
	hwin_t*pWIN;
	GUI_top_t*pM = m_pTop_list_head;
	if ( !pM )    return FAIL;
	while ( pM )
	{
		pWIN = pM->pWIN;
		if ( m_cur_hwin == pWIN ) return FAIL;
		if ( GUI_window_is_son( m_cur_hwin, pWIN ) )return FAIL;
		xmin = pWIN->lcdxs - 1;
		xmax = pWIN->lcdxe + 1;
		ymin = pWIN->lcdys - 1;
		ymax = pWIN->lcdye + 1;
		if (( x > xmin && x < xmax ) && ( y > ymin && y < ymax ) )
		{
			return TURE;
		}
		pM = pM->pNext;
	}
	return FAIL;
}

BOOL  GUI_foucus_area_divide( int x, int y, int wide, int high )
{
	int x1, y1;
	hwin_t*pWIN;
	GUI_top_t*pM = m_pTop_list_head;
	if ( !pM )    return FAIL;
	while ( pM )
	{
		pWIN = pM->pWIN;
		if ( m_cur_hwin == pWIN ) return FAIL;
		if ( GUI_window_is_son( m_cur_hwin, pWIN ) )return FAIL;
		pM = pM->pNext;
	}
	x1 = x;
	y1 = y;
	if ( GUI_foucus_dot_divide( x1, y1 ) )
		return TURE;
	x1 = x;
	y1 = y + high;
	if ( GUI_foucus_dot_divide( x1, y1 ) )
		return TURE;
	x1 = x + wide;
	y1 = y;
	if ( GUI_foucus_dot_divide( x1, y1 ) )
		return TURE;
	x1 = x + wide;
	y1 = y + high;
	if ( GUI_foucus_dot_divide( x1, y1 ) )
		return TURE;
	x1 = x + wide / 2;
	y1 = y + high / 2;
	if ( GUI_foucus_dot_divide( x1, y1 ) )
		return TURE;
	return FAIL;
}

/*获取一个空闲窗口*/
static hwin_t*GUI_free_window_obtain( void )
{
	hwin_t*pobj;
	pobj = GUI_malloc( sizeof( hwin_t ) );
	GUI_MEMCLR( pobj, sizeof( hwin_t ) );
	return pobj;
}

static void GUI_window_free( hwin_t*hp )
{
	GUI_TopWIN_cancel( hp );
	if ( hp->pwidget )
		GUI_free( hp->pwidget );
	if ( hp->pMSG )
		GUI_MsgFIFO_del( hp );
	GUI_free( hp );
	return ;
}

uint16_t LCD_Color2Index_565( uint32_t Color )
{
	uint32_t r, g, b;
	r = ( Color >> ( 0 + 3 ) ) & 0x1f;
	g = ( Color >> ( 8 ) ) & 0x3f;
	b = ( Color >> ( 16 + 3 )) & 0x1f;
	return b + ( g << 5 ) + ( r << 11 );
}

uint16_t LCD_RGB_565(uint16_t R,uint16_t G,uint16_t B)
{
	return (B + ( G << 5 ) + ( R << 11 ));
}




void GUI_draw_color_line( int x0, int y0, int x1, int y1, uint16_t color )
{
	GUI_set_Fgcolor( color );
	GUI_draw_line( x0, y0, x1, y1 );
	return;
}

void GUI_widget_3D_paint( uint16_t x, uint16_t y, Bsize_t wide, Bsize_t high )
{
	uint16_t x0 = x, y0 = y, x1 = x + wide, y1 = y + high;
#if GUI_CORLOR_BIT == 8
	GUI_set_Fgcolor( 0 );
	GUI_draw_Rect( x0, y0, wide, high );
	GUI_draw_color_line( x0 + 1, y0 + 1, x1 - 2, y0 + 1, 0xff );
	GUI_draw_color_line( x0 + 1, y0 + 1, x0 + 1, y1 - 2, 0xff );
	GUI_draw_color_line( x0 + 1, y1 - 1, x1 - 1, y1 - 1, 109 );
	GUI_draw_color_line( x1 - 1, y0 + 1, x1 - 1, y1 - 2, 109 );
#else
	GUI_set_Fgcolor( 0 );
	GUI_draw_Rect( x0, y0, wide, high );          	/* Draw rectangle around it */
	GUI_draw_color_line( x0 + 1, y0 + 1, x1 - 2, y0 + 1, 0xffff ); /* Draw top line */
	GUI_draw_color_line( x0 + 1, y0 + 1, x0 + 1, y1 - 2, 0xffff );
	GUI_draw_color_line( x0 + 1, y1 - 1, x1 - 1, y1 - 1, LCD_Color2Index_565( 0x555555 ) );
	GUI_draw_color_line( x1 - 1, y0 + 1, x1 - 1, y1 - 2, LCD_Color2Index_565( 0x555555 ) );
#endif
	return;
}


void GUI_widget_3D_DrawDownRect( uint16_t x, uint16_t y, Bsize_t wide, Bsize_t high )
{
#define  UPCOR     LCD_Color2Index_565(0x555555)
#define  DOWNCOR   0xffff
#define  GUI_DrawHLine(y,x1,x2,color) {GUI_set_Fgcolor(color);GUI_draw_Hline(x1,x2-1,y);}
#define  GUI_DrawVLine(x,y1,y2,color) {GUI_set_Fgcolor(color);GUI_draw_Vline(x,y1,y2);}
	uint16_t x0, y0, x1, y1;
// x = x+1;y=y;
// high=high+1;
	wide = wide + 1;
	x0 = x - 1;
	y0 = y;
	x1 = x + wide;
	y1 = y + high;
#if GUI_CORLOR_BIT ==8
	GUI_set_Fgcolor( 0 );
	GUI_draw_Rect( x0, y0, wide, high );
	GUI_draw_color_line( x0 + 1, y0 + 1, x1 - 2, y0 + 1, 0xff );
	GUI_draw_color_line( x0 + 1, y0 + 1, x0 + 1, y1 - 2, 0xff );
	GUI_draw_color_line( x0 + 1, y1 - 1, x1 - 1, y1 - 1, 109 );
	GUI_draw_color_line( x1 - 1, y0 + 1, x1 - 1, y1 - 2, 109 );
#else
	GUI_DrawHLine( y0, x0, x1, UPCOR );
	GUI_DrawVLine( x0, y0 + 1, y1, UPCOR );
	GUI_DrawHLine( y0 + 1, x0 + 1, x1 - 1, 0 );
	GUI_DrawVLine( x0 + 1, y0 + 2, y1 - 1, 0 );
	GUI_DrawHLine( y1, x0 + 1, x1, DOWNCOR );
	GUI_DrawVLine( x1 - 1, y0 + 1, y1, DOWNCOR );
// GUI_DrawHLine(y1 - 1, x0 + 2,x1-1,LCD_Color2Index_565(0xc0c0c0));
// GUI_DrawVLine(x1 - 1, y0 + 2,y1-1,LCD_Color2Index_565(0xc0c0c0));
#endif
	return;
}
void GUI_widget_sink_3D_paint( uint16_t x, uint16_t y, Bsize_t wide, Bsize_t high )
{
	uint16_t x0 = x, y0 = y, x1 = x + wide, y1 = y + high;
#if GUI_CORLOR_BIT ==8
	GUI_set_Fgcolor( 0 );
	GUI_draw_Rect( x0, y0, wide, high );
	GUI_draw_color_line( x0 + 1, y0 + 1, x1 - 0, y0 + 1, 109 );
	GUI_draw_color_line( x0 + 1, y0 + 1, x0 + 1, y1 - 0, 109 );
	GUI_draw_color_line( x0 + 1, y1 - 1, x1 - 1, y1 - 1, 0xff );
	GUI_draw_color_line( x1 - 1, y0 + 1, x1 - 1, y1 - 2, 0xff );
#else
	GUI_set_Fgcolor( 0 );
	GUI_draw_Rect( x0, y0, wide, high );     /* Draw rectangle around it */
	GUI_draw_color_line( x0 + 1, y0 + 1, x1 - 0, y0 + 1, LCD_Color2Index_565( 0x555555 ) );    /* Draw top line */
	GUI_draw_color_line( x0 + 1, y0 + 1, x0 + 1, y1 - 0, LCD_Color2Index_565( 0x555555 ) );
	GUI_draw_color_line( x0 + 1, y1 - 1, x1 - 1, y1 - 1, 0xffff );
	GUI_draw_color_line( x1 - 1, y0 + 1, x1 - 1, y1 - 2, 0xffff );
#endif
	return;
}

void GUI_windows_paint( hwin_t*parg, Bsize_t flags )
{
	Bsize_t flags_shadow = flags;
	if ( flags == 0 ) return;
	GUI_window_hwin_req( parg );
	flags =  flags & WIN_PAINT;
	if ( WIN_PAINT == flags )
	{
		GUI_draw_Rect( parg->win_x, parg->win_y, parg->win_wide, parg->win_high );
	}
	flags = flags_shadow;
	flags =  flags & WIN_FILL;
	if ( WIN_FILL == flags )
	{
		if (( flags_shadow&WIN_3D ) == WIN_3D )
		{
			parg->win_Bcor = GUI_conText.Bcolor;
			GUI_fill_Rect( parg->win_x, parg->win_y + 1, parg->win_wide - 1, parg->win_high - 1 );
		}
		else
		{
			GUI_fill_Rect( parg->win_x + 1, parg->win_y + 1,
						   parg->win_wide - 1, parg->win_high - 1 );
		}
	}
	flags = flags_shadow;
	flags = flags & WIN_3D;
	if ( flags == WIN_3D )
	{
		GUI_widget_3D_paint( parg->win_x, parg->win_y, parg->win_wide, parg->win_high );
	}
	GUI_window_hwin_release( parg );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :获取父窗口下的子窗口句柄
 * @输入  :
 * @输出  :NONE
********************************************************************/
hwin_t* GUI_childobj_obtian( Bsize_t id, Bsize_t type, hwin_t*fatherhp )
{
	hwin_t* ptemp;
	Bsize_t mID;
	ptemp = m_hwin_list_head;
	while ( ptemp )
	{
		mID = ( ptemp->flag & 0xff );
		if ( ptemp->type == type && mID == id && ptemp->father_hwin == fatherhp )
		{
			return ptemp;
		}
		ptemp = ptemp->next_hwin;
	}
	return 0;
}
hwin_t* GUI_Getchild( Bsize_t id, hwin_t*fatherhp )
{
	hwin_t* ptemp;
	Bsize_t mID;
	ptemp = m_hwin_list_head;
	while ( ptemp )
	{
		mID = ( ptemp->flag & 0xff );
		if ( mID == id && ptemp->father_hwin == fatherhp )
		{
			return ptemp;
		}
		ptemp = ptemp->next_hwin;
	}
	return 0;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :创建一个子窗口
 * @输入 :(x_coord,y_coord)起点坐标,wide 窗口宽度，high窗口高度，
 *                flag|WIN_3D  :窗口边框具有3D属性
                   flag|WIN_PAINT :绘制边框，按当前前景色
                   flag|WIN_FILL  :填充窗口，按当前底色填充
 *                fatherhp 对应父窗口句柄
 *@输出 : NONE
********************************************************************/
hwin_t* GUI_window_child_creat( 
		uint16_t x_coord ,
		uint16_t y_coord,
		Bsize_t wide,
		Bsize_t high,
		Bsize_t flags,
		hwin_t*fatherhp
)
{
	hwin_t* parg;
	if ( !fatherhp ) return PNULL;
	GUI_LOCK();
	parg = GUI_free_window_obtain();
	if ( parg )
	{
		parg->flag = flags;
		parg->type = CHILD_WIN;
		parg->win_x = x_coord + fatherhp->win_x + BORGAP;
		parg->win_y = y_coord + fatherhp->win_y + BORGAP;
		parg->win_wide    = wide;
		parg->win_high    = high;
		parg->lcdxs   	  = parg->win_x;
		parg->lcdxe   	  = parg->win_x + parg->win_wide;
		parg->lcdys  	  = parg->win_y;
		parg->lcdye   	  = parg->win_y + parg->win_high;
		parg->win_depth   = fatherhp->win_depth + 1;
		if ( GET_XBORDER( parg ) <= ( GET_XBORDER( fatherhp ) - BORGAP ) )
		{
			parg->win_wide = wide;
		}
		else
		{
			parg->win_wide = GET_XBORDER( fatherhp ) - parg->win_x - BORGAP;
		}
		if ( GET_YBORDER( parg ) <= ( GET_YBORDER( fatherhp ) - BORGAP ) )
		{
			parg->win_high = high ;
		}
		else
		{
			parg->win_high = GET_YBORDER( fatherhp ) - parg->win_y - BORGAP ;
		}
		parg->win_Bcor = GUI_conText.Bcolor;
		parg->win_Fcor = GUI_conText.Fcolor;
		if ( parg->win_wide > LCD_WIDE ) parg->win_wide = LCD_WIDE;
		if ( parg->win_high > LCD_HIGH ) parg->win_high = LCD_HIGH;
		parg->csr_x_pos = parg->win_x + BORGAP;
		parg->csr_y_pos = parg->win_y + BORGAP;
		( m_hwin_list_end )->next_hwin = parg;
		parg->last_hwin = m_hwin_list_end;
		parg->next_hwin = 0;
		m_hwin_list_end = parg ;
		parg->father_hwin = fatherhp;
		GUI_windows_paint( parg, flags );
		GUI_UNLOCK();
		return parg;
	}
	GUI_UNLOCK();
	return PNULL;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :创建一个主窗口
 * @输入 :(x_coord,y_coord)起点坐标,wide 窗口宽度，high窗口高度，
 *                flag|WIN_3D  :窗口边框具有3D属性
                   flag|WIN_PAINT :绘制边框，按当前前景色
                   flag|WIN_FILL  :填充窗口，按当前底色填充
 *
 *@输出 : NONE
********************************************************************/

hwin_t* GUI_window_creat( uint16_t x_coord , uint16_t y_coord, Bsize_t wide,
						  Bsize_t high , Bsize_t flags )
{
	hwin_t* parg;
	GUI_LOCK();
	parg = GUI_free_window_obtain();
	if ( parg )
	{
		parg->flag = flags;
		parg->win_x = x_coord;
		parg->win_y = y_coord;
		( x_coord + wide > LCD_WIDE ) ?
		( parg->win_wide = LCD_WIDE - x_coord )
		: ( parg->win_wide = wide );
		( y_coord + high > LCD_HIGH ) ?
		( parg->win_high = LCD_HIGH - y_coord )
		: ( parg->win_high = high );
		parg->type = FATHER_WIN;
		parg->win_Bcor    = GUI_conText.Bcolor;
		parg->win_Fcor    = GUI_conText.Fcolor;
		parg->lcdxs   	  = parg->win_x;
		parg->lcdxe   	  = parg->win_x + parg->win_wide;
		parg->lcdys  	  = parg->win_y;
		parg->lcdye   	  = parg->win_y + parg->win_high;
		parg->next_hwin   = 0;
		parg->last_hwin   = 0;
		parg->father_hwin = 0;
		parg->win_depth   = 0; //设置深度为0
		if ( !m_hwin_list_end )
		{
			m_hwin_list_end =  parg;
			m_hwin_list_head = parg;
		}
		else
		{
			( m_hwin_list_end )->next_hwin = parg;
			parg->last_hwin = m_hwin_list_end;
			m_hwin_list_end = parg ;
		}
		parg->csr_x_pos = parg->win_x + BORGAP;
		parg->csr_y_pos = parg->win_y + BORGAP;
		GUI_windows_paint( parg, flags );
		GUI_UNLOCK();
		return parg;
	}
	GUI_UNLOCK();
	return PNULL;
}


/********************************************************************
 * @创建人:揭成
 * @功能  :更改窗口尺寸
 * @输入  :
 *@输出   : NONE
********************************************************************/
void GUI_WinSize_modify( int x, int y, int wide, int high, hwin_t*pOBJ )
{
	GUI_LOCK();
	pOBJ->win_x = x;
	pOBJ->win_y = y;
	( x + wide > LCD_WIDE ) ?
	( pOBJ->win_wide = LCD_WIDE - x )
	: ( pOBJ->win_wide = wide );
	( y + high > LCD_HIGH ) ?
	( pOBJ->win_high = LCD_HIGH - y )
	: ( pOBJ->win_high = high );
	pOBJ->lcdxs   	  = pOBJ->win_x;
	pOBJ->lcdxe   	  = pOBJ->win_x + pOBJ->win_wide;
	pOBJ->lcdys  	  = pOBJ->win_y;
	pOBJ->lcdye   	  = pOBJ->win_y + pOBJ->win_high;
	GUI_UNLOCK();
	return;
}

static BOOL GUI_window_is_son( hwin_t*pCUR, hwin_t*farther )
{
	Bsize_t depth;
	hwin_t* ptemp;
	hwin_t* pFWIN;
	depth = pCUR->win_depth;
	ptemp = pCUR;
	if ( depth > 10 ) return FAIL;
	if ( depth == 0 )return FAIL;
	while ( depth != 0 )
	{
		pFWIN = ptemp->father_hwin;
		ptemp = pFWIN;
		depth --;
		if ( pFWIN == farther ) return TURE;
	}
	return FAIL;
}

BOOL GUI_IS_Topwidget( hwin_t*p )
{
	GUI_top_t*pTop  = m_pTop_list_head;
	hwin_t   *pfather;
	while ( pTop )
	{
		pfather = pTop->pWIN;
		if ( pfather == p || GUI_window_is_son( p, pfather ) )
		{
			return TURE;
		}
		pTop = pTop->pNext;
	}
	return FAIL;
}

static void GUI_window_delete_from_link( hwin_t* hp )
{
	if ( m_CURwidget == hp ) m_CURwidget = 0;
	if ( hp->next_hwin )
	{
		( hp->next_hwin )->last_hwin = hp->last_hwin;
	}
	else
	{
		m_hwin_list_end = hp->last_hwin;
	}
	( hp->last_hwin )->next_hwin = hp->next_hwin;
	GUI_window_free( hp );
	return;
}

static void GUI_window_destory_son( hwin_t* hp )
{
	hwin_t*ptemp;
	hwin_t*plist;
	ptemp = m_hwin_list_head->next_hwin;
	while ( ptemp )
	{
		if ( GUI_window_is_son( ptemp, hp ) )
		{
			plist = ptemp->last_hwin;
			if ( ptemp->type == WIDGET_EDIT
				 || ptemp->type == WIDGET_RADIO
				 || ptemp->type == WIDGET_LISTVIEW )
			{
				GUI_edit_allmember_release( ptemp );
			}
			GUI_window_delete_from_link( ptemp );
			ptemp = plist->next_hwin;
			continue;
		}
		ptemp = ptemp->next_hwin;
	}
	return;
}
void  GUI_window_destory( hwin_t* hp )
{
	if ( !m_hwin_list_head ) return;
	if ( !hp ) return;
	GUI_LOCK();
	GUI_window_destory_son( hp );
	if ( hp->type == WIDGET_EDIT
		 || hp->type == WIDGET_RADIO
		 || hp->type == WIDGET_LISTVIEW )
	{
		GUI_edit_allmember_release( hp );
	}
	if ( hp != m_hwin_list_head )
	{
		GUI_window_delete_from_link( hp );
		m_hwin_list_end->next_hwin = 0;
	}
	else
	{
		m_hwin_list_head = 0;
		m_hwin_list_end  = 0;
		GUI_window_free( hp );
	}
	GUI_UNLOCK();
	return;
}

/*窗口相关参数保存*/
static void GUI_window_arg_save( void )
{
	m_GUItext_bak[m_win_depth] = GUI_conText;
	return;
}
/*窗口相关参数还原*/
static void GUI_window_arg_revert( void )
{
	GUI_conText = m_GUItext_bak[m_win_depth];
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :设置窗口为当前窗口，并且钳住窗口
 * @输入  :
 * @输出  :NONE
********************************************************************/
void GUI_window_hwin_req( hwin_t* hp )
{
	GUI_LOCK();
	if ( hp->type != INVALID_WIN )
	{
		m_Gui_locked_win[m_win_depth] = m_cur_hwin;
		GUI_window_arg_save();
		m_cur_hwin = hp;
		m_win_depth++;
	}
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :释放当前被钳住的窗口
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_window_hwin_release( hwin_t* hp )
{
	if ( m_win_depth != 0 )
	{
		m_win_depth--;
		if ( m_win_depth >= 0 )
		{
			m_cur_hwin = m_Gui_locked_win[m_win_depth];
			GUI_window_arg_revert();
		}
	}
	GUI_UNLOCK();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :窗口内坐标合法性判断
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
static void GUI_coord_lawful_decide( uint16_t *x, uint16_t *y )
{
	(( *x ) > m_cur_hwin->win_x ) ?
	(( *x ) = ( *x ) ) :
	(( *x ) = m_cur_hwin->win_x + BORGAP );

	(( *x ) < GET_XBORDER( m_cur_hwin ) ) ?
	(( *x ) = ( *x ) ) :
	(( *x ) = GET_XBORDER( m_cur_hwin ) - BORGAP );

	(( *y ) > m_cur_hwin->win_y ) ?
	(( *y ) = ( *y ) ) :
	(( *y ) = m_cur_hwin->win_y + BORGAP );

	(( *y ) < GET_YBORDER( m_cur_hwin ) ) ?
	(( *y ) = ( *y ) ) :
	(( *y ) = GET_YBORDER( m_cur_hwin ) - BORGAP );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :窗口边界判断
 * @输?:
 *
 *@输出 : NONE
********************************************************************/
void GUI_window_border_decide( void )
{
	GUI_coord_lawful_decide(
		&m_cur_hwin->csr_x_pos,
		&m_cur_hwin->csr_y_pos );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :设置当前X坐标
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_x_set( uint16_t x )
{
	m_cur_hwin->csr_x_pos = ( x + m_cur_hwin->win_x + BORGAP );
	GUI_window_border_decide();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :移动当前X坐标
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_x_shift( int offset )
{
	m_cur_hwin->csr_x_pos += offset;
	GUI_window_border_decide();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :设置当前Y坐标
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_y_set( uint16_t y )
{
	m_cur_hwin->csr_y_pos = ( y + m_cur_hwin->win_y + BORGAP );
	GUI_window_border_decide();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :设置当前坐标(x,y)
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_xy_set( uint16_t x, uint16_t y )
{
	m_cur_hwin->csr_x_pos = ( x + m_cur_hwin->win_x + BORGAP );
	m_cur_hwin->csr_y_pos = ( y + m_cur_hwin->win_y + BORGAP );
	GUI_window_border_decide();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :窗口内，在当前位置显示字符串
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
GUI_csr_t* GUI_WIN_cursor_obtain( hwin_t*pOBJ );
void GUI_string_display( char*pstr )
{
	GUI_csr_t*p;
	int maxx = m_cur_hwin->win_wide
			   + m_cur_hwin->win_x - BORGAP;
	int maxy = m_cur_hwin->win_high
			   + m_cur_hwin->win_y - BORGAP;
	if ( !m_cur_hwin->type )
		return;
	GUI_print_string(
		m_cur_hwin->csr_x_pos,
		m_cur_hwin->csr_y_pos,
		maxx, maxy, pstr );
	m_cur_hwin->csr_x_pos
	+= ( global_strlen( pstr ) * GUI_conText.CUR_Front->CHwide );

	p = GUI_WIN_cursor_obtain( m_cur_hwin );
	if ( p )GUI_cursor_reload( p );
	GUI_window_border_decide();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :在(x,y) 显示字符串
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_string_display_At( uint16_t x, uint16_t y, char*pstr )
{
	GUI_xy_set( x, y );
	GUI_string_display( pstr );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :在窗口x,y的位置显示字符串
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_string_window_display_At(
	hwin_t*pOBJ,
	uint16_t x,
	uint16_t y,
	char*pstr,
	uint16_t BGcor,
	uint16_t FGcor
)
{
	GUI_window_hwin_req( pOBJ );
	GUI_set_Bgcolor( BGcor );
	GUI_set_Fgcolor( FGcor );
	GUI_xy_set( x, y );
	GUI_string_display( pstr );
	GUI_window_hwin_release( pOBJ );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :在(x,y) 显示字符串,最大宽度为WIDE，如果超过就省略
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_string_wide_display_At( uint16_t x, uint16_t y, uint16_t wide, char*pstr )
{
	char*pstr_shadow;
	uint16_t xcoord;
	GUI_FONT*pFONT;
	pFONT = ( GUI_FONT* )GUI_GetFront();
	pstr_shadow = GUI_MALLOC( wide + 1 );
	if ( !pstr_shadow ) return;
	GUI_MEMCLR( pstr_shadow, ( wide + 1 ) );
	if ( wide >= GUI_STRLEN( pstr ) )
	{
		GUI_memcpy( pstr_shadow, pstr, wide );
	}
	else
	{
		GUI_memcpy( pstr_shadow, pstr, wide - 3 );
		GUI_strcat( pstr_shadow, "..." );
	}
	xcoord = x + MIDDLE_COORD_GET( wide * pFONT->CHwide, pstr_shadow, pFONT->CHwide );
	GUI_xy_set( xcoord, y );
	GUI_string_display( pstr_shadow );
	GUI_FREE( pstr_shadow );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :在当前位置显示浮点数
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_float_display( float data, Bsize_t bit )
{
	char temp[20] = "";
	GUI_FTOA( data, temp, bit );
	GUI_string_display( temp );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :在(x,y) 显示浮点数
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_float_display_At( uint16_t x, uint16_t y, float data, Bsize_t bit )
{
	GUI_xy_set( x, y );
	GUI_float_display( data, bit );
	return;
}

/********************************************************************
 * @创建人:揭成
 * @功能 :在当前位置显示整数
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_int_display( long data )
{
	char temp[20] = "";
	global_ltoa(( long )data, temp );
	GUI_string_display( temp );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :在(x,y) 显示整数
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_int_display_At( uint16_t x, uint16_t y, long data )
{
	GUI_xy_set( x, y );
	GUI_int_display( data );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :在窗口内画点
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void  GUI_window_draw_dot( uint16_t x, uint16_t y )
{
	GUI_xy_set( x, y );
	GUI_draw_dot( m_cur_hwin->csr_x_pos, m_cur_hwin->csr_y_pos );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :在窗口内画线
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_window_line_display( uint16_t x1, uint16_t y1,
							  uint16_t x2, uint16_t y2 )
{
	if ( !m_cur_hwin->type )
		return;
	x1 += m_cur_hwin->win_x;
	x2 += m_cur_hwin->win_x;
	y1 += m_cur_hwin->win_y;
	y2 += m_cur_hwin->win_y;
    if(x1==x2)GUI_draw_Vline(x1,y1,y2);
    else if(y1==y2)GUI_draw_Hline(x1,x2,y1);
	else  GUI_draw_line( x1, y1, x2, y2 );
	return;
}

void GUI_libASC_display_At( int x, int y, uint16_t wide, uint16_t high, char*plib )
{
	GUI_xy_set( x, y );
	GUI_libASC_display( m_cur_hwin->csr_x_pos, m_cur_hwin->csr_y_pos, wide, high, plib );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 绘制矢量线段，在窗口内
 *
 * @输入 :(x1,y1)起点坐标，angle 角度, len 线段长度 ,flags =0 不带箭头.
 *
 *@输出  :NONE
 *
********************************************************************/
#include"math.h"
static const GUI_POINT _aNeedle_1[] =
{
	{ -4, 0}, {0, -12},
	{4, 0}
};

void  GUI_window_angle_line_display(
	uint16_t x1, uint16_t y1,
	float angle, uint16_t len,
	uint8_t flags, char*title
)
{
	int x2 , y2, pos;
	GUI_POINT src[4], des[4];
	if ( !m_cur_hwin->type )return;
	x1 += m_cur_hwin->win_x;
	y1 += m_cur_hwin->win_y;
	for ( pos = 0;pos < SIZE_OF_ARRAY( _aNeedle_1 );pos++ )
	{
		src[pos].x = _aNeedle_1[pos].x;
		src[pos].y = _aNeedle_1[pos].y - len + 12;
	}
	GUI_polygon_rotate( des, src, SIZE_OF_ARRAY( _aNeedle_1 ), angle - 90 );
	x2 = x1 + des[2].x;
	y2 = y1 + des[2].y;
	if ( angle > 100 && angle <= 200 )
	{
		y2 -= 16;
	}
	else if ( angle > 200 && angle <= 300 )
	{
		y2 -= 10;
		x2 -= ( GUI_conText.CUR_Front->CHwide * 2 + 2 );
	}
	else if ( angle > 300 && angle <= 360 )
	{
		x2 -= ( GUI_conText.CUR_Front->CHwide * 2 + 2 );
	}
	GUI_print_string( x2, y2, LCD_WIDE, LCD_HIGH, title );
	GUI_polygon_fill( x1, y1, des, SIZE_OF_ARRAY( _aNeedle_1 ), GUI_conText.Fcolor );
	GUI_draw_line( des[1].x + x1, des[1].y + y1, x1, y1 );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 清除当前窗口内容
 *
 * @输入 :
 *
 *@输出  :NONE
 *
********************************************************************/
void GUI_clean_window( void )
{
	if ( !m_cur_hwin->type )
		return;
	if (( m_cur_hwin->flag&WIN_3D ) == WIN_3D )
	{
		if ( m_cur_hwin->type == WIDGET_TEXTBOX )
			GUI_fill_Rect_At(
				m_cur_hwin->win_x + 1,
				m_cur_hwin->win_y + 1,
				GET_XBORDER( m_cur_hwin ) - 1,
				GET_YBORDER( m_cur_hwin ) - 1 );
		else
			GUI_fill_Rect_At(
				m_cur_hwin->win_x + 2,
				m_cur_hwin->win_y + 2,
				GET_XBORDER( m_cur_hwin ) - 1,
				GET_YBORDER( m_cur_hwin ) - 1 );
	}
	else
	{
		GUI_fill_Rect_At(
			m_cur_hwin->win_x + 1,
			m_cur_hwin->win_y + 1,
			GET_XBORDER( m_cur_hwin ),
			GET_YBORDER( m_cur_hwin ) );
	}
	m_cur_hwin->win_Bcor = GUI_conText.Bcolor;
	return;
}
uint16_t GUI_GetwinFgcor( void )
{
	return m_cur_hwin->win_Fcor;
}

uint16_t GUI_GetwinBgcor( void )
{
	return m_cur_hwin->win_Bcor;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :清除当前窗口内容
 * @输入 :
 * @输出 :NONE
********************************************************************/
void GUI_SetWin_desktop( uint16_t Bgcorlor )
{
	GUI_set_Bgcolor( Bgcorlor );
	GUI_clean_window();
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 : 清除窗口内区域
 *
 * @输入 :
 *
 *@输出  :NONE
 *
********************************************************************/
void GUI_fill_window_area( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 )
{
	if ( !m_cur_hwin->type )
		return;
	x1 += m_cur_hwin->win_x;
	x2 += m_cur_hwin->win_x;
	y1 += m_cur_hwin->win_y;
	y2 += m_cur_hwin->win_y;

	if ( x1 == m_cur_hwin->win_x )
		x1 = m_cur_hwin->win_x - 1;
	if ( x2 >= GET_XBORDER( m_cur_hwin ) )
		x2 = GET_XBORDER( m_cur_hwin ) - 1;
	if ( y1 == m_cur_hwin->win_y )
		y1 = m_cur_hwin->win_y - 1;
	if ( y2 >= GET_YBORDER( m_cur_hwin ) )
		y2 = GET_YBORDER( m_cur_hwin );
	GUI_fill_Rect_At( x1, y1, x2, y2 );
	return;
}
#if GUI_TOUCH_EN >0
/********************************************************************
 * @创建人:揭成
 * @功能  :判断触摸点是否在顶层窗口里
 * @输入  :
 * @输出  :NONE
********************************************************************/
static BOOL  GUI_touch_dot_inTop( int x, int y )
{
	int xmax, xmin, ymax, ymin;
	hwin_t*pWIN;
	GUI_top_t*pM = m_pTop_list_head;
	if ( !pM )    return FAIL;
	while ( pM )
	{
		pWIN = pM->pWIN;
		xmin = pWIN->lcdxs - 1;
		xmax = pWIN->lcdxe + 1;
		ymin = pWIN->lcdys - 1;
		ymax = pWIN->lcdye + 1;
		if (( x > xmin && x < xmax ) && ( y > ymin && y < ymax ) )
		{
			return TURE;
		}
		pM = pM->pNext;
	}
	return FAIL;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :窗口触摸处理
 * @输入  :
 * @输出  :NONE
********************************************************************/
static  void GUI_widget_touch_handle( app_touch_t*data )
{
	hwin_t *ptemp = m_hwin_list_head;
	GUI_LOCK();
	while ( ptemp )
	{
		if ( ptemp->WIN_Paint )
		{
			ptemp->WIN_Paint = 0;
			ptemp->_paint( ptemp );
		}
		if ( !ptemp->_ontouch )
		{
			ptemp = ptemp->next_hwin;
			continue;
		}
		if ( ptemp->_ontouch( ptemp, data ) )
		{
			break;
		}
		ptemp = ptemp->next_hwin;
	}
	GUI_UNLOCK();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :顶层窗口触摸处理
 * @输入  :
 * @输出  :NONE
********************************************************************/
static  void GUI_Topwidget_touch_handle( app_touch_t*data )
{
	GUI_top_t*pTop  = m_pTop_list_head;
	hwin_t   *ptemp = m_hwin_list_head;
	hwin_t   *pfather;
	while ( ptemp )
	{
		pTop  = m_pTop_list_head;
		while ( pTop )
		{
			pfather = pTop->pWIN;
			if ( pfather == ptemp || GUI_window_is_son( ptemp, pfather ) )
			{
				break;
			}
			pTop = pTop->pNext;
		}
		if ( !pTop )
		{
			ptemp = ptemp->next_hwin;
			continue;
		}
		if ( !ptemp->_ontouch )
		{
			ptemp = ptemp->next_hwin;
			continue;
		}
		GUI_window_hwin_req( ptemp );
		if ( ptemp->WIN_Paint )
		{
			ptemp->_paint( ptemp );
		}
		if ( ptemp->_ontouch( ptemp, data ) )
		{
			GUI_window_hwin_release( ptemp );
			break;
		}
		GUI_window_hwin_release( ptemp );
		ptemp = ptemp->next_hwin;
	}
	return;
}
#endif
/********************************************************************
 * @创建人:揭成
 * @功能  :窗口重画
 * @输入  :
 * @输出  :NONE
********************************************************************/
static void WM_exc( void )
{
	hwin_t *ptemp = m_hwin_list_head;
	GUI_LOCK();
	while ( ptemp )
	{
		if ( ptemp->WIN_Paint )
		{
			ptemp->_paint( ptemp );
		}
		ptemp = ptemp->next_hwin;
	}
	GUI_UNLOCK();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能  :窗口重画,app接口，使用时机:
            当应用层需要锁住部分窗口，使得GUITASK 处在等待GUILOCK
 * @输入  :
 * @输出  :NONE
********************************************************************/
void GUI_WINrepaint( void )
{
	WM_exc();
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :GUI 核心任务，自实现功能跟外部无关
 * @输入 :
 *
 *@输出 : NONE
 *实现方式: 1，利用定时器轮寻，可以节约一个任务资源
                           2，开一个任务，
  目前采用第一种方案。
********************************************************************/
void  GUI_task( void*arg )
{
#if GUI_TOUCH_EN >0
	app_touch_t touch_data;
	Bsize_t size;
#endif
	while ( 1 )
	{
		msleep( 1 );
#if GUI_TOUCH_EN >0
		GUI_keypad_read();
		size = GUI_touch_read(( void* ) & touch_data, sizeof( app_touch_t ) );
		GUI_CSR_TMR_handle();
		if ( size == 0 )
		{
			WM_exc();
			continue;
		}
		GUI_handPen_check( &touch_data );
		if ( touch_data.state == 0 )
		{
			WM_exc();
			continue;
		}
		if ( GUI_touch_dot_inTop( touch_data.x, touch_data.y ) )
			GUI_Topwidget_touch_handle( &touch_data );
		else
			GUI_widget_touch_handle( &touch_data );
#if  GUI_ARROW_EN >0
		GUI_LOCK();
		GUI_Set_arrow_pos( touch_data.x, touch_data.y );
		GUI_UNLOCK();
#endif
#else
		WM_exc();
		GUI_CSR_TMR_handle();
#endif
	}
}
/********************************************************************
 * @创建人:揭成
 * @功能 :GUI 资源申请
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_arrow_init( void );
void GUI_listview_init( void );
void GUI_os_exit( void );
void GUI_Memdev_init( void );
void GUI_keypad_init( void );
void GUI_resource_req( void )
{
	m_win_depth 		= 0;
	m_cur_hwin 			= 0;
	m_hwin_list_end 	= PNULL;
	m_hwin_list_head 	= PNULL;
	m_pTop_list_end     = PNULL;
	m_pTop_list_head 	= PNULL;
	m_CURwidget         = 0;
	m_pMember           = 0;
	GLOBAL_MEMCLR(&GUI_conText,sizeof(GUI_const_t));
	GUI_mem_maneger_init();
	GUI_os_init();
#if GUI_LIBCONNECT_EN>0
	GUI_lib_init();              /*GUI 字库初始化*/
#endif
	GUI_lcd_resource_req();   	/*初始化LCD 模组*/
#if GUI_TOUCH_EN >0
	GUI_touch_resource_req();   /*初始化触摸屏*/
	GUI_keypad_init();
#endif
#if  GUI_CSR_EN  >0
	GUI_cursor_init();
#endif
#if  GUI_WIDGET_EDIT_EN  >0
	GUI_edit_init();
#endif
#if GUI_WIDGET_MSGBOX_EN >0
	Gui_MsgBox_init();
#endif
#if GUI_3D_WINDOW_EN>0
	GUI_3D_window_init();
#endif
#if   GUI_BUTTON_EN >0
	GUI_button_init();
#endif
#if GUI_DROP_DOWN_LIST_EN  >0
	Gui_drop_list_init();
#endif
#if   GUI_DIALOG_EN>0
	GUI_dialogBox_init();
#endif
#if   GUI_LISTVIEW_EN>0
	GUI_listview_init();
#endif
#if  GUI_ARROW_EN >0
	GUI_arrow_init();
#endif
	GUI_Memdev_init();
//	GUI_set_Bgcolor( 0x0 );
//	GUI_set_Fgcolor( 0xffff );
//	GUI_clean_screen();
	if ( !m_Gui_hwin )
		m_Gui_hwin = GUI_window_creat(
						 GUI_WIN_X, GUI_WIN_Y,
						 LCD_WIDE, LCD_HIGH, 0 );
	GUI_set_Fgcolor( 0xffff );
	GUI_set_Bgcolor( 0x0 );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :	GUI 资源释放
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void GUI_resource_release( void )
{
	GUI_lcd_resource_release();   		/*释放LCD 模组*/
#if GUI_TOUCH_EN >0
	GUI_touch_resource_release();
#endif
	GUI_os_exit();
	m_Gui_hwin = 0;
	return;
}
