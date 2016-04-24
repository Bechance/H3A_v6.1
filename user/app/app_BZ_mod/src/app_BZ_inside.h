#include "../../../includes.h"
#include "../../../Gui\Gui.h"
/*test界面模块内部头文件*/
#ifndef _PEC_APP_BZMOD_INSIDE_2013_H
#define _PEC_APP_BZMOD_INSIDE_2013_H
/*********************************************************************
* APP_testMOD-H  for testMOD fun
**********************************************************************/
void 		app_BZmod_window_creat(void);
void 		app_BZmod_window_destroy(void);
void 		app_BZmod_fun_entry( void );
void 		app_BZmod_fun_exit( void );
enum
{ 	
	EDIT_U_A = 1 ,
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
/*********************************************************************
* APP_testMOD-H  for testMOD INTERFACE
**********************************************************************/
#define   	BZmod_BG_COLOR          	    		g_Bg_color       /*界面底色*/
#define   	BZmod_FG_COLOR               			g_Fg_color       /*界面前景色*/
#define   	BZmod_WIN_BG_COLOR       				g_win_Bg_color   /*窗口底色*/
extern      hwin_t*   m_BZ_WIN;
extern      uint16_t  m_GUI_3D_window_Bg_cor;
#endif
