#include "../../../includes.h"
#include "../../../Gui\Gui.h"
#ifndef _PEC_APP_ERRMOD_INSIDE_2010_H
#define _PEC_APP_ERRMOD_INSIDE_2010_H
/*********************************************************************
* APP_ERRMOD-H  for ERRMOD fun
**********************************************************************/
void 		app_ERRmod_fun_entry( void );
void 		app_ERRmod_fun_exit( void );
#define     app_ERRmod_DL_float_display(ch,id)		GUI_edit_float_display_at(ch,id,9)
/*********************************************************************
* APP_ERRMOD-H  for ERRMOD INTERFACE
**********************************************************************/
extern      hwin_t* 		  m_correct_win;
extern      measure_param_t   g_DL_param;
extern      DL_app_user_all_info_t    g_user_arg ;
extern      dev_id_t          g_DL_dev;
extern      hwin_t* 		  m_correct_option_edit;
extern      uint8_t           m_ERRmod_phase;
#define   	ERRmod_BG_COLOR          	    	g_Bg_color
#define   	ERRmod_FG_COLOR               		g_Fg_color	
#define   	ERRmod_WIN_BG_COLOR       		    g_win_Bg_color 

enum
{
	UCORRECT_EDIT    = 0,
	ICORRECT_USER_EDIT ,
	ICORRECT_ERR_EDIT ,
	ICORRECT_DL_EDIT ,
	ICORRECT_WAY_RADIO ,
	ICORRECT_PHASE_RADIO ,
	ICORRECT_CORRECT_DATA_EDIT ,
	ICORRECT_CORRECT_XNFZ ,
	ICORRECT_NOTICE_EDIT 
};


typedef struct
{
	char c_base_U[8];
	char c_base_I[8];
}correct_option_t;

extern           correct_option_t   m_correct_option;
enum
{
	ERR_U_A    = 1,
	ERR_U_B,
	ERR_U_C,
	ERR_I_A,
	ERR_I_B,
	ERR_I_C,
	ERR_P_A,
	ERR_P_B,
	ERR_P_C,
	ERR_Q_A,
	ERR_Q_B,
	ERR_Q_C,
	ERR_J_A,
	ERR_J_B,
	ERR_J_C,
	ERR_SUM_S,
	ERR_SUM_M,	
	ERR_ERR,
	ERR_COS,
	ERR_CIRCLE,
	ERR_CORRECT_A,
	ERR_CORRECT_B,
	ERR_CORRECT_C,
	ERR_CONST,
	ERR_QS,
	ERR_MODE,
	ERR_BB1,
	ERR_BB2,
	ERR_BASE_P,
	ERR_BASE_Q,
	ERR_BASE_U,
	ERR_BASE_I,
	ERR_U_LEVEL,
	ERR_QB_OFFSET,
	ERR_QA_MOD
};


enum
{
	ERR_U_CORRECT_MAIN    = 1,
	ERR_I_CORRECT_MAIN,
	ERR_U_CORRECT_LINE,
	ERR_I_CORRECT_LINE,
	ERR_U_CORRECT_ANGLE,
	ERR_I_CORRECT_ANGLE,
	ERR_U_CORRECT_ANGLELINE,
	ERR_I_CORRECT_ANGLELINE,
	ERR_U_CORRECT_CANGLELINE,
	ERR_I_CORRECT_CANGLELINE,	
	ERR_I_CORRECT_IS,
	ERR_I_CORRECT_IUAGE,
	ERR_I_CORRECT_US,
	ERR_I_CORRECT_COS,
	ERR_I_CORRECT_FREQ
};

enum
{
	GUI_KEY_SET=100,
	GUI_KEY_ENTER,
	GUI_KEY_EXIT,
	GUI_KEY_SW,
	GUI_KEY_SETTING,
	GUI_KEY_START,
	GUI_KEY_STOP,
	GUI_KEY_PHASE,
	GUI_KEY_WAY 
};

extern hwin_t*m_errmod_correct_win;
extern hwin_t*m_errmod_Ucorrect_win;
extern hwin_t*m_errmod_Icorrect_win;
extern hwin_t*m_errmod_drop_dlsr;
extern hwin_t*m_errmod_TMRcorrect_win;
void app_ERRmod_window_creat(void);
void app_ERRmod_window_destroy(void);
void app_ERRmod_Uwindow_creat(void);
void app_ERRmod_Uwindow_destroy(void);
void app_ERRmod_1or3_set(void);

void app_ERRmod_Ucorrect_task(void*arg);
void app_ERRmod_correct_step_send( uint8_t main_step,uint8_t sub_step,uint8_t phase,uint8_t pos);
void app_ERRmod_Iwindow_creat(void);
void app_ERRmod_Iwindow_destroy(void);
void app_ERRmod_Icorrect_task(void*arg);
void app_ERRmod_Err_display(float*perr);
void app_ERRmod_circle_display(Bsize_t circle);
Bsize_t app_global_path1_circle_obtain( void );
void app_ERRmod_TMRwindow_creat(void);
void app_ERRmod_TMRwindow_destroy( void );
void app_ERRmod_TMRcorrect_task(void*arg);

void 	meausre_DDS_out_select( uint32_t a_phase, uint32_t b_phase, uint32_t c_phase );
extern  BOOL    m_IAUTO_level_F;
/*********************************************************************
* APP_ERRMOD-H  for ERRMOD AUTO CORRECT
**********************************************************************/
void app_ERRmod_path_select(BOOL UEN,BOOL IEN,uint8_t phase);
void app_ERRmod_XNFZ_open(void);
void app_ERRmod_XNFZ_close(void);
#endif
