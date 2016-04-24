/*************************************************************************
*
* 模块名称:selfCOR模块界面函数集合
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_BZmod_interface.c
* @创建人:揭成
* 功能描述:提供变比模块界面画屏函数API
***************************************************************************/
#include "app_BZ_inside.h"
hwin_t*   m_BZ_WIN      = 0;
hwin_t*   m_BZ_Useredit = 0;
hwin_t*   m_BZ_DLedit   = 0;
/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示窗口初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_BZmod_DLmember_window_init(hwin_t*hp)
{
#define   DL_XLAB 15
#define   DL_GAP  90
#define   DL_VGAP 32
	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(m_GUI_3D_window_Bg_cor);
    GUI_fill_window_area(4,2,hp->win_wide,DL_VGAP);
    GUI_set_Fgcolor(BZmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(24+DL_XLAB+DL_GAP*0, 2, "U(V)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*1, 2, "I(A)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*2, 2, "Φ(°)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*3, 2, "P(W)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*4, 2, "Q(Var)" );
	GUI_string_display_At(30+DL_XLAB+DL_GAP*5, 2, "ΦABC(°)" );

	GUI_set_Bgcolor(BZmod_WIN_BG_COLOR);
    GUI_set_Fgcolor(BZmod_FG_COLOR);
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
	GUI_set_Fgcolor(BZmod_FG_COLOR);
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
/********************************************************************
 * @作者 : 揭成
 * @功能 : 功率校正窗口
 * @输入 : NONE
 * @输出  :NONE
********************************************************************/
void app_BZmod_window_creat(void)
{
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Bgcolor( BZmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( BZmod_FG_COLOR );
	m_BZ_WIN  = Gui_3D_window_creat(0,0,LCD_WIDE-2,LCD_HIGH,"中试误差检测",WIN_3D|WIN_FILL,0);
	GUI_window_hwin_req(m_BZ_WIN);
    m_BZ_Useredit    = GUI_edit_creat(2,2,300,200,0,m_BZ_WIN);
	m_BZ_DLedit      = GUI_edit_creat(2,242,630,210,0,m_BZ_WIN);
    app_BZmod_DLmember_window_init(m_BZ_DLedit);
	GUI_window_hwin_release(m_BZ_WIN);
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}


void app_BZmod_window_destroy(void)
{
	GUI_window_destory( m_BZ_WIN );
    return;
}
