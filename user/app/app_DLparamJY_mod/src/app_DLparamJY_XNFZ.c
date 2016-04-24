/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:自动校验虚拟负载
* 创建时间:
* 所属公司:科荟
* 文件名称:XNFZ.C
* 作者    :
* 功能描述:实现虚拟负载功能
******************************************************************************************/
#include 	"app_DLparamJY_inside.h"

hwin_t*     m_DLparam_XNFZ_win 		= 0;
hwin_t*     m_DLparam_XNFZ_edit 	= 0;
Bsize_t  	m_DLparam_sec;
uint8_t   	task_DLparam_XNFZ_id 	= INVALID_PRO;

extern  dev_id_t        g_UIout_dev;
#define XNFZ_WIN        m_DLparam_XNFZ_edit
#define GET_RADIO(id) 	GUI_childobj_obtian(id,WIDGET_RADIO,XNFZ_WIN)
static  const float 	_fangle_tab[] ={-75.5,-60.0,-37.0,0,75.5,60,37};
static  const float	 	_Ibuage_tab[] ={1.0,0.5,0.2,0.1,0.05};
static  const float 	_fUb_tab[]    ={380.0,220.0,100.0,57.7};
static  float 			_fIb = 10;
static  float 			_fUb;
static  float 			_freq;
static  hwin_t* 		_checkboxU = 0;
static  hwin_t* 		_checkboxI = 0;


typedef struct
{
	char  		Ib[8];
	char  		freq[8];
    char  		Ub_pos;
	char    	angle_pos;
    char  		Ibuage_pos;
    char  	    p_3or4;
	uint8_t 	UIout_level;
	UIout_ctl_t UIout_EN;
}XNFZ_option_t;

static  XNFZ_option_t    _XNFZ_option;

enum
{
	GUI_KEY_XNFZ_SETTING  = 152 ,
//	GUI_KEY_XNFZ_START  ,
//	GUI_KEY_XNFZ_CLOSE ,
	GUI_KEY_XNFZ_SELECT_3OR4,
	GUI_KEY_XNFZ_SELECT_IB ,
	GUI_KEY_XNFZ_SELECT_COS ,
	GUI_KEY_XNFZ_SELECT_U ,
	GUI_KEY_XNFZ_CHECKBOXU,
	GUI_KEY_XNFZ_CHECKBOXI
};

