/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:���ʼ���
* ����ʱ��:2011-8-29
* ������˾ :����
* �ļ�����:app_WIRmod_redeem.c
* ������ :�ҳ�
* ��������:���߼��׷���ʼ���
******************************************************************************************/
#include"app_WIRmod_inside.h"
#include"math.h"

float m_WIRmod_RUL = 1;  	//A��ʵ�ʵ�ѹ
float m_WIRmod_RIL = 1;  	//B��ʵ�ʵ�ѹ
float m_WIRmod_RJ  = 0;  	//����ƽ���½Ƕ�
float m_WIRmod_RJa = 0;  	//����ƽ����A��Ƕ�
float m_WIRmod_RJb = 0;  	//����ƽ����B��Ƕ�
float m_WIRmod_RJc = 0;  	//����ƽ����C��Ƕ�
float m_WIRmod_EJa = 0;  	//a��ʵ�ʽǶ�
float m_WIRmod_EJb = 0;  	//b��ʵ�ʽǶ�
float m_WIRmod_EJc = 0;  	//c��ʵ�ʽǶ�
uint8_t m_redeem_JXMOD = 0;	// 0--3��4��   1--3��3��
/********************************************************************
 * @������ :�ҳ�
 * @���� :�����������µ���׷����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
#define XCRADIAN(angle) (( angle) * 3.1415926 / 180 )
#define POBTAIN(phase)	(phase.f_U*phase.f_I)
float app_WIRmod_GX_obtain(void)
{
	float TRUE_P=0; //��ʵ�ܹ���
	float ERR_P=0; //��ʵ�ܹ���
	float GX = 0; 	//׷����
	TRUE_P+= m_WIRmod_RUL*m_WIRmod_RIL*cos(XCRADIAN(m_WIRmod_RJa));
	if(m_redeem_JXMOD==0)
	{
		TRUE_P+=m_WIRmod_RUL*m_WIRmod_RIL*cos(XCRADIAN(m_WIRmod_RJb));
	}
	TRUE_P+=m_WIRmod_RUL*m_WIRmod_RIL*cos(XCRADIAN(m_WIRmod_RJc));

	ERR_P+=m_WIRmod_RUL*m_WIRmod_RIL*cos(XCRADIAN(m_WIRmod_EJa));
	if(m_redeem_JXMOD==0)
	{
		ERR_P+= m_WIRmod_RUL*m_WIRmod_RIL*cos(XCRADIAN(m_WIRmod_EJb));
	}
	ERR_P+=m_WIRmod_RUL*m_WIRmod_RIL*cos(XCRADIAN(m_WIRmod_EJc));
    if(ERR_P==0)
    {
		return 1000;
	}
	GX = TRUE_P / ERR_P;
	return  (GLOBAL_ABS(GX)>1000)?(1000):(GX);
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :���������������ʵ���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static float app_redeem_AtrueJ_obtain(
	WIRresult_t*presult,
	float angle
)
{
	float  base_j= angle;
	if(global_memcmp(presult->Result_MUa,"Ua",2))
	base_j+=0;
	else if(global_memcmp(presult->Result_MUa,"Ub",2))
	base_j-=120;
	else if(global_memcmp(presult->Result_MUa,"Uc",2))
	base_j+=120;
	if(global_memcmp(presult->Result_MIa,"ia",2))
	{
		return base_j;
	}
	else if(global_memcmp(presult->Result_MIa,"-ia",3))
	{
		return base_j+180.0;
	}
	else if(global_memcmp(presult->Result_MIa,"ib",2))
	{
		return base_j+120;
	}
	else if(global_memcmp(presult->Result_MIa,"-ib",3))
	{
		return base_j+120+180.0;
	}
	else if(global_memcmp(presult->Result_MIa,"ic",2))
	{
		return base_j-120;
	}
	else if(global_memcmp(presult->Result_MIa,"-ic",3))
	{
		return base_j-120+180;
	}	
	return 0;
}
static float app_redeem_BtrueJ_obtain(
	WIRresult_t*presult,
	float angle
)
{
	float  base_j= angle;
	if(global_memcmp(presult->Result_MUb,"Ua",2))
		base_j+=120;
	else if(global_memcmp(presult->Result_MUb,"Ub",2))
		base_j-=0;
	else if(global_memcmp(presult->Result_MUb,"Uc",2))
		base_j-=120;
	if(global_memcmp(presult->Result_MIb,"ia",2))
	{
		return base_j-120;
	}
	else if(global_memcmp(presult->Result_MIb,"-ia",3))
	{
		return base_j-120+180;
	}
	else if(global_memcmp(presult->Result_MIb,"ib",2))
	{
		return base_j;
	}
	else if(global_memcmp(presult->Result_MIb,"-ib",3))
	{
		return base_j+180.0;
	}
	else if(global_memcmp(presult->Result_MIb,"ic",2))
	{
		return base_j+120;
	}
	else if(global_memcmp(presult->Result_MIb,"-ic",3))
	{
		return base_j+120+180.0;
	}	
	return 0;
}

static float app_redeem_CtrueJ_obtain(
	WIRresult_t*presult,
	float angle
)
{
	float  base_j= angle;
	if(global_memcmp(presult->Result_MUc,"Ua",2))
	base_j-=120;
	else if(global_memcmp(presult->Result_MUc,"Ub",2))
	base_j+=120;
	else if(global_memcmp(presult->Result_MUc,"Uc",2))
	base_j-=0;
	if(global_memcmp(presult->Result_MIc,"ia",2))
	{
		return base_j+120;
	}
	else if(global_memcmp(presult->Result_MIc,"-ia",3))
	{
		return base_j+120+180;
	}
	else if(global_memcmp(presult->Result_MIc,"ib",2))
	{
		return base_j-120;
	}
	else if(global_memcmp(presult->Result_MIc,"-ib",3))
	{
		return base_j-120+180;
	}
	else if(global_memcmp(presult->Result_MIc,"ic",2))
	{
		return base_j;
	}
	else if(global_memcmp(presult->Result_MIc,"-ic",3))
	{
		return base_j+180.0;
	}	
	return 0;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :����׷������ʼ��
 * @���� :presult ���߽����Pdl ��ǰ�繤������JXmod ����ģʽ
 * @��� :NONE
********************************************************************/
#define  _ANGLE_TRANS(angle)  (angle>180)?(angle-360):(angle)
void app_WIRmod_redeem_init(
	WIRresult_t*presult,
	measure_param_t*Pdl,
	uint8_t JXmod
	)
{
	m_redeem_JXMOD = JXmod;
    if(JXmod==0) /*3��4��*/
    {
		m_WIRmod_RJa = m_WIRmod_RJ;
		m_WIRmod_RJb = m_WIRmod_RJ;
		m_WIRmod_RJc = m_WIRmod_RJ;
	    m_WIRmod_EJa=_ANGLE_TRANS(app_redeem_AtrueJ_obtain(presult,m_WIRmod_RJa));
	    m_WIRmod_EJb=_ANGLE_TRANS(app_redeem_BtrueJ_obtain(presult,m_WIRmod_RJb));
	    m_WIRmod_EJc=_ANGLE_TRANS(app_redeem_CtrueJ_obtain(presult,m_WIRmod_RJc));
	}
    else
    {
		m_WIRmod_RJa = m_WIRmod_RJ;
		m_WIRmod_RJb = m_WIRmod_RJ;
		m_WIRmod_RJc = m_WIRmod_RJ;
	    m_WIRmod_EJa=_ANGLE_TRANS(app_redeem_AtrueJ_obtain(presult,m_WIRmod_RJa));
	    m_WIRmod_EJb=_ANGLE_TRANS(app_redeem_BtrueJ_obtain(presult,m_WIRmod_RJb));
	    m_WIRmod_EJc=_ANGLE_TRANS(app_redeem_CtrueJ_obtain(presult,m_WIRmod_RJc));
		m_WIRmod_RJa = 30+m_WIRmod_RJ;
		m_WIRmod_RJc = m_WIRmod_RJ-30;
		if(presult->phaseDir==PHASE_ADD)
		{
			m_WIRmod_EJa += 30;
	        m_WIRmod_EJc -= 30;
		}
		else
		{
			m_WIRmod_EJa -= 30;
	        m_WIRmod_EJc += 30;
		}
	}
	return;
}
/*------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------*/
/*------------------connect----for-----wirmod-----------------------------------------------------*/
/*------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------*/
/***************************************************************************************************
���³���Ϊ������ʾ�ӿڣ����漰�㷨�����Ϻ���Ϊ�㷨�ӿڡ�
***************************************************************************************************/
extern  	 uint8_t m_WIRmod_Cline_task_pro;
char 		 m_WIR_cos[10];
char 		 m_WIR_cos_icon[2];
hwin_t * 	 m_WIRmod_redeem_win;
hwin_t * 	 m_WIRmod_redeem_edit;

