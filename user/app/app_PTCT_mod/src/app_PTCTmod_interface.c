/*************************************************************************
* 模块名称:PTCT模块界面函数
* 创建时间:
* 所属公司:科荟
* 文件名称:app_PTCTmod_interface.c
* 创建人  :
* 功能描述:提供变比模块界面画屏函数API
***************************************************************************/
#include "app_PTCTmod_inside.h"
static hwin_t* _WIN  		 = PNULL;
hwin_t* m_PTdrop      = PNULL;
/********************************************************************
* @创建人 :揭成
* @功能   :PTCT模块主界面
* @输入   :NONE
* @输出   :NONE
********************************************************************/
void app_PTCTmod_window_main_creat(hwin_t**edit)
{
#define XWAP  	110
#define YWAP  	40
#define YLAB 	40
    hwin_t*	 pOBJ = PNULL;
	uint8_t  offset;
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Bgcolor( PTCTmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( PTCTmod_FG_COLOR );
	_WIN = Gui_3D_window_creat(0,0,LCD_WIDE-2,LCD_HIGH,"TV二次负荷,TA二次负载测量", WIN_3D | WIN_FILL, GET_GUIWIN );
	GUI_window_hwin_req( _WIN );
	pOBJ=GUI_edit_creat(20,30,600,_WIN->win_high,0, _WIN );
	GUI_window_hwin_req( pOBJ );
	GUI_set_Bgcolor( PTCTmod_WIN_BG_COLOR );
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Fgcolor( C_GUI_YELLOW);
	GUI_string_display_At(15,YLAB,"A");
	GUI_set_Fgcolor( C_GUI_GREEN);
	GUI_string_display_At(15,YLAB+YWAP,"B");
	GUI_set_Fgcolor( C_GUI_RED);
	GUI_string_display_At(15,YLAB+YWAP*2,"C");
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Fgcolor(PTCTmod_FG_COLOR);
	GUI_string_display_At(40+XWAP*0+24,12,"U(V)");
	GUI_string_display_At(40+XWAP*1+20,12,"I(A)");
	GUI_string_display_At(40+XWAP*2+10,12,"Φ(°)");
	GUI_string_display_At(40+XWAP*3+21,12,"P(W)");
	GUI_string_display_At(40+XWAP*4+10,12,"Q(Var)");
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	for(offset=0;offset<5;offset++)
	{
		GUI_edit_memeber_add(30+XWAP*offset, YLAB, 			8, "", MEMBER_3D |1+offset*3);
		GUI_edit_memeber_add(30+XWAP*offset, YLAB+YWAP, 	8, "", MEMBER_3D |2+offset*3);
		GUI_edit_memeber_add(30+XWAP*offset, YLAB+YWAP*2, 	8, "", MEMBER_3D |3+offset*3);
	}
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Fgcolor( C_GUI_YELLOW);
	GUI_string_display_At(15,20+YLAB+YWAP*4+4,"A");
	GUI_set_Fgcolor( C_GUI_GREEN);
	GUI_string_display_At(15,20+YLAB+YWAP*5+4,"B");
	GUI_set_Fgcolor( C_GUI_RED);
	GUI_string_display_At(15,20+YLAB+YWAP*6+4,"C");
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Fgcolor(PTCTmod_FG_COLOR);
	GUI_string_display_At(30+(XWAP+8)*0+8,30+YWAP*4,"电导(mS)");
	GUI_string_display_At(30+(XWAP+8)*1+8,30+YWAP*4,"电纳(mS)");
	GUI_string_display_At(30+(XWAP+8)*2,  30+YWAP*4,"视在功率(VA)");
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	for(offset=0;offset<3;offset++)
	{
		GUI_edit_memeber_add(30+(XWAP+8)*offset, 20+YLAB+YWAP*4, 9, "", MEMBER_3D |16+offset*3);
		GUI_edit_memeber_add(30+(XWAP+8)*offset, 20+YLAB+YWAP*5, 9, "", MEMBER_3D |17+offset*3);
		GUI_edit_memeber_add(30+(XWAP+8)*offset, 20+YLAB+YWAP*6, 9, "", MEMBER_3D |18+offset*3);
	}
	GUI_set_Fgcolor(PTCTmod_FG_COLOR);
	GUI_edit_memeber_add(30+(XWAP+8)*3,20+YLAB+YWAP*6, 9,"F", MEMBER_3D |EDIT_FREQ);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor( C_GUI_WHITE);
	GUI_WINDIS_HLINE(0,pOBJ->win_wide,30+YWAP*4-10);
	GUI_window_hwin_release( pOBJ );
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Fgcolor( C_GUI_GREEN);
	GUI_string_display_At(40,375, "键盘操作提示 - ->" );
	GUI_set_Fgcolor( C_GUI_YELLOW);
	GUI_string_display_At(40,410, "按<F2>选择电流输入,按<取消>返回主界面." );

	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Fgcolor( C_GUI_WHITE);
	m_PTdrop=Gui_drop_list_creat(90,5,g_dlsr_option[g_language],
					SIZE_OF_ARRAY(g_dlsr_option[g_language]),
					KEY_F2,_WIN);
	GUI_set_Fgcolor( C_GUI_WHITE);
	GUI_string_display_At(40,7, "输入");

	GUI_window_hwin_release(_WIN);
	*edit = pOBJ;
	GUI_window_hwin_release(GET_GUIWIN);
	return ;
}


void app_PTCTmod_window_main_destory(void)
{
	GUI_window_destory(_WIN);
	return;
}
