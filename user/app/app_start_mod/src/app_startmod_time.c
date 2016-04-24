/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:变比模块主体程序
* 创建时间:
* 所属公司:科荟
* 文件名称:app_startmod_time.c
* 创建人  :
* 功能描述:时间设置
******************************************************************************************/
#include    "app_startmod_inside.h"
static dev_id_t    _time_dev = INVALID_DEV_ID;
void hand_wirte_input_box_pos_set( uint16_t x , uint16_t y );
/********************************************************************
 * @作者 :揭成
 * @功能 :获取时间BUF
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
static void _time_obtain( time_string_t*ptime )
{
	os_driver_ioctl( _time_dev, TMR_CMD_TIME_STRING_GET, ptime );
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :时间设置消息盒子回调函数
 *
 * @输入 :NONE
 *
 *@输出  :NONE
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
	Bsize_t offset, len;
	Bsize_t member;
	time_string_t tmr_buf;
	global_memset( &( tmr_buf ), 0, sizeof( time_string_t ) );
	_time_obtain( &tmr_buf );
	hand_wirte_input_box_pos_set( 190, 350 );
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
		else if (( nch[0] = app_global_decide_inputkey( key ) ) != FAIL )
		{
			key_input_method_assign( KEY_INPUT_123 );        /*强制指定为数字输入*/
			offset = GUI_edit_Boffset_obtain();
			member = GUI_edit_id_obtain();
			if ( len = key_method_input( key, nch ) )
			{
				pch = ( char* )tmr_buf.year_buf + ( member - 1 ) * 3 + offset;
				global_memcpy( pch, nch, len );
			}
			if ( offset == 1 )GUI_edit_shift( EDIT_NEXT );
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
	GUI_button_title_display( button, "确  定" );
	button = Gui_button_creat( 215, 90, 90, 35, KEY_EXIT | BUTTON_PAINT, pwin );
	GUI_button_title_display( button, "退  出" );
	GUI_window_hwin_release( pwin );
	hp = GUI_edit_creat( 30, 10, pwin->win_wide - 30, 46, WIN_TOUCH_EN, pwin );
	GUI_window_hwin_req( hp );
	GUI_SetFront( &GUI_HZK24_EXTASCII );
	GUI_set_Bgcolor( 0xce79 );
	GUI_set_Fgcolor( 0 );
	GUI_edit_memeber_add( YPOS, POSY, 2, "年", EDIT_ADD_END | 1 );
	GUI_window_line_display( YPOS + 2, LPOSY,  YPOS + 2 + 12*2, LPOSY );
	GUI_edit_memeber_add( MPOS, POSY, 2, "月", EDIT_ADD_END | 2 );
	GUI_window_line_display( MPOS + 2, LPOSY,  MPOS + 2 + 12*2, LPOSY );
	GUI_edit_memeber_add( DPOS, POSY, 2, "日", EDIT_ADD_END | 3 );
	GUI_window_line_display( DPOS + 2, LPOSY,  DPOS + 2 + 12*2, LPOSY );
	GUI_edit_memeber_add( HPOS, POSY, 2, "时", EDIT_ADD_END | 4 );
	GUI_window_line_display( HPOS , LPOSY, HPOS + 12*2, LPOSY );
	GUI_edit_memeber_add( MINPOS, POSY, 2, "分", EDIT_ADD_END | 5 );
	GUI_window_line_display( MINPOS + 2, LPOSY, MINPOS + 2 + 12*2, LPOSY );
	GUI_edit_memeber_add( SPOS, POSY, 2, "秒", EDIT_ADD_END | 6 );
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
 * @作者 :
 * @功能 :进入时间设置
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_startmod_Stime_entery( void)
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
	hp = Gui_3D_window_creat( x1 + 10, y1 + 10, 380, 180, "时间设置", WIN_3D | WIN_FILL, GET_GUIWIN );
	GUI_window_hwin_release( GET_GUIWIN );
	GUI_TopWIN_Set( hp );
	_Stime_callback( hp );
	GUI_window_destory( hp );
	GUI_write_block_Extp( x1, y1, x2, y2, sec );
	return;
}
/********************************************************************
 * @创建人 :背光设置
 * @功能   :进入时间设置
 * @输入   :NONE
 * @输出   :NONE
********************************************************************/
static void _lcd_BLK(int uage,int max)
{
#define CMD_LCD_BLK_B         0x408F
#define CMD_LCD_BLK_L         0x4090
	int  lcdBLK_ALL = 469;
	int  lcdBLK_B ;
	int  lcdBLK_L ;
	float fuage;
	fuage = (float)uage/(float)max;
    lcdBLK_B = (int)((float)lcdBLK_ALL*(1.0-fuage));
    lcdBLK_L = (int)((float)lcdBLK_ALL*(fuage));
	BUS_WRITE(lcdBLK_L,CMD_LCD_BLK_L);
	BUS_WRITE(lcdBLK_B,CMD_LCD_BLK_B);
    return;
}
static int _lcdBLK_callback( hwin_t*pwin )
{
	hwin_t*  hp;
	uint8_t  key_buf;
	int      uage     = 10;
	int      max      = 20;
	GUI_window_hwin_req( pwin );
	hp = GUI_slider_create( 15,(pwin->win_high-40)/2,pwin->win_wide - 30,max,0,pwin);
	GUI_set_Bgcolor( 0xce79 );
	GUI_set_Fgcolor( 0 );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(20, (pwin->win_high-40)/2-20,"暗");  
	GUI_string_display_At(pwin->win_wide-30,(pwin->win_high-40)/2-20,"亮");  
	GUI_string_display_At(20, pwin->win_high-20,"按<左>,<右>调整背光亮度.");
	fatfs_readARG("LCDblk",&uage,sizeof(int));
	GUI_slider_setCURpos(hp,uage);
	GUI_window_hwin_release(pwin);
	while(1)
	{
		if ( app_global_key_obtain( &key_buf, 1 ) == 1 ) 
		{
			if ( key_buf == KEY_RIGHT)
			{
			    uage = (uage<max)?(uage+1):(max);
				GUI_slider_setCURpos(hp,uage);
			    _lcd_BLK(uage,max);
			}
			else if ( key_buf == KEY_LEFT)
			{
				 uage = (uage>1)?(uage-1):(1);
				 GUI_slider_setCURpos(hp,uage);
				 _lcd_BLK(uage,max);
			}
			else if ( key_buf == KEY_ENTER )
			{
				fatfs_writeARG("LCDblk",&uage,sizeof(int));
				break;
			}
			else if ( key_buf == KEY_EXIT )
			{
				break;
			}
		}
	}
	return 0;
}

