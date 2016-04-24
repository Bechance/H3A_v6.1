/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:按键驱动
* 创建时间:2010-07-14
* 所属公司 :科荟
* 文件名称:keydriver
* 创建人 :揭成
* 功能描述:按键驱动支持,中断响应，不支持连键，
*                      不支持组合键。
*----------------------------------------------------------------
修改记录:

修改目的:

修改日期:

修改人:

*********************************************************************************************************
*/
#include "../Osdriver.h"
#define 		  KEY_FIFO_SIZE  2

static uint8_t 		_key_fifo[KEY_FIFO_SIZE] = {0, };
static fifo_tcb_t   _key_fifo_tcb_body 	     = {0, };
static fifo_tcb_t*  _key_fifo_tcb 	         = &_key_fifo_tcb_body;

void GUI_key_create(uint8_t key);

#define           FPGAKEY_ADDR   0X4001

#define           KEY_INT_EN()   {;}
#define           KEY_INT_DIS()  {;}
/********************************************************************
 * @创建人 :揭成
 * @功能: 驱动参数设置接口
 *
 * @输入:cmd 控制命令  arg控制参数
 *
 * @输出:TURE 成功 FAIL 失败
********************************************************************/
void  key_driver_ioctl( uint8_t cmd, void*arg )
{
	uint8_t *key_buf = ( uint8_t* )arg;
	switch ( cmd )
	{
	case  KEY_CMD_DUMMY_PRESS:
		fifo_buffer_push(_key_fifo_tcb, key_buf, 1 );
		GUI_key_create(*key_buf);
		break;
	case  KEY_CMD_BUF_EMPTY:
		fifo_buffer_empty(_key_fifo_tcb);
		break;
	default:
		break;
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:没有定义
 * @输入:buf 待写缓冲
 *       size:要写入大小
 * @输出:实际写入字节
********************************************************************/
Bsize_t key_driver_write( uint8_t*buf, Bsize_t size , void*pos )
{
	return 0;
}

/********************************************************************
 * @创建人 :揭成
 * @功能:按键扫描
 * @输入:none
 * @输出:TURE 成功 FAIL 失败
********************************************************************/
void key_read_isr( uint8_t key)
{
	fifo_buffer_push( _key_fifo_tcb, &key, 1 );
	GUI_key_create(key);
	return;
}

/********************************************************************
 * @创建人: 揭成
 * @功能: 获得当前键值
 *
 * @输入:buf 存放读出数据缓冲
 *       size:需要读出的大小
 *
 * @输出:实际读出大小
********************************************************************/
Bsize_t key_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	Bsize_t real_size;
	KEY_INT_DIS();
	real_size = fifo_buffer_get_size( _key_fifo_tcb );
	if ( !real_size )
	{
		KEY_INT_EN();
		return 0;
	}
	if ( real_size > size )
	{
		real_size = size;
	}
	fifo_buffer_pop( _key_fifo_tcb, buf, real_size );
	KEY_INT_EN();
	return real_size;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 按键驱动初始化
 * @输入: NONE
 * @输出: NONE
********************************************************************/
void key_driver_init( void )
{
	fifo_buffer_init( _key_fifo_tcb, KEY_FIFO_SIZE, _key_fifo );/*初始化按键fifo*/
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 按键驱动卸载
 * @输入: NONE
 * @输出: NONE
********************************************************************/
void key_driver_uninstall( void )
{
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 按键驱动注册
 * @输入:none
 * @输出:NONE
********************************************************************/
dev_id_t key_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init  = key_driver_init;
	temp.os_driver_ioctl = key_driver_ioctl;
	temp.os_driver_read  = key_driver_read;
	temp.os_driver_write = key_driver_write;
	temp.os_driver_uninstall = key_driver_uninstall;
	ret = os_driver_add( &temp, "KEY" );
	return ret;
}
/*driver AUTO Register*/
#pragma DATA_SECTION (key_list,"DR_LIST_ADDR")
const DR_member_t key_list[]=
{
	key_driver_register,
	0x5a5a
};

