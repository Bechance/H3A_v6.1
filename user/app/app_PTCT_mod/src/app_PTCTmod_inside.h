#include "../../../includes.h"
#include "../../../Gui\Gui.h"
/*test����ģ���ڲ�ͷ�ļ�*/
#ifndef _PEC_APP_PTCTMOD_INSIDE_2010_H
#define _PEC_APP_PTCTMOD_INSIDE_2010_H
/*********************************************************************
* APP_testMOD-H  for testMOD fun
**********************************************************************/
void 		app_PTCTmod_window_main_creat(hwin_t**edit);
void 		app_PTCTmod_window_main_destory(void);
void 		app_PTCTmod_fun_entry( void );
void 		app_PTCTmod_fun_exit( void );
/*********************************************************************
* APP_testMOD-H  for testMOD INTERFACE
**********************************************************************/
#define   	PTCTmod_BG_COLOR          	    		g_Bg_color       /*�����ɫ*/
#define   	PTCTmod_FG_COLOR               			g_Fg_color       /*����ǰ��ɫ*/
#define   	PTCTmod_WIN_BG_COLOR       				g_win_Bg_color   /*���ڵ�ɫ*/

enum
{
	 EDIT_UA=1,
	 EDIT_UB,
	 EDIT_UC,
	 EDIT_IA,
	 EDIT_IB,
	 EDIT_IC,
	 EDIT_JA,
	 EDIT_JB,
	 EDIT_JC,
	 EDIT_PA,
	 EDIT_PB,
	 EDIT_PC,
	 EDIT_QA,
	 EDIT_QB,
	 EDIT_QC,     //15
	 EDIT_DDA,
	 EDIT_DDB,
	 EDIT_DDC,
	 EDIT_DNA,
	 EDIT_DNB,
	 EDIT_DNC,    //21
	 EDIT_SZ_A,
	 EDIT_SZ_B,
	 EDIT_SZ_C,   //24
	 EDIT_FREQ,
	 EDIT_SZ_ALL

};
extern  measure_param_t   g_DL_param;
extern 	uint16_t 		  m_GUI_3D_window_Bg_cor;
extern  hwin_t       	 *m_PTdrop;
#endif