void app_startmod_lcdBLK_AA(void)
{
	int      uage     = 10;
	fatfs_readARG("LCDblk",&uage,sizeof(int));
	_lcd_BLK(uage,20);
    return;
}

void app_startmod_lcdBLK(void)
{
	hwin_t*hp;
	uint16_t sec;
	int x1, y1, x2, y2;
	x1 = 120;
	y1 = 100;
	x2 = x1 + 400;
	y2 = y1 + 200;
	sec = GUI_read_block_Extp( x1, y1, x2, y2 );
	GUI_window_hwin_req( GET_GUIWIN );
	Gui_button_set_color( C_GUI_LOW_BLUE, 0xce79 );
	Gui_3D_window_set_color( C_GUI_WHITE, C_GUI_LOW_BLUE, C_GUI_RED );
	GUI_set_Bgcolor( 0xce79 );
	GUI_set_Fgcolor( 0 );
	hp = Gui_3D_window_creat( x1 + 10, y1 + 10, 380, 180, "LCD亮度调整", WIN_3D | WIN_FILL, GET_GUIWIN );
	GUI_window_hwin_release( GET_GUIWIN );
	GUI_TopWIN_Set( hp );
	_lcdBLK_callback( hp );
	GUI_window_destory( hp );
	GUI_write_block_Extp( x1, y1, x2, y2, sec );
    return;
}
