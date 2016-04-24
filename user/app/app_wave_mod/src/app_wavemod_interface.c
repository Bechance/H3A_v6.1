/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块界面函数集合
* 创建时间:2010-11-19
* 所属公司 :科荟
* 文件名称:app_wavemod_interface.c
* 创建人:揭成
* 功能描述:提供系统设置界面画屏函数API
******************************************************************************************/
#include"app_wavemod_inside.h"
hwin_t*	m_Wave3DWIN  		= PNULL;
hwin_t*	m_WaveWIN    		= PNULL;
hwin_t*	m_WaveDL    		= PNULL;

hwin_t*	m_UIcheckbox[6]     = {PNULL,};
const   uint16_t    m_WaveCor[6] = {
	C_GUI_YELLOW,C_GUI_AQUA,
	C_GUI_GREEN,C_GUI_FUCHSIA,
	C_GUI_RED,C_GUI_ORANGE};
	
const   char*  m_checkboxname[6] = {"   Ua","   Ia","   Ub","   Ib","   Uc","0-Ic"};

#define   REC_LAB     20
#define   REC_XS      (m_Wave3DWIN->win_x+REC_LAB)
#define   REC_YS      (m_Wave3DWIN->win_y+5)
#define   REC_XE      (REC_XS+500)
#define   REC_YE      (REC_YS+400)

#define   LBK_CORLOR   C_GUI_WHITE

