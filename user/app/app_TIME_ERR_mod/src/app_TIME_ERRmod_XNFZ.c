/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:变比模块主体程序
* 创建时间:
* 所属公司:科荟
* 文件名称:XNFZ.C
* 创建人    :
* 功能描述:实现源输出控制
******************************************************************************************/
#include    "app_TIME_errmod_inside.h"
extern  dev_id_t        g_UIout_dev;
#define XNFZ_WIN        m_TIME_ERR_XNFZ_win
#define GET_RADIO(id) 	GUI_childobj_obtian(id,WIDGET_RADIO,XNFZ_WIN)
static  const float 	_fangle_tab[] ={-75.5,-60.0,-37.0,0,75.5,60,37};
static  const float	 	_Ibuage_tab[] ={1.0,0.5,0.2,0.1,0.05};
static  const float 	_fUb_tab[]    ={380.0,220.0,100.0,57.7};
static  float 			_fIb = 10;
static  float 			_fUb;
static  float 			_freq;
static  hwin_t* 		_checkboxU = 0;
static  hwin_t* 		_checkboxI = 0;
static  int     		_stateCNT = 0;


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
	GUI_KEY_XNFZ_SETTING  = 102 ,
	GUI_KEY_XNFZ_START  ,
	GUI_KEY_XNFZ_CLOSE ,
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
    GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_string_display_At(2,2,"源输出设置");
    GUI_checkBox_create("U",C_GUI_WHITE,146, 2,WIN_MSG|WIN_TOUCH|GUI_KEY_XNFZ_CHECKBOXU,hOBJ);
    GUI_checkBox_create("I",C_GUI_WHITE,250,2,WIN_MSG|WIN_TOUCH|GUI_KEY_XNFZ_CHECKBOXI,hOBJ);
	GUI_set_Fgcolor(C_GUI_YELLOW);
    GUI_edit_memeber_add(49, 15+LAB_Y,6, "Ib(A)",MEMBER_3D|XNFZ_IB);
    GUI_edit_memeber_add(203,15+LAB_Y,6, "F(Hz)",MEMBER_3D|XNFZ_FEQ);
    pOBJ=GUI_radio_create(3,50+LAB_Y,377,40,WIN_MSG|WIN_PAINT|WIN_TOUCH|GUI_KEY_XNFZ_SELECT_3OR4,hOBJ);
    GUI_window_hwin_req(pOBJ);
	GUI_set_Fgcolor(C_GUI_RED);
    GUI_string_display_At(2,7,"接线");	
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_radio_memeber_add(62,7,"三相四线", 0);
	GUI_radio_memeber_add(62+122,7,"三相三线", 1);
	GUI_radio_memeber_add(62+243,7,"单相", 2);
    GUI_window_hwin_release(pOBJ);
    pOBJ=GUI_radio_create(3,90+LAB_Y,377,40,WIN_MSG|WIN_PAINT|WIN_TOUCH|GUI_KEY_XNFZ_SELECT_IB,hOBJ);
    GUI_window_hwin_req(pOBJ);
	GUI_set_Fgcolor(C_GUI_RED);
    GUI_string_display_At(2,7,"Ib%>");	
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_radio_memeber_add(62,7,"100%",0);
	GUI_radio_memeber_add(62+80*1+5,7,"50%", 1);
	GUI_radio_memeber_add(62+80*2+3,7,"20%", 2);
	GUI_radio_memeber_add(62+80*3,7,"10%", 3);
    GUI_window_hwin_release(pOBJ);
	pOBJ=GUI_radio_create(3,130+LAB_Y,377,70,WIN_MSG|WIN_PAINT|WIN_TOUCH|GUI_KEY_XNFZ_SELECT_COS,hOBJ);
    GUI_window_hwin_req(pOBJ);
	GUI_set_Fgcolor(C_GUI_RED);
    GUI_string_display_At(2,7,"COSΦ");	
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_radio_memeber_add(62+80*0,7,"0.25C",0);
	GUI_radio_memeber_add(62+80*1+10,7,"0.5C", 1);
	GUI_radio_memeber_add(62+80*2+12,7,"0.8C", 2);
	GUI_radio_memeber_add(62+80*3+12,7,"1.0",  3);
	GUI_radio_memeber_add(62+80*0,38,"0.25L",4);
	GUI_radio_memeber_add(62+80*1+10,38,"0.5L" ,5);
	GUI_radio_memeber_add(62+80*2+12,38,"0.8L" ,6);
    GUI_window_hwin_release(pOBJ);
	pOBJ=GUI_radio_create(3,200+LAB_Y,377,40,WIN_MSG|WIN_PAINT|WIN_TOUCH|GUI_KEY_XNFZ_SELECT_U,hOBJ);
    GUI_window_hwin_req(pOBJ);
	GUI_set_Fgcolor(C_GUI_RED);
    GUI_string_display_At(2,7,"电压");	
	GUI_set_Fgcolor(C_GUI_WHITE);
	GUI_SetFront(&GUI_HZK24_EXTASCII);
	GUI_radio_memeber_add(62+75*0,7,"380V", 0);
	GUI_radio_memeber_add(62+75*1,7,"220V", 1);
	GUI_radio_memeber_add(62+75*2,7,"100V", 2);
	GUI_radio_memeber_add(62+75*3,7,"57.7V",3);
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
	hand_wirte_input_box_pos_set(20,315);
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
	Bsize_t level;
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
	UIoutOP.JX_F  			= _XNFZ_option.p_3or4; 
    fatfs_writeARG("XNFZp",&_XNFZ_option,sizeof(XNFZ_option_t));
	os_driver_ioctl(g_UIout_dev,UIOUT_DLARG_SET,&UIoutOP);
	level = _XNFZ_option.Ub_pos;
	os_driver_ioctl(g_DL_dev,MEASURE_CMD_SET_UDL_LEVEL,&level);
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
		g_TIME_3or4_display_flag = _XNFZ_option.p_3or4;
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
	else if(keyMSG == GUI_KEY_XNFZ_START)
	{
		if ( app_TIME_XNFZ_3X4X_3X3X_judge() )
		{
			if (MsgBox(200,200,200,200,
				"提示!","请检查电表电流量程与实际设置的量程是否一致!按<确定>启动源,按<退出>不启动",APP_HALF_SEC*30) 
				== GUIE_KEY_ENTER )
			{			
				_XNFZ_path_select();
				_ARG_load();
				os_driver_ioctl(g_UIout_dev,UIOUT_OPEN,0);
			}
			else os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
		}
		else
		{
			MsgBox(200,200,200,200,
			"提示!","请检查源输出电压选择是否符合要求:三相三线:100V,380V;三相四线:57.7V,220V;",APP_HALF_SEC*30);
		}
	}
	else if(keyMSG == GUI_KEY_XNFZ_CLOSE)
	{
		os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
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
	_WINpatin(m_TIME_ERR_XNFZ_win);
	_checkboxI = GUI_childobj_obtian(GUI_KEY_XNFZ_CHECKBOXI,WIDGET_CHECK_BOX,XNFZ_WIN);
	_checkboxU = GUI_childobj_obtian(GUI_KEY_XNFZ_CHECKBOXU,WIDGET_CHECK_BOX,XNFZ_WIN);
	GLOBAL_MEMCLR(&_XNFZ_option,sizeof(XNFZ_option_t));
    fatfs_readARG("XNFZp",&_XNFZ_option,sizeof(XNFZ_option_t));
	GUI_checkBox_select(_checkboxI,_XNFZ_option.UIout_EN.I_EN[A_PHASE]);
	GUI_checkBox_select(_checkboxU,_XNFZ_option.UIout_EN.U_EN[A_PHASE]);
	_XNFZ_path_select();
	GUI_MsgFIFO_create(XNFZ_WIN,2);
	_ARG_view();
	//_ARG_load();
	g_TIME_3or4_display_flag = _XNFZ_option.p_3or4;
    return;
}

