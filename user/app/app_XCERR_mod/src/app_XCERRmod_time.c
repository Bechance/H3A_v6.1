/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:���ģ���������
* ����ʱ��:
* ������˾:����
* �ļ�����:app_XCERRmod_time.c
* ������  :
* ��������:ʱ������
******************************************************************************************/
#include    "app_XCERRmod_inside.h"
static dev_id_t    _time_dev = INVALID_DEV_ID;
void hand_wirte_input_box_pos_set( uint16_t x , uint16_t y );
/********************************************************************
 * @���� :�ҳ�
 * @���� :��ȡʱ��BUF
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
static void _time_obtain( time_string_t*ptime )
{
	os_driver_ioctl( _time_dev, TMR_CMD_TIME_STRING_GET, ptime );
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :ʱ��������Ϣ���ӻص�����
 *
 * @���� :NONE
 *
 *@���  :NONE
********************************************************************/
static void _time_display( time_string_t*ptime )
{
	GUI_edit_select( 1 );
	GUI_edit_string_display( ptime->year_buf );
	GUI_edit_select( 2 );
	GUI_edit_string_display( ptime->month_buf );
	GUI_edit_select( 3 );
	GUI_edit_string_display( ptime->day_buf );
	GUI_edit_select( 4 );
	GUI_edit_string_display( ptime->hour_buf );
	GUI_edit_select( 5 );
	GUI_edit_string_display( ptime->min_buf );
	GUI_edit_select( 6 );
	GUI_edit_string_display( ptime->sec_buf );
	return;
}


static uint8_t _time_setting( hwin_t*hp )
{
	uint8_t key;
	char    nch[3] = "";
	char*pch;
	Bsize_t offset =0;
	Bsize_t member;
	time_string_t tmr_buf;
	global_memset( &( tmr_buf ), 0, sizeof( time_string_t ) );
	_time_obtain( &tmr_buf );
	hand_wirte_input_box_pos_set( 190, 350 );
	key_input_method_assign( KEY_INPUT_123 );        /*ǿ��ָ��Ϊ��������*/
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_set_Bgcolor( 0xce79 );
		GUI_set_Fgcolor( 0 );
		if ( key == KEY_DOWN )GUI_edit_shift( EDIT_NEXT );
		else if ( key == KEY_UP )GUI_edit_shift( EDIT_LAST );
		else if ( key == KEY_RIGHT )GUI_edit_csr_shift( CSR_RIGHT_SHIFT );
		else if ( key == KEY_LEFT )GUI_edit_csr_shift( CSR_LEFT_SHIFT );
		else if ((app_global_decide_inputkey( key ) ) != FAIL )
		{
			member = GUI_edit_id_obtain();
			pch = ( char* )tmr_buf.year_buf + ( member - 1 ) * 3;
			global_memcpy(nch,pch,2);
			offset = GUI_edit_Boffset_obtain();
			if ( key_method_input_shadow( key, nch ) )
			{
				global_memcpy( pch, nch, 2 );
			}
			if (offset >= 1)GUI_edit_shift( EDIT_NEXT );
		}
		else if ( key == KEY_ENTER )
		{
			os_driver_ioctl( _time_dev, TMR_CMD_TIME_STRING_SAVE, &tmr_buf );
			break;
		}
		else if ( key == KEY_EXIT )
		{
			break;
		}
		GUI_window_hwin_release( hp );
	}
	GUI_window_hwin_release( hp );
	return key;
}

