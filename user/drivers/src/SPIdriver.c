/******************************Copyright (c)***************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:28335 spi ����
* ���ʱ��2011-5-17
* ������˾ :����
* �ļ�����:SPIdriver.c
* ������ :�ҳ�
* ��������:ʵ��SPI��д����
*----------------------------------------------------------------
�޸ļ�¼:

�޸�����:

�޸���:

*********************************************************************************************************
*/
#include "../Osdriver.h"
a_lock_t   *m_spi_lock = INVALID_LOCK;

/********************************************************************
 * @������: �ҳ�
 * @����: ��������һ���ֽ�
 * @����: SPI ��д����
 * @���: �յ�������
********************************************************************/
int16 spi_write_read_byte(char bit_size,int16 data)
{
	int16 ret;
	SpiaRegs.SPICTL.all=0x1f;
	SpiaRegs.SPICCR.all=0x80|(bit_size-1);
	if(bit_size==8)
	{
		SpiaRegs.SPITXBUF= data<<8;			//���������Բ�����ʱ��
		SpiaRegs.SPISTS.bit.BUFFULL_FLAG=1;
		while(SpiaRegs.SPISTS.bit.INT_FLAG==0){}
		ret=SpiaRegs.SPIRXBUF& 0xff; 
	}
	else
	{
		SpiaRegs.SPITXBUF= data;			//���������Բ�����ʱ��
		SpiaRegs.SPISTS.bit.BUFFULL_FLAG=1;
		while(SpiaRegs.SPISTS.bit.INT_FLAG==0);
		ret=SpiaRegs.SPIRXBUF; 
	}      
	return(ret);
}
/********************************************************************
 * @������:�ҳ�
 * @����:�����ײ�����
 * @����:buf ���뻺�� 
 		 sizeҪ�����ݵĴ�С
 * @���:ʵ�ʶ����Ĵ�С
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
 * @������:�ҳ�
 * @����:�����ײ�����
 * @����:buf ���뻺��      
 		 sizeҪ�����ݵĴ�С
 * @���:ʵ�ʶ����Ĵ�С
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
 * @������: �ҳ�
 * @����: �����ײ�����
 * @����:
 * @���:	ʵ�ʶ����Ĵ�С
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
 * @������: �ҳ�
 * @����: ������������
 * @����: cmd ��������  arg ���Ʋ���
 * @���: NONE
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
 * @������: �ҳ�
 * @����: ����ж��
 * @����:	NONE
 * @���:	NONE
********************************************************************/
static void spi_driver_uninstall( void )
{
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @����: 28335 spi gpio ��ʼ��
 *
 * @����:none
 *
 *@���:none
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
 * @������ :�ҳ�
 * @����: 28335 spi ����
 *
 * @����:none
 *
 *@���:none
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
 * @������: �ҳ�
 * @����: ������ʼ��
 * @����:	NONE
 * @���:	NONE
********************************************************************/
static void spi_driver_init( void )
{
	spi_driver_init_GPIO();
	spi_driver_config();
	os_lock_creat( &m_spi_lock );
	return ;
}
/********************************************************************
 * @������: �ҳ�
 * @����:����ע��
 * @����:none
 * @���:NONE
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

