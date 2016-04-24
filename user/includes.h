#ifndef     INCLUDES_H
#define     INCLUDES_H

/*系统IDE LIB*/
#include 	  	"math.h"
#include 	 	<stdlib.h>
#include 	  	<stdio.h>
#include 	  	<string.h>
#include 		<linkage.h>
/*内核LIB*/
#include    	"../kernel\includes\ucos_ii.h"
/*BSP*/
#include	    "../BSP\INCLUDE\DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include 	    "../BSP\INCLUDE\DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include 	    "../BSP\bsp.h"
/*支持模块LIB*/
#include     	"compile_cfg.h"
#include        "usertype.h"
#include		"global_user_data.h"
#include        "os_connect\os_connect.h"
#include 	    "support\app_support.h"
#include     	"drivers\Osdriver.h"
#include		"Gui\Gui.h"
#include 		"Fatfs\FatFs.h"
#include 		"USBstack\USB_connect.h"


typedef struct
{
  uint16_t is_first_flag;
  uint32_t hzk16_start_addr;
  uint32_t hzk16_size;
  uint32_t Asc16_start_addr;
  uint32_t Asc16_size;
  uint32_t hzk24_start_addr;
  uint32_t hzk24_size;
}HZlib_info_t;
/*********************************************************************
* GLOBAL-H  for global connect
**********************************************************************/
void app_global_DLarg_obtain( void*pDLarg );
void app_global_path1_DLerr_obtain( void*pDLarg );
void app_global_path2_DLerr_obtain( void*pDLarg );
void app_global_path3_DLerr_obtain( void*pDLarg );
void app_global_DLflag_obtain( void*pDL_flag);
void app_global_set_user_dl_arg(void*arg);
void app_global_QB_select( uint8_t A_dlsr,uint8_t B_dlsr,uint8_t C_dlsr );
void app_global_measure_state_check( void );
void app_global_DL_measure_entery( void );
void app_global_DL_measure_exit( void );
void app_global_DL_driver_add( void );
void app_global_DL_driver_delete( void );
void app_global_resouce_init( void );
void app_global_resouce_req( void*arg );
void app_global_resouce_release( void*arg );
void app_Systime_obtain( void);
void app_save_PECD(const char*file,const void*p,int size);

uint8_t app_USERinfo_Save(void);
void app_help_view(void);
void GUI_waitWIN_create(char*str);
void GUI_waitWIN_CHmod(char*str);
void GUI_waitWIN_destory(void);

#define UER_DSIZE 31
typedef struct
{
   uint8_t n;
   char    mID[UER_DSIZE];
}Slist_t;

typedef struct
{
   void(*_record_detail)(Slist_t*plist);
   char* name;
}search_fun_t;
/*********************************************************************
*GLOBAL-H  FOR flash
**********************************************************************/
enum
{
	SECTOR_0  = 0,
	SECTOR_1,
	SECTOR_3,
	SECTOR_4,
	SECTOR_5,
	SECTOR_6,
	SECTOR_7,
	SECTOR_8,
	SECTOR_9,
	SECTOR_10,
	SECTOR_11,
	SECTOR_12,
	SECTOR_13,
	SECTOR_14,
	SECTOR_15
};
/*
   flash  空间分配
   0x00000---0x0FFFF                 sector0
   0x10000---0x1FFFF                 sector1
   0x20000---0x2FFFF                 sector2
   0x30000---0x3FFFF                 sector3
   0x40000---0x4FFFF                 sector4
   0x50000---0x5FFFF                 sector5
   0x60000---0x6FFFF                 sector6
   0x70000---0x7FFFF                 sector7
   0x80000---0x8FFFF                 sector8
   0x90000---0x9FFFF                 sector9
   0xA0000---0xAFFFF                 sector10
   0xB0000---0xBFFFF                 sector11
   0xC0000---0xCFFFF                 sector12
   0xD0000---0xDFFFF                 sector13
   0xE0000---0xEFFFF                 sector14
   0xF0000---0xFFFFF                 sector15
*/
typedef struct
{
	char 	offset[5];
	char 	addr[9];
	char  	size[5];
}MB_member_t;

#define               	SECTOR_SIZE        				0x10000
#define               	GET_SECTOR_HEAD_ADDR(sector)   ((sector)*(SECTOR_SIZE))
#define               	GET_SECTOR_END_ADDR(sector)    ((sector+1)*(SECTOR_SIZE)-1)
#define      	    	CORRECT_BACKUP_SECTOR           13
#define      	    	CORRECT_BACKUP_ADDR             (GET_SECTOR_HEAD_ADDR(CORRECT_BACKUP_SECTOR))
#define      	    	HZLIB_SECTOR                    0
#define  		    	HZLIB_INFO_ADDR          	    (GET_SECTOR_HEAD_ADDR(HZLIB_SECTOR))
#define  		   	    HZK16_START_ADDR          	    (GET_SECTOR_HEAD_ADDR(HZLIB_SECTOR)+256)
#include    			"Keyinput_method\key_input method.h"
#include    			"voice\app_buz.h"
/*GLOBAL FUN*/
void  						app_load_eEXE( const char*file );
void 						app_module_color_select(uint8_t flags);
void 						app_free( void*pmem );
void *						app_malloc( uint32_t  mem_size );
#define  					BUS_WRITE(data,addr)  		(*((uint16_t*)addr)=data)
#define  					BUS_READ(addr)   			(*((uint16_t*)addr))
extern Bsize_t    				g_mod_branch ;     /*模块分支变量，控制进入模块内不同分支*/
extern Bsize_t	   				g_select_branch;   //分支数据选择，=1主测量，=2查线，=3谐波,=4变比，=5读表
extern uint16_t   	 			g_Fg_color;         // 全局前景
extern uint16_t	 				g_Bg_color ;        // 全局背景
extern uint16_t	 				g_win_Bg_color;     // 全局窗口背景
extern uint16_t	        		g_color_pos;        // 指示全局颜色位置
extern const  char*				g_dlsr_option[2][9];
extern dev_id_t        			g_gpio_dev  ;
extern dev_id_t					g_piccom_dev ;
extern dev_id_t					g_UIout_dev  ;
extern uint8_t					g_language ;
extern dev_id_t 		 		g_spi_dev_id;
extern DL_app_user_all_info_t  	g_user_arg;
extern measure_param_t  	  	g_DL_param;
extern dev_id_t            		g_DL_dev  ;
#endif
