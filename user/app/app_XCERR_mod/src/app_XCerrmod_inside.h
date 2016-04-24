#include "../../../includes.h"
#include "../../../Gui\Gui.h"
#ifndef _PEC_APP_XCerrmod_INSIDE_2010_H
#define _PEC_APP_XCerrmod_INSIDE_2010_H
enum
{
	GUI_KEY_METER_SETTING = 100,
	GUI_KEY_LISTVIEW
};
enum
{
	METER1_CS = 1,  	
	METER2_CS,
	METER3_CS,
	METER1_QS,    
	METER2_QS, 
	METER3_QS,
	TMR1_CS,
	TMR2_CS,
	TMR3_CS,
	CT_BL,
	XNFZ_IB,  	
	XNFZ_FEQ,  	
	XNFZ_IMAX,  	
	XNFZ_U,  	
	EDIT_U_A,
	EDIT_U_B,
	EDIT_U_C,
	EDIT_I_A,
	EDIT_I_B,
	EDIT_I_C,
	EDIT_J_A,
	EDIT_J_B,
	EDIT_J_C,	
	EDIT_P_A,
	EDIT_P_B,
	EDIT_P_C,
	EDIT_Q_A,
	EDIT_Q_B,
	EDIT_Q_C,
	EDIT_JUAB,
	EDIT_JUAC,
	EDIT_JUBC,
	EDIT_JIAC,
	EDIT_JIAB,
	EDIT_JIBC,
	EDIT_QABC,
	EDIT_PABC,
	EDIT_COS,
	EDIT_FEQ,
	EIDT_USER_U,
	EIDT_USER_I,
	EIDT_USER_JA,
	EIDT_USER_JB,
	EIDT_USER_JC,
	EIDT_USER_JUAB,
	EIDT_USER_JUAC,
	EIDT_USER_JUBC,
	EDIT_CS_CHECK1,
	EDIT_CS_CHECK2
};

enum{
		KEY_CONST_SETTING= 100,
		KEY_I_IN         ,
		KEY_CONST_SELECT ,
		KEY_QS_SELECT    ,
		KEY_PAUSE        ,
		KEY_START        ,
		KEY_CLOSE        ,
		KEY_METER_SELECT ,
		KEY_YGWG_SELECT  ,
		KEY_GD_SELECT    ,
		KEY_CS_CHECK     ,
		KEY_RETURN
};

#define     XCerrmod_BG_COLOR               g_Bg_color
#define   	XCerrmod_FG_COLOR               g_Fg_color
#define   	XCerrmod_WIN_BG_COLOR           g_win_Bg_color  	

extern      hwin_t*     m_XCERR_3DWIN;
extern      hwin_t*     m_XCERR_DL_win;
extern      hwin_t*     m_XCERR_LJT_win;

void 		app_XCerrmod_window_creat(void);
void 		app_DLview_task( void*arg );
void 		app_ERR_task(void*arg);

Bsize_t 	app_global_path1_circle_obtain( void );
Bsize_t 	app_global_path2_circle_obtain( void );
#define  	INC_DECIDE_LOOP_(val,MAX)    (val=((val<MAX) ?( (val)+1 ):(val=0)))
#define  	DEC_DECIDE_LOOP_(val,MAX)  	 (val=((val>0) ?( (val)-1):(val=MAX)))

void 		app_Systime_obtain( void);
void 		app_XCerrmod_window_destory(void);
void 		app_XCERR_search( void *arg );
void  		app_XCERR_errPAINT(void);
void 		app_XCERR_DLpaint(void);

void 		app_XCerrmod_fun_entry();
void 		app_XCerrmod_fun_exit();
void 		app_LJT_init( hwin_t*hp);
void 		app_XCERR_XNFZ_state_view(uint16_t color);
void 	 	app_LJT_display( hwin_t*hp, measure_param_t*Pdl,DL_flag_t*Pdl_flag,int flag);

uint16_t 	GUI_read_block_Extp( int x1, int y1, int x2, int y2);
void	 	GUI_write_block_Extp( int x1, int y1, int x2, int y2, uint16_t sec );
void		Gui_button_set_color(uint16_t Fgcor,uint16_t Bgcor);

void 		app_XNFZ_UIout_EN(BOOL U_EN,BOOL I_EN);
void 		app_XNFZ_UIout_SW(const void*str);
void 		app_XNFZ_task(void*arg);
uint16_t 	LCD_RGB_565(uint16_t R,uint16_t G,uint16_t B);
uint16_t 	GUI_GetwinBgcor( void );
void 		app_XCERRmod_check_entry(void);


uint16_t 	LCD_RGB_565(uint16_t R,uint16_t G,uint16_t B);

extern      hwin_t *     	 m_XCERR_ERR_win;
extern 		uint16_t 		 m_GUI_3D_window_Bg_cor;
extern      measure_param_t  g_DL_param;

void 	app_XCERR_battery_display( void );
void 	app_XCERR_external_display( void );
void 	app_XCERR_charge_display( void );
void 	app_XCERR_adapter_protect_display( void );
void 	app_XCERRmod_Stime_entery( void);

/*********************************************************
* ��ӡ���
*********************************************************/

#define 	XCERRMOD_LCD_WIDTH	640
#define 	XCERRMOD_LCD_HEIGHT	480
#define 	XCERRMOD_UART_DEV_NAME "D232"
#define		XCERRMOD_BAUD		19200
#define		XCERRMOD_UART_NO_CHECHK 0

void 	app_XCERRmod_record_print(Errsave_t*pD);
extern 	dev_id_t	m_XCERRmod_print_dev_id;
void 	app_XCerrmod_record_detail_win_creat(void);

