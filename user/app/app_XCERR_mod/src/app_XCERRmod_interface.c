#include    "app_XCerrmod_inside.h"
hwin_t*     m_XCERR_3DWIN       = 0;
hwin_t*     m_XCERR_DL_win      = 0;
hwin_t*     m_XCERR_LJT_win     = 0;
hwin_t*     m_XCERR_ERR_win     = 0;

hwin_t*		m_battery_win		= 0;
hwin_t*		m_external_win		= 0;
hwin_t*		m_charge_win		= 0;
/*--  电池 --*/
/*--  宽度x高度=16x32  --*/
const char m_battery_flag_icon[6][64] =
{
	{//空电量
		0x0F,0xF0,0x0F,0xF0,0xFF,0xFF,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
		0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
		0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
		0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0xFF,0xFF, 
	},
	{// 1格电量
		0x0F,0xF0,0x0F,0xF0,0xFF,0xFF,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
		0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
		0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
		0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x80,0x01,0xFF,0xFF, 
	},
	{// 2格电量
		0x0F,0xF0,0x0F,0xF0,0xFF,0xFF,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
		0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
		0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,
		0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x80,0x01,0xFF,0xFF, 
	},
	{// 3格电量
		0x0F,0xF0,0x0F,0xF0,0xFF,0xFF,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
		0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,
		0x9F,0xF9,0x9F,0xF9,0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,
		0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x80,0x01,0xFF,0xFF,
	},
	{// 4格电量
		0x0F,0xF0,0x0F,0xF0,0xFF,0xFF,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
		0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,
		0x9F,0xF9,0x9F,0xF9,0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,
		0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x80,0x01,0xFF,0xFF,
	},
	{// 满格电量
		0x0F,0xF0,0x0F,0xF0,0xFF,0xFF,0x80,0x01,0x9F,0xF9,0x9F,0xF9,0x80,0x01,0x80,0x01,
		0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,
		0x9F,0xF9,0x9F,0xF9,0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,
		0x80,0x01,0x80,0x01,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x9F,0xF9,0x80,0x01,0xFF,0xFF,
	},
};

/*--  开关电源 --*/
/*--  宽度x高度=16x32  --*/
const char m_extral_flag_icon[64] = 
{
	0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7F,0xFE,0x7B,0xDE,
	0x7B,0xDE,0x78,0x1E,0x3F,0xFC,0x07,0xE0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,
	0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,
	0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*--  适配器 --*/
/*--  宽度x高度=16x32  --*/
const char m_adapter_flag_icon[64] = 
{
	0x00,0x00,0x00,0x00,0x07,0xE0,0x04,0x20,0x07,0xE0,0x04,0x20,0x04,0x20,0x04,0x20,
	0x3F,0xFC,0x3F,0xFC,0x3F,0xFC,0x20,0x04,0x3F,0xFC,0x1F,0xF8,0x18,0x18,0x07,0xE0,
	0x07,0xE0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,
	0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x00,0x00,0x00,0x00,
};

/*--  充电状态标志 --*/
/*--  宽度x高度=16x32  --*/
const char m_charge_flag_icon[64] = 
{
	0x00,0x00,0x00,0x7F,0x00,0xFF,0x01,0xFE,0x03,0xFC,0x07,0xF8,0x0F,0xF0,0x1F,0xE0,
	0x3F,0xC0,0x7F,0xFF,0xFF,0xFE,0xFF,0xFC,0x00,0xF8,0x01,0xF0,0x03,0xE0,0x07,0xC0,
	0x0F,0x80,0x1F,0x00,0x3E,0x00,0x7C,0x00,0xF8,0x00,0xF0,0x00,0xE0,0x00,0xC0,0x00,
	0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/********************************************************************
 * @作者 : 周炳权
 * @功能 : 电池标志显示
 * @输入 : NONE
 * @输出 : NONE
********************************************************************/
#define  PEXT_Y    			5
void app_XCERR_battery_display( void )
{
	Bsize_t    num = 1;
	num = g_battery_num;
	GUI_window_hwin_req( m_battery_win );
	GUI_set_Bgcolor( XCerrmod_WIN_BG_COLOR );
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
			GUI_set_Fgcolor( XCerrmod_WIN_BG_COLOR );
			GUI_libASC_display_At( 1, PEXT_Y, 16, 32, ( char* )0 );
			GUI_window_hwin_release( m_battery_win );
			return;
		}
	}
	GUI_libASC_display_At( 1, PEXT_Y, 16, 32, ( char* )m_battery_flag_icon[num] );
	GUI_window_hwin_release( m_battery_win );
	return;
}

