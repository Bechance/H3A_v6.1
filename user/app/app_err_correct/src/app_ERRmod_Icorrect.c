/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统设置模块主体程序
* 创建时间:2012-04-01
* 所属公司 :科荟
* 文件名称:app_ERRmod_Icorrect.c
* 作者 :揭成
* 功能描述:实现电流修正
******************************************************************************************/
#include				"app_ERRmod_inside.h"
correct_data_t  		m_errmod_Dcorrect;
uint8_t					m_errmod_phase_shadow=6;
int                     m_errmod_mode        = 0;
static                  int  _QA_mod         = 0;
/*-------------------------------------------------------------------*/
void app_ERRmod_Icorrect_userinfo_display(DL_app_user_all_info_t*pUSER);
/*-------------------------------------------------------------------*/
/********************************************************************
 * @作者 :揭成
 * @功能 :显示修正系数
 * @输入 :
 * @输出 :NONE
********************************************************************/
static void app_ERRmod_Icorrectdata_view(float fdata,Bsize_t id,uint8_t flag)
{
	if(fdata==0)
	{
		GUI_edit_select( id );
		GUI_edit_clean();	 
		GUI_edit_string_display("0.00");
	}
	else if(flag==1)
	{
		fdata = (fdata)*100.0;
		if(fdata==0)
		{
			GUI_edit_select( id );
			GUI_edit_clean();	 
			GUI_edit_string_display("0.00");
		}
		else
		{
			app_ERRmod_DL_float_display(fdata,id);
		}
	}
	else
	{
		app_ERRmod_DL_float_display(fdata,id);
	}
    return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :判断修正数据是否改变
 * @输入 :
 * @输出 :NONE
********************************************************************/
static BOOL app_ERRmod_correct_data_is_change(correct_data_t*pcorrect,uint8_t phase)
{
     BOOL ret;	 
     ret=global_memcmp(&m_errmod_Dcorrect,pcorrect,sizeof(correct_data_t));
	 if(!ret)
	 {
        global_memcpy(&m_errmod_Dcorrect,pcorrect,sizeof(correct_data_t));
	 }
	 if(m_errmod_phase_shadow!=phase)
	 {
        m_errmod_phase_shadow = phase;
	 	return FAIL;
	 }
     return ret;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :电流修正数据显示
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
void app_ERRmod_Icorrect_correct_data_display(uint8_t phase)
{
	correct_data_t*pcorrect;
    float  err_j;
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_GET_CORRECT_DATA,&pcorrect);
    if(!app_ERRmod_correct_data_is_change(pcorrect,phase))
    {
	    GUI_window_hwin_req( GET_DOBJ(ICORRECT_CORRECT_DATA_EDIT) );
		GUI_set_Fgcolor(ERRmod_FG_COLOR);
		GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
		app_ERRmod_Icorrectdata_view(pcorrect->k_Umain[phase],ERR_U_CORRECT_MAIN,1);
		app_ERRmod_Icorrectdata_view(pcorrect->k_Imain[phase],ERR_I_CORRECT_MAIN,1);
		app_ERRmod_Icorrectdata_view(pcorrect->k_Uline[phase],ERR_U_CORRECT_LINE,0);	 
		app_ERRmod_Icorrectdata_view(pcorrect->k_Iline[phase],ERR_I_CORRECT_LINE,1);	 
		app_ERRmod_Icorrectdata_view(0,ERR_U_CORRECT_ANGLE,0);	 
		err_j = pcorrect->k_dxangle[phase]*60.0*0.05;
		app_ERRmod_Icorrectdata_view(err_j,ERR_I_CORRECT_ANGLE,0);	
		app_ERRmod_Icorrectdata_view(0,    ERR_U_CORRECT_ANGLELINE,0);	
		err_j = pcorrect->k_Jline[phase]*60.0*0.05;
		app_ERRmod_Icorrectdata_view(err_j,ERR_I_CORRECT_ANGLELINE,0);
		app_ERRmod_Icorrectdata_view(0,    ERR_U_CORRECT_CANGLELINE,0);	
		err_j = pcorrect->kC_Jline[phase]*60.0*0.05;
		app_ERRmod_Icorrectdata_view(err_j,ERR_I_CORRECT_CANGLELINE,0);
		GUI_window_hwin_release( GET_DOBJ(ICORRECT_CORRECT_DATA_EDIT) );
	}
}
/********************************************************************
 * @作者 :揭成
 * @功能 :清空当前相电参数数据
 * @输入 :
 * @输出 :NONE
********************************************************************/
static app_ERRmod_Icorrect_DLarg_clean_edit(uint8_t phase)
{
	GUI_window_hwin_req( GET_DOBJ(ICORRECT_DL_EDIT) );
	GUI_set_Fgcolor(ERRmod_FG_COLOR);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);

	GUI_edit_string_display_At((char*)" ",ERR_U_A);
	GUI_edit_string_display_At((char*)" ",ERR_I_A);
	GUI_edit_string_display_At((char*)" ",ERR_J_A);	 
	GUI_edit_string_display_At((char*)" ",ERR_P_A);  
	GUI_edit_string_display_At((char*)" ",ERR_Q_A); 

	GUI_edit_string_display_At((char*)" ",ERR_U_B);
	GUI_edit_string_display_At((char*)" ",ERR_I_B);
	GUI_edit_string_display_At((char*)" ",ERR_J_B);	 
	GUI_edit_string_display_At((char*)" ",ERR_P_B);  
	GUI_edit_string_display_At((char*)" ",ERR_Q_B); 

	GUI_edit_string_display_At((char*)" ",ERR_U_C);
	GUI_edit_string_display_At((char*)" ",ERR_I_C);
	GUI_edit_string_display_At((char*)" ",ERR_J_C);	 
	GUI_edit_string_display_At((char*)" ",ERR_P_C);  
	GUI_edit_string_display_At((char*)" ",ERR_Q_C); 

	GUI_window_hwin_release( GET_DOBJ(ICORRECT_DL_EDIT) );
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :电流修正数据显示
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
extern   float           m_meter_SUM[3];
extern   float           m_PEC_SUM[3];
void app_ERRmod_Icorrect_DLarg_display(measure_param_t*pDL,uint8_t phase)
{
	GUI_window_hwin_req( GET_DOBJ(ICORRECT_DL_EDIT) );
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	if(phase==0||phase==3)
	{
		GUI_set_Fgcolor(C_GUI_DLA_COLOR);
		app_ERRmod_DL_float_display(pDL->phase_a.f_U,ERR_U_A);
		app_ERRmod_DL_float_display(pDL->phase_a.f_I,ERR_I_A);
		app_ERRmod_DL_float_display(pDL->phase_a.f_J,ERR_J_A);	 
		app_ERRmod_DL_float_display(pDL->phase_a.f_P,ERR_P_A);  
		app_ERRmod_DL_float_display(pDL->phase_a.f_Q,ERR_Q_A);  
	}
	if(phase==1||phase==3)
	{
		GUI_set_Fgcolor(C_GUI_DLB_COLOR);
		app_ERRmod_DL_float_display(pDL->phase_b.f_U,ERR_U_B);
		app_ERRmod_DL_float_display(pDL->phase_b.f_I,ERR_I_B);
		app_ERRmod_DL_float_display(pDL->phase_b.f_J,ERR_J_B);	 
		app_ERRmod_DL_float_display(pDL->phase_b.f_P,ERR_P_B);  
		app_ERRmod_DL_float_display(pDL->phase_b.f_Q,ERR_Q_B); 
	}
	if(phase==2||phase==3)
	{
		GUI_set_Fgcolor(C_GUI_DLC_COLOR);
		app_ERRmod_DL_float_display(pDL->phase_c.f_U,ERR_U_C);
		app_ERRmod_DL_float_display(pDL->phase_c.f_I,ERR_I_C);
		app_ERRmod_DL_float_display(pDL->phase_c.f_J,ERR_J_C);	 
		app_ERRmod_DL_float_display(pDL->phase_c.f_P,ERR_P_C);  
		app_ERRmod_DL_float_display(pDL->phase_c.f_Q,ERR_Q_C); 
	}
	GUI_set_Fgcolor(C_GUI_WHITE);
    GUI_edit_float_display_at(m_PEC_SUM[0],ERR_SUM_S,10);
    GUI_edit_float_display_at(m_meter_SUM[0],ERR_SUM_M,10);
	GUI_window_hwin_release( GET_DOBJ(ICORRECT_DL_EDIT) );
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :电流修正用户参数显示
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
void app_ERRmod_Icorrect_userinfo_display(DL_app_user_all_info_t*pUSER)
{
	GUI_window_hwin_req( GET_DOBJ(ICORRECT_USER_EDIT) );
	GUI_set_Fgcolor(ERRmod_FG_COLOR);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_edit_string_display_At(g_user_arg.DL_app_user_set.cs, ERR_CONST);
	GUI_edit_string_display_At(g_user_arg.DL_app_user_set.qs, ERR_QS);
	GUI_edit_string_display_At(g_user_arg.DL_app_user_set.bb1, ERR_BB1);
    if(m_errmod_mode==0)
    {
		GUI_edit_string_display_At("周期模式", ERR_MODE);
	}
    else
    {
		GUI_edit_string_display_At("积分模式", ERR_MODE);
	}
	if(_QA_mod==0)
	{
		GUI_edit_string_display_At("修正", ERR_QA_MOD);
	}
	else
	{
		GUI_edit_string_display_At("QA模式", ERR_QA_MOD);
	}
	GUI_window_hwin_release( GET_DOBJ(ICORRECT_USER_EDIT) );
    return;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :电流修正功能回调
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
uint8_t app_ERRmod_Icorrect_callback(hwin_t*hp)
{
	uint8_t key;
	Bsize_t member;
	DL_app_user_all_info_t user_info;
	GUI_window_hwin_req( hp );
	key_input_method_assign( KEY_INPUT_123 );
	GUI_window_hwin_release( hp );
	global_memcpy(&user_info,&g_user_arg,sizeof(DL_app_user_all_info_t));
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_obtain( &key, 1 ) != 1 )/*判断驱动是否收到按键响应*/
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_set_Fgcolor(ERRmod_FG_COLOR);
		GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
		if (( app_global_decide_inputkey( key ) ) != FAIL )/*判断是否是输入法相关按键按下*/
		{
			member = GUI_edit_id_obtain();    			 /*获取当前编辑成员ID*/
			if ( member == ERR_CONST)
			key_method_input_shadow( key,user_info.DL_app_user_set.cs);
			else if( member == ERR_QS)
			key_method_input_shadow( key,user_info.DL_app_user_set.qs);
		}
		else if(key == KEY_RIGHT )GUI_edit_csr_shift(CSR_RIGHT_SHIFT);
		else if(key == KEY_LEFT )GUI_edit_csr_shift(CSR_LEFT_SHIFT);
		else if(key==KEY_DOWN||key==KEY_UP)
		{
			member = GUI_edit_id_obtain();    
			if(member==ERR_CONST)GUI_edit_select(ERR_QS);
			else if(member==ERR_QS)GUI_edit_select(ERR_CONST);
		}
		GUI_window_hwin_release( hp );
		if(key==KEY_EXIT)
		{
			 app_ERRmod_Icorrect_userinfo_display(&g_user_arg);
             break;
		}
		if(key==KEY_ENTER)
		{
			global_memcpy(&g_user_arg,&user_info,sizeof(DL_app_user_all_info_t));
			app_ERRmod_Icorrect_userinfo_display(&g_user_arg);
			app_global_set_user_dl_arg(&g_user_arg);
			fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
			break;
		}
	}
    return key;
}
/********************************************************************
 * @作者 :揭成
 * @功能 :修正合法性判断
 * @输入 :NONE
 * @输出  :NONE
********************************************************************/
//{"1A端子","10A端子","1A钳表","5A钳表","20A钳表""100A钳""500A钳","1000A钳","1500A钳"};
const float  mERRmod_Itable[]={1.0,5.0,1.0,5.0,20.0,100.0,500.0,1000.0,1500.0};
BOOL app_ERRmod_Icorrect_is_lawful(
	measure_param_t*pdl,
	uint8_t main_setp,
	DL_app_user_all_info_t*pUser,
	uint8_t phase
)
{
     float Main_I=mERRmod_Itable[pUser->DL_app_user_set.dlsr];
     float Line_I=Main_I*0.1;
	 float real_I,dxI;
	 float real_J,dxJ;
	 float Main_J=0;
	 float Line_J=60.0;
	 float errI  = Main_I*0.3;
	 if(phase==A_PHASE){real_I=pdl->phase_a.f_I;real_J=pdl->phase_a.f_J;}
	 else if(phase==B_PHASE){real_I=pdl->phase_b.f_I;real_J=pdl->phase_b.f_J;}
	 else if(phase==C_PHASE){real_I=pdl->phase_c.f_I;real_J=pdl->phase_c.f_J;}
     if(main_setp ==UCORRECT_SETP) return FAIL;
     else if(main_setp>JLINECORRECT_SETP)return FAIL;
	 else if(main_setp==ICORRECT_SETP)
     {
     	dxI = Main_I-real_I;
	 	dxJ = Main_J-real_J;
        if(GLOBAL_ABS(dxI)>errI)return FAIL;
        if(GLOBAL_ABS(dxJ)>5)return FAIL;
	 }
	 else if(main_setp==ILINECORRECT_SETP)
     {
     	dxI = Line_I-real_I;
	 	dxJ = Main_J-real_J;
        if(GLOBAL_ABS(dxI)>errI)return FAIL;
        if(GLOBAL_ABS(dxJ)>5)return FAIL;
	 }
	 else if(main_setp==JCORRECT_SETP)
     {
     	dxI = Main_I-real_I;
	 	dxJ = Line_J-real_J;
        if(GLOBAL_ABS(dxI)>errI)return FAIL;
        if(GLOBAL_ABS(dxJ)>5)return FAIL;
	 } 
     else if(main_setp==JLINECORRECT_SETP)
     {
     	dxI = Line_I-real_I;
	 	dxJ = Line_J-real_J;
        if(GLOBAL_ABS(dxI)>errI)return FAIL;
        if(GLOBAL_ABS(dxJ)>5)return FAIL;
	 }
     return TURE;
}



static int _Qs_select(float W,float Const)
{
    float N;
    N=(W*2.0*Const)/(3600.0*1000.0)+0.5; 
	if(N<2)N=2;
    return(int)N;
}
static void _Constselect_paint(hwin_t* pOBJ)
{
#define _XGAP  90
#define _YGAP  35
//	hwin_t*p;
	uint16_t BK = 0xce79;
	GUI_window_hwin_req(pOBJ);
    GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Bgcolor(BK);
	GUI_set_Fgcolor(C_GUI_BlACK);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*0,"  460",0);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*0,"  800",1);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*0,"  1200",2);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*1," 2400",3);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*1," 4800",4);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*1,"  3600",5);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*2," 2000",6);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*2," 5000",7);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*2,"  8000",8);
    GUI_radio_memeber_add(10+_XGAP*0,10+_YGAP*3,"10000",9);
    GUI_radio_memeber_add(10+_XGAP*1,10+_YGAP*3,"36000",10);
    GUI_radio_memeber_add(10+_XGAP*2,10+_YGAP*3,"100000",11);
	GUI_MsgFIFO_create(pOBJ,2);
	GUI_TopWIN_Set(pOBJ);
	GUI_radio_select(pOBJ,0);
	GUI_window_hwin_release(pOBJ);
    return;
}