#define  YPOS  		 5
#define  CH_WIDE     12
#define  MPOS  		(YPOS+CH_WIDE*4)
#define  DPOS  		(MPOS+CH_WIDE*4)
#define  HPOS  		(DPOS+CH_WIDE*4+CH_WIDE*2)
#define  MINPOS  	(HPOS+CH_WIDE*4)
#define  SPOS  		(MINPOS+CH_WIDE*4)
#define  POSY       (50-HZ_HIGH)/2
#define  LPOSY      (POSY+HZ_HIGH+2)
static int _Stime_callback( hwin_t*pwin )
{
	hwin_t*hp, *button;
	time_string_t tmr_buf;
	GUI_window_hwin_req( pwin );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	button = Gui_button_creat( 80, 90, 90, 35, KEY_ENTER | BUTTON_PAINT, pwin );
	GUI_button_title_display( button, "ȷ  ��" );
	button = Gui_button_creat( 215, 90, 90, 35, KEY_EXIT | BUTTON_PAINT, pwin );
	GUI_button_title_display( button, "��  ��" );
	GUI_window_hwin_release( pwin );
	hp = GUI_edit_creat( 30, 10, pwin->win_wide - 30, 46, WIN_TOUCH_EN, pwin );
	GUI_window_hwin_req( hp );
	GUI_SetFront( &GUI_HZK24_EXTASCII );
	GUI_set_Bgcolor( 0xce79 );
	GUI_set_Fgcolor( 0 );
	GUI_edit_memeber_add( YPOS, POSY, 2, "��", EDIT_ADD_END | 1 );
	GUI_window_line_display( YPOS + 2, LPOSY,  YPOS + 2 + 12*2, LPOSY );
	GUI_edit_memeber_add( MPOS, POSY, 2, "��", EDIT_ADD_END | 2 );
	GUI_window_line_display( MPOS + 2, LPOSY,  MPOS + 2 + 12*2, LPOSY );
	GUI_edit_memeber_add( DPOS, POSY, 2, "��", EDIT_ADD_END | 3 );
	GUI_window_line_display( DPOS + 2, LPOSY,  DPOS + 2 + 12*2, LPOSY );
	GUI_edit_memeber_add( HPOS, POSY, 2, "ʱ", EDIT_ADD_END | 4 );
	GUI_window_line_display( HPOS , LPOSY, HPOS + 12*2, LPOSY );
	GUI_edit_memeber_add( MINPOS, POSY, 2, "��", EDIT_ADD_END | 5 );
	GUI_window_line_display( MINPOS + 2, LPOSY, MINPOS + 2 + 12*2, LPOSY );
	GUI_edit_memeber_add( SPOS, POSY, 2, "��", EDIT_ADD_END | 6 );
	GUI_window_line_display( SPOS + 2, LPOSY, SPOS + 2 + 12*2, LPOSY );
	global_memset( &( tmr_buf ), 0, sizeof( time_string_t ) );
	_time_obtain( &tmr_buf );
	_time_display( &tmr_buf );
	GUI_window_hwin_release( hp );
	GUI_edit_start( hp, _time_setting );
	GUI_window_destory( button );
	return 0;
}
/********************************************************************
 * @���� :
 * @���� :����ʱ������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_XCERRmod_Stime_entery( void)
{
	hwin_t*hp;
	uint16_t sec;
	int x1, y1, x2, y2;
	x1 = 180;
	y1 = 140;
	x2 = x1 + 400;
	y2 = y1 + 200;
	_time_dev = os_driver_open( "TIME" );
	sec = GUI_read_block_Extp( x1, y1, x2, y2 );
	GUI_window_hwin_req( GET_GUIWIN );
	Gui_button_set_color( C_GUI_LOW_BLUE, 0xce79 );
	Gui_3D_window_set_color( C_GUI_WHITE, C_GUI_LOW_BLUE, C_GUI_RED );
	GUI_set_Bgcolor( 0xce79 );
	GUI_set_Fgcolor( 0 );
	hp = Gui_3D_window_creat( x1 + 10, y1 + 10, 380, 180, "ʱ������", WIN_3D | WIN_FILL, GET_GUIWIN );
	GUI_window_hwin_release( GET_GUIWIN );
	GUI_TopWIN_Set( hp );
	_Stime_callback( hp );
	GUI_window_destory( hp );
	GUI_write_block_Extp( x1, y1, x2, y2, sec );
	return;
}


