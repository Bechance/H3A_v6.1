#include "../../../includes.h"
#include "../../../Gui\Gui.h"
/*test����ģ���ڲ�ͷ�ļ�*/
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
#define   	FILmod_BG_COLOR          	    		g_Bg_color       /*�����ɫ*/
#define   	FILmod_FG_COLOR               			g_Fg_color       /*����ǰ��ɫ*/
#define   	FILmod_WIN_BG_COLOR       				g_win_Bg_color   /*���ڵ�ɫ*/
extern      hwin_t*   m_FIL_WIN;
#endif
