#include "../Gui.h"
#include "../GUI_config.h"

/********************************************************************
 * 用户HOOK ，为了使程序模块化，预留的用户函数
********************************************************************/
extern Bsize_t  key_method_input_shadow( uint8_t key, char*pshadow );	
extern void 	key_input_method_icon_open( void );
extern void 	key_input_method_icon_close( void );
extern void 	key_input_method_switch( void );
extern void    	key_input_method_filter( uint16_t flag );
extern void    	app_global_key_FIFO_empty( void );
extern uint8_t 	app_global_key_force_obtain( uint8_t*buf, Bsize_t size );
extern uint8_t 	app_global_key_obtain( uint8_t*buf, Bsize_t size );
extern void    	app_global_key_control_power_req( void );
extern void    	app_global_key_control_power_release( void );

/*启动编辑框HOOK*/
void GUI_edit_start_hook( void )
{
	key_input_method_icon_open();
	key_input_method_filter( 0x0f );
	return;
}
void GUI_edit_end_hook( void )
{
	key_input_method_icon_close();
	return;
}
void GUI_edit_run_hook( uint8_t key )
{
    
	if ( key == KEY_RIGHT )
	{
		GUI_edit_csr_shift( CSR_RIGHT_SHIFT );/*光标右移动一位*/
	}
	else if ( key == KEY_LEFT )
	{
		GUI_edit_csr_shift( CSR_LEFT_SHIFT );/*光标左移一位*/
	}
	if ( key == KEY_SWITCH )
	{
		key_input_method_switch();
	}
	return;
}

uint8_t GUI_edit_setting(hwin_t*hp)
{
	uint8_t  key;
	uint8_t  method;
	Bsize_t  _curID = 0,_lastID = 0xffff;
	app_global_key_control_power_req();
	while(1)
	{
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		_curID = GUI_edit_id_obtain();
		if(_curID!=_lastID)
		{ 
		   _lastID = _curID;
		   method  = GUI_edit_GetUNflag(hp,_curID)&0xff;
		   key_input_method_filter(method);
		}
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( C_GUI_WHITE);		
		if ( app_global_decide_inputkey( key ) != FAIL )
		{
			key_method_input_shadow(key,0);			
		}
		else if(key == KEY_DOWN)GUI_edit_shift( EDIT_NEXT);	
		else if(key == KEY_UP)  GUI_edit_shift( EDIT_LAST );	
		GUI_window_hwin_release( hp );
	    if (key == KEY_EXIT||key==KEY_ENTER)
		{
			break;
		}	
	}
	app_global_key_control_power_release();
    return key;
}
/*按键输入设备接口*/
uint8_t 	m_GUI_Key;
BOOL    	m_GUI_Key_new = 0;

void GUI_keyExt_Lock( void )
{
	app_global_key_control_power_req();
}
void GUI_keyExt_unLock( void )
{
	app_global_key_control_power_release();
}

void GUI_key_empty( void )
{
	app_global_key_FIFO_empty();
}

void GUI_dummy_press_Ekey( uint8_t key)
{
	app_global_key_dummy_press(key);
}


void GUI_SetEXTkey( uint8_t key )
{
	m_GUI_Key	  = key;
	m_GUI_Key_new = 1;
	return;
}

BOOL GUI_GetEXTkey( uint8_t*key )
{
	BOOL state;
	*key  = m_GUI_Key;
	state = m_GUI_Key_new;
	if ( m_GUI_Key_new )
	{
		m_GUI_Key_new = 0;
	}
	GUI_key_empty();
	return state;
}

void GUI_key_create( uint8_t key )
{
	if ( key == KEY_RIGHT )			GUI_SetEXTkey( GUIE_KEY_RIGHT );
	else if ( key == KEY_LEFT )		GUI_SetEXTkey( GUIE_KEY_LEFT );
	else if ( key == KEY_UP )		GUI_SetEXTkey( GUIE_KEY_UP );
	else if ( key == KEY_DOWN )		GUI_SetEXTkey( GUIE_KEY_DOWN );
	else if ( key == KEY_1 )		GUI_SetEXTkey( GUIE_KEY_1 );
	else if ( key == KEY_2 )		GUI_SetEXTkey( GUIE_KEY_2 );
	else if ( key == KEY_3 )		GUI_SetEXTkey( GUIE_KEY_3 );
	else if ( key == KEY_4 )		GUI_SetEXTkey( GUIE_KEY_4 );
	else if ( key == KEY_5 )		GUI_SetEXTkey( GUIE_KEY_5 );
	else if ( key == KEY_6 )		GUI_SetEXTkey( GUIE_KEY_6 );
	else if ( key == KEY_7 )		GUI_SetEXTkey( GUIE_KEY_7 );
	else if ( key == KEY_8 )		GUI_SetEXTkey( GUIE_KEY_8 );
	else if ( key == KEY_9 )		GUI_SetEXTkey( GUIE_KEY_9 );
	else if ( key == KEY_0 )		GUI_SetEXTkey( GUIE_KEY_0 );
	else if ( key == KEY_DEL )		GUI_SetEXTkey( GUIE_KEY_ESC );
	else if ( key == KEY_ENTER )	GUI_SetEXTkey( GUIE_KEY_ENTER );
	else if ( key == KEY_EXIT )		GUI_SetEXTkey( GUIE_KEY_EXIT );
	else GUI_SetEXTkey( key );
	return;
}
