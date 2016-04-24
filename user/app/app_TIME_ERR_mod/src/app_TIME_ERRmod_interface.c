#include    "app_TIME_errmod_inside.h"
hwin_t*     m_TIME_ERR_3DWIN = 0;
hwin_t*     m_TIME_ERR_main_win = 0;
hwin_t*     m_TIME_ERR_DL_win = 0;
hwin_t*     m_TIME_ERR_LJT_win = 0;

/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示窗口初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_TIME_ERR_DLmember_window_init(hwin_t*hp)
{
#define   DL_XLAB 15
#define   DL_GAP  90
#define   DL_VGAP 32
//    hwin_t*pOBJ;
	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(m_GUI_3D_window_Bg_cor);
    GUI_fill_window_area(4,2,hp->win_wide,DL_VGAP);
    GUI_set_Fgcolor(TIME_errmod_FG_COLOR);
	
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(24+DL_XLAB+DL_GAP*0, 2, "U(V)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*1, 2, "I(A)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*2, 2, "Φ(°)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*3, 2, "P(W)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*4, 2, "Q(var)" );
	GUI_string_display_At(30+DL_XLAB+DL_GAP*5, 2, "ΦABC(°)" );
	
	GUI_set_Bgcolor(TIME_errmod_WIN_BG_COLOR);
    GUI_set_Fgcolor(TIME_errmod_FG_COLOR);
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_string_display_At(2,DL_VGAP*1+2, "A" );
	GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_string_display_At(2,DL_VGAP*2+2, "B" );
	GUI_set_Fgcolor( C_GUI_RED );
	GUI_string_display_At(2,DL_VGAP*3+2, "C" );

	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_U_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_U_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_U_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_I_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_I_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_I_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_J_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_J_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_J_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_P_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_P_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_P_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*1, 7, "", MEMBER_3D | EDIT_Q_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*2, 7, "", MEMBER_3D | EDIT_Q_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*3, 7, "", MEMBER_3D | EDIT_Q_C |EDIT_DIS);
	GUI_set_Fgcolor(TIME_errmod_FG_COLOR);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*4,7, "",  MEMBER_3D | EDIT_PABC |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*4,7, "",  MEMBER_3D | EDIT_QABC |EDIT_DIS);
	GUI_edit_memeber_add(2,DL_VGAP*4,7,   "cosΦ", MEMBER_3D | EDIT_COS |EDIT_DIS);
	GUI_edit_memeber_add(159,DL_VGAP*4,6, "F", MEMBER_3D | EDIT_FEQ |EDIT_DIS);

	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*1,7, "ΦUab", MEMBER_3D | EDIT_JUAB );
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*2,7, "ΦUac", MEMBER_3D | EDIT_JUAC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*3,7, "ΦUbc", MEMBER_3D | EDIT_JUBC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*4,7, "ΦIac", MEMBER_3D | EDIT_JIAC);
	
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    GUI_string_display_At(DL_XLAB+DL_GAP*3-16,DL_VGAP*4+4, "∑");
	GUI_window_hwin_release(hp);
    return;
}


void app_TIME_errmod_window_creat(void)
{
    hwin_t*pOBJ;
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Bgcolor(0);
	GUI_clean_screen();
	GUI_set_Bgcolor(TIME_errmod_WIN_BG_COLOR);    
	GUI_set_Fgcolor(TIME_errmod_FG_COLOR);    
    m_TIME_ERR_3DWIN	= Gui_3D_window_creat(0,0,LCD_WIDE-2,LCD_HIGH,"基频误差校验",WIN_3D|WIN_FILL|WIN_CLOSE_ICON_DIS,GET_GUIWIN);
    m_TIME_ERR_main_win	= GUI_edit_creat(2,0,630,242,0,m_TIME_ERR_3DWIN);
    m_TIME_ERR_DL_win   = GUI_edit_creat(2,242,630,210,0,m_TIME_ERR_3DWIN );
    app_TIME_ERR_DLmember_window_init(m_TIME_ERR_DL_win);
	//GUI_set_Bgcolor(0);    
    //pOBJ=m_TIME_ERR_DL_win;
    GUI_window_hwin_req(m_TIME_ERR_3DWIN);
	GUI_set_Bgcolor(TIME_errmod_WIN_BG_COLOR);    
	GUI_set_Fgcolor(TIME_errmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	pOBJ=Gui_button_creat(20,m_TIME_ERR_3DWIN->win_high-39,135,34,BUTTON_PAINT,m_TIME_ERR_3DWIN);
	GUI_button_title_display(pOBJ,"暂停/开始");
	GUI_window_hwin_release(m_TIME_ERR_3DWIN);
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}



void app_TIME_errmod_window_destroy(void)
{
  	GUI_window_destory(m_TIME_ERR_3DWIN);
	return;
}