redeem_arg_t m_WIRmod_redeem_arg;
float        m_WIRmod_GX=1;

#define      REDEEM_BG          0xce79
#define      REDEEM_FG          0

/********************************************************************
 * @������ :�ҳ�
 * @���� :׷��������ʾ�ӿ�
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_WIRmod_redeem_float_display(float data,Bsize_t id)
{
   char temp[10]="";
   if(data!=0)
   {
	   global_ftoa(data,temp,2);
   }
   GUI_edit_string_display_At(temp,id);
   return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :׷��������ʾ
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_WIRmod_redeem_arg_display(redeem_arg_t*parg)
{
	const char* wm[2]={"���ͣת","Stop"};
	float fdata;
	float real_p;
	GUI_window_hwin_req( m_WIRmod_redeem_edit );
	GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
	GUI_set_Fgcolor( C_GUI_WHITE);
	
	GUI_edit_string_display_At(parg->cur_cos,1);
	GUI_edit_string_display_At(parg->last_P,3);
	GUI_edit_string_display_At(parg->cur_P,4);
    if(m_WIRmod_GX!=1000)
    {
		app_WIRmod_redeem_float_display(m_WIRmod_GX,5);
		fdata = global_atof(parg->cur_P)-global_atof(parg->last_P);
	    real_p = fdata*m_WIRmod_GX;
		app_WIRmod_redeem_float_display(real_p,6);	
		fdata = real_p-fdata;
		app_WIRmod_redeem_float_display(fdata,7);
    }
	else
	{
		GUI_edit_string_display_At("",3);
		GUI_edit_string_display_At("",4);
		//GUI_edit_string_display_At("���ͣת",5);
		GUI_edit_string_display_At((char*)wm[g_language],5);	
		GUI_edit_string_display_At("",6);
		GUI_edit_string_display_At("",7);
	}
	GUI_window_hwin_release( m_WIRmod_redeem_edit );
    return;
}
/********************************************************************
* @������ :�ҳ�
* @���� :����׷������
* @���� :NONE
* @��� :NONE
********************************************************************/
void app_WIRmod_redeem_window_creat(hwin_t*pOBJ)
{
	const char* wz1[2]={"��������","     PF"}; 	//power factor
	const char* wz2[2]={"�ϴε׶�","L-power"};	//last power
	const char* wz3[2]={"���ε׶�","C-power"};	//current power
	const char* wz4[2]={"����ϵ��","modulus"};	//modify modulus
	const char* wz5[2]={"ʹ�õ���","U-power"};	//used power
	const char* wz6[2]={"׷������","R-Power"};	//redeem power
	const char* wz7[2]={"������λ:(kW��h)��","power unit:(kW��h)."};
    hwin_t*p;
    GUI_window_hwin_req(pOBJ);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    m_WIRmod_redeem_edit
    =GUI_edit_creat(0, 1, 310, 228,WIN_TOUCH_EN, pOBJ );
	GUI_window_hwin_req( m_WIRmod_redeem_edit );
	GUI_set_Bgcolor(REDEEM_BG);
	GUI_set_Fgcolor(REDEEM_FG);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
    GUI_edit_memeber_add(0,8,9,(char*)wz1[g_language],MEMBER_3D|1);
	GUI_edit_memeber_add(0,35,9,(char*)wz2[g_language],MEMBER_3D|3);
	GUI_edit_memeber_add(155,35,9,(char*)wz3[g_language],MEMBER_3D|4);
    GUI_edit_memeber_add(155,8,9,(char*)wz4[g_language],MEMBER_3D|5);
    GUI_edit_memeber_add(0,85,9,(char*)wz5[g_language],MEMBER_3D|6);
    GUI_edit_memeber_add(155,85,9,(char*)wz6[g_language],MEMBER_3D|7);
	GUI_string_display_At(144,10,m_WIR_cos_icon);
	GUI_WINDIS_HLINE(0,310,65);
	GUI_WINDIS_HLINE(0,310,135);
	GUI_window_hwin_release( m_WIRmod_redeem_edit );
	GUI_set_Fgcolor( C_GUI_YELLOW);
	GUI_string_display_At(5,115,(char*)wz7[g_language]);
    p=Gui_button_creat((pOBJ->win_wide-160)/3,pOBJ->win_high-40,80,30,WIN_MSG_EN|BUTTON_PAINT|KEY_ENTER,pOBJ);
	GUI_button_title_display(p,"ȷ��");
    p=Gui_button_creat(2*(pOBJ->win_wide-160)/3+80,pOBJ->win_high-40,80,30,WIN_MSG_EN|BUTTON_PAINT|KEY_EXIT,pOBJ);
	GUI_button_title_display(p,"�˳�");
	GUI_window_hwin_release( pOBJ );
    return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :����׷������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_WIRmod_redeem_window_destory(void)
{
   GUI_window_destory(m_WIRmod_redeem_win);
   return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :��ȡ������λ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
float app_WIRmod_cos_obtain(char*pcos)
{
  char temp[10]="";	
  float angle;
  float fangle =0;
  global_strcpy(temp,pcos);
  global_string_filter(temp,' ');
  if(m_WIR_cos_icon[0]=='L')
  {
        fangle = 0+acos(global_atof(temp));
  }
  else if(m_WIR_cos_icon[0]=='C')
  {
     fangle = 0-acos(global_atof(temp));
  }
  angle = fangle*180/3.1415926;
  return angle;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :ˢ��׷����ʾ
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_WIRmod_redeem_view(void)
{
	const char* wj[2] ={"��ʾ: ���ͣת��","Notice:meter stop.        "};
	const char* wj1[2]={"��ʾ: �����ת��","Notice:meter run slowly   "};
	const char* wj2[2]={"��ʾ: ���ת��","Notice:meter run reverse. "};
	const char* wj3[2]={"��ʾ: �����ת��","Notice:meter run positive."};	
	GUI_window_hwin_req( m_WIRmod_redeem_win );
	GUI_set_Bgcolor(REDEEM_BG);
	GUI_set_Fgcolor(REDEEM_FG);
	GUI_SetFront(&GUI_HZK16_EXTASCII);
	if(m_WIRmod_GX==1000)
	{
		GUI_string_display_At(5,145,(char*)wj[g_language]);
	}
	else if(m_WIRmod_GX>1)
	{
		GUI_string_display_At(5,145,(char*)wj1[g_language]);
	}
	else if(m_WIRmod_GX<0)
	{
		GUI_string_display_At(5,145,(char*)wj2[g_language]);
	}
	else if(m_WIRmod_GX==1)
	{
		GUI_string_display_At(5,145,(char*)wj3[g_language]);
	}
	GUI_window_hwin_release( m_WIRmod_redeem_win );
    return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :���ܺ����ص�
 * @���� :NONE
 * @��� :NONE
********************************************************************/
uint8_t app_WIRmod_redeem_callback( hwin_t*hp )
{
	uint8_t key;
	redeem_arg_t shadow;
	Bsize_t member;
	global_memcpy(&shadow,&m_WIRmod_redeem_arg,sizeof(redeem_arg_t));
	GUI_window_hwin_req( hp );
	key_input_method_filter(KEY_INPUT_123);
	GUI_window_hwin_release(hp);
	while ( 1 )
	{
		msleep( TYPE_DELAY );
		if ( app_global_key_obtain( &key, 1 ) != 1 )/*�ж������Ƿ��յ�������Ӧ*/
		{
			continue;
		}
		GUI_window_hwin_req( hp );
		GUI_set_Bgcolor( EDIT_MEMBER_BG_COLOR );
		GUI_set_Fgcolor( C_GUI_WHITE);
		if (( app_global_decide_inputkey(key))!=FAIL)/*�ж��Ƿ������뷨��ذ�������*/
		{
			member = GUI_edit_id_obtain();     		/*��ȡ��ǰ�༭��ԱID*/
			if ( member == 1 )key_method_input_shadow( key, shadow.cur_cos);
			else if ( member == 2)key_method_input_shadow( key,shadow.cur_rate);
			else if ( member == 3)key_method_input_shadow( key,shadow.last_P);
			else if ( member == 4)key_method_input_shadow( key,shadow.cur_P);
		}
		else if ( key == KEY_DOWN ) 
		{
			member = GUI_edit_id_obtain();     		/*��ȡ��ǰ�༭��ԱID*/
			if(member!=4)
			{
				GUI_edit_shift( EDIT_NEXT );
			}
		}
		else if ( key == KEY_UP) 
		{
			member = GUI_edit_id_obtain();     		/*��ȡ��ǰ�༭��ԱID*/
			if(member!=1)
			{
				GUI_edit_shift( EDIT_LAST);
			}
		}
		else if ( key == KEY_RIGHT )
			GUI_edit_csr_shift( CSR_RIGHT_SHIFT );/*������ƶ�һλ*/
		else if ( key == KEY_LEFT)
			GUI_edit_csr_shift( CSR_LEFT_SHIFT );/*�������һλ*/
		GUI_window_hwin_release( hp );
		if(key==KEY_ENTER)
		{
			global_memcpy(&m_WIRmod_redeem_arg,&shadow,sizeof(redeem_arg_t));
			m_WIRmod_RJ = app_WIRmod_cos_obtain(m_WIRmod_redeem_arg.cur_cos);
			app_WIRmod_redeem_init(&m_WIRresult,&g_DL_param,m_WIR_jxmod);
			m_WIRmod_GX = app_WIRmod_GX_obtain();
			app_WIRmod_redeem_view();
			break;
		}
		else if(key==KEY_EXIT)
		{
			break;
		}
	}
  	return key;
}

static void _cos_create(void)
{
    global_strcpy(m_WIR_cos,m_WIRresult.cos);
	m_WIR_cos_icon[1] = 0;m_WIR_cos_icon[0] = 0;
	if (global_string_locate_first(m_WIR_cos,"C"))
	m_WIR_cos_icon[0] ='C';
	else if (global_string_locate_first(m_WIR_cos,"L"))
	m_WIR_cos_icon[0] ='L';
	global_string_filter(m_WIR_cos,'L');
	global_string_filter(m_WIR_cos,'C');
	return;
}
/********************************************************************
 * @������ :�ҳ�
 * @���� :���ܺ������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_WIRmod_redeem_function(void)
{
    uint16_t sec;
	const char* wz[2] ={"����׷��","Redeem power"};
    uint8_t key_buf,err;
    int x1,y1,x2,y2;
	OSTaskSuspend(m_WIRmod_Cline_task_pro);
	_cos_create();
	GUI_window_hwin_req(GET_GUIWIN);
	Gui_button_set_color(C_GUI_LOW_BLUE,REDEEM_BG);
	Gui_3D_window_set_color(C_GUI_WHITE,C_GUI_LOW_BLUE,C_GUI_RED);	
	x1 = 200;y1 =100;x2 = x1+320;y2 = y1+260;
	sec=GUI_read_block_Extp(x1,y1,x2, y2);
	GUI_set_Bgcolor(REDEEM_BG);
	m_WIRmod_redeem_win
	= Gui_3D_window_creat(x1-1,y1-1,310,250,(char*)wz[g_language],WIN_3D|WIN_FILL,GET_GUIWIN);
	app_WIRmod_redeem_window_creat(m_WIRmod_redeem_win);
    GUI_TopWIN_Set(m_WIRmod_redeem_win);
	GUI_window_hwin_release(GET_GUIWIN);
	
	GLOBAL_MEMCLR(&m_WIRmod_redeem_arg,sizeof(redeem_arg_t));
	global_strcpy(m_WIRmod_redeem_arg.cur_cos,m_WIR_cos);
	m_WIRmod_RJ = app_WIRmod_cos_obtain(m_WIR_cos);
	app_WIRmod_redeem_init(&m_WIRresult,&g_DL_param,m_WIR_jxmod);
	m_WIRmod_GX = app_WIRmod_GX_obtain();
	global_ftoa(m_WIRmod_GX,m_WIRmod_redeem_arg.ch_GX,2);
	app_WIRmod_redeem_view();
	app_WIRmod_redeem_arg_display(&m_WIRmod_redeem_arg);
	app_global_key_dummy_press(KEY_SETTING);
	while(1)
    {
		msleep( TYPE_DELAY );
		if ( app_global_key_obtain(&key_buf,1)==1)/*��ȡ��������ֵ*/
		{
            if(key_buf==KEY_EXIT)
            {
               break;
			}
			else if(key_buf==KEY_SETTING||GUI_KEY_EDIT_START==key_buf)
			{
               GUI_edit_start(m_WIRmod_redeem_edit,
			   	app_WIRmod_redeem_callback);
 			    app_WIRmod_redeem_arg_display(&m_WIRmod_redeem_arg);
			}
		}
	}
	app_WIRmod_redeem_window_destory();
	app_module_color_select(2);
	GUI_write_block_Extp(x1,y1,x2, y2,sec);
	OSTaskResume(m_WIRmod_Cline_task_pro);
	OSSemSet( m_WIRmod_Resem,0,&err ); 
    return;
}

void app_WIRmod_redeem_search(redeem_arg_t*p)
{
    uint16_t sec;
	const char* wz[2] ={"����׷��","Redeem power"};
    uint8_t key_buf;
    int x1,y1,x2,y2;
	GUI_window_hwin_req(GET_GUIWIN);
	Gui_button_set_color(C_GUI_LOW_BLUE,REDEEM_BG);
	Gui_3D_window_set_color(C_GUI_WHITE,C_GUI_LOW_BLUE,C_GUI_RED);	
	x1 = 200;y1 =100;x2 = x1+320;y2 = y1+260;
	sec=GUI_read_block_Extp(x1,y1,x2, y2);
	GUI_set_Bgcolor(REDEEM_BG);
	m_WIRmod_redeem_win
	= Gui_3D_window_creat(x1-1,y1-1,310,250,(char*)wz[g_language],WIN_3D|WIN_FILL,GET_GUIWIN);
	app_WIRmod_redeem_window_creat(m_WIRmod_redeem_win);
    GUI_TopWIN_Set(m_WIRmod_redeem_win);
	GUI_window_hwin_release(GET_GUIWIN);
    global_memcpy(&m_WIRmod_redeem_arg,p,sizeof(redeem_arg_t));
	m_WIRmod_GX = global_atof(m_WIRmod_redeem_arg.ch_GX);
	app_WIRmod_redeem_view();
	app_WIRmod_redeem_arg_display(&m_WIRmod_redeem_arg);
    while(1)
    {
        msleep(1);
		if ( app_global_key_force_obtain( &key_buf, 1 )== 1 )
		{
			if(key_buf==KEY_EXIT)break;
		}
	}
	app_WIRmod_redeem_window_destory();
	app_module_color_select(2);
	GUI_write_block_Extp(x1,y1,x2, y2,sec);
    return;
}
