#include "../../Gui.h"
#include "../GUI_lcd_driver.h"
#if	GUI_TXTBOX_EN  >0
hwin_t* Gui_txtbox_creat(
	uint16_t xcoord,
	uint16_t ycoord,
	Bsize_t wide,
	Bsize_t high,
	Bsize_t flags,
	hwin_t* fatherhp
)
{
	hwin_t *hp;
	hp = GUI_window_child_creat(
			 xcoord,
			 ycoord,
			 wide,
			 high,
			 0, fatherhp);
	hp->type = WIDGET_TEXTBOX;
	GUI_window_hwin_req( hp );
	if (( flags&WIN_FILL ) == WIN_FILL )
	{
		GUI_fill_Rect(
			hp->win_x,
			hp->win_y + 1,
			hp->win_wide - 1,
			hp->win_high - 1 );
	}
	if (( flags&WIN_3D ) == WIN_3D )
	{
		GUI_widget_3D_DrawDownRect( hp->win_x,
									hp->win_y,
									hp->win_wide,
									hp->win_high );
	}
	hp->flag = flags;
	GUI_window_hwin_release( hp );
	return hp;
}

void GUI_txtbox_print( hwin_t*hp, char*pst )
{
	if ( hp->type == WIDGET_TEXTBOX )
	{
		GUI_window_hwin_req( hp );
		GUI_clean_window();
		GUI_xy_set( 0, 0 );
		GUI_string_display( pst );
		GUI_window_hwin_release( hp );
	}
	return;
}
#endif
