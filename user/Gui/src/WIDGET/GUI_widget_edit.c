#include "../../Gui.h"
#include "../GUI_lcd_driver.h"
#if   GUI_WIDGET_EDIT_EN >0
uint8_t          edit_is_start 		 = FAIL;
GUI_csr_t*   	 m_edit_csr;
hwin_t*		     start_edit_win;
uint8_t          m_eidt_end_flag 	 = 0;

uint16_t  m_edit_member_Fgcor = C_GUI_WHITE;
uint16_t   m_edit_member_Bgcor = C_GUI_BlACK;
void GUI_widget_3D_DrawDownRect( uint16_t x, uint16_t y, Bsize_t wide, Bsize_t high );
BOOL GUI_edit_ontouch( hwin_t*hp, app_touch_t*data );
/********************************************************************
 * @创建人:揭成
 * @功能  :设置编辑成员颜色
 * @输入  :NONE
 * @输出  :成员位置
********************************************************************/
void GUI_edit_member_set_color( uint16_t Fgcor, uint16_t Bgcor )
{
	m_edit_member_Fgcor  = Fgcor;
	m_edit_member_Bgcor  = Bgcor;
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能: 
 *
 * @输入:NONE

 * @输出:成员位置
********************************************************************/
subord_mem_t *GUI_edit_mem_req( void )
{
	subord_mem_t *pret;
	pret = GUI_malloc( sizeof( subord_mem_t ) );
	pret->edit_shadow = 0;
	return pret;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:释放所以成员
 *
 * @输入:编辑框句柄

 * @输出:NONE
********************************************************************/
void GUI_edit_allmember_release( hwin_t *hp )
{
	subord_mem_t*pedit;
	while ( hp->subord_head )
	{
		pedit = ( hp->subord_head )->next_edit;
		if(hp->subord_head->edit_shadow)
		{
		    GUI_free( hp->subord_head->edit_shadow );	
		}
		GUI_free( hp->subord_head );
		hp->subord_head = pedit;
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 创建一个编辑框控件
 * @输入:(xcoord    ycoord)起始坐标
 * @输出:控件句柄
********************************************************************/
hwin_t* GUI_edit_creat( uint16_t xcoord,
						uint16_t ycoord,
						Bsize_t wide,
						Bsize_t high,
						Bsize_t flags,
						hwin_t* fatherhp )
{
	hwin_t *hp;
	hp = GUI_window_child_creat(
			 xcoord,
			 ycoord,
			 wide,
			 high,
			 flags,
			 fatherhp );
	if ( !hp ) return PNULL;
	hp->type = WIDGET_EDIT;
#if GUI_TOUCH_EN>0
	hp->_ontouch = GUI_edit_ontouch;
#endif
	return hp;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:设置编辑框成员
 *
 * @输入:

 * @输出:
********************************************************************/
#define EDIT_3D_GAP  3
void GUI_edit_memeber_add(
	uint16_t x,
	uint16_t y,
	Bsize_t lenth,
	char*title,
	Bsize_t flags
)
{
	subord_mem_t*pedit;
	Bsize_t  tflag;
	Bsize_t  flag_shadow;
	if ( m_cur_hwin->type != WIDGET_EDIT ) return;
	pedit = GUI_edit_mem_req();
	if ( !pedit ) return;
	pedit->CHhigh = m_GUI_CHhigh;
	pedit->CHwide = m_GUI_CHwide;
	pedit->pfront = GUI_GetFront();
	flag_shadow = flags & MEMBER_3D;
	if ( flag_shadow == MEMBER_3D )
	{
		x += EDIT_3D_GAP;
		y += 2;
	}
	GUI_xy_set( x, y );
	pedit->UNflag    	 = 0xff;
	pedit->id 			 = flags & 0xff;
	pedit->y 			 = y;
	pedit->flag          = flags;
	pedit->eidt_lenth    = lenth;
	pedit->surplus_lenth = pedit->eidt_lenth;
	pedit->next_edit   	 = PNULL;
	if((pedit->flag&EDIT_DIS )==0){
	pedit->edit_shadow 	 = GUI_malloc(sizeof(char)*(lenth+1));
	GUI_MEMCLR(pedit->edit_shadow,sizeof(char)*(lenth+1));}
	tflag = ( flags & EDIT_ADD_END );
	if ( tflag != EDIT_ADD_END )
	{
		GUI_string_display( title );
		flag_shadow = flags & MEMBER_3D;
		if ( flag_shadow == MEMBER_3D )
		{
			pedit->x = x + global_strlen( title ) * pedit->CHwide + EDIT_3D_GAP;
		}
		else
		{
			pedit->x = x + global_strlen( title ) * pedit->CHwide;
		}
	}
	else
	{
		pedit->x = x;
		flag_shadow = flags & MEMBER_3D;
		if ( flag_shadow == MEMBER_3D )
		{
			GUI_xy_set( x + ( pedit->eidt_lenth )*pedit->CHwide + EDIT_3D_GAP, y );
		}
		else
		{
			GUI_xy_set( x + ( pedit->eidt_lenth )*pedit->CHwide, y );
		}
		GUI_string_display( title );
	}
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
	flag_shadow = flags & MEMBER_3D;
	if ( flag_shadow == MEMBER_3D )
	{
		GUI_window_hwin_req( m_cur_hwin );
		GUI_set_Bgcolor( m_edit_member_Bgcor );
		GUI_set_Fgcolor( m_edit_member_Fgcor );
		GUI_fill_window_area(
			pedit->x, ( pedit->y ),
			pedit->x + ( pedit->eidt_lenth*pedit->CHwide ) + 2,
			pedit->y + pedit->CHhigh + 1 );

		GUI_widget_3D_DrawDownRect( m_cur_hwin->win_x + pedit->x - 1,
									m_cur_hwin->win_y + pedit->y - 1,
									pedit->eidt_lenth*pedit->CHwide + 3,
									pedit->CHhigh + 2 );

		GUI_window_hwin_release( m_cur_hwin );
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:选择指定成员
 *
 * @输入:

 * @输出:
********************************************************************/
BOOL  GUI_edit_select( Bsize_t id )
{
	subord_mem_t*plist;
	plist = m_cur_hwin->subord_head;
	while ( 1 )
	{
		if ( plist->id == id ) break;
		plist = plist->next_edit;
		if ( plist == NULL )
		{
			return FAIL ;
		}
	}
	plist->surplus_lenth = plist->eidt_lenth;
	m_cur_hwin->subord_cur = plist;
	GUI_xy_set( plist->x, plist->y );
	return TURE;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:移动当前成员
 *
 * @输入:

 * @输出:
********************************************************************/
BOOL GUI_edit_shift( Bsize_t flag )
{
	subord_mem_t*plist;
	plist = m_cur_hwin->subord_cur;
	if ( flag == EDIT_NEXT )
	{
	    while(1)
	    {
			if ( !plist->next_edit )
			{
				return FAIL;
			}
			plist = plist->next_edit;
			if (( plist->flag&EDIT_DIS ) == 0 )break;
	    }
	}
	else
	{
	   while(1){
		if ( !plist->last_edit)
		{
			return FAIL;
		}
		plist = plist->last_edit;
		if (( plist->flag&EDIT_DIS ) == 0 )break;}
	}
	plist->surplus_lenth = plist->eidt_lenth;
	m_cur_hwin->subord_cur = plist;
	GUI_xy_set( plist->x, plist->y );
	m_eidt_end_flag = FAIL;
	return TURE;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:获取当前光标偏移，相对与创建总长而言
 *
 * @输入:

 * @输出:
********************************************************************/
Bsize_t GUI_edit_Boffset_obtain( void )
{
	Bsize_t offset;
	if ( !start_edit_win )
		return 0;
	offset = start_edit_win->subord_cur->eidt_lenth - start_edit_win->subord_cur->surplus_lenth;
	return offset;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:获取当前编辑成员ID
 *
 * @输入:

 * @输出:
********************************************************************/
Bsize_t GUI_edit_id_obtain( void )
{
	Bsize_t id;
	if ( !m_cur_hwin->subord_cur )
		return 0x1000;
	id = m_cur_hwin->subord_cur->id;
	return id;
}
/********************************************************************
 *@创建人: 揭成
 *@功  能: 清除当前编辑内容
 *@输  入:
 *@输  出:
********************************************************************/
void GUI_edit_clean( void )
{
	subord_mem_t*plist = m_cur_hwin->subord_cur;
	if ( (plist->flag& MEMBER_3D) == MEMBER_3D )
		GUI_set_Bgcolor( m_edit_member_Bgcor );
	GUI_fill_window_area( plist->x, plist->y + 1,
						  plist->eidt_lenth*plist->CHwide + plist->x + 2,
						  plist->y + plist->CHhigh + 1 );
	if(plist->edit_shadow)
		GUI_MEMCLR(plist->edit_shadow, plist->eidt_lenth);
	GUI_edit_select( m_cur_hwin->subord_cur->id );
	return;
}
/********************************************************************
 *@创建人: 揭成
 *@功  能: 获取编辑框内容
 *@输  入:
 *@输  出:
********************************************************************/
void GUI_edit_content_obtain(char*str,Bsize_t id,hwin_t*edit)
{
	subord_mem_t*plist;
	GUI_LOCK();
	plist = edit->subord_head;
	while ( 1 )
	{
		if ( plist->id == id ) break;
		plist = plist->next_edit;
		if ( plist == NULL )
		{
			GUI_UNLOCK();
			return ;
		}
	}
	if(plist->edit_shadow);
    	GUI_strcpy(str, plist->edit_shadow);
	GUI_UNLOCK();
    return;
}

/*输入法控制 2013 0718*/
void GUI_edit_SetUNflag(hwin_t*edit,Bsize_t id,uint8_t flag)
{
	subord_mem_t*plist;
    GUI_LOCK();
	plist = edit->subord_head;
	while ( 1 )
	{
		if ( plist->id == id ) break;
		plist = plist->next_edit;
		if ( plist == NULL )
		{
			GUI_UNLOCK();
			return ;
		}
	}
	plist->UNflag = flag;
	GUI_UNLOCK();
    return;
}

uint16_t GUI_edit_GetUNflag(hwin_t*edit,Bsize_t id)
{
	subord_mem_t*plist;
    GUI_LOCK();
	plist = edit->subord_head;
	while ( 1 )
	{
		if ( plist->id == id ) break;
		plist = plist->next_edit;
		if ( plist == NULL )
		{
			GUI_UNLOCK();
			return 0xff;
		}
	}
	GUI_UNLOCK();
    return plist->UNflag;
}

static void __edit_shadow(char*str)
{
	int offset;
	int dx;
	int chWIDE;
	subord_mem_t*plist;
	plist  = m_cur_hwin->subord_cur;
	if(!plist->edit_shadow)return;
	dx     = (m_cur_hwin->csr_x_pos - m_cur_hwin->win_x-plist->x-1);
	chWIDE = GUI_conText.CUR_Front->CHwide;
	offset = dx/chWIDE;
    GUI_strcpy(&plist->edit_shadow[offset],str);
    return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:对当前编辑成员显示
 *
 * @输入:

 * @输出:
********************************************************************/
void GUI_edit_string_display( char*pstr )
{
	subord_mem_t*plist;
	char*pch;
	const GUI_FONT*pfront;
	pfront = GUI_GetFront();
	pch = GUI_MALLOC(60);
	if ( !pch ) return;
	GUI_strcpy( pch, pstr );
	plist = m_cur_hwin->subord_cur;
	if ( (plist->flag& MEMBER_3D) == MEMBER_3D )
		GUI_set_Bgcolor( m_edit_member_Bgcor );
	if ( plist->surplus_lenth < GUI_strlen( pstr ) )
	{
		pch[plist->surplus_lenth-1] = ASCII_NULL;
	}
	GUI_SetFront( plist->pfront );
	plist->surplus_lenth -= GUI_strlen( pch );
	if (edit_is_start)
	{
	    __edit_shadow(pch);
		GUI_string_display( pch );
		if ( plist->surplus_lenth == 0 )
		{
			if ( m_cur_hwin == start_edit_win )
			{
				if ( m_eidt_end_flag == FAIL ) m_eidt_end_flag = TURE;
				GUI_xy_set( plist->x + ( plist->eidt_lenth - 1 )*plist->CHwide, plist->y );
				plist->surplus_lenth = 1;
			}
		}
	}
	else
	{
	    __edit_shadow(pch);
		GUI_string_display( pch );
	}
	app_free( pch );
	GUI_SetFront( pfront );
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:对当前编辑成员进行显示，并清空原来内容
 *
 * @输入:

 * @输出:
********************************************************************/
void GUI_edit_string_display_At( char*pstr, Bsize_t id )
{
	if ( GUI_edit_select( id ) )
	{
		GUI_edit_clean();
		GUI_edit_string_display( pstr );
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:在当前编辑框内显示整数
 * @输入:
 * @输出:
********************************************************************/
void GUI_edit_int_display_At( long data, Bsize_t id )
{
	char pstr[20] = "";
	GUI_edit_select( id );
	GUI_edit_clean();
	GUI_ltoa( data, pstr );
	GUI_edit_string_display( pstr );
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:在当前编辑成员中显示浮点数
 *
 * @输入:

 * @输出:
********************************************************************/
void GUI_edit_float_display_at( float fdata, Bsize_t id, Bsize_t ch_len )
{
	char pst[20] = "";
	char str[10] = "";
	Bsize_t len;
	GUI_edit_select( id );
	GUI_edit_clean();
	if ( GUI_ABS( fdata ) > 0.00001 && GUI_ABS( fdata ) < 5000000.0 )
	{
		GUI_ftoa( fdata, pst,6);
		len = GUI_strlen( pst );
		if ( len > ch_len )
		{
			pst[ch_len] = ASCII_NULL;
		    if(pst[ch_len]>'5')pst[ch_len-1]+=1;//四舍五入
		}
		if ( pst[ch_len-1] == '.' )
		{
			pst[ch_len-1] = ASCII_NULL;
		}
		else if ( len < ch_len )
		{
			if ( GUI_string_locate( pst, "." ) )
			{
				GUI_memset( str, ( uint8_t )'0', ( ch_len - len ) );
				GUI_strcat( pst, str );
			}
			else
			{
				GUI_memset( str, ( uint8_t )'.', 1 );
				GUI_memset( str + 1, ( uint8_t )'0', ( ch_len - len - 1 ) );
				GUI_strcat( pst, str );
			}
		}
		GUI_edit_string_display( pst );
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:编辑框内光标移动，FLAG  0  前移 1 后移
 *
 * @输入:

 * @输出:
********************************************************************/
#if  0
void GUI_edit_csr_shift( Bsize_t flag )
{ 
	subord_mem_t    *plist;
    Bsize_t         pos = 0 ;
    Bsize_t         size;
    Bsize_t         Used_lenth;
    char            *pshadow;

    plist       = start_edit_win->subord_cur;
	Used_lenth  = plist->eidt_lenth - plist->surplus_lenth;
	pshadow     = plist->edit_shadow;
	if(flag==CSR_RIGHT_SHIFT && plist->surplus_lenth)
	{
	    size=key_method_input_char_handle(pshadow,Used_lenth,&pos);
		if (plist->surplus_lenth>=1 && size==1)
		{
			GUI_x_shift( plist->CHwide*size );
			plist->surplus_lenth -= size;
		}
		if(plist->surplus_lenth>=2 && size==2)
		{
            GUI_x_shift( plist->CHwide*size );
			plist->surplus_lenth -= size;
		}
	}
	else  if(flag==CSR_LEFT_SHIFT && Used_lenth)
	{
        size=key_method_input_char_handle(pshadow,Used_lenth-1,&pos);
        if (Used_lenth>=1 && size==1)
		{
			start_edit_win->csr_x_pos -= plist->CHwide*size;
			start_edit_win->subord_cur->surplus_lenth += size;
            m_eidt_end_flag = FAIL;
		}
        if(Used_lenth>=2 && size==2)
	    {
	        start_edit_win->csr_x_pos -= plist->CHwide*size;
			start_edit_win->subord_cur->surplus_lenth += size;
            m_eidt_end_flag = FAIL;
	    } 
	}
	return;
}
#else
void GUI_edit_csr_shift( Bsize_t flag )
{
	subord_mem_t*plist;
	plist = start_edit_win->subord_cur;
	if ( flag == CSR_RIGHT_SHIFT )
	{
		if ( plist->surplus_lenth > 1 )
		{
			GUI_x_shift( plist->CHwide );
			plist->surplus_lenth--;
		}
	}
	else  if ( flag == CSR_LEFT_SHIFT )
	{
		if ( plist->surplus_lenth < plist->eidt_lenth )
		{
			start_edit_win->csr_x_pos -= plist->CHwide;
			start_edit_win->subord_cur->surplus_lenth++;
			m_eidt_end_flag = FAIL;
		}
	}
	return;
}
#endif
/********************************************************************
 * @创建人: 揭成
 * @功能:启动光标
 * @输入: NONE
 * @输出: NONE
********************************************************************/
static void GUI_edit_csr_start( void )
{
	m_edit_csr = GUI_cursor_start( m_cur_hwin,1,
								   m_cur_hwin->subord_cur->CHhigh-2);
	return;
}
/********************************************************************
 * @创建人	:揭成
 * @功能	:停止光标
 * @输入	:NONE
 * @输出	:NONE
********************************************************************/
static void GUI_edit_csr_stop( void )
{
	GUI_cursor_stop( m_edit_csr );
	return;
}
static hwin_t*  	    _start_editobj     = 0;
static subord_mem_t*    _start_edit_member = 0;
static int              _edit_touch_times       = 0;
static int              _edit_touch_jiffies     = 0;

/********************************************************************
 * @创建人: 揭成
 * @功能:开始编辑文字内容
 *
 * @输入:

 * @输出:
********************************************************************/
uint8_t GUI_edit_start( hwin_t*hp, unsigned char( *callback )( hwin_t*hp ) )
{
	uint8_t exit_value;
	subord_mem_t*plist;
	void*p1, *p2;
	p1 = ( void* )hp->subord_head->pfront;
	p2 = ( void* ) & GUI_HZK24_EXTASCII;
	if ( edit_is_start ) return 0XFF;
	if ( p1 != p2 )
	{
		p2 = ( void* ) & GUI_HZK16_EXTASCII;
		if ( p1 != p2 )return 0XFF;
	}
	if (( _start_editobj == hp ) && _start_edit_member )
		plist = _start_edit_member;
	else
	{
		plist = hp->subord_head;
	}
	GUI_LOCK();
	GUI_edit_start_hook();
	GUI_UNLOCK();
	GUI_window_hwin_req( hp );
	plist->surplus_lenth = plist->eidt_lenth;
	hp->subord_cur = plist;
	GUI_xy_set( plist->x, plist->y );
	GUI_edit_csr_start();
	start_edit_win = hp;
	edit_is_start = TURE;
	GUI_window_hwin_release( hp );
	if(callback){exit_value = callback( hp );}
	else{exit_value=GUI_edit_setting(hp);}
	GUI_LOCK();
	GUI_edit_end_hook();
	GUI_UNLOCK();
	GUI_edit_csr_stop();
	edit_is_start           = FAIL;
	m_eidt_end_flag         = FAIL;
	_start_editobj     		= 0;
	_start_edit_member 		= 0;
	return exit_value;
}
#if GUI_TOUCH_EN >0
hwin_t* Gui_start_editOBJ_obtain( void )
{
	return _start_editobj;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:
 * @输入:
 * @输出:
********************************************************************/
static void Gui_editstart_touch_handle( subord_mem_t*pedit, app_touch_t*data, hwin_t*hp )
{
    uint8_t key;
	GUI_wait_touch_release();
	_start_editobj        = hp;
	_start_edit_member    = pedit;
	key =  hp->flag&0xff;
	if(key)
		GUI_key_dummy_press( hp, key);
	else
		GUI_key_dummy_press( hp, GUI_KEY_EDIT_START);
/*
    if(++_edit_touch_times==2)
    { 
        if((GUI_os_jiffies_obtain()-_edit_touch_jiffies)<50)
        {
	        _edit_touch_times     = 0;
	        _edit_touch_jiffies   = GUI_os_jiffies_obtain();
			_start_editobj        = hp;
			_start_edit_member    = pedit;
			key =  hp->flag&0xff;
			if(key)
				GUI_key_dummy_press( hp, key);
			else
				GUI_key_dummy_press( hp, GUI_KEY_EDIT_START);
        }
		else
		{
		   _edit_touch_times   = 1;
	       _edit_touch_jiffies = GUI_os_jiffies_obtain();
		}
    }
	else
	{
		   _edit_touch_times   = 1;
	       _edit_touch_jiffies = GUI_os_jiffies_obtain();
	}*/
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:
 * @输入:
 * @输出:
********************************************************************/
static void Gui_editdo_touch_handle( subord_mem_t*pedit, app_touch_t*data, hwin_t*hp )
{
	int offset;
	offset = (( data->x - ( hp->win_x + pedit->x ) ) / pedit->CHwide );
	GUI_window_hwin_req( hp );
	if ( hp->subord_cur != pedit )
	{
		GUI_edit_select( pedit->id );
		GUI_wait_touch_release();
		GUI_key_dummy_press( hp, GUI_KEY_EDIT_MEMEBER_SW );
	}
	else
	{
		if ( offset < pedit->eidt_lenth )
		{
			GUI_edit_select( pedit->id );
			GUI_x_shift( offset*pedit->CHwide );
			pedit->surplus_lenth -= offset;
			GUI_wait_touch_release();
		}
	}
	GUI_window_hwin_release( hp );
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:编辑框正在编辑状态坐标匹配
 * @输入:
 * @输出:
********************************************************************/
static BOOL Gui_edit_match( subord_mem_t*pedit, app_touch_t*data, hwin_t*hp )
{
	int xmax, xmin, ymax, ymin;
	xmax = hp->win_x + pedit->x + pedit->eidt_lenth * pedit->CHwide;
	xmin = hp->win_x + pedit->x ;
	ymax = hp->win_y + pedit->y + HZ_HIGH + 3;
	ymin = hp->win_y + pedit->y - 3;
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
 * @创建人:揭成
 * @功能:编辑触摸处理
 * @输入:
 * @输出:
********************************************************************/
BOOL GUI_edit_ontouch( hwin_t*hp, app_touch_t*data )
{
	int xmax, xmin, ymax, ymin;
	subord_mem_t*plist;
	plist = hp->subord_head;
	if (( hp->flag&WIN_TOUCH_EN ) == 0 )
		return FAIL;
	if ( edit_is_start == FAIL )
	{
		xmax = hp->win_x + hp->win_wide;
		xmin = hp->win_x  ;
		ymax = hp->win_y + hp->win_high;
		ymin = hp->win_y;
		( xmax > LCD_WIDE ) ? ( xmax = LCD_WIDE ) : ( xmax = xmax );
		( xmin < 0 ) ? ( xmin = 0 ) : ( xmin = xmin );
		( ymax > LCD_HIGH ) ? ( ymax = LCD_HIGH ) : ( ymax = ymax );
		( ymin < 0 ) ? ( ymin = 0 ) : ( ymin = ymin );
		if ( data->x < xmin ) return FAIL;
		if ( data->x > xmax ) return FAIL;
		if ( data->y < ymin ) return FAIL;
		if ( data->y > ymax ) return FAIL;
		plist = hp->subord_head;
		while ( 1 )
		{
			if ( plist == NULL )
				return FAIL;
			if (( plist->flag&EDIT_DIS ) == 0 )
			{
				if ( Gui_edit_match( plist, data, hp ) )
				{
					Gui_editstart_touch_handle( plist, data, hp );
					return TURE;
				}
			}
			plist = plist->next_edit;
		}
	}
	else
	{
		if ( start_edit_win != hp )
			return FAIL;
		plist = hp->subord_head;
		while ( 1 )
		{
			if ( plist == NULL )
				return FAIL;
			if (( plist->flag&EDIT_DIS ) == 0 )
			{
				if ( Gui_edit_match( plist, data, hp ) )
				{
					Gui_editdo_touch_handle( plist, data, hp );
					return TURE;
				}
			}
			plist = plist->next_edit;
		}
	}
}
#endif
/********************************************************************
 * @创建人: 揭成
 * @功能:编辑框控件初始化
 *
 * @输入:

 * @输出:
********************************************************************/
void GUI_edit_init( void )
{
	edit_is_start      		= FAIL;
	m_eidt_end_flag    		= FAIL;
	m_edit_member_Fgcor 	= EDIT_MEMBER_FRAME_COLOR;
	m_edit_member_Bgcor 	= EDIT_MEMBER_BG_COLOR;
	_edit_touch_times       = 0;
	_edit_touch_jiffies     = 0;
	_start_editobj 			= 0;
	_start_edit_member 		= 0;
	return;
}
#endif