/********************************************************************
 * @作者 : 周炳权
 * @功能 : 电池充电标志显示
 * @输入 : NONE
 * @输出 : NONE
********************************************************************/
void app_XCERR_charge_display( void )
{
	GUI_window_hwin_req( m_charge_win );
	GUI_set_Bgcolor( XCerrmod_WIN_BG_COLOR );
	if ( g_battery_charge == 0 )
		GUI_set_Fgcolor( C_GUI_WHITE );
	else
		GUI_set_Fgcolor( C_GUI_GREEN );
	
	GUI_libASC_display_At( 2, 10, 16, 32, ( char* )m_charge_flag_icon );
	GUI_window_hwin_release( m_charge_win );
	return;
}

/********************************************************************
 * @作者 : 周炳权
 * @功能 : 外部电源标志显示
 * @输入 : NONE
 * @输出 : NONE
********************************************************************/
void app_XCERR_external_display( void )
{
	GUI_window_hwin_req( m_external_win );
	GUI_set_Bgcolor( XCerrmod_WIN_BG_COLOR );
	if ( g_cur_power_up == 0 )
	GUI_set_Fgcolor( C_GUI_GREEN );
	else GUI_set_Fgcolor( C_GUI_WHITE );
	
	if ( g_power_state == PS_ADAPTER || g_power_state == PS_ADAPTER_BATTERY ||
		g_power_state == PS_ADAPTER_EXTRAL || 
		g_power_state == PS_ADAPTER_EXTRAL_BATTERY )
	{
		GUI_libASC_display_At( 2, 7, 16, 32, ( char* )m_adapter_flag_icon );
	}
	else if ( g_power_state == PS_EXTRAL || 
			g_power_state == PS_EXTRAL_BATTERY ||
			g_power_state == PS_PA_EXTRAL || 
			g_power_state == PS_PA_EXTRAL_BATTERY )
	{
		GUI_libASC_display_At( 2, 7, 16, 32, ( char* )m_extral_flag_icon );
	}
	else
	{
		GUI_set_Fgcolor( XCerrmod_WIN_BG_COLOR );
		GUI_libASC_display_At( 2, 7, 16, 32, ( char* )0 );
	}
	GUI_window_hwin_release( m_external_win );
	return;
}

/********************************************************************
 * @作者 : 周炳权
 * @功能 : 适配器异常保护显示
 * @输入 : NONE
 * @输出 : NONE
********************************************************************/

