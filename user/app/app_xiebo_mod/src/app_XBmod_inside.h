#include "../../../includes.h"
#include "../../../Gui\Gui.h"
#ifndef _PEC_APP_XB_INSIDE_2010_H
#define _PEC_APP_XB_INSIDE_2010_H
typedef struct
{
   Bsize_t offset;
   float   Usage;
}xb_data_t;


extern  hwin_t*	m_XBmod_view;
/*********************************************************************
* APP_XBMOD-H  for XBMOD fun
**********************************************************************/
void 		app_XBmod_fun_entry( void );
void 		app_XBmod_fun_exit( void );
enum
{
	GUI_KEY_RETURN = 100,
    GUI_KEY_SELECT_PAGE,
    GUI_KEY_SELECT_PHASE,
    GUI_KEY_SELECT_WAY

};

/*********************************************************************
* APP_XBMOD-H  for XBMOD INTERFACE
**********************************************************************/
#define   		XBmod_BG_COLOR          	    		g_Bg_color/*界面底色*/
#define   		XBmod_FG_COLOR               			g_Fg_color       /*界面前景色*/
#define   		XBmod_WIN_BG_COLOR       				g_win_Bg_color/*窗口底色*/
void 			app_XBmod_graph_display(int len,int n,xb_data_t*pfdata,hwin_t*hp);
void 			app_XBmod_ruler_clean(void);
void 			app_XBmod_THDdisplay(float fuage,hwin_t*hp);
void 			app_XBmod_window_create(void);
void 			app_XBmod_window_destory(void);
void 			app_XBmod_GRtask(void*arg);
void 			app_XBmod_Numtask(void*arg);
void 			app_XBmod_NumWIN_create(void);


extern 	hwin_t*	m_XBmod_win_Obj  ;
extern 	hwin_t*	m_XBmod_win_radio;
extern  dev_id_t g_DL_dev;
extern  hwin_t*	m_XBmod_win_drop_list;

extern const unsigned char gImage_entery[];
extern const unsigned char gImage_return[];
extern const unsigned char gImage_save[];
extern const unsigned char gImage_exit[];
#endif