/***************************************************************/
//API
/***************************************************************/
static void _state_view(BOOL state)
{
	BOOL riseDES = FAIL;
	if(!state)
		app_TIME_ERR_XNFZ_state_view(C_GUI_RED);
	else
	{
		os_driver_ioctl( g_UIout_dev,UIOUT_IS_UPDES, &riseDES);
		if(riseDES)app_TIME_ERR_XNFZ_state_view(C_GUI_GREEN);
		else app_TIME_ERR_XNFZ_state_view(C_GUI_YELLOW);
	}
	return;
}

/********************************************************************
 * @作者 :揭成
 * @功能 :源报警处理
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_TIME_ERRmod_XNFZ_unlawful_close( void )
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
			MsgBox(200,200,250,200, "警告!","非法操作,启动源的同时必须切断外部电压,请确认后重新操作.",APP_HALF_SEC*100);
			os_driver_ioctl(g_UIout_dev,UIOUT_LAWFLAG_CLEAN,0);
		}
		if(flag.bits.DL_alerting == 1)
		{
		    os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
			MsgBox(200,200,250,200, "警告!","电流回路异常保护,请确认电流回路是否正常,然后重新操作.",APP_HALF_SEC*100);
			os_driver_ioctl(g_UIout_dev,UIOUT_LAWFLAG_CLEAN,0);
		}
		if(flag.bits.KL_alerting== 1)
		{
		    os_driver_ioctl(g_UIout_dev,UIOUT_CLOSE,0);
			MsgBox(200,200,250,200, "警告!","电压回路异常保护,请确认电压回路是否正常,然后重新操作.",APP_HALF_SEC*100);
			os_driver_ioctl(g_UIout_dev,UIOUT_LAWFLAG_CLEAN,0);
		}	
	}
	if(++_stateCNT%10==0)
		_state_view(state);
	return;
}
/********************************************************************
 * @作者 :周炳权
 * @功能 :升流源稳定状态获取
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
BOOL app_TIME_ERRmod_XNFZ_state_get( void )
{
	BOOL m_rise_state = FAIL;
	os_driver_ioctl( g_UIout_dev,UIOUT_IS_UPDES, &m_rise_state);
    return m_rise_state;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :升流源切换端子处理
 * @输入 :m_dz:端子档位偏移
 * @输出 :NONE
********************************************************************/
void app_TIME_XNFZ_UIout_dz_handle( uint8_t m_dz )
{
	GUI_keyMSG_post(XNFZ_WIN,GUI_KEY_XNFZ_CLOSE);
	if ( m_dz == 0 )
	{
		global_memcpy(_XNFZ_option.Ib, "1.0", 4);
	}
	else if ( m_dz == 1 )
	{
		global_memcpy(_XNFZ_option.Ib, "10.0", 5);
	}
	_ARG_view();
	fatfs_writeARG("XNFZp",&_XNFZ_option,sizeof(XNFZ_option_t));
	return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :切换到无负荷状态
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void app_TIME_XNFZ_into_WFH(void)
{
	_XNFZ_option.UIout_EN.I_EN[A_PHASE] = 1;
	_XNFZ_option.UIout_EN.U_EN[A_PHASE] = 1;
	GUI_checkBox_select(_checkboxI,_XNFZ_option.UIout_EN.I_EN[A_PHASE]);
	GUI_checkBox_select(_checkboxU,_XNFZ_option.UIout_EN.U_EN[A_PHASE]);
	_XNFZ_path_select();
	_ARG_load();
    return;
}

/********************************************************************
 * @作者 :周炳权
 * @功能 :三相四线、三相三线状态
 * @输入 :NONE
 * @输出 :TRUE OR FAIL
********************************************************************/
uint8_t app_TIME_XNFZ_3X4X_3X3X_judge(void)
{
	uint8_t m_ret = FAIL;

	if ( _XNFZ_option.p_3or4 == 1 )			//三相三线
	{
		if ( (_XNFZ_option.Ub_pos == 0) || (_XNFZ_option.Ub_pos == 2) )
			 m_ret = TURE;
		else m_ret = FAIL;
	}
	else if ( _XNFZ_option.p_3or4 == 0 )	//三相四线
	{
		if ( (_XNFZ_option.Ub_pos == 1) || (_XNFZ_option.Ub_pos == 3) )
			 m_ret = TURE;
		else m_ret = FAIL;
	}
	else if ( _XNFZ_option.p_3or4 == 2 )	//单相
	{
		m_ret = TURE;
	}
	
	return m_ret;
}

//三相源开关
void app_TIME_XNFZ_UIout_SW(const void*str)
{
    if(global_memcmp((void*)str,"open",4))
		GUI_keyMSG_post(XNFZ_WIN,GUI_KEY_XNFZ_START);
    else if(global_memcmp((void*)str,"close",4))
		GUI_keyMSG_post(XNFZ_WIN,GUI_KEY_XNFZ_CLOSE);
	return;
}
//三相源通道设置
void app_TIME_XNFZ_UIout_EN(BOOL U_EN,BOOL I_EN)
{
	_XNFZ_option.UIout_EN.I_EN[A_PHASE] = U_EN;
	_XNFZ_option.UIout_EN.U_EN[A_PHASE] = I_EN;
	GUI_checkBox_select(_checkboxI,_XNFZ_option.UIout_EN.I_EN[A_PHASE]);
	GUI_checkBox_select(_checkboxU,_XNFZ_option.UIout_EN.U_EN[A_PHASE]);
	GUI_keyMSG_post(XNFZ_WIN,GUI_KEY_XNFZ_CHECKBOXI);
	return;
}
//三相源控制信息接收任务
void app_TIME_XNFZ_task(void*arg)
{
    uint8_t keyMSG;
	_stateCNT = 8; 
	_XNFZ_init();
	while(1)
	{   
		msleep(TYPE_DELAY);
		app_TIME_ERRmod_XNFZ_unlawful_close();
		if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ )
		{
			os_task_delete( SELF_PRO );   			
		}
        if(GUI_key_read(XNFZ_WIN,&keyMSG,1)==1)
        {
           _keyMSG_handle(keyMSG);
        }
	}
}

