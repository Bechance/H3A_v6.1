/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块界面函数集合
* 创建时间:2010-11-19
* 所属公司 :科荟
* 文件名称:app_ERRmod_interface.c
* 创建人:揭成
* 功能描述:提供系统设置界面画屏函数API
******************************************************************************************/
#include"app_ERRmod_inside.h"
/*********************************************************************************/
hwin_t*m_errmod_correct_win  = PNULL;
hwin_t*m_errmod_Ucorrect_win = PNULL;
hwin_t*m_errmod_Icorrect_win = PNULL;
hwin_t*m_errmod_TMRcorrect_win = PNULL;
hwin_t*m_errmod_drop_dlsr 	= PNULL;
/********************************************************************
 * @作者 :揭成
 * @功能 :编辑成员创建
 *
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void app_ERRmod_Icorrect_DLarg_add(hwin_t*hp)
{
#define IGAP_X  12
#define IGAP_Y  25
#define Y_IGAP  29
	GUI_window_hwin_req(hp);
	GUI_edit_memeber_add(IGAP_X+120*0,IGAP_Y+Y_IGAP*0,9,"",MEMBER_3D|ERR_U_A);
	GUI_edit_memeber_add(IGAP_X+120*1,IGAP_Y+Y_IGAP*0,9,"",MEMBER_3D|ERR_I_A);
	GUI_edit_memeber_add(IGAP_X+120*2,IGAP_Y+Y_IGAP*0,9,"",MEMBER_3D|ERR_J_A);
	GUI_edit_memeber_add(IGAP_X+120*3,IGAP_Y+Y_IGAP*0,9,"",MEMBER_3D|ERR_P_A);
	GUI_edit_memeber_add(IGAP_X+120*4,IGAP_Y+Y_IGAP*0,9,"",MEMBER_3D|ERR_Q_A);

	GUI_edit_memeber_add(IGAP_X+120*0,IGAP_Y+Y_IGAP*1,9,"",MEMBER_3D|ERR_U_B);
	GUI_edit_memeber_add(IGAP_X+120*1,IGAP_Y+Y_IGAP*1,9,"",MEMBER_3D|ERR_I_B);
	GUI_edit_memeber_add(IGAP_X+120*2,IGAP_Y+Y_IGAP*1,9,"",MEMBER_3D|ERR_J_B);
	GUI_edit_memeber_add(IGAP_X+120*3,IGAP_Y+Y_IGAP*1,9,"",MEMBER_3D|ERR_P_B);
	GUI_edit_memeber_add(IGAP_X+120*4,IGAP_Y+Y_IGAP*1,9,"",MEMBER_3D|ERR_Q_B);

	GUI_edit_memeber_add(IGAP_X+120*0,IGAP_Y+Y_IGAP*2,9,"",MEMBER_3D|ERR_U_C);
	GUI_edit_memeber_add(IGAP_X+120*1,IGAP_Y+Y_IGAP*2,9,"",MEMBER_3D|ERR_I_C);
	GUI_edit_memeber_add(IGAP_X+120*2,IGAP_Y+Y_IGAP*2,9,"",MEMBER_3D|ERR_J_C);
	GUI_edit_memeber_add(IGAP_X+120*3,IGAP_Y+Y_IGAP*2,9,"",MEMBER_3D|ERR_P_C);
	GUI_edit_memeber_add(IGAP_X+120*4,IGAP_Y+Y_IGAP*2,9,"",MEMBER_3D|ERR_Q_C);

	GUI_edit_memeber_add(IGAP_X+120*0,IGAP_Y+Y_IGAP*3,12,"SUM_S:",ERR_SUM_S|MEMBER_3D);
	GUI_edit_memeber_add(IGAP_X+120*3-70,IGAP_Y+Y_IGAP*3,12,"SUM_M:",ERR_SUM_M|MEMBER_3D);


	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	GUI_string_display_At(1,IGAP_Y+Y_IGAP*0+2,"A");
	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	GUI_string_display_At(1,IGAP_Y+Y_IGAP*1+2,"B");
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	GUI_string_display_At(1,IGAP_Y+Y_IGAP*2+2,"C");
	GUI_set_Fgcolor(ERRmod_FG_COLOR);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(IGAP_X+120*0+34,5,"U(V)");
	GUI_string_display_At(IGAP_X+120*1+34,5,"I(A)");
	GUI_string_display_At(IGAP_X+120*2+34,5,"Φ(°)");
	GUI_string_display_At(IGAP_X+120*3+34,5,"P(W)");
	GUI_string_display_At(IGAP_X+120*4+34,5,"Q(var)");
    GUI_window_hwin_release(hp);
   return;
}

void app_ERRmod_radio_way_init(hwin_t *hp)
{
	GUI_window_hwin_req(hp);
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
    GUI_radio_memeber_add(2+90*0,3,"满度",1);
    GUI_radio_memeber_add(2+90*1,3,"线性",2);
    GUI_radio_memeber_add(2+90*2,3,"角差",3);
    GUI_radio_memeber_add(2+90*3,3,"角线",4);
    GUI_radio_memeber_add(2+90*4,3,"容线",5);
	GUI_window_hwin_release(hp);
	GUI_radio_select(hp,1);
    return;
}
void app_ERRmod_radio_phase_init(hwin_t *hp)
{
	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
    GUI_radio_memeber_add(2+46*0,3,"A",0);
	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
    GUI_radio_memeber_add(2+46*1,3,"B",1);
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
    GUI_radio_memeber_add(2+46*2,3,"C",2);
	GUI_set_Fgcolor(C_GUI_WHITE);
    GUI_radio_memeber_add(2+46*3,3,"合",3);
	GUI_window_hwin_release(hp);
	GUI_radio_select(hp,0);
    return;
}
void app_ERRmod_notice_init(hwin_t *hp)
{
	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_string_display_At(10,5,"按<确认>修正,<切换>选择对象,上下键选择相别,按<系统>切换误差模式.");
	GUI_window_hwin_release(hp);
    return;
}

static void app_ERRmod_Err_cat(float err,Bsize_t ch_len,char*spst)
{
	char str[10] = "";
	char pst[15] = "";
	Bsize_t len;
    if(err>0)     {spst[0]='+';}
    else if(err<0){spst[0]='-';err=err*(-1.0);}
	
	global_ftoa(err,pst,3);
	len = global_strlen(pst);
	if ( len > ch_len )
	{
		pst[ch_len] = ASCII_NULL;
	    if(pst[ch_len]>'5')pst[ch_len-1]+=1;//四舍五入
	}
	if ( pst[ch_len-1] == '.' )
	{
		pst[ch_len-1] = ASCII_NULL;
	}
	else if ( len < ch_len )
	{
		if ( global_string_locate_first( pst, "." ) )
		{
			global_memset( str, ( uint8_t )'0', ( ch_len - len ) );
			global_strcat( pst, str );
		}
		else
		{
			global_memset( str, ( uint8_t )'.', 1 );
			global_memset( str + 1, ( uint8_t )'0', ( ch_len - len - 1 ) );
			global_strcat( pst, str );
		}
	}
	global_strcat(spst,pst);
    return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :误差显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
void app_ERRmod_Err_display(float*perr)
{
	char pst[20] = "";
	GUI_window_hwin_req(GET_DOBJ(ICORRECT_ERR_EDIT));
	GUI_set_Bgcolor(C_GUI_LOW_BLUE);
	GUI_set_Fgcolor(C_GUI_GREEN);
	app_ERRmod_Err_cat(perr[0],5,pst);
	global_strcat(pst,"% ");
	GUI_SetFront(&GUI_F16X32_ASCII);
    GUI_string_display_At(25,60,pst);  
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GLOBAL_MEMCLR(pst,20);
	app_ERRmod_Err_cat(perr[3],5,pst);
	GUI_string_display_At(250,10,"         ");
	GUI_string_display_At(250,10,pst);
	GLOBAL_MEMCLR(pst,20);
	app_ERRmod_Err_cat(perr[2],5,pst);
	GUI_string_display_At(250,30,"         ");
	GUI_string_display_At(250,30,pst);
	GLOBAL_MEMCLR(pst,20);
	app_ERRmod_Err_cat(perr[1],5,pst);
	GUI_string_display_At(250,50,"         ");
	GUI_string_display_At(250,50,pst);
	GUI_window_hwin_release(GET_DOBJ(ICORRECT_ERR_EDIT));
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :圈数显示
 * @输入 :
 * @输出 :NONE
********************************************************************/
void app_ERRmod_circle_display(Bsize_t circle)
{
    char ctemp[10]="";
	GUI_window_hwin_req(GET_DOBJ(ICORRECT_ERR_EDIT));
	GUI_set_Bgcolor(C_GUI_LOW_BLUE);
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	global_itoa(circle,ctemp);
    GUI_string_display_At(2,30,"    ");
    GUI_string_display_At(2,30,ctemp);
	GUI_window_hwin_release(GET_DOBJ(ICORRECT_ERR_EDIT));
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :用户设置窗口回调
 * @输入 :
 * @输出 : NONE
********************************************************************/
void app_ERRmod_user_edit_init(hwin_t *hp)
{
	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_edit_memeber_add(0,  2, 8, "常数", MEMBER_3D|ERR_CONST);
	GUI_edit_memeber_add(160,2, 8, "圈数", MEMBER_3D|ERR_QS);
	GUI_edit_memeber_add(0,  34,8, "计量", MEMBER_3D|ERR_MODE);
	GUI_edit_memeber_add(160,34,8, "倍率", MEMBER_3D|ERR_BB1);
	GUI_string_display_At(3, 70,   "输入");
	GUI_edit_memeber_add(170,70,8, "模式", MEMBER_3D|ERR_QA_MOD);
	m_errmod_drop_dlsr=Gui_drop_list_creat(53,68,
		g_dlsr_option[g_language],
		SIZE_OF_ARRAY(g_dlsr_option[g_language]),KEY_F2,hp);
	GUI_window_hwin_release(hp);
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :误差窗口回调
 * @输入 :
 * @输出 : NONE
********************************************************************/
void app_ERRmod_Err_edit_init(hwin_t *hp)
{
	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(C_GUI_LOW_BLUE);
	GUI_clean_window();
	GUI_set_Fgcolor(C_GUI_WHITE);	
	GUI_string_display_At(2,2,"Err:");
	GUI_window_hwin_release(hp);
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :修正窗口回调
 * @输入 :
 * @输出 : NONE
********************************************************************/
void app_ERRmod_Err_correct_init(hwin_t *hp)
{
#define LAB_Y  30
#define GAP_Y  32
	GUI_window_hwin_req(hp);
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
	GUI_set_Fgcolor(ERRmod_FG_COLOR);	
	GUI_edit_memeber_add(2,	 LAB_Y+GAP_Y*0,8,"满度",MEMBER_3D|ERR_U_CORRECT_MAIN);
	GUI_edit_memeber_add(180,LAB_Y+GAP_Y*0,8,"",MEMBER_3D|ERR_I_CORRECT_MAIN);
	GUI_edit_memeber_add(2,  LAB_Y+GAP_Y*1,8,"线性",MEMBER_3D|ERR_U_CORRECT_LINE);
	GUI_edit_memeber_add(180,LAB_Y+GAP_Y*1,8,"",MEMBER_3D|ERR_I_CORRECT_LINE);
	GUI_edit_memeber_add(2,  LAB_Y+GAP_Y*2,8,"角差",MEMBER_3D|ERR_U_CORRECT_ANGLE);
	GUI_edit_memeber_add(180,LAB_Y+GAP_Y*2,8,"",MEMBER_3D|ERR_I_CORRECT_ANGLE);
	GUI_edit_memeber_add(2,  LAB_Y+GAP_Y*3,8,"角线",MEMBER_3D|ERR_U_CORRECT_ANGLELINE);
	GUI_edit_memeber_add(180,LAB_Y+GAP_Y*3,8,"",MEMBER_3D|ERR_I_CORRECT_ANGLELINE);
	GUI_edit_memeber_add(2,  LAB_Y+GAP_Y*4,8,"容线",MEMBER_3D|ERR_U_CORRECT_CANGLELINE);
	GUI_edit_memeber_add(180,LAB_Y+GAP_Y*4,8,"",MEMBER_3D|ERR_I_CORRECT_CANGLELINE);
	GUI_string_display_At(92,1,"U(%)");
    GUI_string_display_At(200,1,"I(%)");
	GUI_window_hwin_release(hp);
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :源输出
 * @输入 :
 * @输出 :NONE
********************************************************************/
void app_ERRmod_XFNZ_init(hwin_t *hp)
{
	return;
}

const dialogBox_t  app_errmod_Idialogbox[]=
{
	{WIDGET_EDIT ,ICORRECT_USER_EDIT        ,""	,2	,0	,320,100,WIN_3D|WIN_FILL|WIN_TOUCH_EN ,0,0,app_ERRmod_user_edit_init},	
	{WIDGET_EDIT ,ICORRECT_ERR_EDIT         ,""	,2	,100,320,100,WIN_3D						  ,0,0,app_ERRmod_Err_edit_init},	
	{WIDGET_EDIT ,ICORRECT_CORRECT_DATA_EDIT,""	,322,0	,316,200,WIN_3D|WIN_FILL			  ,0,0,app_ERRmod_Err_correct_init},	
	{WIDGET_RADIO,ICORRECT_PHASE_RADIO      ,""	,2	,200,186,35	,WIN_3D|WIN_FILL|GUI_KEY_PHASE,0,0,app_ERRmod_radio_phase_init},	
	{WIDGET_RADIO,ICORRECT_WAY_RADIO		,""	,188,200,450,35	,WIN_3D|WIN_FILL|GUI_KEY_WAY  ,0,0,app_ERRmod_radio_way_init},	
	{WIDGET_EDIT ,ICORRECT_DL_EDIT			,""	,2	,235,636,144,WIN_3D|WIN_FILL	          ,0,0,app_ERRmod_Icorrect_DLarg_add},
	{WIDGET_EDIT ,ICORRECT_NOTICE_EDIT		,""	,2	,379,636,80	,WIN_3D|WIN_FILL		      ,0,0,app_ERRmod_notice_init},
};
/********************************************************************
 * @作者 : 揭成
 * @功能 : 功率校正窗口
 * @输入 : NONE
 * @输出  :NONE
********************************************************************/
void app_ERRmod_Iwindow_creat(void)
{
    hwin_t*pOBJ;
    GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Fgcolor(ERRmod_FG_COLOR);
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
	m_errmod_Icorrect_win = GUI_dialogBox_create("电流修正",0,0,LCD_WIDE-2,LCD_HIGH,
	WIN_3D|WIN_FILL,
	app_errmod_Idialogbox,
	SIZE_OF_ARRAY(app_errmod_Idialogbox));

	GUI_SetFront(&GUI_HZK16_EXTASCII);
    pOBJ=Gui_button_creat(45+110*0,m_errmod_Icorrect_win->win_high-38,108,35,BUTTON_PAINT,m_errmod_Icorrect_win);
	GUI_button_title_display(pOBJ,"电流输入");
    pOBJ=Gui_button_creat(45+110*1,m_errmod_Icorrect_win->win_high-38,108,35,BUTTON_PAINT,m_errmod_Icorrect_win);
	GUI_button_title_display(pOBJ,"匹配圈数");
    pOBJ=Gui_button_creat(45+110*2,m_errmod_Icorrect_win->win_high-38,108,35,BUTTON_PAINT,m_errmod_Icorrect_win);
	GUI_button_title_display(pOBJ,"推荐常数");
    pOBJ=Gui_button_creat(45+110*3,m_errmod_Icorrect_win->win_high-38,108,35,BUTTON_PAINT,m_errmod_Icorrect_win);
	GUI_button_title_display(pOBJ,"清除全部");
    pOBJ=Gui_button_creat(45+110*4,m_errmod_Icorrect_win->win_high-38,108,35,BUTTON_PAINT,m_errmod_Icorrect_win);
	GUI_button_title_display(pOBJ,"清除当前");

	
    GUI_window_hwin_release(GET_GUIWIN);
	return;
}

void app_ERRmod_Iwindow_destroy(void)
{
	GUI_window_destory(GET_DOBJ(ICORRECT_USER_EDIT));
	GUI_window_destory(GET_DOBJ(ICORRECT_ERR_EDIT));
	GUI_window_destory(GET_DOBJ(ICORRECT_CORRECT_DATA_EDIT));
	GUI_window_destory(GET_DOBJ(ICORRECT_PHASE_RADIO));
	GUI_window_destory(GET_DOBJ(ICORRECT_WAY_RADIO));
	GUI_window_destory(GET_DOBJ(ICORRECT_DL_EDIT));
	GUI_window_destory(GET_DOBJ(ICORRECT_NOTICE_EDIT));
	GUI_window_destory( m_errmod_drop_dlsr );
	GUI_window_destory( m_errmod_Icorrect_win );
	
    return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :编辑成员创建
 *
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void app_ERRmod_Ucorrect_arg_add(hwin_t*hp)
{
#define LSB_YN  45
#define GAP_YN  56
	hwin_t *button;
	GUI_window_hwin_req(hp);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_edit_memeber_add(8,10,8,"基准",MEMBER_3D|ERR_BASE_U);
	GUI_edit_memeber_add(186,10,8,"    ",MEMBER_3D|ERR_U_LEVEL);
	GUI_edit_memeber_add(0,LSB_YN+GAP_YN*1,8,"     ", MEMBER_3D|ERR_U_A);
	GUI_edit_memeber_add(210,LSB_YN+GAP_YN*1,8,"  ",  MEMBER_3D|ERR_CORRECT_A);
	GUI_edit_memeber_add(0,LSB_YN+GAP_YN*2,8,"     ", MEMBER_3D|ERR_U_B);
	GUI_edit_memeber_add(210,LSB_YN+GAP_YN*2,8,"  ",  MEMBER_3D|ERR_CORRECT_B);
	GUI_edit_memeber_add(0,LSB_YN+GAP_YN*3,8,"     ", MEMBER_3D|ERR_U_C);
	GUI_edit_memeber_add(210,LSB_YN+GAP_YN*3,8,"  ",  MEMBER_3D|ERR_CORRECT_C);
	GUI_set_Fgcolor(ERRmod_FG_COLOR);
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
    GUI_window_line_display(0,68,440,68);
    GUI_window_line_display(0,260,440,260);
	GUI_string_display_At(170,12,"V");
	GUI_string_display_At(75,76,"实测(V)");
	GUI_string_display_At(255,76,"精度(%)");
	GUI_set_Fgcolor(C_GUI_DLA_COLOR);
	GUI_string_display_At(20,LSB_YN+GAP_YN*1+2,"A");
	GUI_set_Fgcolor(C_GUI_DLB_COLOR);
	GUI_string_display_At(20,LSB_YN+GAP_YN*2+2,"B");
	GUI_set_Fgcolor(C_GUI_DLC_COLOR);
	GUI_string_display_At(20,LSB_YN+GAP_YN*3+2,"C");
	GUI_set_Fgcolor(C_GUI_YELLOW);
	GUI_string_display_At(10,LSB_YN+GAP_YN*4+2,
		"按<确认>开始修正电压,按<设置>调整基准,按<F5>清除修正数据.");
	GUI_window_hwin_release(hp);
    button=Gui_button_creat(30,400, 90,40,BUTTON_PAINT|GUI_KEY_SET,hp);
    GUI_button_title_display(button,"设置");	
	button=Gui_button_creat(130,400,90,40,BUTTON_PAINT|KEY_F5,hp);
    GUI_button_title_display(button,"清除");
    button=Gui_button_creat(230,400,90,40,BUTTON_PAINT|GUI_KEY_ENTER,hp);
    GUI_button_title_display(button,"确认");
	button=Gui_button_creat(330,400,90,40,BUTTON_PAINT|GUI_KEY_EXIT,hp);
    GUI_button_title_display(button,"退出");
    return;
}
const dialogBox_t  app_errmod_Udialogbox[]=
{
	{WIDGET_EDIT,UCORRECT_EDIT,"",100,5,420,460,WIN_TOUCH_EN,0,0,app_ERRmod_Ucorrect_arg_add}
};
/********************************************************************
 * @作者 : 揭成
 * @功能 : 功率校正窗口
 * @输入 : NONE
 * @输出  :NONE
********************************************************************/
void app_ERRmod_Uwindow_creat(void)
{
    GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Fgcolor(ERRmod_FG_COLOR);
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
	m_errmod_Ucorrect_win = GUI_dialogBox_create("电压修正",0,0,LCD_WIDE-2,LCD_HIGH,
	WIN_3D|WIN_FILL,
	app_errmod_Udialogbox,
	SIZE_OF_ARRAY(app_errmod_Udialogbox));
    GUI_window_hwin_release(GET_GUIWIN);
	return;
}
void app_ERRmod_Uwindow_destroy( void )
{
	GUI_window_destory( m_errmod_Ucorrect_win );
	return;
}


void app_ERRmod_TMRwindow_creat(void)
{
	hwin_t *button;
	hwin_t *hp;
    GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Fgcolor(ERRmod_FG_COLOR);
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
	m_errmod_TMRcorrect_win= Gui_3D_window_creat(
	0,0,LCD_WIDE-2,LCD_HIGH,(char*)"时钟误差修正",
	WIN_3D|WIN_FILL,0);
	hp = m_errmod_TMRcorrect_win;
	GUI_window_hwin_req(hp);
	GUI_set_Fgcolor(C_GUI_YELLOW);
	
	GUI_string_display_At(10,45+56*4+2,
		"按<确认>开始修正误差,按<设置>调整参数,按<F5>清除修正数据.");
	GUI_window_hwin_release(hp);
    button=Gui_button_creat(40+120*0,400,90,35,BUTTON_PAINT|GUI_KEY_SET,hp);
    GUI_button_title_display(button,"设置");	
	button=Gui_button_creat(40+120*1,400,90,35,BUTTON_PAINT|KEY_F5,hp);
    GUI_button_title_display(button,"清除");
    button=Gui_button_creat(40+120*2,400,90,35,BUTTON_PAINT|GUI_KEY_ENTER,hp);
    GUI_button_title_display(button,"确认");
	button=Gui_button_creat(40+120*3,400,90,35,BUTTON_PAINT|GUI_KEY_EXIT,hp);
    GUI_button_title_display(button,"退出");
    GUI_window_hwin_release(GET_GUIWIN);
	return;
}

void app_ERRmod_TMRwindow_destroy( void )
{
	GUI_window_destory( m_errmod_TMRcorrect_win );
	return;
}

/**************************************************************************************************************************************/
//主窗口部分
/**************************************************************************************************************************************/
const dialogBox_t  app_errmod_dialogbox[]=
{
	{WIDGET_BUTTON,1,"1.电压修正  ",220,60+90*0,200,50,BUTTON_PAINT|KEY_1,0,0,0},
	{WIDGET_BUTTON,2,"2.电流修正  ",220,60+90*1,200,50,BUTTON_PAINT|KEY_2,0,0,0},
	{WIDGET_BUTTON,3,"3.时钟误差修正",220,60+90*2,200,50,BUTTON_PAINT|KEY_3,0,0,0},
	{WIDGET_BUTTON,4,"4.返回上一级",220,60+90*3,200,50,BUTTON_PAINT|KEY_4,0,0,0}
};
/********************************************************************
 * @作者 : 揭成
 * @功能 : 功率校正窗口
 * @输入 : NONE
 * @输出  :NONE
********************************************************************/
void app_ERRmod_window_creat(void)
{
    GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Fgcolor(ERRmod_FG_COLOR);
	GUI_set_Bgcolor(ERRmod_WIN_BG_COLOR);
	m_errmod_correct_win = GUI_dialogBox_create("系统修正",0,0,LCD_WIDE-2,LCD_HIGH,
	WIN_3D|WIN_FILL,
	app_errmod_dialogbox,
	SIZE_OF_ARRAY(app_errmod_dialogbox));
    GUI_window_hwin_release(GET_GUIWIN);
    return;
}

void app_ERRmod_window_destroy(void)
{
	GUI_window_destory( m_errmod_correct_win );
    return;
}