//窗口内容创建
static void _WINpatin(hwin_t*hOBJ)  //(WIDE,HIGH)=(314,272)
{
#define  LAB_Y  20
    hwin_t*pOBJ;
    GUI_window_hwin_req(hOBJ);
	GUI_set_Fgcolor(C_GUI_GREEN);
	GUI_set_Bgcolor(799);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_string_display_At(2,2,"源输出设置");
    GUI_SetFront(&GUI_HZK24_EXTASCII);
    GUI_checkBox_create("U",C_GUI_WHITE,96, 2,WIN_MSG|WIN_TOUCH|GUI_KEY_XNFZ_CHECKBOXU,hOBJ);
    GUI_checkBox_create("I",C_GUI_WHITE,200,2,WIN_MSG|WIN_TOUCH|GUI_KEY_XNFZ_CHECKBOXI,hOBJ);
    GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_set_Fgcolor(C_GUI_YELLOW);
    GUI_edit_memeber_add(60, 12+LAB_Y,6, "",MEMBER_3D|XNFZ_IB);
    GUI_edit_memeber_add(215,12+LAB_Y,6, "",MEMBER_3D|XNFZ_FEQ);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
    GUI_string_display_At(2,  18+LAB_Y,"  Ib(A)");
    GUI_string_display_At(157,18+LAB_Y,"Feq(Hz)");
    pOBJ=GUI_radio_create(3,50+LAB_Y,302,40,WIN_MSG|WIN_PAINT|WIN_TOUCH|GUI_KEY_XNFZ_SELECT_3OR4,hOBJ);
    GUI_window_hwin_req(pOBJ);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor(C_GUI_RED);
    GUI_string_display_At(2,2,"接线方式>>");	
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_radio_memeber_add(10+100*0,19,"三相四线", 0);
	GUI_radio_memeber_add(10+100*1,19,"三相三线", 1);
	GUI_radio_memeber_add(10+100*2,19,"单相", 2);
    GUI_window_hwin_release(pOBJ);
    pOBJ=GUI_radio_create(3,90+LAB_Y,302,40,WIN_MSG|WIN_PAINT|WIN_TOUCH|GUI_KEY_XNFZ_SELECT_IB,hOBJ);
    GUI_window_hwin_req(pOBJ);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor(C_GUI_RED);
    GUI_string_display_At(2,2,"Ib%   >>");	
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_radio_memeber_add(10+75*0,16,"100%",0);
	GUI_radio_memeber_add(10+75*1,16,"50%", 1);
	GUI_radio_memeber_add(10+75*2,16,"20%", 2);
	GUI_radio_memeber_add(10+75*3,16,"10%", 3);
    GUI_window_hwin_release(pOBJ);
	pOBJ=GUI_radio_create(3,130+LAB_Y,302,65,WIN_MSG|WIN_PAINT|WIN_TOUCH|GUI_KEY_XNFZ_SELECT_COS,hOBJ);
    GUI_window_hwin_req(pOBJ);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor(C_GUI_RED);
    GUI_string_display_At(2,2,"COSΦ >>");	
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_radio_memeber_add(10+75*0,16,"0.25C",0);
	GUI_radio_memeber_add(10+75*1,16,"0.5C", 1);
	GUI_radio_memeber_add(10+75*2,16,"0.8C", 2);
	GUI_radio_memeber_add(10+75*3,16,"1.0",  3);
	GUI_radio_memeber_add(10+75*0,45,"0.25L",4);
	GUI_radio_memeber_add(10+75*1,45,"0.5L" ,5);
	GUI_radio_memeber_add(10+75*2,45,"0.8L" ,6);
    GUI_window_hwin_release(pOBJ);
	pOBJ=GUI_radio_create(3,195+LAB_Y,302,40,WIN_MSG|WIN_PAINT|WIN_TOUCH|GUI_KEY_XNFZ_SELECT_U,hOBJ);
    GUI_window_hwin_req(pOBJ);
    GUI_SetFront(&GUI_HZK16_EXTASCII);
	GUI_set_Fgcolor(C_GUI_RED);
    GUI_string_display_At(2,2," U   >>");	
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_radio_memeber_add(10+75*0,16,"380V", 0);
	GUI_radio_memeber_add(10+75*1,16,"220V", 1);
	GUI_radio_memeber_add(10+75*2,16,"100V", 2);
	GUI_radio_memeber_add(10+75*3,16,"57.7V",3);
    GUI_window_hwin_release(pOBJ);
    GUI_window_hwin_release(hOBJ);
	return;
}

