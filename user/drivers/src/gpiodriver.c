/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:2812 IO 控制
* 创建时间:2013-8-10
* 所属公司:科荟
* 文件名称:gpiodriver.c
* 创建人  :揭成
* 功能描述:对系统相关GPIO 提供支持
*----------------------------------------------------------------
修改记录:

修改目的:

修改日期:

修改人:

*********************************************************************************************************
*/
#include "../Osdriver.h"
/*-----------------------------------------------------------*/
// IO 扩展
/*-----------------------------------------------------------*/
/*------------------START--------------------------------*/
uint8_t		m_EXTIO_state[7];
#define 	IO_EXT_CLK			   (GpioDataRegs.GPADAT.bit.GPIO23)
#define		IO_EXT_DATA			   (GpioDataRegs.GPADAT.bit.GPIO22)
#define 	IO_EXT_EN			   (GpioDataRegs.GPADAT.bit.GPIO21)

static void _udelay( Bsize_t us )
{
	volatile int t_index, h_index;
	for ( t_index = 0;t_index < us;t_index++ )
	{
		for ( h_index = 0;h_index <250;h_index++ )
		{
			asm( "	nop" );
			asm( "	nop" );
			asm( "	nop" );
			asm( "	nop" );
		}
	}
	return;
}

static void EXTIO_one_byte_send(uint8_t cur_data)
{
    uint8_t offset;
	for ( offset = 0;offset < 8;offset++ )
	{
		IO_EXT_DATA =(!(cur_data&0x80))?(0):(1);
		_udelay(10);
		IO_EXT_CLK  = 1;
		_udelay(10);
		IO_EXT_CLK  = 0;
		cur_data = cur_data << 1;
	}
   return;
}

static void EXTIO_send_start(void)
{
	return;
}

static void EXTIO_send_end(void)
{
	IO_EXT_EN = 0;
	_udelay(10);
	IO_EXT_EN = 1;
	_udelay(10);
	IO_EXT_EN = 0;
	_udelay(20);
	return;
}

void EXTIO_send_Ndata(void)
{
   uint8_t*pnData = m_EXTIO_state;
   EXTIO_send_start();
   EXTIO_one_byte_send(pnData[4]);
   EXTIO_one_byte_send(pnData[3]);
   EXTIO_one_byte_send(pnData[2]);
   EXTIO_one_byte_send(pnData[1]);
   EXTIO_one_byte_send(pnData[0]);
   EXTIO_send_end();
   return;
}

void*EXTIO_buf_obtain(void)
{
    return (void*)m_EXTIO_state;
}
/*-------------------END---------------------------------------*/
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动底层读入口
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
Bsize_t gpio_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	return 0;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动模块控制
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
void  gpio_driver_ioctl( uint8_t cmd, void*arg )
{
	switch ( cmd )
	{                                                         
    case GPIO_CMD_SET_EXT_IO_DATA:
		EXTIO_send_Ndata();
		break;
	case GPIO_CMD_GET_EXT_BUF:
		*((uint8_t**)arg)=m_EXTIO_state;
		break;
	case GPIO_CMD_SET_BUZ_ON:
		GpioDataRegs.GPACLEAR.bit.GPIO9=1;	
		break;
    case GPIO_CMD_SET_BUZ_OFF:
		GpioDataRegs.GPASET.bit.GPIO9=1;	
		break;
	default:
		break;
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动底层写入口
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
Bsize_t gpio_driver_write( uint8_t*buf, Bsize_t size , void*pos )
{
	return 0;
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动卸载
 * @输入:NONE
 * @输出:NONE
********************************************************************/
void gpio_driver_uninstall( void )
{
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动初始化
 * @输入:NONE
 * @输出:NONE
********************************************************************/
void gpio_driver_init( void )
{
	EALLOW;
	GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;   
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;   
	GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;  
	GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;  
	
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0; 
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;
	
	GpioCtrlRegs.GPADIR.bit.GPIO21  = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO22  = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO23  = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO24  = 1;

	
	GpioDataRegs.GPASET.bit.GPIO21  = 1;
	GpioDataRegs.GPASET.bit.GPIO22  = 1;
	GpioDataRegs.GPASET.bit.GPIO23  = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO24= 1;
	

	GpioCtrlRegs.GPAPUD.bit.GPIO9   = 0;   
	GpioCtrlRegs.GPAMUX1.bit.GPIO9  = 0; 
	GpioCtrlRegs.GPADIR.bit.GPIO9   = 1;

	GpioCtrlRegs.GPAMUX1.bit.GPIO4  = 0; 
	GpioCtrlRegs.GPADIR.bit.GPIO4   = 1;
	GpioDataRegs.GPASET.bit.GPIO4   = 0;


	EDIS;
	GLOBAL_MEMCLR(m_EXTIO_state,7);
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:驱动注册
 * @输入:none
 * @输出:NONE
********************************************************************/
dev_id_t gpio_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init  = gpio_driver_init;
	temp.os_driver_ioctl = gpio_driver_ioctl;
	temp.os_driver_read  = gpio_driver_read;
	temp.os_driver_write = gpio_driver_write;
	temp.os_driver_uninstall = gpio_driver_uninstall;
	ret = os_driver_add( &temp, "GPIO" );
	
	return ret;
}

#pragma DATA_SECTION (GPIO_list,"DR_LIST_ADDR")
const DR_member_t GPIO_list[]=
{
	gpio_driver_register,
	0x5a5a
};


