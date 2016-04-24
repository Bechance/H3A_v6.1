#ifndef _PEC_APP_UARTmod_INSIDE_2010_H
#define _PEC_APP_UARTmod_INSIDE_2010_H

#include "../../../includes.h"
#include "../../../Gui\Gui.h"
#include "../../../Gui\GUI_config.h"
#include "../../../support\app_support.h"



#define HAD_MODEL		"PEC-H3A"
#define HAD_ADDR		"FFFFFFFFFFFF"
#define MAX_UART_SEND_SIZE 	512
#define UPDATED_FALG	0x55
#define SINGEL_METER	0
//#define UARTMOD_UART_DEV_NAME "D232"
#define BTMOD_UART_DEV_NAME "serialB1 Driver"

#define FRAME_HEAD_LEN		13
#define DATA_LEN_HIGH_POS	9
#define DATA_LEN_LOW_POS	10
#define CTL_CODE_POS		8
#define DATA_FLAG_1ST_POS 	11
#define DATA_FLAG_2TH_POS	12
#define ERR_ECHO_FRAME_LEN	16

#define UPLOAD_ALL_RERORD		1
#define UPLOAD_NEW_RECORD		0


#define     BTmod_BG_CLOLR		g_Bg_color
#define   	BTmod_FG_COLOR		g_Fg_color
#define   	BTmod_WIN_BG_COLOR     799    			/*窗口底色*/
#define 	BTmod_PHASE_PARAM_T_MEMBERS 6

extern uint8_t 		g_language;
extern 	uint32_t 	BT_total_JB_items_uploaded;
extern  uint32_t	BT_current_JB_item_num;

#define MEMBER_OFFSET(type,member)	(uint32_t)(&(((type*)0)->member))
#define ARRAY_SIZE(type)	(sizeof(type)/sizeof(type[0]))
#define	UARTMOD_BAUD		38400
//#define	UARTMOD_BAUD		9600
#define	UARTMOD_UART_NO_CHECHK 0

extern void app_BLUETOOTHmod_fun_exit( void );

extern DL_app_user_all_info_t  	g_user_arg;
extern hwin_t* m_edit_win;
extern void app_Sysmod_read_FPGA_version( char*buf );
extern void app_BLUETOOTHmod_window_refresh_comm_stat(uint8_t flag);
extern void app_BLUETOOTHmod_window_destroy( void );
extern void app_BLUETOOTHmod_fun_entry( void );
extern void app_BLUETOOTHmod_window_main_creat(void);



enum
{
	MANUALJB_NEW_RECORD = 0,
	MANUALJB_ALL_RECORD,
	WAIT_FOR_CONNECTION,
	RECORD_UPLOAD_FINISHED
};


enum
{
	UARTMOD_SET_WORKID =0,
	UARTMOD_SET_UID,
	UARTMOD_SET_UNAME,
	UARTMOD_SET_HB,
	UARTMOD_SET_MODEL,
	UARTMOD_SET_WORKER,
	UARTMOD_SET_TIME,
	UARTMOD_SET_ORDER,
	UARTMOD_SET_DJ,
	UARTMOD_SET_DLSR,
	UARTMOD_SET_BB,
	UARTMOD_SET_CS,
	UARTMDO_SET_QS,
	UARTMOD_SET_WAY,
	UARTMOD_SET_PQOPT
};


enum
{
	ERR_UART_TIME_OUT = 1,			//串口接收超时
	ERR_UART_SUM_CHECK,				//帧校验错误
	ERR_UART_DATA_FLAG_NOT_EXIT,	//未知标识符
	ERR_UART_NO_MEM,
	
	ERR_NO_MANUALJB_RECORD = 10,
	ERR_NO_NEW_MANUALJB_RECORD,
	ERR_MANUALJB_FR_OPEN,
	
	ERR_NO_AUTOJB_RECORD=20,
	ERR_NO_NEW_AUTOJB_RECORD,
	ERR_AUTOJB_FR_OPEN
};






extern void app_BLUETOOTHmod_manualJB_plan_download(uint8_t *m_recv_buf,const uint16_t size);
extern void app_BLUETOOTHmod_manualJB_upload_all(uint8_t *m_recv_buf);
extern void app_BLUETOOTHmod_manualJB_upload_new(uint8_t *m_recv_buf);


#endif