//源参数设置
static uint8_t _ARG_setting(hwin_t*hp)
{
	uint8_t  key;
	Bsize_t  member;
	XNFZ_option_t    arg;
	global_memcpy(&arg,&_XNFZ_option,sizeof(XNFZ_option_t));
	hand_wirte_input_box_pos_set(20,394);
	key_input_method_filter(KEY_INPUT_123);
	while(1)
	{
		if ( app_global_key_force_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_edit_run_hook( key );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( C_GUI_WHITE);		
		if ( app_global_decide_inputkey( key ) != FAIL )
		{
			member = GUI_edit_id_obtain();
			if(member      == XNFZ_IB)  key_method_input_shadow(key,arg.Ib);
			else if(member == XNFZ_FEQ) key_method_input_shadow(key,arg.freq);				
		}
		else if(key == KEY_DOWN)GUI_edit_shift( EDIT_NEXT);	
		else if(key == KEY_UP)  GUI_edit_shift( EDIT_LAST );	
		GUI_window_hwin_release( hp );
	    if (key == KEY_EXIT)
		{
			break;
		}
	    else if (key == KEY_ENTER)
		{
			global_memcpy(&_XNFZ_option,&arg,sizeof(XNFZ_option_t));
			break;
		}
	}
    return key;
}
//源参数导入
static void _ARG_load(void)
{
    UIout_rise_t  UIoutOP;
	GLOBAL_MEMCLR(&UIoutOP,sizeof(UIout_rise_t));
	_fIb   = global_atof(_XNFZ_option.Ib);
	_freq  = global_atof(_XNFZ_option.freq);
	_fUb   = _fUb_tab[_XNFZ_option.Ub_pos];
	UIoutOP.f_I[A_PHASE]    = _fIb*_Ibuage_tab[_XNFZ_option.Ibuage_pos];
	UIoutOP.f_J[A_PHASE]    = _fangle_tab[_XNFZ_option.angle_pos];
	UIoutOP.f_U[A_PHASE]    = _fUb;
    UIoutOP.freq            = _freq;
	UIoutOP.f_CRTI[A_PHASE] = _fIb;
    fatfs_writeARG("XNFZp",&_XNFZ_option,sizeof(XNFZ_option_t));
	app_DLparamJYmod_measure_auto_change_Ilevel(UIoutOP.f_I[A_PHASE]);
	os_driver_ioctl(g_UIout_dev,UIOUT_DLARG_SET,&UIoutOP);
    return;
}
//源参数显示
static void _ARG_view(void)
{
	GUI_window_hwin_req(XNFZ_WIN);
	GUI_radio_select(GET_RADIO(GUI_KEY_XNFZ_SELECT_IB),   _XNFZ_option.Ibuage_pos);
	GUI_radio_select(GET_RADIO(GUI_KEY_XNFZ_SELECT_COS),  _XNFZ_option.angle_pos);
	GUI_radio_select(GET_RADIO(GUI_KEY_XNFZ_SELECT_U),    _XNFZ_option.Ub_pos);
	GUI_radio_select(GET_RADIO(GUI_KEY_XNFZ_SELECT_3OR4), _XNFZ_option.p_3or4);
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_set_Bgcolor(EDIT_MEMBER_BG_COLOR);
	GUI_edit_string_display_At(_XNFZ_option.Ib,  		XNFZ_IB);
	GUI_edit_string_display_At(_XNFZ_option.freq,		XNFZ_FEQ);
	GUI_window_hwin_release(XNFZ_WIN);
	return;
}


static void _XNFZ_path_select(void)
{
   UIout_ctl_t*p = &_XNFZ_option.UIout_EN;
   if(_XNFZ_option.p_3or4==0)  //3-4
   {
       p->U_EN[1]= p->U_EN[0];
       p->U_EN[2]= p->U_EN[0];
       p->I_EN[1]= p->I_EN[0];
       p->I_EN[2]= p->I_EN[0];
   }
   else if(_XNFZ_option.p_3or4==1)//3-3
   {
       p->U_EN[1]= 0;
       p->U_EN[2]= p->U_EN[0];
       p->I_EN[1]= 0;
       p->I_EN[2]= p->I_EN[0];
   }
   else                         //A PHASE
   {
       p->U_EN[1]= 0;
       p->U_EN[2]= 0;
       p->I_EN[1]= 0;
       p->I_EN[2]= 0;
   }
   os_driver_ioctl(g_UIout_dev,UIOUT_CTL_SET,&_XNFZ_option.UIout_EN);
   return;
}


static void _keyMSG_handle(uint8_t keyMSG)
{
	Bsize_t pos;
	if ( keyMSG == GUI_KEY_XNFZ_SELECT_IB)
	{
	    pos = GUI_radio_curmember_obtain(GET_RADIO(keyMSG));
		_XNFZ_option.Ibuage_pos = pos;
		_ARG_load();
	}
	else if(keyMSG == GUI_KEY_XNFZ_SELECT_COS)
	{
	    pos = GUI_radio_curmember_obtain(GET_RADIO(keyMSG));
		_XNFZ_option.angle_pos= pos;
		_ARG_load();
	}
	else if(keyMSG == GUI_KEY_XNFZ_SELECT_3OR4)
	{
	    pos = GUI_radio_curmember_obtain(GET_RADIO(keyMSG));
		_XNFZ_option.p_3or4= pos;
		_ARG_load();
	}
	else if(keyMSG == GUI_KEY_XNFZ_SELECT_U)
	{
	    pos = GUI_radio_curmember_obtain(GET_RADIO(keyMSG));
		_XNFZ_option.Ub_pos = pos;
		_ARG_load();
	}
	else if(keyMSG == GUI_KEY_EDIT_START)
	{
	    app_global_key_control_power_req();
	    GUI_edit_start(XNFZ_WIN,_ARG_setting);
	    app_global_key_control_power_release();
		_ARG_view();
		_ARG_load();
 	}
	else if(keyMSG == GUI_KEY_XNFZ_CHECKBOXI||keyMSG == GUI_KEY_XNFZ_CHECKBOXU)
	{
	   _XNFZ_option.UIout_EN.I_EN[A_PHASE] = GUI_checkbox_state_obtain(_checkboxI);
	   _XNFZ_option.UIout_EN.U_EN[A_PHASE] = GUI_checkbox_state_obtain(_checkboxU);
	   _ARG_load();
	   _XNFZ_path_select();
	}
	return;
}
static void _XNFZ_init(void)
{
	_WINpatin(XNFZ_WIN);
	_checkboxI = GUI_childobj_obtian(GUI_KEY_XNFZ_CHECKBOXI,WIDGET_CHECK_BOX,XNFZ_WIN);
	_checkboxU = GUI_childobj_obtian(GUI_KEY_XNFZ_CHECKBOXU,WIDGET_CHECK_BOX,XNFZ_WIN);
	GLOBAL_MEMCLR(&_XNFZ_option,sizeof(XNFZ_option_t));
    fatfs_readARG("XNFZp",&_XNFZ_option,sizeof(XNFZ_option_t));
	GUI_checkBox_select(_checkboxI,_XNFZ_option.UIout_EN.I_EN[A_PHASE]);
	GUI_checkBox_select(_checkboxU,_XNFZ_option.UIout_EN.U_EN[A_PHASE]);
	_XNFZ_path_select();
	GUI_MsgFIFO_create(XNFZ_WIN,2);
	_ARG_view();
	_ARG_load();
    return;
}

/***************************************************************/
//API
/***************************************************************/
/********************************************************************
 * @作者 :周炳权
 * @功能 :虚拟负载界面创建
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_xnfz_window_create( void )
{
	int x1,y1,x2,y2;
	hwin_t*pOBJ;
	const char *m_start[2]= {"启动",""};
	const char *m_stop[2] = {"停止",""};
	
	x1 = 200;y1 = 30;x2 = x1+314;y2 = y1+350;

	m_DLparam_sec = GUI_read_block_Extp(x1-5,y1,x2+10, y2+5);
	GUI_window_hwin_req(GET_GUIWIN);
	Gui_button_set_color(C_GUI_LOW_BLUE,DLparamJYmod_WIN_BG_COLOR);
	Gui_3D_window_set_color(C_GUI_WHITE,DLparamJYmod_WIN_BG_COLOR,C_GUI_RED);
	GUI_set_Bgcolor( DLparamJYmod_WIN_BG_COLOR );
	m_DLparam_XNFZ_win = Gui_3D_window_creat(x1, y1, x2-x1,y2-y1,"虚拟负载",WIN_MSG_EN|WIN_3D|WIN_FILL, 0 );
	GUI_window_hwin_req(m_DLparam_XNFZ_win);
    m_DLparam_XNFZ_edit = GUI_edit_creat(0,0,314,330,WIN_MSG_EN|WIN_3D|WIN_TOUCH_EN,m_DLparam_XNFZ_win);
	_XNFZ_init();
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	pOBJ=Gui_button_creat(60,275,70,25,BUTTON_PAINT|GUI_KEY_XNFZ_START,m_DLparam_XNFZ_edit);
	GUI_button_title_display(pOBJ,(char *)m_start[g_language]);
	pOBJ=Gui_button_creat(190,275,70,25,BUTTON_PAINT|GUI_KEY_XNFZ_CLOSE,m_DLparam_XNFZ_edit);
	GUI_button_title_display(pOBJ,(char *)m_stop[g_language]);
	GUI_TopWIN_Set(m_DLparam_XNFZ_edit);
	GUI_window_hwin_release(m_DLparam_XNFZ_win);
	GUI_window_hwin_release(GET_GUIWIN);
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :虚拟负载界面删除
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_xnfz_window_destory( void )
{
	int x1,y1,x2,y2;

	x1 = 200;y1 = 30;x2 = x1+314;y2 = y1+350;

	Gui_button_set_color(BUTTON_TXT_COLOR,BUTTON_BG_COLOR);
	Gui_3D_window_set_color(C_GUI_WHITE,C_GUI_LOW_BLUE,C_GUI_WHITE);
	GUI_window_destory(m_DLparam_XNFZ_edit);
	GUI_window_destory(m_DLparam_XNFZ_win);
	GUI_write_block_Extp(x1-5,y1,x2+10, y2+5, m_DLparam_sec);
	
	return;
}

/********************************************************************
 * @作者 :揭成
 * @功能 :源报警处理
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_XNFZ_unlawful_close( void )
{
    BOOL state;
	UIout_alerting_u flag;
    os_driver_ioctl(g_UIout_dev,UIOUT_STATE_GET,&state);
	if(state)
	{
    	os_driver_ioctl(g_UIout_dev,UIOUT_LAWFLAG_GET,&(flag.all));
		if(flag.bits.onlineU_alerting == 1)
		{
		    os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
			g_DLparamJY_UIOUT_start = 0;
			MsgBox(200,200,250,200, "警告!","非法操作,启动源的同时必须切断外部电压,请确认后重新操作.",APP_HALF_SEC*100);
			os_driver_ioctl(g_UIout_dev,UIOUT_LAWFLAG_CLEAN,0);
		}
		if(flag.bits.DL_alerting == 1)
		{
		    os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
			g_DLparamJY_UIOUT_start = 0;
			MsgBox(200,200,250,200, "警告!","电流回路异常保护,请确认电流回路是否正常,然后重新操作.",APP_HALF_SEC*100);
			os_driver_ioctl(g_UIout_dev,UIOUT_LAWFLAG_CLEAN,0);
		}
		if(flag.bits.KL_alerting== 1)
		{
		    os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
			g_DLparamJY_UIOUT_start = 0;
			MsgBox(200,200,250,200, "警告!","电压回路异常保护,请确认电压回路是否正常,然后后重新操作.",APP_HALF_SEC*100);
			os_driver_ioctl(g_UIout_dev,UIOUT_LAWFLAG_CLEAN,0);
		}	
	}
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :升流源稳定状态获取
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
BOOL app_DLparamJYmod_XNFZ_state_get( void )
{
	BOOL m_rise_state = FAIL;
	os_driver_ioctl( g_UIout_dev,UIOUT_IS_UPDES, &m_rise_state);
    return m_rise_state;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :测量电流自动换档
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_DLparamJYmod_measure_auto_change_Ilevel( float m_IB_grade )
{
	if( m_IB_grade > 1 )
		g_user_arg.DL_app_user_set.dlsr = DZ_N_10A;
	else if( m_IB_grade <= 1 )
		g_user_arg.DL_app_user_set.dlsr = DZ_N_1A;

	app_global_set_user_dl_arg(&g_user_arg);
	return;
}
//三相源开关
void app_DLparam_XNFZ_UIout_SW(const void*str)
{
    if(global_memcmp((void*)str,"open",4))
		GUI_keyMSG_post(XNFZ_WIN,GUI_KEY_XNFZ_START);
    else if(global_memcmp((void*)str,"close",5))
		GUI_keyMSG_post(XNFZ_WIN,GUI_KEY_XNFZ_CLOSE);
	return;
}
//三相源通道设置
void app_DLparam_XNFZ_UIout_EN(BOOL U_EN,BOOL I_EN)
{
	_XNFZ_option.UIout_EN.I_EN[A_PHASE] = U_EN;
	_XNFZ_option.UIout_EN.U_EN[A_PHASE] = I_EN;
	GUI_checkBox_select(_checkboxI,_XNFZ_option.UIout_EN.I_EN[A_PHASE]);
	GUI_checkBox_select(_checkboxU,_XNFZ_option.UIout_EN.U_EN[A_PHASE]);
	GUI_keyMSG_post(XNFZ_WIN,GUI_KEY_XNFZ_CHECKBOXI);
	return;
}
//三相源控制信息接收任务
void app_DLparam_XNFZ_task(void*arg)
{
    uint8_t keyMSG;
	//GUI_keyMSG_post(XNFZ_WIN,GUI_KEY_EDIT_START);
	while(1)
	{   
		msleep(TYPE_DELAY);
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			app_DLparamJYmod_xnfz_window_destory();
			os_task_delete( task_DLparam_XNFZ_id );
			task_DLparam_XNFZ_id = INVALID_PRO;
		}
        if(GUI_key_read(XNFZ_WIN,&keyMSG,1)==1)
        {
           _keyMSG_handle(keyMSG);
        }
	}
}



