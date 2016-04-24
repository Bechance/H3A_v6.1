#include "../../../includes.h"
#include "../../../Gui\Gui.h"
/*test界面模块内部头文件*/
#ifndef _PEC_APP_FILMOD_INSIDE_2013_H
#define _PEC_APP_FILMOD_INSIDE_2013_H
/*********************************************************************
* APP_testMOD-H  for testMOD fun
**********************************************************************/
void 		app_FILmod_window_creat(void);
void 		app_FILmod_window_destroy(void);
void 		app_FILmod_fun_entry( void );
void 		app_FILmod_fun_exit( void );
/*********************************************************************
* APP_testMOD-H  for testMOD INTERFACE
**********************************************************************/
#define   	FILmod_BG_COLOR          	    		g_Bg_color       /*界面底色*/
#define   	FILmod_FG_COLOR               			g_Fg_color       /*界面前景色*/
#define   	FILmod_WIN_BG_COLOR       				g_win_Bg_color   /*窗口底色*/
extern      hwin_t*   m_FIL_WIN;
#endif
