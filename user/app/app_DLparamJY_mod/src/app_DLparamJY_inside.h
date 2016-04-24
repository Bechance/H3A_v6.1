#include "../../../includes.h"
#include "../../../Gui\Gui.h"
/*界面模块内部头文件*/

#ifndef _PEC_APP_DLparamJYmod_INSIDE_2013_H
#define _PEC_APP_DLparamJYmod_INSIDE_2013_H
extern DL_app_user_all_info_t  	g_user_arg;
void lcd_driver_draw_line( uint16_t x_coord_start,
						   uint16_t y_coord_start,
						   uint16_t x_coord_end,
						   uint16_t y_coord_end,
						   uint16_t color );
void lcd_driver_fill_rectangle(uint16_t coord_x, uint16_t coord_y,
								uint16_t x_len, uint16_t y_len,
								uint16_t color );

extern 		uint16_t 		 m_GUI_3D_window_Bg_cor;
extern const unsigned char gImage_entery[];
extern const unsigned char gImage_return[];
extern const unsigned char gImage_save[];
extern const unsigned char gImage_exit[];
void  	bsp_delay_ms( unsigned long	ms );

/*********************************************************************
* APP_CSverifymod-H  for DLparamJYmod fun
**********************************************************************/
void app_DLparamJYmod_fun_entry( void );
void app_DLparamJYmod_fun_exit( void );

enum
{
	GUI_KEY_METER_SETTING = 100,
	GUI_KEY_METER_ENTER,
	GUI_KEY_METER_EXIT,
	GUI_KEY_METER_1OR3,
	GUI_KEY_TERATY_SELECT,
	GUI_KEY_BEGIN,
	GUI_KEY_END,
	GUI_KEY_SET,
	GUI_KEY_EXIT,
	GUI_KEY_ENTER,
	GUI_KEY_SELFUP_UI,
	GUI_KEY_XNFZ_ENTER,
	GUI_KEY_XNFZ_BACK,
	//GUI_KEY_XNFZ_SETTING,
	GUI_KEY_XNFZ_START,
	GUI_KEY_XNFZ_CLOSE,
	//GUI_KEY_XNFZ_SELECT_IB,
	//GUI_KEY_XNFZ_SELECT_COS,
	//GUI_KEY_XNFZ_SELECT_U,
	GUI_KEY_MENU_ZDXY,
	GUI_KEY_MENU_SDXY,
	GUI_KEY_MENU_SAVE,
	GUI_KEY_MENU_SEARCH,
	GUI_KEY_MENU_ZOUZI,
	GUI_KEY_MENU_USB,
	GUI_KEY_MENU_DLPARAMXY,
	GUI_KEY_MENU_QDQD,
	GUI_KEY_MENU_SYS,
	GUI_KEY_MENU_METER,
	GUI_KEY_MENU_XB
};

typedef struct
{
	float meter_U[3];
	float meter_Uerr[3];
	float meter_I[3];
	float meter_Ierr[3];
	float meter_P[3];
	float meter_Perr[3];
	float meter_Q[3];
	float meter_Qerr[3];
	float meter_COS[3];
	float meter_COSerr[3];
}DLparam_JY_t;

extern	measure_param_t  	g_DL_param;
extern	uint8_t		g_DLparamJY_07_or_97;
extern  dev_id_t    g_gpio_dev;
extern	DLparam_JY_t g_DLparam_buf;
extern	uint8_t     g_DLparamJY_UIOUT_start;

void app_DLparamJYmod_err1_calc( void );
void app_DLparamJYmod_err2_calc( void );
void app_DLparamJYmod_err3_calc( void );
void app_DLparamJYmod_err4_calc( void );
/*********************************************************************
* APP_CSverifymod-H  for DLparamJYmod interface
**********************************************************************/
#define     DLparamJYmod_BG_CLOLR               g_Bg_color
#define   	DLparamJYmod_FG_COLOR               g_Fg_color
#define   	DLparamJYmod_WIN_BG_COLOR       	g_win_Bg_color   //LCD_Color2Index_565(0xc0c0c0)	
#define   	DLparamJYmod_VIEW_BG_COLOR          0