/********��ӡ��ؽ���*****/

extern const char*   m_XCERRmod_MSG_01[2][7];
extern const char*m_XCERRmod_MSG_02[2];//{"���� ",""};
extern const char*m_XCERRmod_MSG_03[2];//{"Ȧ��a", ""};
extern const char*m_XCERRmod_MSG_04[2];//{"����a", ""};
extern const char*m_XCERRmod_MSG_05[2];//{"Ȧ��b", ""};
extern const char*m_XCERRmod_MSG_06[2];//{"����b", ""};
extern const char*m_XCERRmod_MSG_07[2];//{" ����",""};
extern const char*m_XCERRmod_MSG_08[2];//{"����", ""};
extern const char*m_XCERRmod_MSG_09[2];//{"˫��", ""};
extern const char*m_XCERRmod_MSG_10[2][3];
extern const char*m_XCERRmod_MSG_11[2];//{"���", ""};
extern const char*m_XCERRmod_MSG_12[2];//{"�ֶ�", ""};
extern const char*m_XCERRmod_MSG_13[2];//{"ȷ��",""};
extern const char*m_XCERRmod_MSG_14[2];//{"�˳�",""};
extern const char*m_XCERRmod_MSG_15[2];//{"��ͣ",""};
extern const char*m_XCERRmod_MSG_16[2];//{"�ȼ�",""};
extern const char*m_XCERRmod_MSG_17[2];//{"��Դ������ʾ",""};
extern const char*m_XCERRmod_MSG_18[2];//{"��ǰ�����Զ���ʽ��ع���״̬,��������Դ�ɹ��л�!",""};
extern const char*m_XCERRmod_MSG_19[2];//{"��ǰ�����ֶ���ʽ��ع���״̬,��������Դ�ɹ��л�!",""};
extern const char*m_XCERRmod_MSG_20[2];//{"��ǰ�����ֶ���ʽ��ع���״̬,�������쳣����,�޷��л�!",""};
extern const char*m_XCERRmod_MSG_21[2];//{"��ǰ�����Զ���ʽ����������״̬,�޵�ؿɹ��л�!",""};
extern const char*m_XCERRmod_MSG_22[2];//{"��ǰ�����Զ���ʽ���ص�Դ����״̬,�޵�ؿɹ��л�!",""};
extern const char*m_XCERRmod_MSG_23[2];//{"ȷ��ǿ���л�����ع���? ��<ȷ��>�л�",""};
extern const char*m_XCERRmod_MSG_24[2];//{"ȷ���л����ⲿ��Դ����? ��<ȷ��>�л�",""};
extern const char*m_XCERRmod_MSG_24_1[2];//{"�����������쳣!���������������ѹ�Ƿ����!",""};

extern const char*m_XCERRmod_MSG_25[2];//{"������ ", ""};
extern const char*m_XCERRmod_MSG_26[2];//{"�û��� ", ""};
extern const char*m_XCERRmod_MSG_27[2];//{"����1", ""};
extern const char*m_XCERRmod_MSG_28[2];//{"����2", ""};
extern const char*m_XCERRmod_MSG_29[2];//{"У��Ա1", ""};
extern const char*m_XCERRmod_MSG_30[2];//{"  ʱ�� ", ""};
extern const char*m_XCERRmod_MSG_31[2];//{"ȷ��", ""};
extern const char*m_XCERRmod_MSG_32[2];//{"�˳�", ""};
extern const char*m_XCERRmod_MSG_33[2];//{"��ʾ", ""};
extern const char*m_XCERRmod_MSG_34[2];//{"�����Ѿ��ɹ�����.", ""};
extern const char*m_XCERRmod_MSG_35[2];//{"���ݱ���ʧ��.", ""};
extern const char*m_XCERRmod_MSG_36[2];//{"�û�����", ""};
extern const char*m_XCERRmod_MSG_37[2];//{"���У��", ""};
extern const char*m_XCERRmod_MSG_38[2];//{"��������", ""};
extern const char*m_XCERRmod_MSG_39[2];//{"ƥ��Ȧ��", ""};
extern const char*m_XCERRmod_MSG_40[2];//{"�Ƽ�����", ""};
extern const char*m_XCERRmod_MSG_41[2];//{"�л���Դ", ""};
extern const char*m_XCERRmod_MSG_42[2];//{"��ͣ/��ʼ", ""};
extern const char*m_XCERRmod_MSG_44[2];//={"F5/��ӡ", ""};
extern const char*m_XCERRmod_MSG_45[2];	//={"�ͻ�ǩ��:", ""};
extern const char*m_XCERRmod_MSG_46[2];	//={"У��Ա:",   ""};
extern const char*m_XCERRmod_MSG_47[2];	//={"У��ʱ��:", ""};
extern const char*m_XCERRmod_MSG_48[2];//={"������ʾ", ""};
extern const char*m_XCERRmod_MSG_49[2];//={"��ȷ�ϴ�ӡ���Ƿ���ֽ!<ȷ��>��ӡ,<�˳�>����ӡ.",""};
extern const char*m_XCERRmod_MSG_50[2];//={"��ȷ�ϴ�ӡ���Ƿ���ֽ!<ȷ��>��ӡ,<�˳�>����ӡ.",""};
extern const char*m_XCERRmod_MSG_51[2];//={"��һУ�˳���:",""};
extern const char*m_XCERRmod_MSG_52[2];//={"���У�˳���:",""};
extern const char*m_XCERRmod_MSG_53[2];//={"У�˳���",""};
extern const char*m_XCERRmod_MSG_54[2];//={"ȷ��",""};
extern const char*m_XCERRmod_MSG_55[2];//={"�˳�",""};


#endif

