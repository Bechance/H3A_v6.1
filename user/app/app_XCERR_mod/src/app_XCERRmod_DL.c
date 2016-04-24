/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:绘制六角图
* 创建时间:2010-11-18
* 所属公司 :科荟
* 文件名称:app_startmod_LJT.c
* 创建人 :揭成
* 功能描述:根据电参数绘制六角图
******************************************************************************************/
#include    "app_XCerrmod_inside.h"
/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
static void XCERR_FDL_view(float fdata,int id)
{
    if(fdata==0){GUI_edit_string_display_At("",id);return;}
	if(GLOBAL_ABS(fdata)<0.0001){GUI_edit_string_display_At("0.00001",id);return;}
	GUI_edit_float_display_at(fdata,id,7);
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :电参数显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
void app_XCERRmod_DLarg_display(measure_param_t*pDL)
{
	char temp[10]="";
	char ch[2]="";
	GUI_window_hwin_req(m_XCERR_DL_win);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	XCERR_FDL_view(pDL->phase_a.f_U,EDIT_U_A);
	XCERR_FDL_view(pDL->phase_a.f_I,EDIT_I_A);
	XCERR_FDL_view(pDL->phase_a.f_J,EDIT_J_A);
	XCERR_FDL_view(pDL->phase_a.f_P,EDIT_P_A);
	XCERR_FDL_view(pDL->phase_a.f_Q,EDIT_Q_A);

	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	XCERR_FDL_view(pDL->phase_b.f_U,EDIT_U_B);
	XCERR_FDL_view(pDL->phase_b.f_I,EDIT_I_B);
	XCERR_FDL_view(pDL->phase_b.f_J,EDIT_J_B);
	XCERR_FDL_view(pDL->phase_b.f_P,EDIT_P_B);
	XCERR_FDL_view(pDL->phase_b.f_Q,EDIT_Q_B);
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	XCERR_FDL_view(pDL->phase_c.f_U,EDIT_U_C);
	XCERR_FDL_view(pDL->phase_c.f_I,EDIT_I_C);
	XCERR_FDL_view(pDL->phase_c.f_J,EDIT_J_C);
	XCERR_FDL_view(pDL->phase_c.f_P,EDIT_P_C);
	XCERR_FDL_view(pDL->phase_c.f_Q,EDIT_Q_C);

	GUI_set_Fgcolor(C_GUI_YELLOW);
	XCERR_FDL_view(pDL->f_Pabc,EDIT_PABC);
	XCERR_FDL_view(pDL->f_Qabc,EDIT_QABC);
	XCERR_FDL_view(pDL->f_JUab,EDIT_JUAB);
	XCERR_FDL_view(pDL->f_JUbc,EDIT_JUBC);
	XCERR_FDL_view(pDL->f_JUac,EDIT_JUAC);
	XCERR_FDL_view(pDL->f_JIac,EDIT_JIAC);
	XCERR_FDL_view(pDL->f_Feq, EDIT_FEQ);
	if(pDL->f_Cos==1)
	{
		GUI_edit_string_display_At("1.00",EDIT_COS);
	}
    else if(pDL->f_Cos==0)
    {
		GUI_edit_string_display_At("   ",EDIT_COS);
    }
	else
	{
		global_ftoa(pDL->f_Cos,temp,2);
		ch[0] = pDL->cos_LC;
		global_strcat(temp,ch);
		GUI_edit_string_display_At(temp,EDIT_COS);
	}
	GUI_window_hwin_release(m_XCERR_DL_win);
	return;
}

void app_DLview_task( void*arg )
{
    int cnt = 0;
	DL_flag_t DL_flag;
	msleep(APP_100_MS);
    app_LJT_init(m_XCERR_LJT_win);
	while ( 1 )
	{
		msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )/*判断是否有删除请求*/
		{
			os_task_delete( SELF_PRO );   			
		}
		if(++cnt%60==0)
		{
		    app_global_DLarg_obtain(&g_DL_param);
		    app_global_DLflag_obtain(&DL_flag);
			app_LJT_display(m_XCERR_LJT_win,&g_DL_param,&DL_flag,1);
			app_XCERRmod_DLarg_display(&g_DL_param);
		}
	} 
}

void app_XCERR_DLpaint(void)
{
	DL_flag_t   DL_flag;
	measure_param_t *pdl = &g_DL_param;
    if(GLOBAL_ABS(pdl->f_JUac)<70&&(pdl->phase_b.f_U <40)
    &&(pdl->phase_a.f_U >40)&&(pdl->phase_c.f_U >40))
    {
        DL_flag.phase3or4 = JXMOD_3_3;
    }
    else 
        DL_flag.phase3or4 = JXMOD_3_4;
	app_LJT_display(m_XCERR_LJT_win,pdl,&DL_flag,0);
	app_XCERRmod_DLarg_display(pdl);
    return;
}