void app_XCERR_adapter_protect_display( void )
{
	if ( g_power_state == PS_PA_BATTERY|| 
		 g_power_state == PS_PA_EXTRAL || 
		 g_power_state == PS_PA_EXTRAL_BATTERY )
	{
		MsgBox(200,130,250,200,(char*)m_XCERRmod_MSG_17[g_language],(char*)m_XCERRmod_MSG_24_1[g_language],APP_HALF_SEC*15);
	}
	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示窗口初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_XCERR_DLmember_window_init(hwin_t*hp)
{
#define   DL_XLAB 15
#define   DL_GAP  90
#define   DL_VGAP 32
	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(m_GUI_3D_window_Bg_cor);
    GUI_fill_window_area(4,2,hp->win_wide,DL_VGAP);
    GUI_set_Fgcolor(XCerrmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(24+DL_XLAB+DL_GAP*0, 2, "U(V)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*1, 2, "I(A)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*2, 2, "Φ(°)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*3, 2, "P(W)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*4, 2, "Q(Var)" );
	GUI_string_display_At(30+DL_XLAB+DL_GAP*5, 2, "ΦABC(°)" );

	GUI_set_Bgcolor(XCerrmod_WIN_BG_COLOR);
    GUI_set_Fgcolor(XCerrmod_FG_COLOR);
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_string_display_At(2,DL_VGAP*1+2, "A" );
	GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_string_display_At(2,DL_VGAP*2+2, "B" );
	GUI_set_Fgcolor( C_GUI_RED );
	GUI_string_display_At(2,DL_VGAP*3+2, "C" );
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_U_A);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_U_B);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_U_C);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_I_A);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_I_B);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_I_C);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_J_A);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_J_B);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_J_C);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_P_A);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_P_B);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_P_C);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_Q_A);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_Q_B);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_Q_C);
	GUI_set_Fgcolor(XCerrmod_FG_COLOR);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*4, 7, "",  MEMBER_3D | EDIT_PABC);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*4, 7, "",  MEMBER_3D | EDIT_QABC);
	GUI_edit_memeber_add(2,DL_VGAP*4,7,   "cosΦ", MEMBER_3D | EDIT_COS );
	GUI_edit_memeber_add(159,DL_VGAP*4,6, "F", MEMBER_3D | EDIT_FEQ);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    GUI_string_display_At(DL_XLAB+DL_GAP*3-16,DL_VGAP*4+4, "∑");
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*1,7, "ΦUab", MEMBER_3D | EDIT_JUAB );
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*2,7, "ΦUac", MEMBER_3D | EDIT_JUAC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*3,7, "ΦUbc", MEMBER_3D | EDIT_JUBC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*4,7, "ΦIac", MEMBER_3D | EDIT_JIAC);
	GUI_WINDIS_VLINE(DL_XLAB+DL_GAP*5+5,DL_VGAP*1,DL_VGAP*5);
	GUI_WINDIS_HLINE(0,hp->win_wide,DL_VGAP*5);
	GUI_window_hwin_release(hp);
    return;
}



