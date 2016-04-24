#ifndef PEC_UCSHELL_2010_H
#define PEC_UCSHELL_2010_H
#include  "../../../includes.h"
#if UC_SHELL_EN >0
#define 			UCSHELL_INPUT_ENTER              	13
#define 			UCSHELL_INPUT_BACKSPACE      	'\b'  /*ASC =0X08*/
/*命令关键字*/
#define       CMD_PST          "pst"
#define       CMD_PSD          "psd"
#define       CMD_PSK          "psk"
#define       CMD_PS       	  "ps"
#define       CMD_EFF          "eff"
#define       CMD_RST          "reboot"
#define       CMD_HELP        "help"
#define       CMD_CONFIG    "config"
#define       CMD_EXIT  	  "exit"
#define       CMD_KILL         "kill"

/*********************************************************************
* UCSHELL-H  for ucshell
**********************************************************************/
typedef struct
{
	char*cmd;
	void( *cmd_run_fun )( void*arg );
	char*cmd_explain;
}ucshell_cmd_manege_t;

void 	ucshell_cmd_help( void*arg );
void 	ucshell_cmd_exit( void*arg );
void 	ucshell_cmd_rst( void*arg );
void 	ucshell_cmd_config( void*arg );
void 	ucshell_cmd_kill( void*arg );
extern uint8_t 			m_max_cpu_usage ;
extern Bsize_t     			m_event_stat_cnt;
extern a_lock_t*			m_ucshell_lock;


/*********************************************************************
* UCSHELL-H  for ucshell_ps
**********************************************************************/
void 	ucshell_cmd_pst( void*arg );
void 	ucshell_cmd_pst_exit( void*arg );
void 	ucshell_cmd_psd( void*arg );
void 	ucshell_cmd_psd_exit( void*arg );
void 	ucshell_cmd_eff( void*arg );
void 	ucshell_cmd_eff_exit( void*arg );
void 	ucshell_cmd_psk( void*arg );
void 	ucshell_cmd_psk_exit( void*arg );
void 	ucshell_cmd_ps( void*arg );
void 	ucshell_cmd_ps_exit( void*arg );
void 	ucshell_ps_cmd_init( void );

/*********************************************************************
* UCSHELL-H  外部调用接口
**********************************************************************/
void 	ucshell_entery( void );
void 	ucshell_exit( void );
#endif
#endif
