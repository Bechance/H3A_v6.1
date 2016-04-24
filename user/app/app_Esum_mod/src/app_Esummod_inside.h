#include "../../../includes.h"
#ifndef _PEC_APP_WIRMANEGE_INSIDE_2010_H
#define _PEC_APP_WIRMANEGE_INSIDE_2010_H
/*********************************************************************
* APP_WIRMOD-H  for WIRMOD fun
**********************************************************************/
void 		app_Esummod_fun_entry( void );
void 		app_Esummod_fun_exit( void );
/*********************************************************************
* APP_WIRMOD-H  for WIRMOD INTERFACE
**********************************************************************/
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
#define     ESummod_BG_COLOR               g_Bg_color
#define   	ESummod_FG_COLOR               g_Fg_color
#define   	ESummod_WIN_BG_COLOR           g_win_Bg_color  	
enum
{
	GUI_KEY_NEXT_PAGE = 100,
	GUI_KEY_LAST_PAGE,
	GUI_KEY_NPAGE,
	GUI_KEY_NR_0,
	GUI_KEY_NR_1,
	GUI_KEY_NR_2,
	GUI_KEY_NR_3,
	GUI_KEY_NR_4,
	GUI_KEY_NR_5,
	GUI_KEY_NR_6,
	GUI_KEY_NR_7,
	GUI_KEY_NR_8,
	GUI_KEY_NR_9,
	GUI_KEY_NR_10,
	GUI_KEY_NR_11,
	GUI_KEY_NR_12,
	GUI_KEY_NR_13,
	GUI_KEY_NR_14,
	GUI_KEY_NR_15,
	GUI_KEY_SEARCH,
	GUI_KEY_SEARCH_EDIT,
	GUI_KEY_SEARCH_EXIT,
	GUI_KEY_DELETEALL,
	GUI_KEY_DELETEONE,
	GUI_KEY_RETURN
};
typedef struct
{
   char start_time[20];
   char end_time[20];
   char BL[10];
   char Eg_1[20];
   char Eg_2[20];
   char second[10];
}Esum_arg_t;


typedef struct
{
	DL_app_user_info_t  DL_app_user_info;
	DL_app_param_str_t  DLarg;
	Esum_arg_t          Esum_arg;
}Esum_save_arg_t;

typedef struct
{
	int Filepos_S ;
	int Filepos_E ;
	int Markpos   ;
	int Maxsize   ;
	int CURpage   ;
	int Pagesize  ;
	int nPage;
	int CURsize;
	uint8_t ofg;
	char FILname[20];
}page_ctl_t;

extern 		uint16_t 		 m_GUI_3D_window_Bg_cor;
extern 		Esum_arg_t       m_Esum_arg;
extern     	hwin_t*			 m_Esummod_edit;      
extern     	hwin_t*   		 m_Esummod_DL_win ;      

void app_Esummod_window_creat(void);
void app_Esummod_window_destroy(void);
void app_Esummod_DLview_task( void*arg );
void app_Esummod_SUM_task( void*arg );
void app_Esummod_record_display(void*data);
void app_Esummod_search( void *arg );
void app_Esumarg_display(Esum_arg_t*p);
void app_Esummod_DLarg_display(measure_param_t*pDL);
void app_upload_main( void );

#endif
