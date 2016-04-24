/*************************************************************************
*
* Ä£¿éÃû³Æ:testÄ£¿é½çÃæº¯Êý¼¯ºÏ
* ´´½¨Ê±¼ä:
* ËùÊô¹«Ë¾ :¿ÆÜö
* ÎÄ¼þÃû³Æ:app_startmod_interface.c
* ´´½¨ÈË :
* ¹¦ÄÜÃèÊö:Ìá¹©±ä±ÈÄ£¿é½çÃæ»­ÆÁº¯ÊýAPI
***************************************************************************/
#include      "app_startmod_inside.h"
static 		   void app_startmod_localapp_obtain( void );
Extapp_name_t   m_apparray[30];
Extapp_name_t  *m_apparray_pCUR;
int  			m_array_pos = 0;
int  			m_menu_start_pos = 0;
int  			m_menu_end_pos   = 0;
int  			m_menu_mark_pos  = 0;
hwin_t         *m_3Dwin 	  	 = 0;
hwin_t         *m_button_last 	 = 0;
hwin_t         *m_button_next 	 = 0;
hwin_t         *m_pTXT 		  	 = 0;

hwin_t*			m_battery2_win	 = 0;
hwin_t*			m_external2_win	 = 0;
hwin_t*			m_charge2_win	 = 0;
ExtBmp_ctl_t*   mBMP_ctl;

/*--  µç³Ø --*/
/*--  ¿í¶Èx¸ß¶È=16x32  --*/
const char m_battery2_flag_icon[6][64] =
{
	{//¿ÕµçÁ¿
		0x0F, 0xF0, 0x0F, 0xF0, 0xFF, 0xFF, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
		0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
		0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
		0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xFF, 0xFF,
	},
	{// 1¸ñµçÁ¿
		0x0F, 0xF0, 0x0F, 0xF0, 0xFF, 0xFF, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
		0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
		0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
		0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x80, 0x01, 0xFF, 0xFF,
	},
	{// 2¸ñµçÁ¿
		0x0F, 0xF0, 0x0F, 0xF0, 0xFF, 0xFF, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
		0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
		0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9,
		0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x80, 0x01, 0xFF, 0xFF,
	},
	{// 3¸ñµçÁ¿
		0x0F, 0xF0, 0x0F, 0xF0, 0xFF, 0xFF, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
		0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9,
		0x9F, 0xF9, 0x9F, 0xF9, 0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9,
		0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x80, 0x01, 0xFF, 0xFF,
	},
	{// 4¸ñµçÁ¿
		0x0F, 0xF0, 0x0F, 0xF0, 0xFF, 0xFF, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
		0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9,
		0x9F, 0xF9, 0x9F, 0xF9, 0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9,
		0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x80, 0x01, 0xFF, 0xFF,
	},
	{// Âú¸ñµçÁ¿
		0x0F, 0xF0, 0x0F, 0xF0, 0xFF, 0xFF, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9, 0x80, 0x01, 0x80, 0x01,
		0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9,
		0x9F, 0xF9, 0x9F, 0xF9, 0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9,
		0x80, 0x01, 0x80, 0x01, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x9F, 0xF9, 0x80, 0x01, 0xFF, 0xFF,
	},
};

