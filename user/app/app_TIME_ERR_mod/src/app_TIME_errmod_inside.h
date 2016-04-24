#include "../../../includes.h"
#include "../../../Gui\Gui.h"
#ifndef _PEC_APP_TIME_errmod_INSIDE_2010_H
#define _PEC_APP_TIME_errmod_INSIDE_2010_H
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
	ERR_DAY,  	
	ERR_DAY_RS,  	
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
		KEY_RJS_CALC     ,
		KEY_RETURN
};

#define     TIME_errmod_BG_CLOLR               	g_Bg_color
#define   	TIME_errmod_FG_COLOR               g_Fg_color
#define   	TIME_errmod_WIN_BG_COLOR           g_win_Bg_color  	

extern      hwin_t*     m_TIME_ERR_3DWIN;
extern      hwin_t*     m_TIME_ERR_main_win;
extern      hwin_t*     m_TIME_ERR_DL_win;
extern      hwin_t*     m_TIME_ERR_LJT_win;

void 		app_Systime_obtain( void);
uint8_t 	app_USERinfo_Save(void);

void 		app_TIME_errmod_window_creat(void);		
void 		app_TIME_errmod_window_destroy(void);
void 		app_TIME_Constselect(void);
int 		app_TIME_Qs_select(float W,float Const);
void 		app_TMR_task(void*arg);
void 		app_TIME_DLview_task( void*arg );
void 		app_TIME_ERR_task(void*arg);

uint8_t 	app_TIME_ERRARG_setting(hwin_t*hp);
void 		app_TIME_ERR_Exc(int*circle,int*last_circle,hwin_t**err_win);
BOOL 		app_TIME_ERR_pause_view(hwin_t*hp);
Bsize_t 	app_global_path1_circle_obtain( void );
Bsize_t 	app_global_path2_circle_obtain( void );

void 		app_TIME_errmod_fun_entry();
void 		app_TIME_errmod_fun_exit();
void 		app_TIME_LJT_init( hwin_t*hp);
void  		app_TIME_LJT_display( hwin_t*hp, measure_param_t*Pdl,DL_flag_t*Pdl_flag);

uint16_t 	GUI_read_block_Extp( int x1, int y1, int x2, int y2);
void	 	GUI_write_block_Extp( int x1, int y1, int x2, int y2, uint16_t sec );

void 		GUI_write_block_Ext( int x1, int y1, int x2, int y2, uint16_t sector );
void 		GUI_read_block_Ext( int x1, int y1, int x2, int y2, uint16_t sector );
void		Gui_button_set_color(uint16_t Fgcor,uint16_t Bgcor);

void 		app_TIME_XNFZ_UIout_EN(BOOL U_EN,BOOL I_EN);
void 		app_TIME_XNFZ_UIout_SW(const void*str);
void 		app_TIME_XNFZ_task(void*arg);
void 		app_TIME_ERRmod_XNFZ_unlawful_close( void );
BOOL 		app_TIME_ERRmod_XNFZ_state_get( void );
void 		app_TIME_XNFZ_UIout_dz_handle( uint8_t m_dz );
void 		app_TIME_XNFZ_into_WFH(void);
uint8_t 	app_TIME_XNFZ_3X4X_3X3X_judge(void);
void 		app_TIME_XNFZ_into_WFH(void);
void  		app_TMR_errPAINT(void);
void        app_TMR_DLpaint(void);
void 		app_TMR_search( void *arg );
void  		app_TMR_err_rePAINT(void);

extern      hwin_t *     m_TIME_errWIN;
extern 		uint16_t 	 m_GUI_3D_window_Bg_cor;
extern      measure_param_t  g_DL_param;
extern const unsigned char gImage_entery[];
extern const unsigned char gImage_return[];
extern const unsigned char gImage_save[];
extern const unsigned char gImage_exit[];
extern uint8_t	g_TIME_3or4_display_flag;
extern float   m_TMR_err[4];
extern float   m_day_timer_error;
#endif
