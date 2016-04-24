#include "../../../includes.h"
#include "../../../Gui\Gui.h"
#include"app_WIRmod_componentlib.h"
#ifndef _PEC_APP_WIRMANEGE_INSIDE_2010_H
#define _PEC_APP_WIRMANEGE_INSIDE_2010_H
/*********************************************************************
* APP_WIRMOD-H  for WIRMOD fun
**********************************************************************/
void 		app_WIRmod_YDWIR_entry( void );
void 		app_WIRmod_YDWIR_exit( void );
void 		app_WIRmod_fun_entry( void );
void 		app_WIRmod_fun_exit( void );
extern           uint8_t                m_WIR_cosoffset;
/*********************************************************************
* APP_WIRMOD-H  for WIRMOD alg
**********************************************************************/
enum
{
	PHASE_INVALID = 0,
	PHASE_ADD = 1,
	PHASE_SUB
};
typedef struct
{
	uint8_t is_lost_UI;
	uint8_t Ua_lost;
	uint8_t Ub_lost;
	uint8_t Uc_lost;
	uint8_t Ia_lost;
	uint8_t Ib_lost;
	uint8_t Ic_lost;
	uint8_t phaseDir;
	char c_cos[20];
	char c_U[20];
	char c_I[20];
	uint8_t jxmod;
}WIR_ALG_t;

extern WIR_ALG_t*	m_alg_presult;
extern a_sem_t *    m_WIRmod_sem;
extern a_sem_t *    m_WIRmod_Resem;

extern uint8_t      m_WIR_is_draw ;
void 				app_WIRmod_alg_entery( void );
void				app_WIRmod_alg_exit( void );
/*********************************************************************
* APP_WIRMOD-H  for WIRMOD INTERFACE
**********************************************************************/
/*颜色配置*/
#define   	WIRmod_BG_COLOR          	    	g_Bg_color    
#define   	WIRmod_FG_COLOR               		g_Fg_color        /*主界面前景色*/
#define   	WIRmod_WIN_BG_COLOR       			g_win_Bg_color    /*主窗口底色*/
#define   	DRAW_WIN_BG_COLOR       			0x10A3    			  /*图形绘制窗口底色*/
#define   	DRAW_WIN_FG_COLOR       		    (C_GUI_WHITE)     /*图形绘制窗口前景色*/

enum
{
	EDIT_U_A    = 1,
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

typedef struct
{
	ELEcomponent_t*pcomp;
	uint8_t branch;
}comp_branch_t;

typedef struct
{
	comp_branch_t Ua;
	comp_branch_t Ub;
	comp_branch_t Uc;
	comp_branch_t Ia_add;
	comp_branch_t Ia_sub;
	comp_branch_t Ib_add;
	comp_branch_t Ib_sub;
	comp_branch_t Ic_add;
	comp_branch_t Ic_sub;
}draw_coord_t;

typedef struct
{
	char  Result_MUa[3];
	char  Result_MUb[3];
	char  Result_MUc[3];
	char  Result_MIa[4];
	char  Result_MIb[4];
	char  Result_MIc[4];
	char  cos[10];
	uint8_t phaseDir;
	uint8_t verdict;
	uint8_t jxmod;
	uint8_t WIR_cosoffset;
}WIRresult_t;

typedef struct
{
	char* Result_U;
	uint16_t x;
	uint16_t y;
}WIRresult_match_t;
void 			app_WIRmod_window_creat(int flag);
void 			app_WIRmod_window_destroy( void );
void 			app_WIRmod_result_lostUI_display( WIR_ALG_t*parg );
void 			app_WIRmod_componet_auto_link( WIRresult_t*presult );
void 			app_WIRmod_result_display( WIRresult_t*presult );
void 			app_WIRmod_draw_creat( uint8_t flag );

extern hwin_t* m_WIR_win 		;
extern hwin_t* m_WIR_info_win 	;
extern hwin_t* m_WIRdraw_win 		;
extern hwin_t* m_WIR_info_EDIT ;
extern hwin_t* m_WIRresjult_win 	;

extern ELEcomponent_t*	m_WIRcomp_ground1;
extern ELEcomponent_t*	m_WIRcomp_ground2;
extern ELEcomponent_t*	m_meter1;
extern ELEcomponent_t*	m_meter2;
extern ELEcomponent_t*	m_meter3;
extern ELEcomponent_t*	m_UHGQ_3_3;
extern ELEcomponent_t*	m_UHGQ_3_4;
extern ELEcomponent_t*	m_IHGQ1;
extern ELEcomponent_t*	m_IHGQ2;
extern ELEcomponent_t*	m_IHGQ3;
extern hwin_t* 			 m_WIR_win ;
extern hwin_t* 			 m_WIR_info_EDIT ;
extern uint8_t  		 m_WIR_jxmod;
extern WIRresult_t  	 m_WIRresult;
extern uint8_t           m_WIR_jxmod;
extern measure_param_t   g_DL_param;

extern    measure_param_t   g_DL_param;
extern    DL_app_user_all_info_t    g_user_arg ;
extern	  Bsize_t	   g_select_branch;
enum
{
 	GUI_KEY_SW = 100,
	GUI_KEY_JXMOD
};
/*********************************************************************
* APP_WIRMOD-H  for WIRMOD redeem
**********************************************************************/
typedef struct
{
   char  cur_cos[10];
   char  last_P[10];
   char  cur_P[10];
   char  cur_rate[10];
   char  ch_GX[10];
}redeem_arg_t;
extern float        m_WIRmod_GX;
extern redeem_arg_t m_WIRmod_redeem_arg;
void app_WIRmod_redeem_function(void);
void app_WIRmod_redeem_search(redeem_arg_t*p);

/*********************************************************************
* APP_WIRMOD-H  for SEARCH
**********************************************************************/
typedef struct
{
	WIR_ALG_t           m_WIR_ALG[4];
	redeem_arg_t		m_redeem_arg;
	uint8_t				m_offset;
}WIR_save_arg_t;
void app_WIRmod_search( void *arg );

void app_WIRmod_WIRresult_display(void*pResult);
BOOL app_WIRmod_resulst_sys( WIRresult_t*presult );
void app_WIRmod_simpleness_alg(WIRresult_t*pwir);
uint8_t app_USERinfo_Save(void);
#endif
