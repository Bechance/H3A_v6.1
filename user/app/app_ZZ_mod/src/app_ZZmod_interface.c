/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:走字模块界面函数集合
* 创建时间:2012-07-10
* 所属公司:科荟
* 文件名称:app_zzmod_interface.c
* 创建人 :周炳权
* 功能描述:提供走字模块画屏函数API
**********************************************************************************************/
#include "app_ZZmod_inside.h"

hwin_t *m_zz_3DWIN    			    = 0;
hwin_t *m_zz_DL_win   			    = 0;
hwin_t *m_zz_edit_win 			    = 0;
//hwin_t *m_meter_edit 			    = 0;
hwin_t *m_instrument_edit 		    = 0;
hwin_t *m_Meter_Treaty_edit		    = 0;
hwin_t *m_zzmod_radio_ITORMC 		= PNULL;
hwin_t *m_zzmod_radio_TREATYSELECT 	= PNULL;

/********************************************************************
 * @作者 :周炳权
 * @功能 :走字编辑参数显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_zzmod_edit_display( void )
{	
	GUI_window_hwin_req(m_zz_edit_win);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At( m_zz_arg.energy_real, EDIT_ENERGY_REAL );
	GUI_edit_string_display_At( m_zz_arg.energy_preset, EDIT_ENERGY_PRESET );
	GUI_edit_string_display_At( m_zz_arg.energy_measure, EDIT_ENERGY_MEASURE );
	
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_edit_string_display_At( m_zz_arg.zz_time_min, EDIT_ENERGY_TIME );
	GUI_edit_string_display_At( m_zz_arg.energy_zz_err , EDIT_ENERGY_ERR );
	
	GUI_set_Fgcolor(zz_FG_COLOR);
	GUI_edit_string_display_At( m_zz_arg.energy_start , EDIT_ENERGY_START );
	GUI_edit_string_display_At( m_zz_arg.energy_end , EDIT_ENERGY_END );

	GUI_window_hwin_release(m_zz_edit_win);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :走字定时时间实时显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_zzmod_time_display( long m_min )
{	
	GUI_window_hwin_req(m_zz_edit_win);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);	
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_edit_int_display_At( m_min, EDIT_ENERGY_TIME );
	GUI_window_hwin_release(m_zz_edit_win);
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :仪器测量电能显示
 * @输入 :m_data:显示参数
 * @输出 :NONE
********************************************************************/
void app_ZZmod_edit_instrument_energy_display( float m_data )
{
	long m_int 	  = 0;
	long m_int_de = 0;
	float m_decimals = 0.0;
	float m_decimals_i = 0.0;
	
	m_int = (long)m_data;
	m_decimals = m_data - (float)m_int;
	m_int = (m_int % 1000);
	m_decimals_i = (m_decimals * 1000.0);
	m_int_de = (long)m_decimals_i;

	if ( ((m_decimals_i - m_int_de) >= 0.9) && ((m_int_de % 10) < 9)) 
		m_int_de += 1;
	
	GUI_window_hwin_req(m_instrument_edit);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_int_display_At( (m_int / 100),EDIT_INSTRUMENT_0);
	GUI_edit_int_display_At( ((m_int % 100) / 10),EDIT_INSTRUMENT_1);
	GUI_edit_int_display_At( (m_int % 10),EDIT_INSTRUMENT_2);

	GUI_edit_int_display_At( (m_int_de / 100),EDIT_INSTRUMENT_3);
	GUI_edit_int_display_At( ((m_int_de % 100) / 10),EDIT_INSTRUMENT_4);
	GUI_edit_int_display_At( (m_int_de % 10),EDIT_INSTRUMENT_5);

	GUI_edit_float_display_at( m_data, EDIT_INSTRUMENT_6, 8);
	GUI_window_hwin_release(m_instrument_edit);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :被测电表电能显示
 * @输入 :m_Sdata:起始电能;m_Edata:结束电能;
 * @输出 :NONE
********************************************************************/
void app_ZZmod_edit_meter_energy_display( float m_Sdata, float m_Edata )
{
/*	long m_Sint 	  	= 0;
	long m_Sint_de 		= 0;
	float m_Sdecimals 	= 0.0;
	float m_Sdecimals_i = 0.0;
	long m_Eint 	  	= 0;
	long m_Eint_de 		= 0;
	float m_Edecimals 	= 0.0;
	float m_Edecimals_i = 0.0;
	
	m_Sint = (long)m_Sdata;
	m_Sdecimals = m_Sdata - (float)m_Sint;
	m_Sint = (m_Sint % 10000);
	m_Sdecimals_i = (m_Sdecimals * 100.0);
	m_Sint_de = (long)m_Sdecimals_i;
	
	m_Eint = (long)m_Edata;
	m_Edecimals = m_Edata - (float)m_Eint;
	m_Eint = (m_Eint % 10000);
	m_Edecimals_i = (m_Edecimals * 100.0);
	m_Eint_de = (long)m_Edecimals_i;

	if ( ((m_Sdecimals_i - m_Sint_de) >= 0.9) && ((m_Sint_de % 10) < 9)) 
		m_Sint_de += 1;
	if ( ((m_Edecimals_i - m_Eint_de) >= 0.9) && ((m_Eint_de % 10) < 9)) 
		m_Eint_de += 1;
	
	GUI_window_hwin_req(m_meter_edit);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);

	GUI_edit_int_display_At( (m_Sint / 1000),EDIT_METER_S0);
	GUI_edit_int_display_At( ((m_Sint % 1000) / 100),EDIT_METER_S1);
	GUI_edit_int_display_At( ((m_Sint % 100) / 10 ),EDIT_METER_S2);
	GUI_edit_int_display_At( (m_Sint % 10),EDIT_METER_S3);
	GUI_edit_int_display_At( (m_Sint_de / 10),EDIT_METER_S4);
	GUI_edit_int_display_At( (m_Sint_de % 10),EDIT_METER_S5);

	GUI_edit_int_display_At( (m_Eint / 1000),EDIT_METER_E0);
	GUI_edit_int_display_At( ((m_Eint % 1000) / 100),EDIT_METER_E1);
	GUI_edit_int_display_At( ((m_Eint % 100) / 10 ),EDIT_METER_E2);
	GUI_edit_int_display_At( (m_Eint % 10),EDIT_METER_E3);
	GUI_edit_int_display_At( (m_Eint_de / 10),EDIT_METER_E4);
	GUI_edit_int_display_At( (m_Eint_de % 10),EDIT_METER_E5);
	
	GUI_window_hwin_release(m_meter_edit);*/
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :走字测量电能动态显示
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_zzmod_energy_trends_display( void )
{
	app_zzmod_measure_energy_calc();
	GUI_window_hwin_req(m_zz_edit_win);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_edit_string_display_At( m_zz_arg.energy_measure , EDIT_ENERGY_MEASURE );
	GUI_window_hwin_release(m_zz_edit_win);
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
void app_ZZmod_DLarg_display(measure_param_t*pDL)
{
	char temp[10]="";
	char ch[2]="";
	GUI_window_hwin_req(m_zz_DL_win);
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
	GUI_window_hwin_release(m_zz_DL_win);
	return;
}

/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示窗口初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_ZZ_DLmember_window_init(hwin_t*hp)
{
#define   DL_XLAB 15
#define   DL_GAP  90
#define   DL_VGAP 32
//    hwin_t*pOBJ;
	
	GUI_window_hwin_req(hp);
	
	GUI_set_Bgcolor(m_GUI_3D_window_Bg_cor);
    GUI_fill_window_area(4,2,hp->win_wide,DL_VGAP);
    GUI_set_Fgcolor(zz_FG_COLOR);
	
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(24+DL_XLAB+DL_GAP*0, 2, "U(V)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*1, 2, "I(A)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*2, 2, "Φ(°)" );
	GUI_string_display_At(24+DL_XLAB+DL_GAP*3, 2, "P(W)" );
	GUI_string_display_At(12+DL_XLAB+DL_GAP*4, 2, "Q(var)" );
	GUI_string_display_At(30+DL_XLAB+DL_GAP*5, 2, "ΦABC(°)" );
	
	GUI_set_Bgcolor(zz_WIN_BG_COLOR);
    GUI_set_Fgcolor(zz_FG_COLOR);
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
	GUI_set_Fgcolor(zz_FG_COLOR);
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

/********************************************************************
 * @创建人 :周炳权
 * @功能 :电表和协议窗口初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_ZZ_meter_treaty_window_init(hwin_t*hp)
{
	const char *m_meterIT[2]	= {"智能电表",""};//多功能电表
	const char *m_meterMC[2]	= {"机电电表",""};

	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(zz_WIN_BG_COLOR);
    GUI_set_Fgcolor(zz_FG_COLOR);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	m_zzmod_radio_ITORMC=GUI_radio_create(0,0,240,40,WIN_MSG|WIN_TOUCH|GUI_KEY_METER_ITMC,hp);
	GUI_window_hwin_req(m_zzmod_radio_ITORMC);
	GUI_radio_memeber_add(2,7,(char *)m_meterIT[g_language],0);
	GUI_radio_memeber_add(122,7,(char *)m_meterMC[g_language],1);
	GUI_window_hwin_release(m_zzmod_radio_ITORMC);
	m_zzmod_radio_TREATYSELECT=GUI_radio_create(240,0,310,40,WIN_MSG|WIN_TOUCH|GUI_KEY_TERATY_SELECT,hp);
	GUI_window_hwin_req(m_zzmod_radio_TREATYSELECT);
	GUI_radio_memeber_add(5,7,"DL/T645-07",1);
	GUI_radio_memeber_add(150,7,"DL/T645-97",0);
	GUI_window_hwin_release(m_zzmod_radio_TREATYSELECT);
	
	GUI_window_hwin_release(hp);
	return;
}

/********************************************************************
 * @创建人 :周炳权
 * @功能 :走字模块操作提示
 * @输入 :m_flag:0:无走字测量;1:正在走字测量;
 * @输出 :NONE
********************************************************************/
void app_ZZ_control_tip(uint8_t m_flag)
{
	GUI_window_hwin_req(m_Meter_Treaty_edit);
	GUI_set_Bgcolor(zz_WIN_BG_COLOR);
    GUI_set_Fgcolor(zz_FG_COLOR);
	if ( m_flag == 0 )
	{
		GUI_set_Fgcolor(C_GUI_GREEN);
		GUI_string_display_At(m_Meter_Treaty_edit->win_wide-90,8, "F5:Run " );
	}
	else
	{
		GUI_set_Fgcolor(C_GUI_RED);
		GUI_string_display_At(m_Meter_Treaty_edit->win_wide-90,8, "F5:Stop" );
	}
	
	GUI_window_hwin_release(m_Meter_Treaty_edit);
	return;
}
/********************************************************************
 * @创建人 :周炳权
 * @功能 :编辑窗口初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_ZZ_edit_window_init(hwin_t*hp)
{
#define   DL_VGAP 35
//	hwin_t*pOBJ;
	//const char *m_ZZbegin[2]	= {"开始走字",""};//多功能电表
	//const char *m_ZZend[2]  	= {"结束走字",""};
	const char *m_ZZSenergy[2]  = {"起始电能:",""};
	const char *m_ZZEenergy[2]  = {"结束电能:",""};
	const char *m_ZZreal[2]  	= {"电表走字电能",""};
	const char *m_ZZenergy[2]  	= {"预置走字电能",""};
	const char *m_ZZtime[2]  	= {"走字定时时间",""};
	const char *m_ZZerr[2]  	= {"走字误差(%):",""};
	const char *m_min[2]  		= {"分钟",""};
	GUI_window_hwin_req(hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(zz_WIN_BG_COLOR);
    GUI_set_Fgcolor(zz_FG_COLOR);
	
	GUI_edit_memeber_add(5,DL_VGAP*0+5, 9, (char *)m_ZZSenergy[g_language], MEMBER_3D | EDIT_ENERGY_START);
	GUI_edit_memeber_add(5,DL_VGAP*1+5, 9, (char *)m_ZZEenergy[g_language], MEMBER_3D | EDIT_ENERGY_END);
	GUI_edit_memeber_add(5,DL_VGAP*2+5, 6, (char *)m_ZZreal[g_language], MEMBER_3D | EDIT_ENERGY_REAL|EDIT_DIS);
	GUI_edit_memeber_add(5,DL_VGAP*3+5, 6, (char *)m_ZZenergy[g_language], MEMBER_3D | EDIT_ENERGY_PRESET);
	GUI_edit_memeber_add(5,DL_VGAP*4+5, 6, (char *)m_ZZtime[g_language], MEMBER_3D | EDIT_ENERGY_TIME);
	GUI_edit_memeber_add(5,DL_VGAP*5+5, 6, (char *)m_ZZerr[g_language], MEMBER_3D | EDIT_ENERGY_ERR|EDIT_DIS);
	GUI_string_display_At(hp->win_wide-55,DL_VGAP*4+5, (char *)m_min[g_language]);

	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(hp->win_wide-55,DL_VGAP*0+5+8, "kW·h" );
	GUI_string_display_At(hp->win_wide-55,DL_VGAP*1+5+8, "kW·h" );
	GUI_string_display_At(hp->win_wide-55,DL_VGAP*2+5+8, "kW·h" );
	GUI_string_display_At(hp->win_wide-55,DL_VGAP*3+5+8, "kW·h" );
	
	/*pOBJ=Gui_button_creat(hp->win_wide-88,hp->win_high-40,80,30,BUTTON_ICON|KEY_EXIT,hp);
	GUI_button_Set_icon(pOBJ,(void*)gImage_exit,0);
	pOBJ=Gui_button_creat(hp->win_wide-(90*3)-13,hp->win_high-40,85,30,BUTTON_PAINT|GUI_KEY_ZZ_BEGIN,hp);
	GUI_button_title_display(pOBJ, (char *)m_ZZbegin[g_language]);
	pOBJ=Gui_button_creat(hp->win_wide-(90*2)-6,hp->win_high-40,85,30,BUTTON_PAINT|GUI_KEY_ZZ_END,hp);
	GUI_button_title_display(pOBJ, (char *)m_ZZend[g_language]);*/
	GUI_window_hwin_release(hp);
	return;
}
/********************************************************************
//编辑框初始化
********************************************************************/
static void _edit_init0(hwin_t*pOBJ)
{
	const char *m_name[2]  	= {"现场校验仪",""};
	const char *m_measure[2]= {"测量电能",""};
	GUI_window_hwin_req(pOBJ);
    GUI_SetFront(&GUI_F32X64_ASCII);
    GUI_edit_memeber_add(20+45*0,10,1,"",MEMBER_3D|EDIT_INSTRUMENT_0);
    GUI_edit_memeber_add(20+45*1,10,1,"",MEMBER_3D|EDIT_INSTRUMENT_1);
    GUI_edit_memeber_add(20+45*2,10,1,"",MEMBER_3D|EDIT_INSTRUMENT_2);
    GUI_edit_memeber_add(20+45*3,10,1,"",MEMBER_3D|EDIT_INSTRUMENT_3);
    GUI_edit_memeber_add(20+45*4,10,1,"",MEMBER_3D|EDIT_INSTRUMENT_4);
    GUI_edit_memeber_add(20+45*5,10,1,"",MEMBER_3D|EDIT_INSTRUMENT_5);
    GUI_fillcircle(pOBJ->win_x+20+45*3-3,55+pOBJ->win_y,3,zz_FG_COLOR);
    GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(zz_WIN_BG_COLOR);    
	GUI_set_Fgcolor(zz_FG_COLOR);   
	GUI_DISPLAY_IN_MID(pOBJ->win_high-35,(char *)m_name[g_language],GUI_conText.CUR_Front->CHwide);
    GUI_edit_memeber_add(15,95,8,(char *)m_measure[g_language],MEMBER_3D|EDIT_INSTRUMENT_6);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(223,105, "kW·h" );
	GUI_window_hwin_release(pOBJ);
    return;
}
static void _edit_init1(hwin_t*pOBJ)
{
	const char *m_meter[2]= {"被测电能表",""};
	const char *m_start[2]= {"起",""};
	const char *m_end[2]  = {"末",""};
	GUI_window_hwin_req(pOBJ);
    GUI_SetFront(&GUI_F32X64_ASCII);
    GUI_edit_memeber_add(30+45*0,10,1,"",MEMBER_3D|EDIT_METER_S0);
    GUI_edit_memeber_add(30+45*1,10,1,"",MEMBER_3D|EDIT_METER_S1);
    GUI_edit_memeber_add(30+45*2,10,1,"",MEMBER_3D|EDIT_METER_S2);
    GUI_edit_memeber_add(30+45*3,10,1,"",MEMBER_3D|EDIT_METER_S3);
    GUI_edit_memeber_add(50+45*4,10,1,"",MEMBER_3D|EDIT_METER_S4);
    GUI_edit_memeber_add(50+45*5,10,1,"",MEMBER_3D|EDIT_METER_S5);
    GUI_edit_memeber_add(30+45*0,90,1,"",MEMBER_3D|EDIT_METER_E0);
    GUI_edit_memeber_add(30+45*1,90,1,"",MEMBER_3D|EDIT_METER_E1);
    GUI_edit_memeber_add(30+45*2,90,1,"",MEMBER_3D|EDIT_METER_E2);
    GUI_edit_memeber_add(30+45*3,90,1,"",MEMBER_3D|EDIT_METER_E3);
    GUI_edit_memeber_add(50+45*4,90,1,"",MEMBER_3D|EDIT_METER_E4);
    GUI_edit_memeber_add(50+45*5,90,1,"",MEMBER_3D|EDIT_METER_E5);
	
    GUI_fillcircle(pOBJ->win_x+40+45*4,70+pOBJ->win_y,3,zz_FG_COLOR);
    GUI_fillcircle(pOBJ->win_x+40+45*4,150+pOBJ->win_y,3,zz_FG_COLOR);

	
    GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(zz_WIN_BG_COLOR);    
	GUI_set_Fgcolor(zz_FG_COLOR);   
	GUI_DISPLAY_IN_MID(pOBJ->win_high-30,(char *)m_meter[g_language],GUI_conText.CUR_Front->CHwide);
    GUI_string_display_At(2,30,(char *)m_start[g_language]);
    GUI_string_display_At(2,110,(char *)m_end[g_language]);
	GUI_window_hwin_release(pOBJ);
    return;
}



/********************************************************************
//主窗口
********************************************************************/
void app_zzmod_window_main_creat(void)
{
    hwin_t*pOBJ;
	const char *m_ZZmeasure[2]= {"走字测量",""};
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Bgcolor(zz_BG_COLOR);
	GUI_clean_screen();
	GUI_set_Bgcolor(zz_WIN_BG_COLOR);    
	GUI_set_Fgcolor(zz_FG_COLOR);   
    m_zz_3DWIN=Gui_3D_window_creat(2,2,636,475,(char *)m_ZZmeasure[g_language],WIN_3D|WIN_FILL|WIN_CLOSE_ICON_DIS,GET_GUIWIN);
	GUI_set_Bgcolor(0);
	pOBJ =Gui_txtbox_creat(5,5,m_zz_3DWIN->win_wide-20,220,WIN_FILL|WIN_3D|0,m_zz_3DWIN);
	GUI_window_hwin_req(pOBJ);
	GUI_set_Bgcolor(zz_WIN_BG_COLOR);    
	GUI_set_Fgcolor(zz_FG_COLOR); 	
	m_instrument_edit = GUI_edit_creat(310,15,312,185,WIN_3D|WIN_FILL|0,pOBJ);
	_edit_init0(m_instrument_edit);
	m_zz_edit_win = GUI_edit_creat(0,0,290,220,WIN_3D|WIN_MSG|WIN_TOUCH|WIN_FILL,pOBJ );
	app_ZZ_edit_window_init(m_zz_edit_win);
	//m_meter_edit = GUI_edit_creat(400,20,350,215,WIN_3D|WIN_FILL|1,pOBJ);
	//_edit_init1(m_meter_edit);
	GUI_window_hwin_release(pOBJ);
	m_zz_DL_win   = GUI_edit_creat(2,230,630,170,WIN_3D,m_zz_3DWIN );
	app_ZZ_DLmember_window_init(m_zz_DL_win);
	m_Meter_Treaty_edit = GUI_edit_creat(2,402,630,45,WIN_3D|WIN_MSG|WIN_TOUCH,m_zz_3DWIN );
	app_ZZ_meter_treaty_window_init(m_Meter_Treaty_edit);
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}
/********************************************************************
 * @创建人 :周炳权
 * @功能 :销毁主窗口
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_zzmod_window_destroy( void )
{
	GUI_window_destory( m_zz_3DWIN );
	return;
}