static void _Constselect1(void)
{
    int x1,y1,x2,y2;
	Bsize_t  sec;
    uint8_t  key;
	hwin_t*  pOBJ;
	char*    pSTR;
	int      ID = 0,IDbak = 0;
	uint16_t BK = 0xce79;
	x1 = 100;y1 = 100;x2 = x1+300;y2 = y1+200;
	sec=GUI_read_block_Extp(x1,y1,x2, y2);
	GUI_window_hwin_req(GET_GUIWIN);
	Gui_button_set_color(C_GUI_LOW_BLUE,0xce79);
	GUI_set_Bgcolor(BK);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
    pOBJ = GUI_radio_create(x1+1,y1+1,(x2-x1-8),(y2-y1-5),WIN_MSG_EN|WIN_3D|WIN_FILL,GET_GUIWIN);
    _Constselect_paint(pOBJ);
	GUI_WINready(pOBJ,1);
	GUI_window_hwin_release(GET_GUIWIN);
	app_global_key_control_power_req();
	while(1)
	{   
		msleep(TYPE_DELAY);
	    if(GUI_key_read(pOBJ,&key,1)==1
		||app_global_key_force_obtain(&key,1)==1)
	    {
			if(key==KEY_ENTER)
			{
				GUI_WINdo();
				pSTR=(char*)GUI_radio_title_obtain(pOBJ);
				GLOBAL_MEMCLR(g_user_arg.DL_app_user_set.cs,9);
				GLOBAL_MEMCLR(g_user_arg.DL_app_user_set.cs2,9);
				global_strcpy(g_user_arg.DL_app_user_set.cs,pSTR);
				global_strcpy(g_user_arg.DL_app_user_set.cs2,pSTR);
				global_string_filter(g_user_arg.DL_app_user_set.cs,' ');
				global_string_filter(g_user_arg.DL_app_user_set.cs2,' ');
				break;
			}
			else if(key==KEY_EXIT)
				break;
			else if(key==KEY_DOWN)
                ID=((ID+3)<=11)?(ID+=3):(ID); 
			else if(key==KEY_UP)
                ID=(ID>2)?(ID-=3):(ID); 
			else if(key==KEY_RIGHT)
                ID=(ID<11)?(ID+=1):(ID); 
			else if(key==KEY_LEFT)
                ID=(ID>0)?(ID-=1):(ID); 
			if(ID!=IDbak){
				IDbak = ID;
				GUI_SUBordready(ID,1);
			}
		}
	}
	GUI_window_destory(pOBJ);
	GUI_write_block_Extp(x1,y1,x2, y2, sec);
	app_global_key_control_power_release();
	app_module_color_select(2);
	return;
}