/********************************************************************
 * @创建人 :揭成
 * @功能 :画波形显示窗口虚线
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_wavemod_window_line_draw(hwin_t*pWIN)
{
#define  GAP_PIX    	50
#define  PIX_DIV    	10   // GAP_PIX/5
	int x,y,n;
	GUI_window_hwin_req( pWIN );
	GUI_set_Fgcolor(LBK_CORLOR);
    for(n=0;n<4;n++)
    {
		for (y = REC_YS+PIX_DIV; y <REC_YE; y+= PIX_DIV)
			GUI_draw_dot(REC_XS+GAP_PIX*n+GAP_PIX,y);
		for (y = REC_YS+PIX_DIV; y <REC_YE; y+= PIX_DIV)
			GUI_draw_dot(REC_XS+GAP_PIX*n+GAP_PIX*6,y);
    }
	for (y = REC_YS+PIX_DIV; y <REC_YE; y+= PIX_DIV)
		GUI_DRAW_HLINE(REC_XS+GAP_PIX*5-1,REC_XS+GAP_PIX*5+1,y);
    for(n=0;n<3;n++)
    {
    	for (x = REC_XS+PIX_DIV; x <REC_XE; x += PIX_DIV)
			GUI_draw_dot(x,REC_YS+GAP_PIX+n*GAP_PIX);
    	for (x = REC_XS+PIX_DIV; x <REC_XE; x += PIX_DIV)
			GUI_draw_dot(x,REC_YS+GAP_PIX*5+n*GAP_PIX);
    }
	for (x = REC_XS+PIX_DIV; x <REC_XE; x += PIX_DIV)
		GUI_DRAW_VLINE(x,REC_YE-GAP_PIX*4+1,REC_YE-GAP_PIX*4-1);
	
	for (x = REC_XS+GAP_PIX; x < REC_XE; x += GAP_PIX)
	{
		GUI_DRAW_VLINE(x,REC_YE-5,REC_YE+1);
		GUI_DRAW_VLINE(x,REC_YS+1,REC_YS+7);
	}
	for (y = REC_YE-GAP_PIX;y>REC_YS;y-= GAP_PIX) 
	{
		GUI_DRAW_HLINE(REC_XS,REC_XS+5,y);
		GUI_DRAW_HLINE(REC_XE-5,REC_XE,y);
	}
	GUI_window_hwin_release( pWIN );
    return;
}




static void _UDL_window_create(int x ,int y)
{
#define LAB  5
#define YGAP 37
	hwin_t*pOBJ; 	
	pOBJ = GUI_edit_creat(x,y,130,400,0,m_Wave3DWIN);
	GUI_window_hwin_req(pOBJ);
	GUI_set_Fgcolor( WAVE_FG_COLOR );
	GUI_set_Bgcolor( WAVE_WIN_BG_COLOR );
    GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_edit_memeber_add(0,LAB+YGAP*0, 6, " UA",  MEMBER_3D | EDIT_U_A);
	GUI_edit_memeber_add(0,LAB+YGAP*1, 6, " IA",  MEMBER_3D | EDIT_I_A);
	GUI_edit_memeber_add(0,LAB+YGAP*2, 6, " UB",  MEMBER_3D | EDIT_U_B);
	GUI_edit_memeber_add(0,LAB+YGAP*3, 6, " IB",  MEMBER_3D | EDIT_I_B);
	GUI_edit_memeber_add(0,LAB+YGAP*4, 6, " UC",  MEMBER_3D | EDIT_U_C);
	GUI_edit_memeber_add(0,LAB+YGAP*5, 6, " IC",  MEMBER_3D | EDIT_I_C);	
	GUI_edit_memeber_add(0,LAB+YGAP*6, 6, "  A",  MEMBER_3D | EDIT_J_A);
	GUI_edit_memeber_add(0,LAB+YGAP*7, 6, "  B",  MEMBER_3D | EDIT_J_B);
	GUI_edit_memeber_add(0,LAB+YGAP*8, 6, "  C",  MEMBER_3D | EDIT_J_C);
	GUI_edit_memeber_add(0,LAB+YGAP*9, 6, "  F",  MEMBER_3D | EDIT_FEQ);	

    GUI_string_display_At(4,LAB+YGAP*6+4,"Φ");
    GUI_string_display_At(4,LAB+YGAP*7+4,"Φ");
    GUI_string_display_At(4,LAB+YGAP*8+4,"Φ");

	m_WaveDL = pOBJ;
	GUI_window_hwin_release(pOBJ);
    return;
}


static void _UIcheckbox_create(int x,int y,int pos)
{
	hwin_t*pOBJ = m_Wave3DWIN; 	
	GUI_window_hwin_req(pOBJ);
    GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor( WAVE_WIN_BG_COLOR );
    m_UIcheckbox[pos]  =GUI_checkBox_create(m_checkboxname[pos],m_WaveCor[pos],x,y,WIN_TOUCH_EN|GUI_KEY_UA,pOBJ);
    m_UIcheckbox[pos+1]=GUI_checkBox_create(m_checkboxname[pos+1],m_WaveCor[pos+1],x+100,y,WIN_TOUCH_EN|GUI_KEY_IA,pOBJ);
    GUI_checkBox_select(m_UIcheckbox[0],1);
	GUI_window_hwin_release(pOBJ);
    return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :USB主窗口
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
void app_wavemod_window_creat(void)
{
//	hwin_t*pOBJ; 	
	int x,y,pos,V;
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Fgcolor( WAVE_FG_COLOR );
	GUI_set_Bgcolor( WAVE_WIN_BG_COLOR );
    m_Wave3DWIN 
	= Gui_3D_window_creat(0,0,LCD_WIDE-2,LCD_HIGH,"波形显示",WIN_3D|WIN_FILL,0);
	GUI_set_Bgcolor( 0 );
    m_WaveWIN =Gui_txtbox_creat(REC_XS-3,REC_YS,REC_XE-REC_XS,REC_YE-REC_YS+1,WIN_FILL|WIN_3D,GET_GUIWIN);
    _UIcheckbox_create(56+100*0-25,  m_Wave3DWIN->win_high-30,0);
    _UIcheckbox_create(56+100*2-20,m_Wave3DWIN->win_high-30,2);
    _UIcheckbox_create(56+100*4-20,m_Wave3DWIN->win_high-30,4);
    _UDL_window_create(517,25);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor( WAVE_FG_COLOR );
	GUI_set_Bgcolor( WAVE_WIN_BG_COLOR );
    y = REC_YS-7;
	V = 3;
	for(pos=1;pos<8;pos++,V--)
	{ 
	    if(V>=0)x = REC_XS-13;
		else    x = REC_XS-21;
    	GUI_int_display_At(x,y+50*pos,V);
	}
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :销毁波形窗口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_wavemod_window_destory(void)
{
   GUI_window_destory(m_Wave3DWIN);
   return;
}
