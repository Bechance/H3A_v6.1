#include "../../../includes.h"
#include "../../../Gui\Gui.h"
#ifndef _PEC_APP_WAVE_INSIDE_2010_H
#define _PEC_APP_WAVE_INSIDE_2010_H
extern  hwin_t*	m_wavemod_view;
/*********************************************************************
* APP_WAVEMOD-H  for WAVEMOD fun
**********************************************************************/
extern      const unsigned char GUI_enter_image[];
void 		app_wavemod_fun_entry( void );
void 		app_wavemod_fun_exit( void );
enum
{
	GUI_KEY_UA=100,
	GUI_KEY_UB,
	GUI_KEY_UC,
	GUI_KEY_IA,
	GUI_KEY_IB,
	GUI_KEY_IC,
	GUI_KEY_ULEVEL,
	GUI_KEY_ILEVEL,
	GUI_KEY_UZOOM,
	GUI_KEY_IZOOM,
	GUI_KEY_COM
};
enum
{	
	EDIT_U_A=1,
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
	EDIT_FEQ
};
typedef struct
{
   uint16_t x;
   uint16_t y;
}m_lastxy_t;
/*********************************************************************
* APP_WAVEMOD-H  for WAVEMOD INTERFACE
**********************************************************************/
#define   		 WAVE_BG_COLOR          	    		g_Bg_color 					/*界面底色*/
#define   		 WAVE_FG_COLOR               			g_Fg_color     				/*界面前景色*/
#define   		 WAVE_WIN_BG_COLOR       				g_win_Bg_color				/*窗口底色*/
void 		     app_wavemod_window_creat(void);
void 			 app_wavemod_window_destory(void);
void 			 app_wavemod_window_line_draw(hwin_t*pWIN);
extern  hwin_t*	m_Wave3DWIN ;
extern  hwin_t*	m_WaveWIN   ;
extern  hwin_t*	m_WaveDL ;
extern  hwin_t*	m_UIcheckbox[6];
extern  dev_id_t g_DL_dev;
extern  measure_param_t  g_DL_param;
extern  const   uint16_t    m_WaveCor[6];

extern const unsigned char gImage_entery[];
extern const unsigned char gImage_return[];
extern const unsigned char gImage_save[];
extern const unsigned char gImage_exit[];

#endif
