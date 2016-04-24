#include "../../../includes.h"
#include "../../../Gui\Gui.h"
#include "../../../USBstack\USB_connect.h"

/*test界面模块内部头文件*/
#ifndef _PEC_APP_startmod_INSIDE_2010_H
#define _PEC_APP_startmod_INSIDE_2010_H
void bmp_file_view( int x0, int y0, const char*bmp );
void GUI_wait_touch_release( void );

#define     Startmod_BG_COLOR               g_Bg_color
#define   	Startmod_FG_COLOR               g_Fg_color
#define   	Startmod_WIN_BG_COLOR           g_win_Bg_color

typedef struct
{
	char  Exe[20];
	char  name[20];
	char  PressBMP[20];
	char  UNpressBMP[20];
}Extapp_name_t;


typedef struct
{	
	char  	 BMPname[20];
	int      wide;
	int      high;
	uint16_t BMP[6000];	 
}ExtBmp_t;

typedef struct
{	
    int             head1;
	int             head2;
	ExtBmp_t        m_BMP[20];
	int             m_BMPpos;
}ExtBmp_ctl_t;



#define   	GRAPH_WIDE   76
#define   	GRAPH_HIGH   87
#define   	GRAPH_ROW    3
#define   	GRAPH_LINE   3
#define     WIDE_B       150

extern   hwin_t*m_3Dwin;
extern   hwin_t*m_button_last;
extern   hwin_t*m_button_next;
extern   hwin_t*m_pTXT;

extern   Extapp_name_t   m_apparray[30];
extern   Extapp_name_t  *m_apparray_pCUR;

#define  TIME_SETTING    160

extern   int  			m_array_pos;
extern   int  			m_menu_start_pos;
extern   int  			m_menu_end_pos;
extern   int  			m_menu_mark_pos;

extern 	DL_app_spread_info_t	g_DL_spread_info;

void 	 app_startmod_window_creat( void );
void 	 app_startmod_fun_entry();
void  	 app_startmod_DelExp_entery( void );
void 	 app_Extapp_updata( void );
void 	 app_startmod_BMPfile_view( int x0, int y0, const char*bmp );
void 	 app_startmod_appgraph_mark( uint16_t scolor, uint16_t sBcolor, uint16_t Lcolor, int pos );
void 	 app_startmod_markExp_start( void );
void 	 app_startmod_Expapp_obtain( void );
void 	 app_startmod_time_display( char *date_and_time );
void 	 app_startmod_WSD_display(float temp,float hum);
void 	 app_startmod_fun_exit();
void	 app_startmod_Stime_entery( void );
BOOL 	 app_continuous_event( int32_t*jiffies, int32_t*times );
void 	 app_start_EXE( void );
void 	 app_next_page( void );
void 	 app_last_page( void );
void  	 app_SDbmp_init(void);


#define   GRAPH_MARK(pos)  app_startmod_appgraph_mark(C_GUI_WHITE,C_GUI_BLUE,C_GUI_WHITE,pos);
#define   GRAPH_RMARK(pos) app_startmod_appgraph_mark(C_GUI_WHITE,C_GUI_BlACK,C_GUI_BlACK,pos);



extern   Extapp_name_t  *m_apparray_pCUR;
extern   const GUI_FONT  GUI_F32X64_ASCII;


void app_Startmod_battery_display( void );
void app_Startmod_charge_display( void );
void app_Startmod_external_display( void );
void app_Startmod_adapter_protect_display( void );

void app_startmod_lcdBLK_AA(void);
void app_startmod_lcdBLK(void);

#endif
