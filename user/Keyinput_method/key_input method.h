#ifndef _PEC_APP_KEY_INPUT_2010_H
#define _PEC_APP_KEY_INPUT_2010_H
#include        "../usertype.h"
#include        "../includes.h"

#define    	    KEY_SPACE     KEY_F4

enum
{
	KEY_INPUT_ABC   = 0x01,
	KEY_INPUT_abc   = 0x02,
	KEY_INPUT_123   = 0x04,
	KEY_INPUT_QP    = 0x08,
	KEY_INPUT_PY    = 0x10,
	KEY_END_INPUT   = KEY_INPUT_QP,
	KEY_FIRST_INPUT = KEY_INPUT_ABC,
	KEY_METHOD_ALL  = 0X0F
};
/**/
extern  uint8_t   		g_key_input_keep_secret;
/*查询码表*/
BOOL 	 key_input_method_is_run( void );
uint8_t* py_ime( uint8_t *input_py_val );
/*用户使用函数*/
void 	 hand_wirte_input_box_pos_set( uint16_t x, uint16_t y );

Bsize_t	 key_method_input( uint8_t key, char*buf ); 			/*按键输入获取*/

Bsize_t  key_method_input_shadow( uint8_t key, char*pshadow );	/*按键输入获取，并保存在shadow*/

void 	 key_input_method_assign( uint16_t flag );				/*指定输入法*/

void 	 key_input_method_switch( void );						/*切换输入法*/

void 	 key_input_method_filter( uint16_t flag );

uint8_t  key_input_method_obtain( void );

void 	 key_input_method_xy_set( uint16_t x, uint16_t y );		/*设置拼音输入框位置*/

void 	 key_input_method_init( void );							/*按键输入法初始化*/

int 	 key_method_QP( uint8_t code_0, char*hzbuf );

int 	 key_method_GB( uint8_t code_0, char*hzbuf );

int 	 key_method_ABC( uint8_t code_0, char*hzbuf );

int 	 key_method_abc( uint8_t code_0, char*hzbuf );

int 	 key_method_123( uint8_t code_0,char*buf );

void 	 key_input_GB_xy_set( uint16_t x, uint16_t y );

void 	 key_input_QP_xy_set( uint16_t x, uint16_t y );

void 	 key_input_ABC_xy_set( uint16_t x, uint16_t y );


typedef  struct
{
   int(*Get_code)(uint8_t code_0,char*buf);
   void(*Setxy)(uint16_t x, uint16_t y );
   
}key_method_fun_t;

typedef struct
{
	uint8_t  	keypad_buf[7];
	uint8_t  	key_offset;
	uint8_t  	key_press_repeat;
	uint8_t  	last_key;
	uint16_t 	input_select;
	uint16_t 	input_flag;
	key_method_fun_t func;
}key_input_ctl_t;
/*********************************************************************
* GLOBAL-H for KEY
**********************************************************************/
char 			app_global_decide_inputkey( uint8_t key );
char 			app_global_decide_Nkey( uint8_t key );
char 			app_global_decide_PYkey( uint8_t key );
char  			app_global_keytoletter( uint8_t key );
uint8_t  		app_global_Ntokey( uint8_t offset );
uint8_t 		app_global_key_force_obtain( uint8_t*buf, Bsize_t size );
uint8_t 		app_global_key_obtain( uint8_t*buf, Bsize_t size );
void 			app_global_key_control_power_req( void );
void 			app_global_key_control_power_release( void );
BOOL 			app_global_key_resource_req( void );
void 			app_global_key_resource_release( void );
void  			app_global_key_dummy_press( uint8_t key );


extern uint8_t 	py_Head_aToz_NUm[26];
extern const char** PY_path[];
void PY_num_count(void);
#endif
