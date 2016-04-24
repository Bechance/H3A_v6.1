#include "../../../includes.h"
#include "../../../Gui\Gui.h"
/*变比界面模块内部头文件*/
#ifndef _PEC_APP_BBMOD_INSIDE_2010_H
#define _PEC_APP_BBMOD_INSIDE_2010_H
/*********************************************************************
* APP_bianbiMOD-H  for bianbiMOD fun
**********************************************************************/
void 		app_BBmod_fun_entry( void );
void 		app_BBmod_fun_exit( void );
/*********************************************************************
* APP_bianbiMOD-H  for bianbiMOD INTERFACE
**********************************************************************/
#define   	BBmod_BG_COLOR          	    	g_Bg_color          /*界面底色*/
#define   	BBmod_FG_COLOR               		g_Fg_color       	/*界面前景色*/
#define   	BBmod_WIN_BG_COLOR       			g_win_Bg_color    	/*窗口底色*/
void 		app_BBmod_window_creat( void );
void 		app_BBmod_DLarg_display( measure_param_t*pDLarg );
void 		app_BBmod_state_display(BOOL state);
void 		app_BBmod_window_destroy( void );

extern    dev_id_t              g_DL_dev ;
extern 	hwin_t *m_bianbi_win;
extern 	hwin_t *m_BB_edit;
extern	hwin_t* m_BB_droplist_1;
extern const unsigned char gImage_entery[];
extern const unsigned char gImage_return[];
extern const unsigned char gImage_save[];
extern const unsigned char gImage_exit[];
enum
{
	CHOOSE2 = 0,
	FIREST_CURRENT,
	SECOND_CURRENT,
	BIANBI,
	XIANGJIAO
};

enum
{
	GUI_KEY_SETTING=100,
	GUI_KEY_ENTER,
	GUI_KEY_EXIT,
	KEY_RETURN
};
/*********************************************************************
* APP_BBMOD-H  for bianbiMOD INTERFACE
**********************************************************************/
void 	app_BBmod_measure_entery( void );
void 	app_BBmod_user_info_diplay(DL_app_user_all_info_t *pdata);

extern    measure_param_t   g_DL_param;
extern    DL_app_user_all_info_t    g_user_arg ;
extern	  Bsize_t	   g_select_branch;

#endif
