/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:系统时间管理
* 创建时间:2010-08-06
* 所属公司 :科荟
* 文件名称:app_global_key
* 创建人 :揭成
* 功能描述:按键驱动相关函数
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:
********************************************************************/
#include 				"key_input method.h"

dev_id_t   	m_key_dev_id = INVALID_DEV_ID;/*按键设备ID*/
a_lock_t*   m_key_lock 	 = INVALID_LOCK;

uint8_t const g_Nkey_tab[][3] =
{
	{KEY_0 , '0', ' '}, {KEY_1, '1', ' '},
	{KEY_2 , '2', 'a'}, {KEY_3 , '3', 'd'},
	{KEY_4 , '4', 'g'}, {KEY_5 , '5', 'j'},
	{KEY_6 , '6', 'm'}, {KEY_7 , '7', 'p'},
	{KEY_8 , '8', 't'}, {KEY_9 , '9', 'w'},
	{KEY_SPACE ,' ', ' '}, {KEY_9 , '9', 'w'},
	{KEY_ADD_SUB,'/',' '},
	{KEY_POINT, '.', ' '},
	{KEY_DEL, ' ', ' '}
};
/********************************************************************
 * @创建人 :揭成
 * @功能 :得到输入键字符
 * @输入 :按键键值
 *
 *@输出 : 如果是数字按键将返回字符，如果不是返回0
********************************************************************/
char app_global_decide_inputkey( uint8_t key )
{
	uint8_t index;
	uint8_t Nkey;
	for ( index = 0;index < SIZE_OF_ARRAY( g_Nkey_tab );index++ )
	{
	    Nkey = g_Nkey_tab[index][0]; 
		if ( key == Nkey)
		{
			return g_Nkey_tab[index][1];
		}
	}
	return 0;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :得到数字按键字符
 * @输入 :按键键值
 *
 *@输出 : 如果是数字按键将返回字符，如果不是返回0
********************************************************************/
char app_global_decide_Nkey( uint8_t key )
{
	uint8_t index;
	for ( index = 0;index < 10;index++ )
	{
		if ( key == g_Nkey_tab[index][0] )
		{
			return g_Nkey_tab[index][1];
		}
	}
	return 0;
}
char app_global_decide_PYkey( uint8_t key )
{
	uint8_t index;
	for ( index = 2;index < 10;index++ )
	{
		if ( key == g_Nkey_tab[index][0] )
		{
			return g_Nkey_tab[index][2];
		}
	}
	return 0;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :根据数字得到数字按键值
 * @输入 :按键键值
 *
 * @输出 :数字符号
********************************************************************/
uint8_t  app_global_Ntokey( uint8_t offset )
{
	uint8_t index;
	uint8_t ch;
	ch = GLOBAL_NTOC( offset );
	for ( index = 0;index < 10;index++ )
	{
		if ( ch == g_Nkey_tab[index][1] )
		{
			return g_Nkey_tab[index][0];
		}
	}
	return 0;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :根据按键得到拼音字母
 * @输入 :按键键值
 *
 *@输出 :TURE OR FAIL
********************************************************************/
char  app_global_keytoletter( uint8_t key )
{
	uint8_t index;
	for ( index = 0;index < SIZE_OF_ARRAY( g_Nkey_tab );index++ )
	{
		if ( key == g_Nkey_tab[index][0] )
		{
			return g_Nkey_tab[index][2];
		}
	}
	return FAIL;
}
/**************以上为跟KEY相关扩展，以下为驱动连接*************************************************/
/********************************************************************
 * @创建人 :揭成
 * @功能 :按键控制权操作，和app_global_key_obtain配合
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void app_global_key_control_power_req( void )
{
	os_lock( m_key_lock );
	return;
}
void app_global_key_control_power_release( void )
{
	os_unlock( m_key_lock );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :得到按键值，不通过按键控制权请求，
 *			用于任务临时获取按键，获取前要先申请控制权利
 *                限制别的任务在获取按键
 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
uint8_t app_global_key_force_obtain( uint8_t*buf, Bsize_t size )
{
	uint8_t ret;
	ret = os_driver_read( m_key_dev_id, buf, size, 0 );
	if ( ret > 0 )
	{
		buz_response( APP_100_MS*2);
		//if ( *buf == KEY_F1 )
		{
			//os_unlock( m_key_lock );
		//	app_helpmod_entery();
			//os_lock( m_key_lock );
		}
	}
	return ret;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :用于模拟按键按下

 * @输入 :
 *
 *@输出 : NONE
********************************************************************/
void  app_global_key_dummy_press( uint8_t key )
{
	os_driver_ioctl( m_key_dev_id, KEY_CMD_DUMMY_PRESS, ( void* )&key );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :得到按键值
 * @输入 :buf 输入缓冲 SIZE 要获取按键长度
 *
 *@输出 : NONE
********************************************************************/
uint8_t app_global_key_obtain( uint8_t*buf, Bsize_t size )
{
	uint8_t ret;
	os_lock( m_key_lock );
	ret = os_driver_read( m_key_dev_id, buf, size, 0 );
	if ( ret > 0 )
	{
		buz_response( APP_100_MS*2);
		/*if ( *buf == KEY_F1 )
		{
			os_unlock( m_key_lock );
			os_lock( m_key_lock );
		}*/
	}
	os_unlock( m_key_lock );
	return ret;
}
/********************************************************************
//empty
********************************************************************/
void app_global_key_FIFO_empty( void )
{
	os_driver_ioctl( m_key_dev_id, KEY_CMD_BUF_EMPTY, 0 );
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :初始化KEY 驱动
 * @输入 :
 * @输出 : NONE
********************************************************************/
BOOL app_global_key_resource_req( void )
{
	m_key_dev_id = os_driver_open("KEY");
	app_global_key_FIFO_empty();
	os_lock_creat( &m_key_lock );
	return TURE;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :释放按键资源
 * @输入 :
 * @输出 : NONE
********************************************************************/
void app_global_key_resource_release( void )
{
	os_driver_close(m_key_dev_id);
	m_key_dev_id = INVALID_DEV_ID;
	return;
}
