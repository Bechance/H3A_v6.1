#include "../../Gui.h"
#include "../GUI_lcd_driver.h"
#if GUI_PROGRESS_BAR_EN  >0
typedef struct
{
	uint16_t   progress_cur_pos;
	uint16_t   progress_max_pos;
	uint16_t   progress_setp_size;
	uint16_t   progress_frame_color;
	uint16_t   progress_BARcolor;
	uint16_t   progress_TXTcolor;
}widget_progress_t;


static widget_progress_t*GUI_widget_obtain( hwin_t *hp )
{
	return ( widget_progress_t* )hp->pwidget;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :�����������ؼ�
 *
 * @���� :(xcoord,ycoord)���� setp_wide���ȵ�λ���ȣ�HIGH��
 *                 �������߶ȣ�setp_num ���ȵ�λ������fatherhp
 *@���  :NONE
***********************************************************/
hwin_t* Gui_progress_bar_creat( uint16_t xcoord,
								uint16_t ycoord,
								Bsize_t wide,
								Bsize_t high,
								Bsize_t setp_num,
								hwin_t* fatherhp )
{
	hwin_t *hp;
	widget_progress_t*pwidget;
	Bsize_t wide_shadow = wide;

	wide_shadow = wide_shadow;
	GUI_window_hwin_req( GET_GUIWIN );
	GUI_set_Bgcolor( PROGRESS_BAR_BG_COLR );
	GUI_set_Fgcolor( PROGRESS_BAR_FG_COLR );

	hp = GUI_window_child_creat(
			 xcoord,
			 ycoord,
			 wide,
			 high,
			 WIN_3D | WIN_FILL,
			 fatherhp );

	hp->type = WIDGET_PROGRESS_BAR;
	hp->pwidget = GUI_malloc( sizeof( widget_progress_t ) );
	pwidget = GUI_widget_obtain( hp );
	pwidget->progress_max_pos = setp_num;
	pwidget->progress_cur_pos = 0;
	pwidget->progress_setp_size = wide / setp_num;
	GUI_window_hwin_release( GET_GUIWIN );
	return hp;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :�޸Ľ�����������Ŀ
 *
 * @���� :
 *@���  :NONE
***********************************************************/
void  Gui_progress_mod_setp( hwin_t*hp, uint16_t setp_size )
{
	widget_progress_t*pwidget;
	pwidget = GUI_widget_obtain( hp );
	pwidget->progress_max_pos = setp_size;
	pwidget->progress_setp_size = hp->win_wide / setp_size;
	if ( pwidget->progress_setp_size == 0 )pwidget->progress_setp_size = 1;
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :������������ʾ
 *
 * @���� :NONE
 *@���  :NONE
***********************************************************/
void Gui_progress_bar_advance( hwin_t*hp, uint16_t setp )
{
	uint16_t x_start, x_end;
	widget_progress_t*pwidget;
	pwidget = GUI_widget_obtain( hp );
	GUI_window_hwin_req( hp );
	GUI_set_Bgcolor( PROGRESS_BAR_STEP_COLR );
	GUI_set_Fgcolor( C_GUI_RED );
	x_start = pwidget->progress_cur_pos * pwidget->progress_setp_size + 1;
	if ( x_start < ( hp->win_wide - 2 ) )
	{
		if (( pwidget->progress_cur_pos + setp ) < pwidget->progress_max_pos )
		{
			x_end = x_start + setp * pwidget->progress_setp_size;
		}
		else
		{
			setp = pwidget->progress_max_pos - pwidget->progress_cur_pos;
			x_end = hp->win_x + hp->win_wide;
		}
		GUI_fill_window_area( x_start, 2, x_end, hp->win_high - 2 );
		pwidget->progress_cur_pos += setp;
	}
	GUI_window_hwin_release( hp );
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :������������ʾ
 *
 * @���� :NONE
 *@���  :NONE
***********************************************************/
void Gui_progress_bar_setValue( hwin_t*hp, uint16_t nstep )
{
	uint16_t x_start, x_end;
	widget_progress_t*pwidget;
	pwidget = GUI_widget_obtain( hp );
	if ( nstep == pwidget->progress_cur_pos )return;
	GUI_window_hwin_req( hp );
	GUI_set_Bgcolor( PROGRESS_BAR_STEP_COLR );
	GUI_set_Fgcolor( C_GUI_RED );
	pwidget->progress_cur_pos = 0;
	x_start = pwidget->progress_cur_pos * pwidget->progress_setp_size + 1;
	if ( x_start < ( hp->win_wide - 2 ) )
	{
		if (( pwidget->progress_cur_pos + nstep ) < pwidget->progress_max_pos )
		{
			x_end = x_start + nstep * pwidget->progress_setp_size;
		}
		else
		{
			nstep = pwidget->progress_max_pos - pwidget->progress_cur_pos;
			x_end = hp->win_x + hp->win_wide;
		}
		GUI_fill_window_area( x_start, 1, x_end, hp->win_high );
		pwidget->progress_cur_pos += nstep;
	}
	GUI_window_hwin_release( hp );
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :�������
 *
 * @���� :NONE
 *@���  :NONE
***********************************************************/
void Gui_progress_bar_empty( hwin_t*hp )
{
	widget_progress_t*pwidget;
	pwidget = GUI_widget_obtain( hp );
	GUI_window_hwin_req( hp );
	GUI_set_Bgcolor( PROGRESS_BAR_BG_COLR );
	GUI_set_Fgcolor( PROGRESS_BAR_FG_COLR );
	GUI_fill_window_area( 1, 1, hp->win_wide, hp->win_high );
	pwidget->progress_cur_pos = 0;
	GUI_window_hwin_release( hp );
	return;
}
#endif
