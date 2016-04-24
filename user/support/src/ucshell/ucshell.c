/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:΢���û���������
* ���ʱ��: 2010-08-30
* ������˾:����
* �ļ�����:ucshell.c
* ������ :�ҳ�
* ��������:�û��������棬ֱ�ӿ��ƺ�̨����
*                       ���ܿ������úܸ��ӣ�Ҳ���Ժܼ򵥣�Ҫ
*                       �����û�ʹ�ù���ȷ����
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

/*���������б�*/
/*����Ϊ ����ؼ��� ������ص�����������˵��*/
ucshell_cmd_manege_t const m_cmd_manege_tab[] =
{
	{CMD_EXIT, 	ucshell_cmd_exit, 	"EXIT +����+�س����˳�����"},
	{CMD_HELP, 	ucshell_cmd_help, 	"HELP +�س����鿴������" },
	{CMD_PST,	ucshell_cmd_pst, 	"pst+�س����鿴��ǰ���е�����"},
	{CMD_PSD,	ucshell_cmd_psd, 	"psd+�س����鿴��ǰ���е�����"},
	{CMD_EFF,	ucshell_cmd_eff, 	"eff+�س����鿴CPUռ����"},
	{CMD_PSK,	ucshell_cmd_psk, 	"psk+�س����鿴��ǰÿ�������ջ���"},
	{CMD_PS,		ucshell_cmd_ps, 	"ps+�س����������в鿴����,��ʵʱ����"},
	{CMD_RST, 	ucshell_cmd_rst, 	"reboot+�س���ϵͳ������"},
	{CMD_KILL,	ucshell_cmd_kill, 	"kill+�������ȼ�+�س���ǿ�йر�����"},
	{CMD_CONFIG, ucshell_cmd_config, "config+�س������������ý��棬������ɽ��Զ�������"},
};
/********************************************************************
 * @������:�ҳ�
 * @����:�˳�����
 *
 * @����:NONE
 *
 *@���:NONE
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
		USER_LOG( "\n������Ĳ�������\n" );
	}
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:��λ����
 *
 * @����:NONE
 *
 *@���:NONE
********************************************************************/
void ucshell_cmd_rst( void*arg )
{
	app_module_maneger_exit( APP_CMD_REBOOT );
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:��λ����
 *
 * @����:NONE
 *
 *@���:NONE
********************************************************************/
void ucshell_cmd_config( void*arg )
{
	app_module_maneger_exit( APP_CONFIG_REBOOT );
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:KILL����
 *
 * @����:NONE
 *
 *@���:NONE
********************************************************************/
void ucshell_cmd_kill( void*arg )
{
	uint8_t *pch = ( uint8_t* )arg;
	uint8_t pro;
	while ( *pch != ' ' )pch++;
	while ( *pch == ' ' )pch++;
	if ( *pch == 0 ) return;
	pro = ( uint8_t )atoi(( char* )pch );
	os_task_delete( pro ); /*ǿ��ɾ��������������Դδ�ͷţ��з���*/
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:�鿴������Ϣ
 *
 * @����:NONE
 *
 *@���:NONE
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
 * @������:�ҳ�
 * @����:����ģ����ʾ��Ϣ
 *
 * @����:NONE
 *
 *@���:NONE
********************************************************************/
void ucshell_cmd_prompt_msg( void )
{
	USER_LOG( "\n\n" );
	USER_LOG( "***************************************************\n" );
	USER_LOG( "*��������󰴻س�ȷ�ϡ�				  *\n" );
	USER_LOG( "*�������벻���ִ�Сд  ��                         *\n" );
	USER_LOG( "*����help�鿴�������� ��                          *\n" );
	USER_LOG( "***************************************************\n" );
	msleep( TYPE_DELAY );
	return;
}
/********************************************************************
 * @������:�ҳ�
 * @����:����ƥ��
 *
 * @����:NONE
 *
 *@���:NONE
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
	USER_LOG( "\n�������������������������" );
	return ( 0xff );
}
/********************************************************************
 * @������:�ҳ�
 * @����:�ȴ���������
 *
 * @����:NONE
 *
 *@���:NONE
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
 *@������ :�ҳ�
 *@���� :�ж��ַ��ǲ�����ĸ
 *@���� :asc ��
 *
 *@��� : ��ĸ������
                 IS_BIG_LETTER : 	��д��ĸ
                 IS_LITTLE_LETTER : Сд��ĸ
                 IS_NO_LETTER: ������ĸ����
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
 * @������ :�ҳ�
 * @���� :��Сд�л�
 * @���� :asc ��
 *
 *@��� : ��ĸ������Ǵ�д�����Сд��
 *			��Сд�������д��
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
 * @������ :�ҳ�
 * @���� :���ַ�ASC ����ΪTYPE ����(��д �� Сд)
 * @���� :asc ��
 *
 *@��� : '0'-'9'
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
 * @������:�ҳ�
 * @���� :ģ�����
 *
 * @����:NONE
 *
 *@���:NONE
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
 * @������:�ҳ�
 * @���� :ģ���˳�
 *
 * @����:NONE
 *
 *@���:NONE
********************************************************************/
void ucshell_exit( void )
{
	os_task_delete( UCSHELL_TASK_PRO );
	return;
}
#endif
