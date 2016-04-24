/******************************Copyright (c)***************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:28335 spi 驱动
* 完成时间2011-5-17
* 所属公司 :科荟
* 文件名称:SPIdriver.c
* 创建人 :揭成
* 功能描述:实现SPI读写操作
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:

*********************************************************************************************************
*/
#include "../Osdriver.h"
a_lock_t   *m_spi_lock = INVALID_LOCK;

/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动传送一个字节
 * @输入: SPI 读写操作
 * @输出: 收到的数据
********************************************************************/
int16 spi_write_read_byte(char bit_size,int16 data)
{
	int16 ret;
	SpiaRegs.SPICTL.all=0x1f;
	SpiaRegs.SPICCR.all=0x80|(bit_size-1);
	if(bit_size==8)
	{
		SpiaRegs.SPITXBUF= data<<8;			//送虚数，以产生读时序
		SpiaRegs.SPISTS.bit.BUFFULL_FLAG=1;
		while(SpiaRegs.SPISTS.bit.INT_FLAG==0){}
		ret=SpiaRegs.SPIRXBUF& 0xff; 
	}
	else
	{
		SpiaRegs.SPITXBUF= data;			//送虚数，以产生读时序
		SpiaRegs.SPISTS.bit.BUFFULL_FLAG=1;
		while(SpiaRegs.SPISTS.bit.INT_FLAG==0);
		ret=SpiaRegs.SPIRXBUF; 
	}      
	return(ret);
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动底层读入口
 * @输入:buf 读入缓存 
 		 size要读数据的大小
 * @输出:实际读到的大小
********************************************************************/
static Bsize_t spi_driver_read( uint8_t*buf, Bsize_t size , void*pos )
{
	if(size==8)
	{
	    *buf=spi_write_read_byte(size,0);
	}
	else
	{
	    *((uint16_t*)buf)=spi_write_read_byte(size,0);
	}
	return size;
}
/********************************************************************
 * @创建人:揭成
 * @功能:驱动底层读入口
 * @输入:buf 读入缓存      
 		 size要读数据的大小
 * @输出:实际读到的大小
********************************************************************/
static Bsize_t spi_driver_write( uint8_t*buf, Bsize_t size , void*pos )
{
	if(size==8)
	{
	    spi_write_read_byte(size,*buf);
	}
	else
	{
	    spi_write_read_byte(size,*((uint16_t*)buf));
	}
	return size;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动底层读入口
 * @输入:
 * @输出:	实际读到的大小
********************************************************************/
void  spi_driver_set_baud( uint32_t baud )
{
	SpiaRegs.SPIBRR = baud;
	return ;
}
static void spi_driver_disable_all_cs(void)
{
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动控制命令
 * @输入: cmd 控制命令  arg 控制参数
 * @输出: NONE
********************************************************************/
static void  spi_driver_ioctl( uint8_t cmd, void*arg )
{
	switch ( cmd )
	{
	case SET_SPI_BAUD:
		spi_driver_set_baud( *(( uint32_t* )arg ) );
		break;
    case SET_SD_CS_EN:
		GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1;
		break;
    case SET_SD_CS_DIS:
		GpioDataRegs.GPBSET.bit.GPIO57   = 1;
		break;
	case DRIVER_LOCK_REVERT:
		break;
	default:
		break;
	}
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动卸载
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void spi_driver_uninstall( void )
{
	return;
}
/********************************************************************
 * @作者 :揭成
 * @功能: 28335 spi gpio 初始化
 *
 * @输入:none
 *
 *@输出:none
********************************************************************/
static void spi_driver_init_GPIO()
{
	EALLOW;
	GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   // Enable pull-up on GPIO54 (SPISIMOA)
	GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;   // Enable pull-up on GPIO55 (SPISOMIA)
	GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
	GpioCtrlRegs.GPBPUD.bit.GPIO53 = 0;   // Enable pull-up on GPIO57 (SPISTEA)

	GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; // Asynch input GPIO54 (SPISIMOA)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 3; // Asynch input GPIO55 (SPISOMIA)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch input GPIO56 (SPICLKA)
//  GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3; // Asynch input GPIO57 (SPISTEA)

	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1; // Configure GPIO54 as SPISIMOA
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1; // Configure GPIO55 as SPISOMIA
	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1; // Configure GPIO56 as SPICLKA

    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO57  = 1;
	GpioDataRegs.GPBCLEAR.bit.GPIO57= 1;
	EDIS;
}
/********************************************************************
 * @创建人 :揭成
 * @功能: 28335 spi 配置
 *
 * @输入:none
 *
 *@输出:none
********************************************************************/

static void spi_driver_config( void )
{
	SpiaRegs.SPICCR.all = 0x0007;	  	/*Reset off, rising edge, 8-bit char bits */
	SpiaRegs.SPICTL.all = 0x000E;       		/*Enable master mode, delayed phase,*/
	SpiaRegs.SPIBRR = 0x0063;
	SpiaRegs.SPICCR.all = 0x0087;		/*Relinquish SPI from Reset */
	SpiaRegs.SPIPRI.bit.FREE = 1; 			/*Transmission not affected by emulator */
	return ;
}
/********************************************************************
 * @创建人: 揭成
 * @功能: 驱动初始化
 * @输入:	NONE
 * @输出:	NONE
********************************************************************/
static void spi_driver_init( void )
{
	spi_driver_init_GPIO();
	spi_driver_config();
	os_lock_creat( &m_spi_lock );
	return ;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:驱动注册
 * @输入:none
 * @输出:NONE
********************************************************************/
dev_id_t spi_driver_register( void )
{
	uint8_t ret;
	os_dirver_operate_t temp;
	temp.os_driver_init   = spi_driver_init;
	temp.os_driver_ioctl  = spi_driver_ioctl;
	temp.os_driver_read  = spi_driver_read;
	temp.os_driver_write = spi_driver_write;
	temp.os_driver_uninstall = spi_driver_uninstall;
	ret = os_driver_add( &temp, "SPI" );
	return ret;
}
#pragma DATA_SECTION (SPI_list,"DR_LIST_ADDR")
const DR_member_t SPI_list[]=
{
	spi_driver_register,
	0x5a5a
};