static int _CUR_pos = 0;
static void _Constselect(void)
{
	GLOBAL_MEMCLR(g_user_arg.DL_app_user_set.cs,9);
	GLOBAL_MEMCLR(g_user_arg.DL_app_user_set.cs2,9);
	if(_CUR_pos==1)
	{
	    _CUR_pos = 0;
		global_strcpy(g_user_arg.DL_app_user_set.cs,"100000");
		global_strcpy(g_user_arg.DL_app_user_set.cs2,"100000");
	}
	else
	{ 
	    _CUR_pos = 1;
		global_strcpy(g_user_arg.DL_app_user_set.cs,"10000");
		global_strcpy(g_user_arg.DL_app_user_set.cs2,"10000");
	}
    return;
}


static void _CORsend(uint8_t main_step,uint8_t sub_step,uint8_t phase,uint8_t pos)
{
	os_driver_ioctl( g_DL_dev, MEASURE_CMD_USER_SELECT_ERRWAY,&m_errmod_mode);
    if(phase<3)
		app_ERRmod_correct_step_send( main_step,sub_step,phase,pos);
	else
		meausre_DDS_out_select(1,1,1);
    return;
}


static void _dlsr_scan(void)
{
     Bsize_t DLSR;
     os_driver_ioctl(g_DL_dev,MEASURE_CMD_GET_REAL_DLSR,&DLSR);
	 if(DLSR!=g_user_arg.DL_app_user_set.dlsr)
	 {
         g_user_arg.DL_app_user_set.dlsr = DLSR;
		 GUI_drop_list_member_assign(m_errmod_drop_dlsr,g_user_arg.DL_app_user_set.dlsr);
	 }
	 return;
}