void app_XCerrmod_window_creat(void)
{
    hwin_t*pOBJ;
	GUI_window_hwin_req(GET_GUIWIN);
	//_GetRGB();
	GUI_set_Bgcolor(XCerrmod_WIN_BG_COLOR);    
	GUI_set_Fgcolor(XCerrmod_FG_COLOR);  
    m_XCERR_3DWIN  = Gui_3D_window_creat(0,0,LCD_WIDE-2,LCD_HIGH,(char*)m_XCERRmod_MSG_37[g_language],WIN_3D|WIN_FILL,GET_GUIWIN);
	GUI_window_hwin_req(m_XCERR_3DWIN);
	GUI_set_Bgcolor(XCerrmod_BG_COLOR);   
	m_XCERR_DL_win  = GUI_edit_creat(2,242,630,210,0,m_XCERR_3DWIN);
	m_XCERR_ERR_win = GUI_edit_creat(2,0,630,242,0,m_XCERR_3DWIN);
	GUI_WINDIS_HLINE(0,m_XCERR_3DWIN->win_wide,244);
	app_XCERR_DLmember_window_init(m_XCERR_DL_win);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    pOBJ=Gui_button_creat(25+110*0,m_XCERR_3DWIN->win_high-38,108,35,BUTTON_PAINT,m_XCERR_3DWIN);
	GUI_button_title_display(pOBJ,(char*)m_XCERRmod_MSG_38[g_language]);
    pOBJ=Gui_button_creat(25+110*1,m_XCERR_3DWIN->win_high-38,108,35,BUTTON_PAINT,m_XCERR_3DWIN);
	GUI_button_title_display(pOBJ,(char*)m_XCERRmod_MSG_39[g_language]);
    pOBJ=Gui_button_creat(25+110*2,m_XCERR_3DWIN->win_high-38,108,35,BUTTON_PAINT,m_XCERR_3DWIN);
	GUI_button_title_display(pOBJ,(char*)m_XCERRmod_MSG_40[g_language]);
    pOBJ=Gui_button_creat(25+110*3,m_XCERR_3DWIN->win_high-38,108,35,BUTTON_PAINT,m_XCERR_3DWIN);
	GUI_button_title_display(pOBJ,(char*)m_XCERRmod_MSG_41[g_language]);
    pOBJ=Gui_button_creat(25+110*4,m_XCERR_3DWIN->win_high-38,108,35,BUTTON_PAINT,m_XCERR_3DWIN);
	GUI_button_title_display(pOBJ,(char*)m_XCERRmod_MSG_42[g_language]);
	GUI_window_hwin_release(m_XCERR_3DWIN);
	GUI_window_hwin_release(GET_GUIWIN);
	GUI_set_Bgcolor( XCerrmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( XCerrmod_FG_COLOR );
	m_battery_win	=	GUI_edit_creat( 615, 435, 20, 40, 0, GET_GUIWIN );
	m_external_win	=	GUI_edit_creat( 575, 435, 20, 40, 0, GET_GUIWIN );
	m_charge_win	=	GUI_edit_creat( 595, 435, 20, 40, 0, GET_GUIWIN );
	return;
}


void app_XCerrmod_record_detail_win_creat(void)
{
    hwin_t*pOBJ;
	GUI_window_hwin_req(GET_GUIWIN);
	//_GetRGB();
	GUI_set_Bgcolor(XCerrmod_WIN_BG_COLOR);    
	GUI_set_Fgcolor(XCerrmod_FG_COLOR);  
    m_XCERR_3DWIN  = Gui_3D_window_creat(0,0,LCD_WIDE-2,LCD_HIGH,(char*)m_XCERRmod_MSG_37[g_language],WIN_3D|WIN_FILL,GET_GUIWIN);
	GUI_window_hwin_req(m_XCERR_3DWIN);
	GUI_set_Bgcolor(XCerrmod_BG_COLOR);   
	m_XCERR_DL_win  = GUI_edit_creat(2,242,630,210,0,m_XCERR_3DWIN);
	m_XCERR_ERR_win = GUI_edit_creat(2,0,630,242,0,m_XCERR_3DWIN);
	GUI_WINDIS_HLINE(0,m_XCERR_3DWIN->win_wide,244);
	app_XCERR_DLmember_window_init(m_XCERR_DL_win);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    pOBJ=Gui_button_creat(25+110*0,m_XCERR_3DWIN->win_high-38,108,35,BUTTON_PAINT,m_XCERR_3DWIN);
	GUI_button_title_display(pOBJ,(char*)m_XCERRmod_MSG_38[g_language]);
    pOBJ=Gui_button_creat(25+110*1,m_XCERR_3DWIN->win_high-38,108,35,BUTTON_PAINT,m_XCERR_3DWIN);
	GUI_button_title_display(pOBJ,(char*)m_XCERRmod_MSG_39[g_language]);
    pOBJ=Gui_button_creat(25+110*2,m_XCERR_3DWIN->win_high-38,108,35,BUTTON_PAINT,m_XCERR_3DWIN);
	GUI_button_title_display(pOBJ,(char*)m_XCERRmod_MSG_40[g_language]);
    pOBJ=Gui_button_creat(25+110*3,m_XCERR_3DWIN->win_high-38,108,35,BUTTON_PAINT,m_XCERR_3DWIN);
	GUI_button_title_display(pOBJ,(char*)m_XCERRmod_MSG_41[g_language]);
    pOBJ=Gui_button_creat(25+110*4,m_XCERR_3DWIN->win_high-38,108,35,BUTTON_PAINT,m_XCERR_3DWIN);
	GUI_button_title_display(pOBJ,(char*)m_XCERRmod_MSG_44[g_language]);

	//为打印补序几条打印边框
	GUI_WINDIS_HLINE(0,635,274);
	GUI_WINDIS_HLINE(2,635,35);
	GUI_WINDIS_VLINE(633,0,m_XCERR_3DWIN->win_high);
	GUI_WINDIS_VLINE(634,0,m_XCERR_3DWIN->win_high);
	
	GUI_window_hwin_release(m_XCERR_3DWIN);
	GUI_window_hwin_release(GET_GUIWIN);
	GUI_set_Bgcolor( XCerrmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( XCerrmod_FG_COLOR );
	m_battery_win	=	GUI_edit_creat( 615, 435, 20, 40, 0, GET_GUIWIN );
	m_external_win	=	GUI_edit_creat( 575, 435, 20, 40, 0, GET_GUIWIN );
	m_charge_win	=	GUI_edit_creat( 595, 435, 20, 40, 0, GET_GUIWIN );
	return;
}



void app_XCerrmod_window_destory(void)
{
   GUI_window_destory(m_XCERR_3DWIN);
   GUI_window_destory(m_battery_win);
   GUI_window_destory(m_external_win);
   GUI_window_destory(m_charge_win);
   return;
}