void app_DLparamJYmod_window_main_creat(void);
void app_DLparamJYmod_window_main_destory(void);
void app_DLparamJYmod_DLarg_display(measure_param_t*pDL);
void app_DLparamJYmod_param_and_err_display( DLparam_JY_t *m_dlparam );
void app_DLparamJYmod_meter1_param_err_display( DLparam_JY_t *m_dlparam );
void app_DLparamJYmod_meter2_param_err_display( DLparam_JY_t *m_dlparam );
void app_DLparamJYmod_meter3_param_err_display( DLparam_JY_t *m_dlparam );
void app_DLparamJYmod_meter4_param_err_display( DLparam_JY_t *m_dlparam );

extern	hwin_t 	*m_DLparamJYmod_win;
extern	hwin_t  *m_DLparamJYmod_radio_TREATYSELECT;

enum
{
	XNFZ_IB = 1,  	
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
	EIDT_USER_JUBC	
};

enum
{
	EDIT_U1 = 0,	
	EDIT_U2,		
	EDIT_U3,
	EDIT_UERR1,	
	EDIT_UERR2,		
	EDIT_UERR3,
	EDIT_I1,	
	EDIT_I2,		
	EDIT_I3,
	EDIT_IERR1,	
	EDIT_IERR2,		
	EDIT_IERR3,
	EDIT_P1,	
	EDIT_P2,		
	EDIT_P3,
	EDIT_PERR1,	
	EDIT_PERR2,		
	EDIT_PERR3,
	EDIT_Q1,	
	EDIT_Q2,		
	EDIT_Q3,
	EDIT_QERR1,	
	EDIT_QERR2,		
	EDIT_QERR3,
	EDIT_COS1,	
	EDIT_COS2,		
	EDIT_COS3,
	EDIT_COSERR1,	
	EDIT_COSERR2,		
	EDIT_COSERR3
};

enum
{
	DLPARAMJYMOD_MENU_EDIT    = 0,
	DLPARAMJYMOD_1OR3METER_WAY,
	DLPARAMJYMOD_TREATYSELECT_WAY,
	DLPARAMJYMOD_DL_EDIT,
	DLPARAMJYMOD_MAIN_WINDOW,
	DLPARAMJYMOD_XNFZ_SET_EDIT,
	DLPARAMJYMOD_NOTICE_EDIT,
	DLPARAMJYMOD_CONTROL_EDIT,
	DLPARAMJYMOD_BUTTON_BEGIN,
	DLPARAMJYMOD_BUTTON_STOP,
	DLPARAMJYMOD_BUTTON_SET,
	DLPARAMJYMOD_BUTTON_EXIT,
	DLPARAMJYMOD_BUTTON_ENTER,
	DLPARAMJYMOD_BUTTON_POWER
};

/*********************************************************************
* APP_ZZMOD-H  for DLparamJYmod Rmeter
**********************************************************************/
/******************************读数据结构*************************************/
typedef struct
{
	uint8_t head[3];//在发送帧信息之前,发送1～4个字节FEH，以唤醒接收方。
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t biaoshi_L;
	uint8_t biaoshi_H;
	uint8_t crc;
	uint8_t end;
}send_data_97_t;    //97读数据结构

typedef struct
{
	uint8_t head[3];//在发送帧信息之前，先发送1～4个字节FEH，以唤醒接收方。
	uint8_t head1;
	uint8_t addr[6];
	uint8_t head2;
	uint8_t cmd;
	uint8_t len;
	uint8_t biaoshi[4];
	uint8_t crc;
	uint8_t end;
}send_data_07_t;	//07读数据结构

extern	uint8_t   _task_cmt_id;
extern	uint8_t	  g_DLparam_data_finish;

void ReadDL_param_handle( void );
void app_DLparamJY_Rmeter_init( void );
void app_DLparamJY_Rmeter_uninstall( void );

/*********************************************************************
* APP_ZZMOD-H  for DLparamJYmod XNFZ
**********************************************************************/
void app_DLparamJYmod_measure_auto_change_Ilevel( float m_IB_grade );
void app_DLparamJYmod_xnfz_window_create( void );
void app_DLparamJYmod_xnfz_window_destory( void );
void app_DLparam_XNFZ_task(void*arg);
void app_DLparamJYmod_XNFZ_unlawful_close( void );
BOOL app_DLparamJYmod_XNFZ_state_get( void );


extern	uint8_t   	task_DLparam_XNFZ_id;

#endif

