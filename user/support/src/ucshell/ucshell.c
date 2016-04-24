/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:微型用户操作界面
* 完成时间: 2010-08-30
* 所属公司:科荟
* 文件名称:ucshell.c
* 创建人 :揭成
* 功能描述:用户操作界面，直接控制后台运行
*                       功能可以做得很复杂，也可以很简单，要
*                       根据用户使用功能确定。
*-------------------------------------------------------------------*/
#include	"ucshell.h"
#if UC_SHELL_EN >0
#define 	    IS_NO_CMD             	0XFF
#define   		MAX_CMD_LEN           	 12
uint8_t 		m_max_cpu_usage 		= 0;
a_lock_t*		m_ucshell_lock 			= INVALID_LOCK;

static Bsize_t		_decide_letter( char asc );
static uint8_t 		_switch_letter_module( char asc );
static uint8_t 		_adjust_letter_module( char asc, uint8_t type );
#define         IS_NO_NCHAR      	0XFF
#define         IS_BIG_LETTER      	 2
#define         IS_LITTLE_LETTER      1
#define         IS_NO_LETTER      	 0

/*所有命令列表*/
/*依次为 命令关键字 ，命令回调函数，命令说明*/
ucshell_cmd_manege_t const m_cmd_manege_tab[] =
{
	{CMD_EXIT, 	ucshell_cmd_exit, 	"EXIT +命令+回车，退出命令"},
	{CMD_HELP, 	ucshell_cmd_help, 	"HELP +回车，查看帮助。" },
	{CMD_PST,	ucshell_cmd_pst, 	"pst+回车，查看当前运行的任务"},
	{CMD_PSD,	ucshell_cmd_psd, 	"psd+回车，查看当前运行的驱动"},
	{CMD_EFF,	ucshell_cmd_eff, 	"eff+回车，查看CPU占用率"},
	{CMD_PSK,	ucshell_cmd_psk, 	"psk+回车，查看当前每个任务堆栈情况"},
	{CMD_PS,		ucshell_cmd_ps, 	"ps+回车，开启所有查看功能,不实时更新"},
	{CMD_RST, 	ucshell_cmd_rst, 	"reboot+回车，系统热启动"},
	{CMD_KILL,	ucshell_cmd_kill, 	"kill+任务优先级+回车，强行关闭任务"},
	{CMD_CONFIG, ucshell_cmd_config, "config+回车，进入主配置界面，配置完成将自动重启动"},
};
/********************************************************************
 * @创建人:揭成
 * @功能:退出命令
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_cmd_exit( void*arg )
{
	uint8_t *pch = ( uint8_t* )arg;
	while ( *pch != ' ' )pch++;
	while ( *pch == ' ' )pch++;
	if ( *pch == 0 ) return;
	if ( global_memcmp( pch, ( uint8_t* )CMD_EFF, sizeof( CMD_EFF ) ) )
	{
		ucshell_cmd_eff_exit( 0 );
		m_max_cpu_usage = 0;
	}
	else if ( global_memcmp( pch, ( uint8_t* )CMD_PST, sizeof( CMD_PST ) ) )
	{
		ucshell_cmd_pst_exit( 0 );
	}
	else if ( global_memcmp( pch, ( uint8_t* )CMD_PSD, sizeof( CMD_PSD ) ) )
	{
		ucshell_cmd_psd_exit( 0 );
	}
	else if ( global_memcmp( pch, ( uint8_t* )CMD_PSK, sizeof( CMD_PSK ) ) )
	{
		ucshell_cmd_psk_exit( 0 );
	}
	else if ( global_memcmp( pch, ( uint8_t* )CMD_PS, sizeof( CMD_PS ) ) )
	{
		ucshell_cmd_ps_exit( 0 );
	}
	else
	{
		USER_LOG( "\n您输入的参数有误\n" );
	}
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:复位命令
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_cmd_rst( void*arg )
{
	app_module_maneger_exit( APP_CMD_REBOOT );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:复位命令
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_cmd_config( void*arg )
{
	app_module_maneger_exit( APP_CONFIG_REBOOT );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:KILL命令
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_cmd_kill( void*arg )
{
	uint8_t *pch = ( uint8_t* )arg;
	uint8_t pro;
	while ( *pch != ' ' )pch++;
	while ( *pch == ' ' )pch++;
	if ( *pch == 0 ) return;
	pro = ( uint8_t )atoi(( char* )pch );
	os_task_delete( pro ); /*强制删除，可能任务资源未释放，有风险*/
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:查看帮助信息
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/

