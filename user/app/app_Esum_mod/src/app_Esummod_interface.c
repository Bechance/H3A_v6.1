/*************************************************************************
*
* ģ������:ģ����溯������
* ����ʱ��:
* ������˾ :����
* �ļ�����:app_Esummodmod_interface.c
* @������:�ҳ�
* ��������:�ṩģ����滭������API
***************************************************************************/
#include "app_Esummod_inside.h"
hwin_t*   m_Esummod_3DWIN 		= 0;
hwin_t*   m_Esummod_DL_win      = 0;
hwin_t*   m_Esummod_edit        = 0;


/********************************************************************
 * @������ :�ҳ�
 * @���� :�������ʾ���ڳ�ʼ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_Esum_DLmember_init(hwin_t*hp)
{
#define   DL_XLAB 15
#define   DL_GAP  90
#define   DL_VGAP 32
	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(m_GUI_3D_window_Bg_cor);
    GUI_fill_window_area(4,2,hp->win_wide,DL_VGAP);
    GUI_set_Fgcolor(ESummod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(24+DL_XLAB+DL_GAP*0, 2, "U(V)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*1, 2, "I(A)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*2, 2, "��(��)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*3, 2, "P(W)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*4, 2, "Q(Var)" );
	GUI_string_display_At(30+DL_XLAB+DL_GAP*5, 2, "��ABC(��)" );

	GUI_set_Bgcolor(ESummod_WIN_BG_COLOR);
    GUI_set_Fgcolor(ESummod_FG_COLOR);
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
	GUI_set_Fgcolor(ESummod_FG_COLOR);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*4, 7, "",  MEMBER_3D | EDIT_PABC);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*4, 7, "",  MEMBER_3D | EDIT_QABC);
	GUI_edit_memeber_add(2,DL_VGAP*4,7,   "cos��", MEMBER_3D | EDIT_COS );
	GUI_edit_memeber_add(159,DL_VGAP*4,6, "F", MEMBER_3D | EDIT_FEQ);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    GUI_string_display_At(DL_XLAB+DL_GAP*3-16,DL_VGAP*4+4, "��");
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*1,7, "��Uab", MEMBER_3D | EDIT_JUAB );
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*2,7, "��Uac", MEMBER_3D | EDIT_JUAC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*3,7, "��Ubc", MEMBER_3D | EDIT_JUBC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*4,7, "��Iac", MEMBER_3D | EDIT_JIAC);
	GUI_WINDIS_VLINE(DL_XLAB+DL_GAP*5+5,DL_VGAP*1,DL_VGAP*5);
	GUI_WINDIS_HLINE(0,hp->win_wide,DL_VGAP*5);
	GUI_window_hwin_release(hp);
    return;
}

void app_Esumarg_display(Esum_arg_t*p)
{
    GUI_window_hwin_req(m_Esummod_edit);
	GUI_set_Fgcolor(ESummod_FG_COLOR);
	GUI_edit_string_display_At(p->start_time,10);
	GUI_edit_string_display_At(p->end_time,11);
	GUI_edit_string_display_At(p->BL,12);
	GUI_edit_string_display_At(p->Eg_2,13);
	GUI_edit_string_display_At(p->Eg_1,14);
	GUI_edit_string_display_At(p->second,15);
    GUI_window_hwin_release(m_Esummod_edit);
	return;
}

void app_Esum_edit_create(hwin_t *pOBJ) 
{ 
	hwin_t *hp = PNULL;
	GUI_window_hwin_req(GET_GUIWIN);
  	GUI_set_Bgcolor(ESummod_WIN_BG_COLOR);
	GUI_set_Fgcolor(ESummod_FG_COLOR);
	m_Esummod_edit  = GUI_edit_creat(2,0,630,228,0,pOBJ );
	GUI_window_hwin_req(m_Esummod_edit);
	GUI_set_Bgcolor(ESummod_WIN_BG_COLOR);
	GUI_set_Fgcolor(ESummod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);		
	GUI_edit_memeber_add(90,10+35*2,6, "        ����:", MEMBER_3D | 12);
	GUI_edit_memeber_add(90,10+35*0,21,"������ʼʱ��:", MEMBER_3D | 10);
	GUI_edit_memeber_add(90,10+35*1,21,"���ֽ���ʱ��:", MEMBER_3D | 11);
	GUI_edit_memeber_add(90,10+35*3,8, "�������ֵ���:", MEMBER_3D | 13);
	GUI_edit_memeber_add(90,10+35*4,8, "һ�����ֵ���:", MEMBER_3D | 14);
	GUI_edit_memeber_add(90,10+35*5,8, "��������ʱ��:", MEMBER_3D | 15);
	GUI_string_display_At(353,25+32*5, "��" );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(353,30+32*3, "kW��h" );
	GUI_string_display_At(353,30+32*4, "kW��h" );
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	hp = Gui_button_creat( 50,pOBJ->win_high-35,80+10,35-5,BUTTON_PAINT|KEY_F1, pOBJ );
	GUI_button_title_display( hp, "��ʼ" );
	hp = Gui_button_creat( 50+110,pOBJ->win_high-35,80+10,35-5,BUTTON_PAINT|KEY_F2, pOBJ );
	GUI_button_title_display( hp, "����" );
	hp = Gui_button_creat( 50+220,pOBJ->win_high-35,80+10,35-5,BUTTON_PAINT|KEY_F3, pOBJ );
	GUI_button_title_display( hp, "����" );
	hp = Gui_button_creat( 50+330,pOBJ->win_high-35,80+10,35-5,BUTTON_PAINT|KEY_F3, pOBJ );
	GUI_button_title_display( hp, "�����ϴ�" );
	GUI_window_hwin_release(m_Esummod_edit);
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}
/********************************************************************
 * @���� : �ҳ�
 * @���� : �����ڴ���
 * @���� : NONE
 * @���  :NONE
********************************************************************/
void app_Esummod_window_creat(void)
{
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Bgcolor( ESummod_WIN_BG_COLOR );
	GUI_set_Fgcolor( ESummod_FG_COLOR);
	m_Esummod_3DWIN  = Gui_3D_window_creat(0,0,LCD_WIDE-2,LCD_HIGH,"���ֲ���",WIN_3D|WIN_FILL,0);
	m_Esummod_DL_win = GUI_edit_creat(2,242,630,210,0,m_Esummod_3DWIN);
	GUI_WINDIS_HLINE(0,m_Esummod_3DWIN->win_wide,264);
	app_Esum_DLmember_init(m_Esummod_DL_win);
	app_Esum_edit_create(m_Esummod_3DWIN);
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}

void app_Esummod_window_destroy(void)
{
	GUI_window_destory( m_Esummod_3DWIN );
    return;
}
