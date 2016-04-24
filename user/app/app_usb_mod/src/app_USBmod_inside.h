#include "../../../includes.h"
#include "../../../Gui\Gui.h"
#include "../../../USBstack\USB_connect.h"
#ifndef _PEC_APP_USBMANEGE_INSIDE_2010_H
#define _PEC_APP_USBMANEGE_INSIDE_2010_H
extern   void app_USERmod_record_toUSB( void*arg );
extern		dev_id_t g_UIout_dev;
/*********************************************************************
* APP_USBMOD-H  for USBMOD fun
**********************************************************************/
void 		app_USBmod_fun_entry( void );
void 		app_USBmod_fun_exit( void );
/*********************************************************************
* APP_USBMOD-H  for USBMOD INTERFACE
**********************************************************************/
#define   	USBmod_BG_COLOR          	    	g_Bg_color    /*界面底色*/
#define   	USBmod_FG_COLOR               		g_Fg_color    /*界面前景色*/
#define   	USBmod_WIN_BG_COLOR       			g_win_Bg_color/*窗口底色*/

void 		app_USBmod_window_creat( void );
BOOL 		app_USBmod_search_window_creat( void );
extern      Bsize_t    g_mod_branch;
Bsize_t 	fatfs_file_locate_icon( char*icon,char*file_name,uint32_t*paddr,Bsize_t addr_size, void(*dis)(Bsize_t num));
/*********************************************************************
* APP_USBMOD-H  for HZLIB
**********************************************************************/
void 		app_hzlib_updata_entery( uint8_t key );
void 		app_help_file_paging(void);

void 		app_USBmod_window_destroy( void );
uint8_t		app_correct_data_read(correct_save_data_t *pbuf,uint32_t b_size);
uint8_t		app_correct_data_backup(correct_save_data_t *pbuf,uint32_t b_size);

#endif
