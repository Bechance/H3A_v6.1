/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GUI光标
* 创建时间:2010-10-18
* 所属公司 :科荟
* 文件名称:GUI_cursor.c
* 创建人 :揭成
* 功能描述:GUI 光标管理
*----------------------------------------------------------------
修改记录:
修改日期:
修改人:
********************************************************************/
#include "../Gui.h"
#if  GUI_CSR_EN  >0
#define      MAX_CURSOR_NUM  							5
static 		 GUI_csr_t  m_csr_tcb[MAX_CURSOR_NUM] 		= {0, };
/********************************************************************
 * @创建人 :揭成
 * @功能 :空闲光标获取
 *
 * @输入 :
 *
 *@输出 :NONE
***********************************************************/
GUI_csr_t* GUI_free_cursor_obtain( void )
{
	Bsize_t offset;
	for ( offset = 0;offset < MAX_CURSOR_NUM;offset++ )
	{
		if ( m_csr_tcb[offset].occupy_flag != BUSY )
		{
			m_csr_tcb[offset].occupy_flag = BUSY;
			return &( m_csr_tcb[offset] );
		}
	}
	return PNULL;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :光标句柄获取
 * @输入 :
 * @输出 :NONE
***********************************************************/
GUI_csr_t* GUI_WIN_cursor_obtain( hwin_t*pOBJ )
{
	int offset;
	GUI_csr_t* p;
	for ( offset = 0;offset < MAX_CURSOR_NUM;offset++ )
	{
		p = &m_csr_tcb[offset];
		if ( p->occupy_flag == BUSY )
		{
			if ( p->hOBJ == pOBJ )
			{
				return p;
			}
		}
	}
	return 0;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:光标位置检测
 * @输入:
 * @输出:
********************************************************************/
static void GUI_csr_check( GUI_csr_t*pCSR )
{
	hwin_t*pOBJ;
	pOBJ = pCSR->hOBJ;
	if (( pCSR->csrx_pos != pOBJ->csr_x_pos )
		|| ( pCSR->csry_pos != pOBJ->csr_y_pos ) )
	{
		pCSR->csrx_pos = pOBJ->csr_x_pos;
		pCSR->csry_pos = pOBJ->csr_y_pos;
		GUI_cursor_shift( pCSR, pOBJ->csr_x_pos, pOBJ->csr_y_pos );
		GUI_cursor_reload( pCSR );
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:   编辑框内光标检测
 * @输入:
 * @输出:
********************************************************************/
void GUI_CSR_TMR_handle( void )
{
	Bsize_t offset;
	uint32_t dx;
	uint32_t CURjiff;
	GUI_csr_t* p;
	CURjiff  = GUI_os_jiffies_obtain();
	for ( offset = 0;offset < MAX_CURSOR_NUM;offset++ )
	{
		p = &m_csr_tcb[offset];
		if ( p->occupy_flag == BUSY )
		{
			GUI_csr_check( p );
			dx = CURjiff - p->TMR.last_jiff;
			if ( dx >= p->TMR.expires )
			{
				p->TMR.last_jiff = CURjiff;
				p->TMR.function( p->TMR.p_arg );
			}
		}
	}
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :光标定时回调函数
 *
 * @输入 :
 *
 *@输出 :NONE
***********************************************************/
static void GUI_callbalck( void*arg )
{
	GUI_csr_t * pcsr = ( GUI_csr_t* )arg;
	GUI_window_hwin_req( pcsr->hOBJ );
	if ( pcsr->flicker_flag == 1 )
	{
		GUI_write_block( pcsr->x, pcsr->y,
						 pcsr->wide + pcsr->x,
						 pcsr->high + pcsr->y,
						 pcsr->csr_shadow, 1 );
		pcsr->flicker_flag = 0;
	}
	else
	{
		GUI_write_block( pcsr->x, pcsr->y,
						 pcsr->wide + pcsr->x,
						 pcsr->high + pcsr->y,
						 pcsr->csr_shadow, 0 );
		pcsr->flicker_flag = 1;
	}
	GUI_window_hwin_release( pcsr->hOBJ );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :光标移动
 *
 * @输入 :光标的位置
 *
 *@输出 :NONE
***********************************************************/
void GUI_cursor_shift( GUI_csr_t * pcsr, int x, int y )
{
	GUI_window_hwin_req( pcsr->hOBJ );
	GUI_write_block( pcsr->x, pcsr->y,
					 pcsr->wide + pcsr->x,
					 pcsr->high + pcsr->y,
					 pcsr->csr_shadow, 0 );
	pcsr->flicker_flag = 1;
	pcsr->x	= x;
	pcsr->y 	= y;
	GUI_read_block( pcsr->x, pcsr->y,
					pcsr->wide + pcsr->x,
					pcsr->high + pcsr->y,
					pcsr->csr_shadow );
	GUI_window_hwin_release( pcsr->hOBJ );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :重新导入光标的数据
 *
 * @输入 :NONE
 *
 *@输出 :NONE
***********************************************************/
void GUI_cursor_reload( GUI_csr_t * pcsr )
{
	GUI_window_hwin_req( pcsr->hOBJ );
	GUI_read_block( pcsr->x, pcsr->y,
					pcsr->wide + pcsr->x,
					pcsr->high + pcsr->y,
					pcsr->csr_shadow );
	pcsr->TMR.last_jiff = 0;
	GUI_window_hwin_release( pcsr->hOBJ );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :获取光标控制块状态
 *
 * @输入 NONE
 *
 *@输出 :BUSY OR IDLE
***********************************************************/
BOOL GUI_cursor_state_obtain( GUI_csr_t * pcsr )
{
	return pcsr->occupy_flag;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :光标开始运行。创建一个定时成员
 *
 * @输入 :光标的位置
 *
 *@输出 :NONE
***********************************************************/
GUI_csr_t* GUI_cursor_start( hwin_t*hOBJ, int wide, int high )
{
	GUI_csr_t * pcsr =  GUI_free_cursor_obtain();
	GUI_window_hwin_req( hOBJ );
	if ( pcsr != PNULL )
	{
		pcsr->x    = hOBJ->csr_x_pos;
		pcsr->y    = hOBJ->csr_y_pos;
		pcsr->wide = wide;
		pcsr->high = high;
		pcsr->occupy_flag = BUSY;
		pcsr->flicker_flag = 0;
		pcsr->hOBJ = hOBJ;
		pcsr->csr_shadow = GUI_MALLOC( sizeof(uint16_t)*high*wide*2);
		if ( !pcsr->csr_shadow )
		{
			GUI_window_hwin_release( hOBJ );
			return 0;
		}
		GUI_memset(( uint8_t * ) pcsr->csr_shadow,
				   0,
				   sizeof(uint16_t)*high*wide*2 );

		GUI_read_block( pcsr->x, pcsr->y,
						pcsr->wide + pcsr->x,
						pcsr->high + pcsr->y,
						pcsr->csr_shadow );
		pcsr->TMR.function = GUI_callbalck;
		pcsr->TMR.p_arg    = pcsr;
		pcsr->TMR.expires  = 50;
		GUI_write_block( pcsr->x, pcsr->y,
						 pcsr->wide + pcsr->x,
						 pcsr->high + pcsr->y,
						 pcsr->csr_shadow, 1 );
	}
	GUI_window_hwin_release( hOBJ );
	return pcsr;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :停止光标显示
 *
 * @输入 :NONE
 *
 *@输出  :NONE
***********************************************************/
void GUI_cursor_stop( GUI_csr_t * pcsr )
{
	if ( !pcsr->csr_shadow )	return;
	if ( pcsr->occupy_flag == IDLE ) return;
	GUI_LOCK();
	GUI_write_block( pcsr->x, pcsr->y,
					 pcsr->wide + pcsr->x,
					 pcsr->high + pcsr->y,
					 pcsr->csr_shadow, 0 );
	pcsr->occupy_flag = IDLE;
	GUI_UNLOCK();
	GUI_FREE(( void* )pcsr->csr_shadow );
	GUI_MEMCLR( pcsr, sizeof( GUI_csr_t ) );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :光标管理初始化
 *
 * @输入 :NONE
 *
 *@输出  :NONE
***********************************************************/
void GUI_cursor_init( void )
{
	GUI_MEMCLR( m_csr_tcb, sizeof( GUI_csr_t )*MAX_CURSOR_NUM );
	return;
}
#endif