void ucshell_cmd_help( void*arg )
{
	Bsize_t index;
	USER_LOG( "\n***************************************************\n" );
	for ( index = 0;index < SIZE_OF_ARRAY( m_cmd_manege_tab );index++ )
	{
		DBGMSG( "%d. %s\n", index, m_cmd_manege_tab[index].cmd_explain[0] );
	}
	USER_LOG( "***************************************************\n" );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:命令模块提示信息
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_cmd_prompt_msg( void )
{
	USER_LOG( "\n\n" );
	USER_LOG( "***************************************************\n" );
	USER_LOG( "*输入命令后按回车确认。				  *\n" );
	USER_LOG( "*命令输入不区分大小写  。                         *\n" );
	USER_LOG( "*输入help查看所有命令 。                          *\n" );
	USER_LOG( "***************************************************\n" );
	msleep( TYPE_DELAY );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:命令匹配
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
Bsize_t ucshell_match_cmd( uint8_t*pcmd, Bsize_t size )
{
	Bsize_t index;
	uint8_t tbuf[MAX_CMD_LEN];
	if ( size == 0 ) return 0xff;
	for ( index = 0;index < size;index++ )
	{
		tbuf[index] = _adjust_letter_module( pcmd[index],
					  IS_LITTLE_LETTER );
	}
	for ( index = 0;index < SIZE_OF_ARRAY( m_cmd_manege_tab );index++ )
	{
		if ( global_memcmp( tbuf, ( uint8_t* )m_cmd_manege_tab[index].cmd, size )
			 && ( m_cmd_manege_tab[index].cmd[size] == 0 ) )
		{
			if ( m_cmd_manege_tab[index].cmd_run_fun )
			{
				os_lock( m_ucshell_lock );
				m_cmd_manege_tab[index].cmd_run_fun(( void* )pcmd );
				os_unlock( m_ucshell_lock );
			}
			return index;
		}
	}
	USER_LOG( "\n您输入的命令有误请重新输入" );
	return ( 0xff );
}
/********************************************************************
 * @创建人:揭成
 * @功能:等待命令任务
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_wait_cmd_task( void*arg )
{
	uint8_t cmd_buf[MAX_CMD_LEN];
	uint8_t cmd_byte;
	Bsize_t offset = 0;
	Bsize_t cmd_len = 0;
	global_memset(( uint8_t * )cmd_buf, 0, 12 );
	msleep( TYPE_DELAY );
	ucshell_cmd_prompt_msg();
	USER_LOG( "\nroot:\\>" );
	while ( 1 )
	{
		if (( log_wait_input_msg( &cmd_byte ) == 1 ) )
		{
			if ( cmd_byte == UCSHELL_INPUT_ENTER )
			{
				( cmd_len == 0 ) ? ( cmd_len = offset ) : ( cmd_len = cmd_len );
				ucshell_match_cmd( cmd_buf, cmd_len );
				global_memset(( uint8_t * )cmd_buf, 0, 12 );
				offset = 0;
				cmd_len = 0;
				USER_LOG( "\nroot:\\>" );
			}
			else if ( cmd_byte == ' ' )
			{
				if ( cmd_len == 0 )
				{
					cmd_len = offset;
				}
				cmd_buf[offset] = cmd_byte;
				offset++;
				DBGMSG( "%c", cmd_byte, LOG_NP );
			}
			else if ( LETTERCHK( cmd_byte )
					  || DECCHK( cmd_byte ) )
			{
				cmd_buf[offset] = cmd_byte;
				offset++;
				DBGMSG( "%c", cmd_byte, LOG_NP );
			}
			else if ( cmd_byte == UCSHELL_INPUT_BACKSPACE )
			{
				if ( offset > 0 )
				{
					USER_LOG( "\b" );
					USER_LOG( " " );
					USER_LOG( "\b" );
					offset--;
					cmd_buf[offset] = 0;
				}
			}
		}
	}
}
/********************************************************************
 *@创建人 :揭成
 *@功能 :判断字符是不是字母
 *@输入 :asc 码
 *
 *@输出 : 字母的类型
                 IS_BIG_LETTER : 	大写字母
                 IS_LITTLE_LETTER : 小写字母
                 IS_NO_LETTER: 不是字母数据
********************************************************************/
static Bsize_t _decide_letter( char asc )
{
	Bsize_t ret = FAIL;
	if (( 65 <= asc ) && ( asc <= 90 ) )
	{
		ret = IS_BIG_LETTER;
	}
	else  if (( 97 <= asc ) && ( asc <= 122 ) )
	{
		ret = IS_LITTLE_LETTER;
	}
	else
	{
		ret = IS_NO_LETTER;
	}
	return ret;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :大小写切换
 * @输入 :asc 码
 *
 *@输出 : 字母输出，是大写的输出小写，
 *			是小写的输出大写。
********************************************************************/
static uint8_t _switch_letter_module( char asc )
{
	Bsize_t  type;
	uint8_t gap = (( uint8_t )'a' -( uint8_t )'A' );
	type = _decide_letter( asc ) ;
	if ( type == IS_BIG_LETTER )
	{
		return ( gap + asc );
	}
	else if ( type == IS_LITTLE_LETTER )
	{
		return ( asc -gap );
	}
	return asc;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :将字符ASC 调整为TYPE 类型(大写 或 小写)
 * @输入 :asc 码
 *
 *@输出 : '0'-'9'
********************************************************************/
static uint8_t _adjust_letter_module( char asc, uint8_t type )
{
	if ( _decide_letter( asc ) == IS_NO_LETTER )
	{
		return asc;
	}
	if (( _decide_letter( asc ) == type ) )
	{
		return asc;
	}
	return _switch_letter_module( asc );
}
/********************************************************************
 * @创建人:揭成
 * @功能 :模块插入
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_entery( void )
{
	ucshell_ps_cmd_init();
	m_max_cpu_usage 		= 0;
	os_task_assign_pro_build( ucshell_wait_cmd_task, UCSHELL_TASK_PRO, 2 );
	os_task_name_set( UCSHELL_TASK_PRO,  "Support--Shell manege" );
	os_lock_creat( &m_ucshell_lock );
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能 :模块退出
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void ucshell_exit( void )
{
	os_task_delete( UCSHELL_TASK_PRO );
	return;
}
#endif
