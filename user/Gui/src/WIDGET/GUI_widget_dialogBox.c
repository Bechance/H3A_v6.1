#include "../../Gui.h"
#include "../GUI_lcd_driver.h"
#if GUI_DIALOG_EN  >0
hwin_t*   _dialogBOX_only;


hwin_t*GUI_dialogBox_member_obtain( Bsize_t id )
{
	hwin_t* ptemp;
	Bsize_t mID;
	ptemp = m_hwin_list_head;
	while ( ptemp )
	{
		mID = ( ptemp->win_id);
		if ( mID == id && ptemp->father_hwin == _dialogBOX_only)
		{
			return ptemp;
		}
		ptemp = ptemp->next_hwin;
	}
	return 0;
}


hwin_t*GUI_dialogBox_create( const char*title,
							 uint16_t x,
							 uint16_t y,
							 uint16_t wide,
							 uint16_t high,
							 Bsize_t flag,
							 const dialogBox_t*pbox,
							 Bsize_t  box_size
						   )
{
	hwin_t* dialog;
	hwin_t* pwin;
	Bsize_t offset;
	dialogBox_t*pmember;
	if ( title[0] != 0 )
		dialog = Gui_3D_window_creat( x, y, wide, high, ( char* )title, flag, 0 );
	else
		dialog = GUI_window_creat( x, y, wide, high, flag );
	dialog->type = WIDGET_DIALOG;
	GUI_window_hwin_req( dialog );
	for ( offset = 0;offset < box_size;offset++ )
	{
		pmember = ( dialogBox_t* ) & pbox[offset];
		if ( pmember->member_type == WIDGET_BUTTON )
		{
			pwin = Gui_button_creat( pmember->x, pmember->y,
									 pmember->wide, pmember->high,
									 pmember->flag, dialog );
			if (( pmember->flag&BUTTON_PAINT ) == BUTTON_PAINT )
				GUI_button_title_display( pwin, pmember->member_title );
		}
		else if ( pmember->member_type == WIDGET_EDIT )
		{
			pwin = GUI_edit_creat( pmember->x, pmember->y,
								   pmember->wide, pmember->high,
								   pmember->flag, dialog );
		}
		else if ( pmember->member_type == WIDGET_DROP_LIST )
		{
			pwin = Gui_drop_list_creat( pmember->x, pmember->y,
										pmember->parg, pmember->arg_size, pmember->flag, dialog );
		}
		else if ( pmember->member_type == WIDGET_PROGRESS_BAR )
		{
			pwin = Gui_progress_bar_creat( pmember->x, pmember->y,
										   pmember->wide, pmember->high,
										   pmember->arg_size, dialog );
		}
		else if ( pmember->member_type == WIDGET_RADIO )
		{
			pwin = GUI_radio_create( pmember->x, pmember->y,
									 pmember->wide, pmember->high,
									 pmember->flag, dialog );
		}
		else if ( pmember->member_type == WIDGET_TEXTBOX )
		{
			pwin = Gui_txtbox_creat( pmember->x, pmember->y,
									 pmember->wide, pmember->high,
									 pmember->flag, dialog );
		}
		else if ( pmember->member_type == WIDGET_LISTVIEW )
		{
			pwin = GUI_listview_create( pmember->x, pmember->y,
										pmember->wide, pmember->high,
										pmember->flag, dialog );
		}
		else if ( pmember->member_type == CHILD_WIN )
		{
			pwin = GUI_window_child_creat( pmember->x, pmember->y,
										   pmember->wide, pmember->high,
										   pmember->flag, dialog );
		}
		else if(pmember->member_type == WIDGET_SCRB)
		{
			pwin = Gui_scrb_create( pmember->x, pmember->y,
										   pmember->wide, pmember->high,
										   pmember->flag, dialog );
		}
		else if(pmember->member_type == WIDGET_SCLIDER)
		{
			pwin = GUI_slider_create( pmember->x, pmember->y,
									pmember->wide, pmember->arg_size,
									pmember->flag, dialog );
		}
		else break;
		if ( pmember->callblack )pmember->callblack( pwin );
		pwin->win_id  = pmember->member_id;
	}
	_dialogBOX_only = dialog;
	GUI_window_hwin_release( dialog );
	return dialog;
}



void GUI_dialogBox_init( void )
{
	return;
}
#endif
