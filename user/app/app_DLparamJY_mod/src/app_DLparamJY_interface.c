/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:模块界面函数集合
* 创建时间:2013年1月7日
* 所属公司:科荟
* 文件名称:app_DLparamJYmod_fun.c
* 作者    :周炳权
* 功能描述:提供变比模块界面画屏函数API
******************************************************************************************/
#include "app_DLparamJY_inside.h"

hwin_t 	*m_DLparamJYmod_win					= PNULL;
hwin_t 	*m_DLparamJYmod_XNFZ_win			= PNULL;
hwin_t  *m_DLparamJYmod_radio_TREATYSELECT 	= PNULL;
Bsize_t  m_DLparamJYmod_XNFZ_mem;
/********************************************************************
 * @作者 :周炳权
 * @功能 :虚拟负载输出画面
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
Bsize_t  mem_sector_malloc( uint32_t size );
Bsize_t  mem_sector_free( uint32_t size );
void  lcd_driver_read_block_Ext( 
			uint16_t x,
			uint16_t y,
			uint16_t wide,
			uint16_t high,
			Bsize_t sector 
);
void  lcd_driver_write_block_Ext( 
			uint16_t x,
			uint16_t y,
			uint16_t wide,
			uint16_t high,
			Bsize_t sector 
);

/********************************************************************
 * @作者 :周炳权
 * @功能 :主窗口电表参数及误差显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_param_and_err_display( DLparam_JY_t *m_dlparam )
{
	GUI_window_hwin_req(GET_DOBJ(DLPARAMJYMOD_MAIN_WINDOW));
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_edit_float_display_at(m_dlparam->meter_U[0],EDIT_U1,6);
	GUI_edit_float_display_at(m_dlparam->meter_I[0],EDIT_I1,6);
	GUI_edit_float_display_at(m_dlparam->meter_P[0],EDIT_P1,6);
	GUI_edit_float_display_at(m_dlparam->meter_Q[0],EDIT_Q1,6);
	GUI_set_Fgcolor( C_GUI_GREEN );
	if ( GLOBAL_ABS(m_dlparam->meter_Uerr[0]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Uerr[0],EDIT_UERR1,6);
	if ( GLOBAL_ABS(m_dlparam->meter_Ierr[0]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Ierr[0],EDIT_IERR1,6);
	if ( GLOBAL_ABS(m_dlparam->meter_Perr[0]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Perr[0],EDIT_PERR1,6);
	if ( GLOBAL_ABS(m_dlparam->meter_Qerr[0]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Qerr[0],EDIT_QERR1,6);
	GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	
	GUI_edit_float_display_at(m_dlparam->meter_U[1],EDIT_U2,6);
	GUI_edit_float_display_at(m_dlparam->meter_I[1],EDIT_I2,6);
	GUI_edit_float_display_at(m_dlparam->meter_P[1],EDIT_P2,6);
	GUI_edit_float_display_at(m_dlparam->meter_Q[1],EDIT_Q2,6);
	GUI_set_Fgcolor( C_GUI_GREEN );
	if ( GLOBAL_ABS(m_dlparam->meter_Uerr[1]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Uerr[1],EDIT_UERR2,6);

	if ( GLOBAL_ABS(m_dlparam->meter_Ierr[1]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Ierr[1],EDIT_IERR2,6);
	if ( GLOBAL_ABS(m_dlparam->meter_Perr[1]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Perr[1],EDIT_PERR2,6);
	if ( GLOBAL_ABS(m_dlparam->meter_Qerr[1]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Qerr[1],EDIT_QERR2,6);
	GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	GUI_edit_float_display_at(m_dlparam->meter_U[2],EDIT_U3,6);
	GUI_edit_float_display_at(m_dlparam->meter_I[2],EDIT_I3,6);
	GUI_edit_float_display_at(m_dlparam->meter_P[2],EDIT_P3,6);
	GUI_edit_float_display_at(m_dlparam->meter_Q[2],EDIT_Q3,6);
	GUI_set_Fgcolor( C_GUI_GREEN );
	if ( GLOBAL_ABS(m_dlparam->meter_Uerr[2]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Uerr[2],EDIT_UERR3,6);
	if ( GLOBAL_ABS(m_dlparam->meter_Ierr[2]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Ierr[2],EDIT_IERR3,6);
	if ( GLOBAL_ABS(m_dlparam->meter_Perr[2]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Perr[2],EDIT_PERR3,6);
	if ( GLOBAL_ABS(m_dlparam->meter_Qerr[2]) > MAX_ERROR )
		GUI_set_Fgcolor(C_GUI_RED);
	else 
		GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_float_display_at(m_dlparam->meter_Qerr[2],EDIT_QERR3,6);
	GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	
	GUI_window_hwin_release(GET_DOBJ(DLPARAMJYMOD_MAIN_WINDOW));
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :主窗口电表1参数及误差显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_meter1_param_err_display( DLparam_JY_t *m_dlparam )
{
	GUI_window_hwin_req(GET_DOBJ(DLPARAMJYMOD_MAIN_WINDOW));
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	if ( m_dlparam->meter_U[0] == 0 )
		GUI_edit_string_display_At("0", EDIT_U1);
	else
		GUI_edit_float_display_at(m_dlparam->meter_U[0],EDIT_U1,6);
	if ( m_dlparam->meter_U[1] == 0 )
		GUI_edit_string_display_At("0", EDIT_U2);
	else 
		GUI_edit_float_display_at(m_dlparam->meter_U[1],EDIT_U2,6);
	if ( m_dlparam->meter_U[2] == 0 )
		GUI_edit_string_display_At("0", EDIT_U3);
	else GUI_edit_float_display_at(m_dlparam->meter_U[2],EDIT_U3,6);
	
	GUI_set_Fgcolor( C_GUI_GREEN );
	if ( m_dlparam->meter_Uerr[0] == 0 )
		GUI_edit_string_display_At("0", EDIT_UERR1);
	else 
		GUI_edit_float_display_at(m_dlparam->meter_Uerr[0],EDIT_UERR1,6);
	if ( m_dlparam->meter_Uerr[1] == 0 )
		 GUI_edit_string_display_At("0", EDIT_UERR2);
	else 
		GUI_edit_float_display_at(m_dlparam->meter_Uerr[1],EDIT_UERR2,6);
	if ( m_dlparam->meter_Uerr[2] == 0 )
		GUI_edit_string_display_At("0", EDIT_UERR3);
	else 
	    GUI_edit_float_display_at(m_dlparam->meter_Uerr[2],EDIT_UERR3,6);
		
	GUI_window_hwin_release(GET_DOBJ(DLPARAMJYMOD_MAIN_WINDOW));
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :主窗口电表2参数及误差显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_meter2_param_err_display( DLparam_JY_t *m_dlparam )
{
	GUI_window_hwin_req(GET_DOBJ(DLPARAMJYMOD_MAIN_WINDOW));
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);

	if ( m_dlparam->meter_I[0] == 0 )
		GUI_edit_string_display_At("0", EDIT_I1);
	else GUI_edit_float_display_at(m_dlparam->meter_I[0],EDIT_I1,6);
	if ( m_dlparam->meter_I[1] == 0 )
		GUI_edit_string_display_At("0", EDIT_I2);
	else GUI_edit_float_display_at(m_dlparam->meter_I[1],EDIT_I2,6);
	if ( m_dlparam->meter_I[2] == 0 )
		GUI_edit_string_display_At("0", EDIT_I3);
	else GUI_edit_float_display_at(m_dlparam->meter_I[2],EDIT_I3,6);
	
	GUI_set_Fgcolor( C_GUI_GREEN );
	
	if ( m_dlparam->meter_Ierr[0] == 0 )
		GUI_edit_string_display_At("0", EDIT_IERR1);
	else GUI_edit_float_display_at(m_dlparam->meter_Ierr[0],EDIT_IERR1,7);
	if ( m_dlparam->meter_Ierr[1] == 0 )
		 GUI_edit_string_display_At("0", EDIT_IERR2);
	else GUI_edit_float_display_at(m_dlparam->meter_Ierr[1],EDIT_IERR2,7);
	if ( m_dlparam->meter_Ierr[2] == 0 )
		 GUI_edit_string_display_At("0", EDIT_IERR3);
	else GUI_edit_float_display_at(m_dlparam->meter_Ierr[2],EDIT_IERR3,7);
		
	GUI_window_hwin_release(GET_DOBJ(DLPARAMJYMOD_MAIN_WINDOW));
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :主窗口电表3参数及误差显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_meter3_param_err_display( DLparam_JY_t *m_dlparam )
{
	GUI_window_hwin_req(GET_DOBJ(DLPARAMJYMOD_MAIN_WINDOW));
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);

	if ( m_dlparam->meter_P[0] == 0 )
		GUI_edit_string_display_At("0", EDIT_P1);
	else GUI_edit_float_display_at(m_dlparam->meter_P[0],EDIT_P1,6);
	if ( m_dlparam->meter_P[1] == 0 )
		GUI_edit_string_display_At("0", EDIT_P2);
	else GUI_edit_float_display_at(m_dlparam->meter_P[1],EDIT_P2,6);
	if ( m_dlparam->meter_P[2] == 0 )
		GUI_edit_string_display_At("0", EDIT_P3);
	else GUI_edit_float_display_at(m_dlparam->meter_P[2],EDIT_P3,6);
	
	GUI_set_Fgcolor( C_GUI_GREEN );
	
	if ( m_dlparam->meter_Perr[0] == 0 )
		GUI_edit_string_display_At("0", EDIT_PERR1);
	else GUI_edit_float_display_at(m_dlparam->meter_Perr[0],EDIT_PERR1,6);
	if ( m_dlparam->meter_Perr[1] == 0 )
		 GUI_edit_string_display_At("0", EDIT_PERR2);
	else GUI_edit_float_display_at(m_dlparam->meter_Perr[1],EDIT_PERR2,6);
	if ( m_dlparam->meter_Perr[2] == 0 )
		 GUI_edit_string_display_At("0", EDIT_PERR3);
	else GUI_edit_float_display_at(m_dlparam->meter_Perr[2],EDIT_PERR3,6);
		
	GUI_window_hwin_release(GET_DOBJ(DLPARAMJYMOD_MAIN_WINDOW));
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :主窗口电表4参数及误差显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_meter4_param_err_display( DLparam_JY_t *m_dlparam )
{
	GUI_window_hwin_req(GET_DOBJ(DLPARAMJYMOD_MAIN_WINDOW));
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);

	if ( m_dlparam->meter_Q[0] == 0 )
		GUI_edit_string_display_At("0", EDIT_Q1);
	else GUI_edit_float_display_at(m_dlparam->meter_Q[0],EDIT_Q1,6);
	if ( m_dlparam->meter_Q[1] == 0 )
		GUI_edit_string_display_At("0", EDIT_Q2);
	else GUI_edit_float_display_at(m_dlparam->meter_Q[1],EDIT_Q2,6);
	if ( m_dlparam->meter_Q[2] == 0 )
		GUI_edit_string_display_At("0", EDIT_Q3);
	else GUI_edit_float_display_at(m_dlparam->meter_Q[2],EDIT_Q3,6);
	
	GUI_set_Fgcolor( C_GUI_GREEN );
	if ( m_dlparam->meter_Qerr[0] == 0 )
		 GUI_edit_string_display_At("0", EDIT_QERR1);
	else GUI_edit_float_display_at(m_dlparam->meter_Qerr[0],EDIT_QERR1,6);
	if ( m_dlparam->meter_Qerr[1] == 0 )
		 GUI_edit_string_display_At("0", EDIT_QERR2);
	else GUI_edit_float_display_at(m_dlparam->meter_Qerr[1],EDIT_QERR2,6);
	if ( m_dlparam->meter_Qerr[2] == 0 )
		 GUI_edit_string_display_At("0", EDIT_QERR3);
	else GUI_edit_float_display_at(m_dlparam->meter_Qerr[2],EDIT_QERR3,6);
		
	GUI_window_hwin_release(GET_DOBJ(DLPARAMJYMOD_MAIN_WINDOW));
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :协议选择窗口
 * @输入 :hOBJ:当前窗口；
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_TreatySelect_callblack(hwin_t*hOBJ)
{
    GUI_window_hwin_req(hOBJ);
    GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_radio_memeber_add(10,7,"DL/T 645-2007",1);
	GUI_radio_memeber_add(10,43,"DL/T 645-1997",0);
    GUI_window_hwin_release(hOBJ);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :操作初始化窗口
 * @输入 :hOBJ:当前窗口；
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_control_init(hwin_t*hOBJ)
{
	hwin_t *hp;
	const char *m_begin[2] 	= {"开始",""};
	const char *m_stop[2] 	= {"停止",""};
    GUI_window_hwin_req(hOBJ);
	GUI_set_Bgcolor(DLparamJYmod_WIN_BG_COLOR);
    GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
    GUI_SetFront(&GUI_HZK24_EXTASCII);

	m_DLparamJYmod_radio_TREATYSELECT=GUI_radio_create(290,0,310,40,WIN_MSG|WIN_TOUCH|GUI_KEY_TERATY_SELECT,hOBJ);
	GUI_window_hwin_req(m_DLparamJYmod_radio_TREATYSELECT);
	GUI_radio_memeber_add(5,7,"DL/T645-07",1);
	GUI_radio_memeber_add(150,7,"DL/T645-97",0);
	GUI_window_hwin_release(m_DLparamJYmod_radio_TREATYSELECT);

	hp = Gui_button_creat( 50,2,90,35,BUTTON_PAINT|KEY_F1, hOBJ );
	GUI_button_title_display(hp,(char *)m_begin[g_language]);
	hp = Gui_button_creat( 50+110,2,90,35,BUTTON_PAINT|KEY_F2, hOBJ );
	GUI_button_title_display(hp,(char *)m_stop[g_language]);
	
    GUI_window_hwin_release(hOBJ);
	return;
}


/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
static void mod_FDL_view(float fdata,int id)
{
    if(fdata==0){GUI_edit_string_display_At("0.000",id);return;}
	GUI_edit_float_display_at(fdata,id,6);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_DLarg_display(measure_param_t*pDL)
{
	char temp[10]="";
	char ch[2]="";
	GUI_window_hwin_req(GET_DOBJ(DLPARAMJYMOD_DL_EDIT));
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	mod_FDL_view(pDL->phase_a.f_U,EDIT_U_A);
	mod_FDL_view(pDL->phase_a.f_I,EDIT_I_A);
	mod_FDL_view(pDL->phase_a.f_J,EDIT_J_A);
	mod_FDL_view(pDL->phase_a.f_P,EDIT_P_A);
	mod_FDL_view(pDL->phase_a.f_Q,EDIT_Q_A);

	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	mod_FDL_view(pDL->phase_b.f_U,EDIT_U_B);
	mod_FDL_view(pDL->phase_b.f_I,EDIT_I_B);
	mod_FDL_view(pDL->phase_b.f_J,EDIT_J_B);
	mod_FDL_view(pDL->phase_b.f_P,EDIT_P_B);
	mod_FDL_view(pDL->phase_b.f_Q,EDIT_Q_B);
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	mod_FDL_view(pDL->phase_c.f_U,EDIT_U_C);
	mod_FDL_view(pDL->phase_c.f_I,EDIT_I_C);
	mod_FDL_view(pDL->phase_c.f_J,EDIT_J_C);
	mod_FDL_view(pDL->phase_c.f_P,EDIT_P_C);
	mod_FDL_view(pDL->phase_c.f_Q,EDIT_Q_C);

	GUI_set_Fgcolor(C_GUI_YELLOW);
	mod_FDL_view(pDL->f_Pabc,EDIT_PABC);
	mod_FDL_view(pDL->f_Qabc,EDIT_QABC);
	mod_FDL_view(pDL->f_JUab,EDIT_JUAB);
	mod_FDL_view(pDL->f_JUbc,EDIT_JUBC);
	mod_FDL_view(pDL->f_JUac,EDIT_JUAC);
	//mod_FDL_view(pDL->f_JIab,EDIT_JIAB);
	mod_FDL_view(pDL->f_JIac,EDIT_JIAC);
	//mod_FDL_view(pDL->f_JIbc,EDIT_JIBC);
	mod_FDL_view(pDL->f_Feq, EDIT_FEQ);
	if(pDL->f_Cos==1)
	{
		GUI_edit_string_display_At("1.00",EDIT_COS);
	}
	else
	{
		global_ftoa(pDL->f_Cos,temp,2);
		ch[0] = pDL->cos_LC;
		global_strcat(temp,ch);
		GUI_edit_string_display_At(temp,EDIT_COS);
	}
	GUI_window_hwin_release(GET_DOBJ(DLPARAMJYMOD_DL_EDIT));
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示窗口初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparam_DLmember_window_init(hwin_t*hp)
{
#define   DL_XLAB 15
#define   DL_GAP  90
#define   DL_VGAP 32
//    hwin_t*pOBJ;
	GUI_window_hwin_req(hp);

	GUI_set_Bgcolor(m_GUI_3D_window_Bg_cor);
    GUI_fill_window_area(4,2,hp->win_wide,DL_VGAP);
    GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(24+DL_XLAB+DL_GAP*0, 5, "U(V)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*1, 5, "I(A)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*2, 5, "Φ(°)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*3, 5, "P(W)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*4, 5, "Q(var)" );
	GUI_string_display_At(30+DL_XLAB+DL_GAP*5, 2, "ΦABC(°)" );
	
	GUI_set_Bgcolor(DLparamJYmod_WIN_BG_COLOR);
    GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	GUI_set_Fgcolor( C_GUI_YELLOW );
	GUI_string_display_At(2,DL_VGAP*1+2, "A" );
	GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_string_display_At(2,DL_VGAP*2+2, "B" );
	GUI_set_Fgcolor( C_GUI_RED );
	GUI_string_display_At(2,DL_VGAP*3+2, "C" );

	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*1+0, 7, "", MEMBER_3D | EDIT_U_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*2+0, 7, "", MEMBER_3D | EDIT_U_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*0,DL_VGAP*3+0, 7, "", MEMBER_3D | EDIT_U_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*1+0, 7, "", MEMBER_3D | EDIT_I_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*2+0, 7, "", MEMBER_3D | EDIT_I_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*1,DL_VGAP*3+0, 7, "", MEMBER_3D | EDIT_I_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*1+0, 7, "", MEMBER_3D | EDIT_J_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*2+0, 7, "", MEMBER_3D | EDIT_J_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*2,DL_VGAP*3+0, 7, "", MEMBER_3D | EDIT_J_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*1+0, 7, "", MEMBER_3D | EDIT_P_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*2+0, 7, "", MEMBER_3D | EDIT_P_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*3+0, 7, "", MEMBER_3D | EDIT_P_C |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*1+0, 7, "", MEMBER_3D | EDIT_Q_A |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*2+0, 7, "", MEMBER_3D | EDIT_Q_B |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*3+0, 7, "", MEMBER_3D | EDIT_Q_C |EDIT_DIS);
	GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*3,DL_VGAP*4+0,7, "",  MEMBER_3D | EDIT_PABC |EDIT_DIS);
	GUI_edit_memeber_add(DL_XLAB+DL_GAP*4,DL_VGAP*4+0,7, "",  MEMBER_3D | EDIT_QABC |EDIT_DIS);
	GUI_edit_memeber_add(2,DL_VGAP*4+0,7,   "cosΦ", MEMBER_3D | EDIT_COS |EDIT_DIS);
	GUI_edit_memeber_add(159,DL_VGAP*4+0,6, "F", MEMBER_3D | EDIT_FEQ |EDIT_DIS);

	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*1,7, "ΦUab", MEMBER_3D | EDIT_JUAB );
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*2,7, "ΦUac", MEMBER_3D | EDIT_JUAC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*3,7, "ΦUbc", MEMBER_3D | EDIT_JUBC);
	GUI_edit_memeber_add(5+DL_XLAB+DL_GAP*5,DL_VGAP*4,7, "ΦIac", MEMBER_3D | EDIT_JIAC);
	
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    GUI_string_display_At(DL_XLAB+DL_GAP*3-16,DL_VGAP*4+4, "∑");
	
	GUI_window_hwin_release(hp);
    return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :走字模块主窗口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
#define 	OFFSET_Y 	60
#define     OFFSET_X 	5
#define     PER_HEIGHT	25
#define     PER_WIDTH	60
void app_DLparamJYmod_window_creat( hwin_t*hp)
{	
	const char *m_Aphase[2]		= {"A相",""};
	const char *m_Bphase[2]		= {"B相",""};
	const char *m_Cphase[2]		= {"C相",""};
	const char *m_meteU[2]		= {"电表电压U(V):",""};
	const char *m_meterUE[2]	= {"电压误差Err%:",""};
	const char *m_meterI[2]		= {"电表电流I(A):",""};
	const char *m_meterIE[2]	= {"电流误差Err%:",""};
	const char *m_meterP[2]		= {"有功功率P(W):",""};
	const char *m_meterPE[2]	= {"有功误差Err%:",""};
	const char *m_meterQ[2]		= {"无功功率Qvar:",""};
	const char *m_meterQE[2]	= {"无功误差Err%:",""};
	
	GUI_window_hwin_req( hp );
	GUI_set_Bgcolor( DLparamJYmod_WIN_BG_COLOR );
	GUI_set_Fgcolor( DLparamJYmod_FG_COLOR );

	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	GUI_string_display_At( OFFSET_X+190, 5, (char *)m_Aphase[g_language] );
	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	GUI_string_display_At( OFFSET_X+335, 5, (char *)m_Bphase[g_language] );
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	GUI_string_display_At( OFFSET_X+465, 5, (char *)m_Cphase[g_language] );

	GUI_set_Fgcolor( DLparamJYmod_FG_COLOR );
	GUI_edit_memeber_add( OFFSET_X+65, OFFSET_Y-35+PER_HEIGHT*0, 8, (char *)m_meteU[g_language], MEMBER_3D | EDIT_U1 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+302, OFFSET_Y-35+PER_HEIGHT*0, 8, "", MEMBER_3D | EDIT_U2 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+434, OFFSET_Y-35+PER_HEIGHT*0, 8, "", MEMBER_3D | EDIT_U3 | EDIT_DIS);
	GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_memeber_add( OFFSET_X+65, OFFSET_Y-35+PER_HEIGHT*1, 8, (char *)m_meterUE[g_language], MEMBER_3D | EDIT_UERR1 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+302, OFFSET_Y-35+PER_HEIGHT*1, 8, "", MEMBER_3D | EDIT_UERR2 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+434, OFFSET_Y-35+PER_HEIGHT*1, 8, "", MEMBER_3D | EDIT_UERR3 | EDIT_DIS);
	GUI_set_Fgcolor( DLparamJYmod_FG_COLOR );
	GUI_edit_memeber_add( OFFSET_X+65, OFFSET_Y-35+PER_HEIGHT*2, 8, (char *)m_meterI[g_language], MEMBER_3D | EDIT_I1 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+302, OFFSET_Y-35+PER_HEIGHT*2, 8, "", MEMBER_3D | EDIT_I2 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+434, OFFSET_Y-35+PER_HEIGHT*2, 8, "", MEMBER_3D | EDIT_I3 | EDIT_DIS);
	GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_memeber_add( OFFSET_X+65, OFFSET_Y-35+PER_HEIGHT*3, 8, (char *)m_meterIE[g_language], MEMBER_3D | EDIT_IERR1 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+302, OFFSET_Y-35+PER_HEIGHT*3, 8, "", MEMBER_3D | EDIT_IERR2 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+434, OFFSET_Y-35+PER_HEIGHT*3, 8, "", MEMBER_3D | EDIT_IERR3 | EDIT_DIS);
	GUI_set_Fgcolor( DLparamJYmod_FG_COLOR );
	GUI_edit_memeber_add( OFFSET_X+65, OFFSET_Y-35+PER_HEIGHT*4, 8, (char *)m_meterP[g_language], MEMBER_3D | EDIT_P1 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+302, OFFSET_Y-35+PER_HEIGHT*4, 8, "", MEMBER_3D | EDIT_P2 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+434, OFFSET_Y-35+PER_HEIGHT*4, 8, "", MEMBER_3D | EDIT_P3 | EDIT_DIS);
	GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_memeber_add( OFFSET_X+65, OFFSET_Y-35+PER_HEIGHT*5, 8, (char *)m_meterPE[g_language], MEMBER_3D | EDIT_PERR1 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+302, OFFSET_Y-35+PER_HEIGHT*5, 8, "", MEMBER_3D | EDIT_PERR2 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+434, OFFSET_Y-35+PER_HEIGHT*5, 8, "", MEMBER_3D | EDIT_PERR3 | EDIT_DIS);
	GUI_set_Fgcolor( DLparamJYmod_FG_COLOR );
	GUI_edit_memeber_add( OFFSET_X+65, OFFSET_Y-35+PER_HEIGHT*6, 8, (char *)m_meterQ[g_language], MEMBER_3D | EDIT_Q1 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+302, OFFSET_Y-35+PER_HEIGHT*6, 8, "", MEMBER_3D | EDIT_Q2 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+434, OFFSET_Y-35+PER_HEIGHT*6, 8, "", MEMBER_3D | EDIT_Q3 | EDIT_DIS);
	GUI_set_Fgcolor( C_GUI_GREEN );
	GUI_edit_memeber_add( OFFSET_X+65, OFFSET_Y-35+PER_HEIGHT*7, 8, (char *)m_meterQE[g_language], MEMBER_3D | EDIT_QERR1 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+302, OFFSET_Y-35+PER_HEIGHT*7, 8, "", MEMBER_3D | EDIT_QERR2 | EDIT_DIS);
	GUI_edit_memeber_add( OFFSET_X+434, OFFSET_Y-35+PER_HEIGHT*7, 8, "", MEMBER_3D | EDIT_QERR3 | EDIT_DIS);

	GUI_window_hwin_release( hp );
	return;
}

#define    BUTTON_OFFSET_Y		5
#define    BUTTON_PER_Y			70
const dialogBox_t  app_DLparamJYmod_dialogbox[]=
{
	{WIDGET_EDIT,DLPARAMJYMOD_DL_EDIT,"",2,235,630,170,WIN_3D,0,0,app_DLparam_DLmember_window_init},
	{WIDGET_EDIT,DLPARAMJYMOD_MAIN_WINDOW,"",2,0,630,233,WIN_3D|WIN_TOUCH_EN,0,0,app_DLparamJYmod_window_creat},
	{WIDGET_EDIT,DLPARAMJYMOD_CONTROL_EDIT,"",2,405,630,42,WIN_3D|WIN_MSG|WIN_TOUCH,0,0,app_DLparamJYmod_control_init},
};

/********************************************************************
 * @作者 :周炳权
 * @功能 :界面创建
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_window_main_creat(void)
{
	const char *m_title[2] 	= {"智能电能表参数校验",""};

	GUI_set_Bgcolor(DLparamJYmod_BG_CLOLR);
	GUI_clean_screen();
	GUI_set_Fgcolor(DLparamJYmod_FG_COLOR);
	GUI_set_Bgcolor(DLparamJYmod_WIN_BG_COLOR);
	m_DLparamJYmod_win = GUI_dialogBox_create((char *)m_title[g_language],5,2,636,485,
	WIN_3D|WIN_FILL|WIN_CLOSE_ICON_DIS,
	app_DLparamJYmod_dialogbox,
	SIZE_OF_ARRAY(app_DLparamJYmod_dialogbox));
	GUI_radio_select(m_DLparamJYmod_radio_TREATYSELECT,g_DLparamJY_07_or_97);
	os_driver_ioctl(g_gpio_dev,GPIO_CMD_SET_485_COM,0);
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :界面销毁
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_window_main_destory(void)
{
    GUI_window_destory(m_DLparamJYmod_win);
	return;
}