/*--  ¿ª¹ØµçÔ´ --*/
/*--  ¿í¶Èx¸ß¶È=16x32  --*/
const char m_extral2_flag_icon[64] =
{
	0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7F, 0xFE, 0x7B, 0xDE,
	0x7B, 0xDE, 0x78, 0x1E, 0x3F, 0xFC, 0x07, 0xE0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0,
	0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0,
	0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/*--  ÊÊÅäÆ÷ --*/
/*--  ¿í¶Èx¸ß¶È=16x32  --*/
const char m_adapter2_flag_icon[64] =
{
	0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x04, 0x20, 0x07, 0xE0, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,
	0x3F, 0xFC, 0x3F, 0xFC, 0x3F, 0xFC, 0x20, 0x04, 0x3F, 0xFC, 0x1F, 0xF8, 0x18, 0x18, 0x07, 0xE0,
	0x07, 0xE0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0,
	0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00,
};

/*--  ³äµç×´Ì¬±êÖ¾ --*/
/*--  ¿í¶Èx¸ß¶È=16x32  --*/
const char m_charge2_flag_icon[64] =
{
	0x00, 0x00, 0x00, 0x7F, 0x00, 0xFF, 0x01, 0xFE, 0x03, 0xFC, 0x07, 0xF8, 0x0F, 0xF0, 0x1F, 0xE0,
	0x3F, 0xC0, 0x7F, 0xFF, 0xFF, 0xFE, 0xFF, 0xFC, 0x00, 0xF8, 0x01, 0xF0, 0x03, 0xE0, 0x07, 0xC0,
	0x0F, 0x80, 0x1F, 0x00, 0x3E, 0x00, 0x7C, 0x00, 0xF8, 0x00, 0xF0, 0x00, 0xE0, 0x00, 0xC0, 0x00,
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
/********************************************************************
 * @×÷Õß : ÖÜ±þÈ¨
 * @¹¦ÄÜ : µç³Ø±êÖ¾ÏÔÊ¾
 * @ÊäÈë : NONE
 * @Êä³ö : NONE
********************************************************************/
#define  PEXT_Y    			5
void app_Startmod_battery_display( void )
{
	Bsize_t    num = 1;
	num = g_battery_num;
	GUI_window_hwin_req( m_battery2_win );
	GUI_set_Bgcolor( Startmod_WIN_BG_COLOR );
	if ( g_cur_power_up == 1 )
	{
		if ( num <= 1 )
			GUI_set_Fgcolor( C_GUI_RED );
		else if ( num <= 3 )
			GUI_set_Fgcolor( C_GUI_YELLOW );
		else
			GUI_set_Fgcolor( C_GUI_GREEN );
	}
	else if ( g_cur_power_up == 0 )
	{
		if ( g_power_state == PS_BATTERY || g_power_state == PS_PA_BATTERY )
		{
			GUI_set_Fgcolor( C_GUI_GREEN );
		}
		else if ( g_power_state == PS_EXTRAL_BATTERY ||
				  g_power_state == PS_ADAPTER_BATTERY ||
				  g_power_state == PS_ADAPTER_EXTRAL_BATTERY ||
				  g_power_state == PS_PA_EXTRAL_BATTERY )
		{
			GUI_set_Fgcolor( C_GUI_WHITE );
		}
		else if ( g_power_state == PS_EXTRAL || g_power_state == PS_ADAPTER ||
				  g_power_state == PS_ADAPTER_EXTRAL ||
				  g_power_state == PS_PA_EXTRAL )
		{
			GUI_set_Fgcolor( Startmod_WIN_BG_COLOR );
			GUI_libASC_display_At( 1, PEXT_Y, 16, 32, ( char* )0 );
			GUI_window_hwin_release( m_battery2_win );
			return;
		}
	}
	GUI_libASC_display_At( 1, PEXT_Y, 16, 32, ( char* )m_battery2_flag_icon[num] );
	GUI_window_hwin_release( m_battery2_win );
	return;
}

/********************************************************************
 * @×÷Õß : ÖÜ±þÈ¨
 * @¹¦ÄÜ : µç³Ø³äµç±êÖ¾ÏÔÊ¾
 * @ÊäÈë : NONE
 * @Êä³ö : NONE
********************************************************************/
void app_Startmod_charge_display( void )
{
	GUI_window_hwin_req( m_charge2_win );
	GUI_set_Bgcolor( Startmod_WIN_BG_COLOR );
	if ( g_battery_charge == 0 )
		GUI_set_Fgcolor( C_GUI_WHITE );
	else
		GUI_set_Fgcolor( C_GUI_GREEN );

	GUI_libASC_display_At( 2, 10, 16, 32, ( char* )m_charge2_flag_icon );
	GUI_window_hwin_release( m_charge2_win );
	return;
}

/********************************************************************
 * @×÷Õß : ÖÜ±þÈ¨
 * @¹¦ÄÜ : Íâ²¿µçÔ´±êÖ¾ÏÔÊ¾
 * @ÊäÈë : NONE
 * @Êä³ö : NONE
********************************************************************/
void app_Startmod_external_display( void )
{
	GUI_window_hwin_req( m_external2_win );
	GUI_set_Bgcolor( Startmod_WIN_BG_COLOR );
	if ( g_cur_power_up == 0 )
		GUI_set_Fgcolor( C_GUI_GREEN );
	else GUI_set_Fgcolor( C_GUI_WHITE );

	if ( g_power_state == PS_ADAPTER || g_power_state == PS_ADAPTER_BATTERY ||
		 g_power_state == PS_ADAPTER_EXTRAL ||
		 g_power_state == PS_ADAPTER_EXTRAL_BATTERY )
	{
		GUI_libASC_display_At( 2, 7, 16, 32, ( char* )m_adapter2_flag_icon );
	}
	else if ( g_power_state == PS_EXTRAL ||
			  g_power_state == PS_EXTRAL_BATTERY ||
			  g_power_state == PS_PA_EXTRAL ||
			  g_power_state == PS_PA_EXTRAL_BATTERY )
	{
		GUI_libASC_display_At( 2, 7, 16, 32, ( char* )m_extral2_flag_icon );
	}
	else
	{
		GUI_set_Fgcolor( Startmod_WIN_BG_COLOR );
		GUI_libASC_display_At( 2, 7, 16, 32, ( char* )0 );
	}
	GUI_window_hwin_release( m_external2_win );
	return;
}

/********************************************************************
 * @ß : ÖÜ±þÈ¨
 * @¹¦ÄÜ : ÊÊÅäÆ÷Òì³£±£»¤ÏÔÊ¾
 * @ÊäÈë : NONE
 * @Êä³ö : NONE
********************************************************************/
void app_Startmod_adapter_protect_display( void )
{
	if ( g_power_state == PS_PA_BATTERY ||
		 g_power_state == PS_PA_EXTRAL ||
		 g_power_state == PS_PA_EXTRAL_BATTERY )
	{
		MsgBox( 200, 130, 250, 200, "µçÔ´¹ÜÀíÌáÊ¾", "ÊÊÅäÆ÷ÊäÈëÒì³£!Çë¼ì²éÊÊÅäÆ÷ÊäÈëµçÑ¹ÊÇ·ñ¹ý¸ß!", APP_HALF_SEC*15 );
	}
	return;
}


static Extapp_name_t*Exp_list_obtain( int *n )
{
	FIL* fp;
	UINT r;
	Extapp_name_t*phead;
	fp = fatfs_open( "EXPLIST", FA_OPEN_EXISTING | FA_READ );
	if ( !fp ) return 0;
	*n = fp->fsize / sizeof( Extapp_name_t );
	phead = app_malloc( fp->fsize );
	f_read( fp, phead, fp->fsize, &r );
	fatfs_close( fp );
	return phead;
}

static int Bmp_Get(const char*path)
{
   int pos;
   for(pos=0;pos<20;pos++)
   {
      if(global_memcmp(mBMP_ctl->m_BMP[pos].BMPname,
	  	(char*)path,
	  	global_strlen((char*)path)))
      {
          return pos;
	  }
   }
   return -1;
}


void app_startmod_BMPfile_view( int x0, int y0, const char*path )
{
    int 	xpos,ypos;
	int 	pos=0;
    uint16_t*image;
	int  wide,high,xs;
	pos = Bmp_Get(path);
	if(pos<0) return;
    wide = mBMP_ctl->m_BMP[pos].wide;
    high = mBMP_ctl->m_BMP[pos].high;
	image= mBMP_ctl->m_BMP[pos].BMP;
	pos = 0;
	for ( ypos = 0;ypos < high;ypos++ )
	{
	    xs = x0;
		for ( xpos = 0;xpos < wide;xpos++ )
		{
			GUI_conText.LCD_draw_dot(xs++,y0,image[pos++]);
		}
		y0++;
	}
	return;
}

static void EXP_BMP_button_callblack( void*arg )
{
	int x1, y1, x2, y2;
	uint16_t sec;
	uint8_t  pos;
	widget_button_t*pwidget;
	hwin_t*hp = ( hwin_t* )arg;
	pos = (( hp->flag & 0xff ) - 100 );
	x1 = hp->win_x - 1;
	y1 = hp->win_y - 1;
	x2 = x1 + hp->win_wide + 2;
	y2 = y1 + hp->win_high + 2;
	GUI_window_hwin_req( hp );
	sec = GUI_read_block_Extp( x1, y1, x2, y2 );
	pwidget = ( widget_button_t* )( hp->pwidget );
	app_startmod_BMPfile_view( hp->win_x, hp->win_y, pwidget->press_title );
	GUI_wait_touch_release();
	GUI_write_block_Extp( x1, y1, x2, y2, sec );
	GUI_window_hwin_release( hp );
	m_apparray_pCUR = &m_apparray[pos];
	app_start_EXE();
	return;
}

void app_startmod_markExp_start( void )
{
	if ( m_menu_mark_pos >= m_menu_end_pos ) return;
	if ( m_menu_mark_pos < m_menu_start_pos ) return;
	m_apparray_pCUR = &m_apparray[m_menu_mark_pos];
	app_start_EXE();
	return;
}


void app_startmod_Expapp_obtain( void )
{
	int    n, pos;
	char   name[20];
	Extapp_name_t*phead;
	Extapp_name_t*pCUR;
	phead = Exp_list_obtain( &n );
	if ( !phead ) return;
	pCUR = &m_apparray[m_array_pos];
	global_memcpy( pCUR, phead, sizeof( Extapp_name_t )*n );
	m_array_pos += n;
	app_free( phead );
	for ( pos = 0;pos < n;pos++ )
	{
		GLOBAL_MEMCLR( name, 20 );
		global_memcpy( name, pCUR->PressBMP, 20 );
		GLOBAL_MEMCLR( pCUR->PressBMP, 20 );
		global_strcpy( pCUR->PressBMP, "EXP/" );
		global_strcat( pCUR->PressBMP, name );
		GLOBAL_MEMCLR( name, 20 );
		global_memcpy( name, pCUR->UNpressBMP, 20 );
		GLOBAL_MEMCLR( pCUR->UNpressBMP, 20 );
		global_strcpy( pCUR->UNpressBMP, "EXP/" );
		global_strcat( pCUR->UNpressBMP, name );
		pCUR++;
	}
	return;
}

static void app_startmod_Localapp(
	const char*name,
	const char*unpressbmp,
	const char*pressbmp,
	const char*EXEname
)
{
	Extapp_name_t*pCUR;
	pCUR = &m_apparray[m_array_pos++];
	global_strcpy( pCUR->Exe, ( char* )":" );      //±¾µØ³ÌÐò±ê¼Ç
	global_strcat( pCUR->Exe, ( char* )EXEname );
	global_strcpy( pCUR->name, ( char* )name );
	global_strcpy( pCUR->PressBMP, ( char* )"BMP/" );
	global_strcat( pCUR->PressBMP, ( char* )pressbmp );
	global_strcpy( pCUR->UNpressBMP, ( char* )"BMP/" );
	global_strcat( pCUR->UNpressBMP, ( char* )unpressbmp );
	return;
}


void app_startmod_appgraph_mark( uint16_t scolor, 
	uint16_t sBcolor, 
	uint16_t Lcolor, 
	int pos 
)
{
	int   wide, high, x, x0, y0, xs, ys, xe, ye;
	int   xgap, ygap, nline, nrow ;
	Extapp_name_t*pCUR;
	int   npos ;
	wide  =  GRAPH_WIDE;
	high  =  GRAPH_HIGH;
	nline =  GRAPH_LINE;
	nrow  =  GRAPH_ROW;
	if ( pos >= m_menu_end_pos ) return;
	if ( pos < m_menu_start_pos ) return;
	GUI_window_hwin_req( m_pTXT );
	xgap  = ( m_pTXT->win_wide - wide * nrow ) / ( nrow + 1 );
	ygap  = ( m_pTXT->win_high - ( high + 16 ) * nline ) / ( nline + 1 );
	GUI_set_Bgcolor( sBcolor );
	GUI_set_Fgcolor( scolor );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	npos = pos - m_menu_start_pos;
	x0   = xgap + ( npos % nrow ) * ( wide + xgap );
	y0   = ygap + ( npos / nrow ) * (( high + 16 ) + ygap );
	pCUR = &m_apparray[pos];
	x = ( wide - global_strlen( pCUR->name ) * 8 ) / 2 + x0;
	xs = x - 3;
	ys = y0 + high + 5 - 3;
	xe = x + global_strlen( pCUR->name ) * 8 + 3;
	ye = ys + GUI_conText.CUR_Front->high + 6;
	GUI_fill_window_area( xs, ys + 1, xe, ye + 1 );
	GUI_string_display_At( x, y0 + high + 5, pCUR->name );
	GUI_set_Fgcolor( Lcolor );
	for ( npos = xs;npos < xe;npos += 2 )
		GUI_window_draw_dot( npos, ys );
	for ( npos = xs;npos < xe;npos += 2 )
		GUI_window_draw_dot( npos, ye );
	for ( npos = ys;npos < ye;npos += 2 )
		GUI_window_draw_dot( xs, npos );
	for ( npos = ys;npos < ye + 2;npos += 2 )
		GUI_window_draw_dot( xe, npos );
	GUI_window_hwin_release( m_pTXT );
	return;
}



static void _Appgraph_create( hwin_t*pOBJ, int startpos, int endpos )
{
	int pos;
	int wide, high, x, x0, y0;
	Extapp_name_t*pCUR;
	hwin_t*hp;
	int   xgap, ygap, nline, nrow ;
	int   ypos = 0;
	wide  =  GRAPH_WIDE;
	high  =  GRAPH_HIGH;
	nline =  GRAPH_LINE;
	nrow  =  GRAPH_ROW;
	xgap  = ( pOBJ->win_wide -  wide * nrow ) / ( nrow + 1 );
	ygap  = ( pOBJ->win_high - ( high + 16 ) * nline ) / ( nline + 1 );
	GUI_window_hwin_req( pOBJ );
	GUI_set_Bgcolor( C_GUI_BlACK );
	GUI_set_Fgcolor( C_GUI_WHITE );
	GUI_clean_window();
	GUI_window_hwin_release( pOBJ );
	for ( pos = startpos;pos < endpos;pos++ )
	{
		GUI_window_hwin_req( pOBJ );
		GUI_set_Bgcolor( C_GUI_BlACK );
		GUI_set_Fgcolor( C_GUI_WHITE );
		GUI_SetFront( &GUI_HZK16_EXTASCII );
		x0   = xgap + ( ypos % nrow ) * ( wide + xgap );
		y0   = ygap + ( ypos / nrow ) * (( high + 16 ) + ygap );
		pCUR = &m_apparray[pos];
		hp = Gui_button_creat( x0, y0, wide, high, ( 100 + pos ), pOBJ );
		GUI_button_title_display( hp, pCUR->UNpressBMP );
		GUI_2state_button_press_titlecopy( hp, pCUR->PressBMP );
		x = ( wide - global_strlen( pCUR->name ) * 8 ) / 2 + x0;
		GUI_string_display_At( x, y0 + high + 5, pCUR->name );
		app_startmod_BMPfile_view( hp->win_x, hp->win_y, pCUR->UNpressBMP );
		GUI_button_callback_mount( hp, EXP_BMP_button_callblack );
		ypos++;
		GUI_window_hwin_release( pOBJ );
	}
	m_menu_mark_pos = startpos;
	GRAPH_MARK( m_menu_mark_pos );
	return;
}


static void _txtWIN_paint( void )
{
	m_pTXT = Gui_txtbox_creat( 10, 0, m_3Dwin->win_wide - WIDE_B, m_3Dwin->win_high - 35, WIN_3D, m_3Dwin );
}


static void _last_button_callblack( void*arg )
{
	if ( m_menu_start_pos > 0 )
	{
		GUI_window_destory( m_pTXT );
		_txtWIN_paint();
		m_menu_start_pos -= ( GRAPH_ROW * GRAPH_LINE );
		m_menu_end_pos = m_menu_start_pos + ( GRAPH_ROW * GRAPH_LINE );
		_Appgraph_create( m_pTXT, m_menu_start_pos, m_menu_end_pos );
	}
	return;
}


static void _next_button_callblack( void*arg )
{
	int dx;
	if ( m_menu_end_pos < m_array_pos )
	{
		dx = m_array_pos - m_menu_end_pos;
		GUI_window_destory( m_pTXT );
		_txtWIN_paint();
		if ( dx > ( GRAPH_ROW*GRAPH_LINE ) )
		{
			m_menu_start_pos  = m_menu_end_pos;
			m_menu_end_pos    = m_menu_start_pos + ( GRAPH_ROW * GRAPH_LINE );
			_Appgraph_create( m_pTXT, m_menu_start_pos, m_menu_end_pos );
		}
		else
		{
			m_menu_start_pos  = m_menu_end_pos;
			m_menu_end_pos    = m_menu_start_pos + dx;
			_Appgraph_create( m_pTXT, m_menu_start_pos, m_menu_end_pos );
		}
	}
	return;
}



void app_next_page( void )
{
	GUI_window_hwin_req( m_3Dwin );
	_next_button_callblack( 0 );
	GUI_window_hwin_release( m_3Dwin );
	return;
}

void app_last_page( void )
{
	GUI_window_hwin_req( m_3Dwin );
	_last_button_callblack( 0 );
	GUI_window_hwin_release( m_3Dwin );
	return;
}

static void _Read_FPGA_version( char*buf )
{
	int 	temp_y = 0;
	int		temp_md = 0;
	char 	m_temp[5] = "0";
	temp_y  = BUS_READ(0x42D7);
	temp_md = BUS_READ(0x42D6);
	if ( temp_md < 1000 )
	{
		global_itoa(temp_md,(&m_temp[0])+1);
	}
	else 
	{
		GLOBAL_MEMCLR(m_temp, 5);
		global_itoa(temp_md,m_temp);
	}
	global_itoa(temp_y, buf);
	global_strcat( buf, " " );
	global_strcat( buf, m_temp );
	return;
}

static void _ID_VER_display( void )
{
	char str_temp[30] = "";
	FIL*fp;
	UINT r;
	GUI_window_hwin_req( m_3Dwin );
	GUI_set_Bgcolor( Startmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( Startmod_FG_COLOR );
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	GUI_string_display_At(10,m_3Dwin->win_high -32,"DSP  :" );
	global_strcpy( str_temp, __DATE__ );
	global_strcat( str_temp, " H3AV6.1   " );
	GUI_string_display( str_temp );
    GUI_string_display_At(10,m_3Dwin->win_high -18,"FPGA :");
	GLOBAL_MEMCLR( str_temp, 30 );
	_Read_FPGA_version( str_temp );
    GUI_string_display(str_temp);
	GUI_string_display_At(228,m_3Dwin->win_high -23,"ID:" );
	fp = fatfs_open( "SYSID", FA_OPEN_EXISTING | FA_READ );
	if ( !fp )GUI_string_display( "0000-0000-0000" );
	else
	{
		GLOBAL_MEMCLR( str_temp, 30 );
		f_read( fp, str_temp, fp->fsize, &r );
		if ( r == fp->fsize )
		{
			GUI_string_display( str_temp );
		}
		else
		{
			GUI_string_display( "0000-0000-0000" );
		}
		fatfs_close( fp );
	}
	GUI_window_hwin_release( m_3Dwin );
	return;
}

void app_startmod_WSD_display(float hum,float temp)
{
	
	char str_temp[10] = "";
	global_ftoa(temp, str_temp, 1);
	global_strcpy(g_user_arg.DL_app_user_info.temp_buf,str_temp);
	GLOBAL_MEMCLR(str_temp, sizeof(str_temp));
	global_ftoa(hum, str_temp, 1);
	global_strcpy(g_user_arg.DL_app_user_info.hnm_buf,str_temp);
	
	GUI_window_hwin_req( m_3Dwin );
	GUI_set_Bgcolor( Startmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( Startmod_FG_COLOR );
	GUI_SetFront( &GUI_HZK24_EXTASCII );
	GUI_string_display_At( m_3Dwin->win_wide - WIDE_B + 20, 280, "         " );
	GUI_string_display_At( m_3Dwin->win_wide - WIDE_B + 20, 280, " T= " );
	GUI_float_display( temp, 1);
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	m_cur_hwin->csr_y_pos += 6;
	GUI_string_display( "¡æ" );
	GUI_SetFront( &GUI_HZK24_EXTASCII );
	GUI_string_display_At( m_3Dwin->win_wide - WIDE_B + 20, 310, "         " );
	GUI_string_display_At( m_3Dwin->win_wide - WIDE_B + 20, 310, "RH= " );
	GUI_float_display( hum, 1);
	GUI_string_display( "%" );
	GUI_window_hwin_release( m_3Dwin );
    return;
}

/********************************************************************
 * @×÷Õß : ÑôÓÀ·æ
 * @¹¦ÄÜ : Ê±¼äÏÔÊ¾
 * @ÊäÈë : NONE
 * @Êä³ö : NONE
 * @Ê±¼ä¸ñÊ½: 2014-01-04 15:32:11(±¸×¢:ÈÕÆÚÓëÊ±¼äÖ®¼äÓÐ1¸ö¿Õ¸ñ)
********************************************************************/
void app_startmod_time_display( char *date_and_time )
{
	char date[15] = "";
	char hour_min[15] = "";

	global_memcpy(date, date_and_time, 11);
	global_memcpy(hour_min, date_and_time+11, 5);
	
	GUI_window_hwin_req( m_3Dwin );
	GUI_set_Bgcolor( Startmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( Startmod_FG_COLOR );
	GUI_SetFront( &GUI_HZK24_EXTASCII );
	GUI_string_display_At( m_3Dwin->win_wide - WIDE_B + 20, 200, date );
	GUI_SetFront( &GUI_F32X64_ASCII );
	GUI_string_display_At( m_3Dwin->win_wide - WIDE_B + 15, 150, hour_min );
	GUI_window_hwin_release( m_3Dwin );
	return;
}


void app_startmod_window_creat( void )
{
	GLOBAL_MEMCLR( m_apparray, sizeof( Extapp_name_t )*30 );
	m_array_pos 	 = 0;
	m_menu_start_pos = 0;
	m_menu_end_pos   = 0;
	GUI_set_Bgcolor( 0 );
	GUI_clean_screen();
	GUI_window_hwin_req( GET_GUIWIN );
	GUI_set_Bgcolor( Startmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( Startmod_FG_COLOR );
	m_3Dwin = Gui_3D_window_creat( 0, 0, LCD_WIDE - 2, LCD_HIGH, "Ö÷²Ëµ¥", WIN_3D | WIN_FILL, GET_GUIWIN );
	GUI_window_hwin_req( m_3Dwin );
	GUI_set_Bgcolor( C_GUI_BlACK );
	GUI_set_Fgcolor( C_GUI_WHITE );
	_txtWIN_paint();
	app_startmod_localapp_obtain();
	app_startmod_Expapp_obtain();
	app_SDbmp_init();
	if ( m_array_pos > ( GRAPH_ROW*GRAPH_LINE ) )
	{
		m_menu_end_pos += ( GRAPH_ROW * GRAPH_LINE );
		_Appgraph_create( m_pTXT, 0, GRAPH_ROW*GRAPH_LINE );
		GUI_SetFront( &GUI_HZK16_EXTASCII );
		m_button_last = Gui_button_creat( m_3Dwin->win_wide - 265, m_3Dwin->win_high - 29, 80, 25, KEY_F5 | BUTTON_PAINT, m_3Dwin );
		GUI_button_title_display( m_button_last, "ÉÏÒ»Ò³" );
		GUI_button_callback_mount( m_button_last, _last_button_callblack );
		m_button_next = Gui_button_creat( m_3Dwin->win_wide - 165, m_3Dwin->win_high - 29, 80, 25, KEY_F4 | BUTTON_PAINT, m_3Dwin );
		GUI_button_title_display( m_button_next, "ÏÂÒ»Ò³" );
		GUI_button_callback_mount( m_button_next, _next_button_callblack );
	}
	else
	{
		m_menu_end_pos =   m_array_pos;
		_Appgraph_create( m_pTXT, 0, m_array_pos );
	}
	_ID_VER_display();
	GUI_SetFront( &GUI_HZK16_EXTASCII );
	GUI_set_Bgcolor( Startmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_string_display_At( m_3Dwin->win_wide - 135, 45, "°´<ÉèÖÃ>¼üÑ¡ÔñµçÁ÷ÊäÈë" );
	GUI_SetFront( &GUI_HZK24_EXTASCII );
	Gui_drop_list_creat( m_3Dwin->win_wide - 135, 80, g_dlsr_option[g_language], SIZE_OF_ARRAY( g_dlsr_option[g_language] ), KEY_SETTING, m_3Dwin );
	GUI_drop_list_member_assign( GUI_Getchild( KEY_SETTING, m_3Dwin ), g_user_arg.DL_app_user_set.dlsr );
	GUI_window_hwin_release( m_3Dwin );
	GUI_set_Bgcolor( Startmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( Startmod_FG_COLOR );
	m_battery2_win	=	GUI_edit_creat( 615, 435, 20, 40, 0, GET_GUIWIN );
	m_external2_win	=	GUI_edit_creat( 575, 435, 20, 40, 0, GET_GUIWIN );
	m_charge2_win	=	GUI_edit_creat( 595, 435, 20, 40, 0, GET_GUIWIN );
	GUI_window_hwin_release( GET_GUIWIN );
	return;
}


static void app_startmod_localapp_obtain( void )
{
	app_startmod_Localapp( "»ùÆµÎó²î","ERRUP.bin",   "ERRP.bin",     "TERR" );
	app_startmod_Localapp( "µç±í²âÊÔ","RMETERUP.bin","RMETERP.bin",  "RMETER" );
	app_startmod_Localapp( "²ÎÊýÐ£Ñé","PECUP.bin",   "PECP.bin",     "DLPARAM" );
	app_startmod_Localapp( "ÐéÄâµç±í","PECUP.bin",   "PECP.bin",     "VMETER" );
	app_startmod_Localapp( "Îó²îÐÞÕý","PECUP.bin",   "PECP.bin",     "CORRECT" );
	app_startmod_Localapp( "ÎÄ¼þ¹ÜÀí","PECUP.bin",   "PECP.bin",     "FIL" );
	return;
}



static void SDbmp_read(const char*path)
{
	UINT        w;
	Corlor_t*   image;
	char*       cbuf;
	int         wide = 240, high = 180, pos,wpos,fptr = 0;
	char        ctemp[10] = "";
	FIL* 		fp;
	ExtBmp_t	*p ;
	Corlor_t 	Colos;

	for(pos=0;pos<mBMP_ctl->m_BMPpos;pos++)
	{
		  if(global_memcmp(mBMP_ctl->m_BMP[pos].BMPname,
		  	(char*)path,
		  	global_strlen((char*)path)))
		  {
		      return;
		  }
	}
	fp = fatfs_open( path, FA_OPEN_EXISTING | FA_READ );
	if ( !fp )
	{
		return ;
	}
	p   = &mBMP_ctl->m_BMP[mBMP_ctl->m_BMPpos++];
	image = p->BMP;
	GLOBAL_MEMCLR(p->BMPname,20);
	global_strcpy(p->BMPname,(char*)path);
	f_read( fp, ctemp, 8, &w );
	wide = ( int )ctemp[3] * 0x100 + ctemp[2];
	high = ( int )ctemp[5] * 0x100 + ctemp[4];
	p->wide = wide;
	p->high = high;
	cbuf   = app_malloc( wide * 2 );
	for ( pos = 0;pos < high;pos++ )
	{
		f_read( fp, cbuf, wide*2, &w );
		if ( w == wide*2 )
		{
			for ( wpos = 0;wpos < wide;wpos++ )
			{
				Colos         = ( Corlor_t )cbuf[wpos*2+1] * 0x100;
				image[fptr++] = Colos + cbuf[wpos*2];
			}
		}
		BUS_WRITE(200,0x408f);
		BUS_WRITE(200,0x4090);
	}
	fatfs_close( fp );
	app_free( cbuf );
	return;
}


void  app_SDbmp_init(void)
{
    int pos;
	Extapp_name_t*pCUR;
    mBMP_ctl =(void*)0x120000;
	if(mBMP_ctl->head1==0x68&&mBMP_ctl->head2==0x86) return;
	mBMP_ctl->head1=0x68;
	mBMP_ctl->head2=0x86;
	mBMP_ctl->m_BMPpos = 0;
	GUI_waitWIN_create("ÕýÔÚµ¼Èë¹¦ÄÜ...");
	for ( pos = 0;pos < m_array_pos;pos++ )
	{
	    pCUR = &m_apparray[pos];
        SDbmp_read(pCUR->UNpressBMP);
	}
	GUI_waitWIN_destory();
	return;
}