static void _QS_match(measure_param_t  *dl)
{
	float 	fk = 1.0;
	float   beilv = 1.0;	
	int     N = 0;

	fk = (float)atol(g_user_arg.DL_app_user_set.cs);
	N=_Qs_select(dl->f_Pabc,fk);
	if(g_user_arg.DL_app_user_set.dlsr>QB_N_20A)
	{
		beilv = global_atof(g_user_arg.DL_app_user_set.bb1);
		if(beilv != 0)
		{
			N /= beilv;
		}
	}
	if(N == 0) N = 1;
	global_ltoa(N,g_user_arg.DL_app_user_set.qs);
	
	
	fk = (float)atol(g_user_arg.DL_app_user_set.cs2);
	N=_Qs_select(dl->f_Qabc,fk);
	if(g_user_arg.DL_app_user_set.dlsr>QB_N_20A)
	{
		beilv = global_atof(g_user_arg.DL_app_user_set.bb1);
		if(beilv != 0)
		{
			N/=beilv;
		}
	}
	if(N == 0) N = 1;
	global_ltoa(N,g_user_arg.DL_app_user_set.qs2);
	app_global_set_user_dl_arg(&g_user_arg);
	fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
}


/********************************************************************
 * @作者 :揭成
 * @功能 :电流修正
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_ERRmod_Icorrect_task(void*arg)
{
	uint8_t key_buf;
	Bsize_t cnt = 48;
	Bsize_t pos = 1;
	int     circle=0;
	int     last_circle=0;
	uint8_t phase =0,endphase;
	measure_param_t dl;
	uint8_t main_setp = ICORRECT_SETP;
	DL_flag_t dl_flag;
	m_errmod_mode = 0;
	_CORsend( ICORRECT_SETP,1,phase,pos);
	app_ERRmod_Iwindow_creat();
	fatfs_readARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
	app_global_set_user_dl_arg(&g_user_arg);
	GUI_drop_list_member_assign(m_errmod_drop_dlsr,g_user_arg.DL_app_user_set.dlsr);
	app_ERRmod_Icorrect_userinfo_display(&g_user_arg);
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		_dlsr_scan();
		app_ERRmod_Icorrect_correct_data_display(phase);
		app_global_DLflag_obtain(&dl_flag);
		circle = app_global_path1_circle_obtain();
		if(circle!=last_circle)
		{
			last_circle = circle;
			app_ERRmod_circle_display( circle );
		}
		if ( dl_flag.DL_path1_ERR_is_new)
		{
			app_global_path1_DLerr_obtain(&dl );         		/*获取电参数*/
			buz_response( 5 );
			app_ERRmod_Err_display(dl.f_path1_err);
		}
		if(dl_flag.DL_data_is_new&&(++cnt%50==0))
		{
			app_global_DLarg_obtain(&dl);
			app_ERRmod_Icorrect_DLarg_display(&dl,phase);
		}
		if ( app_global_key_obtain( &key_buf, 1 ) != 1 )
		{
			continue;
		}
		if(key_buf==KEY_EXIT || key_buf==GUI_KEY_EXIT)
		{
           	app_ERRmod_Iwindow_destroy();
		   	break;
		}
		else if(key_buf == KEY_USER)
		{
			_QA_mod =(_QA_mod==1)?(0):(1);
			app_ERRmod_Icorrect_userinfo_display(&g_user_arg);
			m_IAUTO_level_F = _QA_mod;
			phase = 0;
			_CORsend( ICORRECT_SETP,1,phase,pos);
			GUI_radio_select(GET_DOBJ(ICORRECT_PHASE_RADIO),phase); 
		}
		else if(key_buf==GUI_KEY_SETTING||key_buf==KEY_SETTING)
		{
			  GUI_edit_start(GET_DOBJ(ICORRECT_USER_EDIT),app_ERRmod_Icorrect_callback);
			  msleep(APP_100_MS);
		} 
		else if(key_buf==KEY_SWITCH || key_buf==GUI_KEY_SW)
		{
			pos=GUI_radio_curmember_obtain(GET_DOBJ(ICORRECT_WAY_RADIO));
			pos=((pos==5)?(1):(pos+1));
			GUI_radio_select(GET_DOBJ(ICORRECT_WAY_RADIO),pos);
			if(pos==1)main_setp = ICORRECT_SETP;
			else if(pos==2)main_setp = ILINECORRECT_SETP;
			else if(pos==3)main_setp = JCORRECT_SETP;
			else if(pos==4)main_setp = JLINECORRECT_SETP;
		}
        else if(key_buf==GUI_KEY_WAY)
        {
           pos=GUI_radio_curmember_obtain(GET_DOBJ(ICORRECT_WAY_RADIO));
		   if(pos==1)main_setp = ICORRECT_SETP;
		   else if(pos==2)main_setp = ILINECORRECT_SETP;
		   else if(pos==3)main_setp = JCORRECT_SETP;
		   else if(pos==4)main_setp = JLINECORRECT_SETP;
		}
		else if(key_buf==GUI_KEY_PHASE)
		{
			app_ERRmod_Icorrect_DLarg_clean_edit(phase);
			phase=GUI_radio_curmember_obtain(GET_DOBJ(ICORRECT_PHASE_RADIO));
			_CORsend( ICORRECT_SETP,1,phase,pos);
		}
		else if(key_buf==KEY_DOWN||key_buf==KEY_UP)
		{
			   cnt =29;
			   phase=GUI_radio_curmember_obtain(GET_DOBJ(ICORRECT_PHASE_RADIO));
			   app_ERRmod_Icorrect_DLarg_clean_edit(phase);
               endphase = (_QA_mod==0)?(C_PHASE):(C_PHASE+1);
			   if(key_buf==KEY_DOWN)
			   {
				   phase=((phase==endphase)?(A_PHASE):(phase+1));
			   }
			   else
			   {
				   phase=((phase==A_PHASE)?(endphase):(phase-1));
			   }
			   _CORsend( ICORRECT_SETP,1,phase,pos);
			   GUI_radio_select(GET_DOBJ(ICORRECT_PHASE_RADIO),phase);  
		}
		else if(key_buf==KEY_SYS)
		{
			m_errmod_mode = (m_errmod_mode==1)?(0):(1);
			os_driver_ioctl( g_DL_dev, MEASURE_CMD_USER_SELECT_ERRWAY,&m_errmod_mode);
			app_ERRmod_Icorrect_userinfo_display(&g_user_arg);
		}
		else if(key_buf==KEY_F1)
		{
           	g_user_arg.DL_app_user_set.dlsr=GUI_drop_list_unfold(m_errmod_drop_dlsr);
		   	app_global_set_user_dl_arg(&g_user_arg);
			fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
		   	msleep(APP_100_MS);
		}
        else if(key_buf==KEY_F2)
        {
	      _QS_match(&dl);
		  app_ERRmod_Icorrect_userinfo_display(&g_user_arg);
		}
        else if(key_buf==KEY_F3)
        {
          _Constselect();   
		  app_global_set_user_dl_arg(&g_user_arg);
		  fatfs_writeARG("USERDATA",&g_user_arg,sizeof(DL_app_user_all_info_t));
		  app_ERRmod_Icorrect_userinfo_display(&g_user_arg);
		}
		else if(key_buf==KEY_F4)
		{
			if(MsgBox(250,100,300,150, "提示",
				  "确认删除全部修正数据",APP_HALF_SEC*100)==GUIE_KEY_ENTER)
			{
				os_driver_ioctl(g_DL_dev,MEASURE_CMD_CELAN_ALL_CORRECT_DATA,0);
				msleep(APP_100_MS);
			}
		}
		else if(key_buf==KEY_F5)
		{
			if(MsgBox(250,100,300,150, "提示",
				  "确认删除当前项修正数据",APP_HALF_SEC*100)==GUIE_KEY_ENTER)
			{
				_CORsend(main_setp,1,phase,pos);
				os_driver_ioctl(g_DL_dev,MEASURE_CMD_CELAN_CUR_CORRECT_DATA,0);
				msleep(APP_100_MS);
			}
		}
		else if(key_buf==KEY_ENTER || key_buf==GUI_KEY_ENTER)
		{
            if(app_ERRmod_Icorrect_is_lawful(&dl,main_setp,&g_user_arg,phase))
            {
				GUI_3D_window_title_mod( m_errmod_Icorrect_win, "开始修正....." );
				_CORsend( main_setp,2,phase,pos);
				msleep( APP_100_MS);
				GUI_3D_window_title_mod( m_errmod_Icorrect_win, "本次修正完成" );
				msleep( APP_100_MS );
				GUI_3D_window_title_mod( m_errmod_Icorrect_win, "电流修正" );
				os_driver_ioctl( g_DL_dev, MEASURE_CMD_USER_SELECT_ERRWAY,&m_errmod_mode);
			}
			else
			{
				GUI_3D_window_title_mod( m_errmod_Icorrect_win, "设置不合法,请查看电流和角度" );
				msleep( APP_100_MS*20);
				GUI_3D_window_title_mod( m_errmod_Icorrect_win, "电流修正" );
			}
		}
	}
   return;
}

