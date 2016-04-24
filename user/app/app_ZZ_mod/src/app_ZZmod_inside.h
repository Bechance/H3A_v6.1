#include "../../../includes.h"
#include "../../../Gui\Gui.h"

#ifndef _PEC_APP_ZZMOD_INSIDE_2011_H
#define _PEC_APP_ZZMOD_INSIDE_2011_H

void  	bsp_delay_ms( unsigned long	ms );
extern 		uint16_t 		 m_GUI_3D_window_Bg_cor;
extern      measure_param_t  g_DL_param;
extern const unsigned char gImage_entery[];
extern const unsigned char gImage_return[];
extern const unsigned char gImage_save[];
extern const unsigned char gImage_exit[];
/*********************************************************************
* APP_ZZMOD-H  for ZZMOD INTERFACE
**********************************************************************/
#define     zz_BG_COLOR                  g_Bg_color
#define   	zz_FG_COLOR               	 g_Fg_color
#define   	zz_WIN_BG_COLOR       		 g_win_Bg_color   //LCD_Color2Index_565(0xc0c0c0)	
extern	hwin_t *m_zzmod_radio_ITORMC;
extern	hwin_t *m_zzmod_radio_TREATYSELECT;
extern	hwin_t *m_zz_3DWIN;
extern	hwin_t *m_zz_DL_win;
extern	hwin_t *m_zz_edit_win;
extern  hwin_t *m_Meter_Treaty_edit;

enum
{
	GUI_KEY_ZZ_BEGIN = 100,
	GUI_KEY_ZZ_END,
	GUI_KEY_ZZ_TIMER,
	GUI_KEY_EXIT,
	GUI_KEY_SET,
	GUI_KEY_ENTER,
	GUI_KEY_XNFZ_BUTTON,
	GUI_KEY_METER_ITMC,
	GUI_KEY_TERATY_SELECT,
	GUI_KEY_XNFZ_START,
	GUI_KEY_XNFZ_CLOSE
};

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
	ZZMOD_MAIN_EDIT = 0,
	ZZMOD_NOTICE_EDIT,
	ZZMOD_TIMER_EDIT,
	ZZMOD_ITORMC_METER,
	ZZMOD_TREATYSELECT_WAY,
	ZZMOD_XNFZ_SET_EDIT,
	ZZMOD_TIP_WINDOW,
	ZZMOD_BUTTON_START,
	ZZMOD_BUTTON_STOP,
	ZZMOD_BUTTON_SET,
	ZZMOD_BUTTON_ENTER,
	ZZMOD_BUTTON_POWER,
	ZZMOD_BUTTON_HALT
};

enum
{
	EDIT_ENERGY_START = 0,	/*表起始电能*/
	EDIT_ENERGY_END,		/*表结束电能*/
	EDIT_ENERGY_REAL,		/*表实计电能*/
	EDIT_ENERGY_MEASURE,	/*实测电能*/
	EDIT_ENERGY_ERR,		/*走字误差*/
	EDIT_ENERGY_TIME,		/*走字定时*/
	EDIT_ENERGY_PRESET		/*预置走字电能*/
};

enum
{
	EDIT_INSTRUMENT_0 = 1,
	EDIT_INSTRUMENT_1,
	EDIT_INSTRUMENT_2,
	EDIT_INSTRUMENT_3,
	EDIT_INSTRUMENT_4,
	EDIT_INSTRUMENT_5,
	EDIT_INSTRUMENT_6
};

enum
{
	EDIT_METER_S0 = 1,
	EDIT_METER_S1,
	EDIT_METER_S2,
	EDIT_METER_S3,
	EDIT_METER_S4,
	EDIT_METER_S5,
	EDIT_METER_E0,
	EDIT_METER_E1,
	EDIT_METER_E2,
	EDIT_METER_E3,
	EDIT_METER_E4,
	EDIT_METER_E5
};

void  	app_zzmod_fun_entry( void );
void  	app_zzmod_fun_exit( void );
void  	app_zzmod_window_main_creat(void);
void  	app_zzmod_window_destroy( void );
void  	app_ZZmod_DLarg_display(measure_param_t*pDL);
void  	app_zzmod_edit_display( void );
void 	app_ZZmod_edit_instrument_energy_display( float m_data );
void 	app_ZZmod_edit_meter_energy_display(float m_Sdata, float m_Edata);
void 	app_zzmod_energy_trends_display( void );
void 	app_zzmod_time_display( long m_min );
void 	app_ZZ_control_tip(uint8_t m_flag);
/*********************************************************************
* APP_ZZMOD-H  for ZZMOD connect
**********************************************************************/
void 		app_zzmod_insmod( void );
void		app_zzmod_exit( void );
Bsize_t 	app_zzmod_add( void );

/*********************************************************************
* APP_ZZMOD-H  for ZZMOD fun
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

typedef struct
{
	char energy_start[12];
	char energy_end[12];
	char energy_real[8];
	char energy_measure[8];
	char energy_preset[8];
	char energy_zz_err[8];
	char zz_time_min[8];
}zzmod_arg;

extern 	zzmod_arg		m_zz_arg;
extern	uint8_t 		g_zz_XNFZ_start;

void app_zzmod_measure_energy_calc( void );

/*********************************************************************
* APP_ZZMOD-H  for ZZMOD XNFZ
**********************************************************************/
void app_zz_xnfz_window_create( void );
void app_zz_xnfz_window_destory( void );
void app_zz_XNFZ_task(void*arg);
void app_zz_XNFZ_UIout_SW(const void*str);
void app_zz_XNFZ_UIout_EN(BOOL U_EN,BOOL I_EN);
void app_zzmod_XNFZ_unlawful_close( void );
BOOL app_zzmod_XNFZ_state_get( void );
void app_zzmod_measure_auto_change_Ilevel( float m_IB_grade );


extern	uint8_t   task_XNFZ_id;

#endif

